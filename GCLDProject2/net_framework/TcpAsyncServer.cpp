
#include <vector>
#include <string>

#include "Logger.h"
#include "OS.h"
#include "def/MmoAssert.h"
#include "TcpAsyncServer.h"
#include "IoServicePool.h"

#include "SConfig.h"

using namespace boost::asio::ip;

TcpAsyncServer::TcpAsyncServer( const std::string &port, const std::string &fixIP /*=""*/ )
	:m_idSeed( 1000 )
	,m_isOpen(false)
	,m_pPackHandler( NULL )
	,m_ioSvc( sIoPool.GetServerIoService() )
	 ,m_acceptor( m_ioSvc )
{
	m_justConnectLimitTimeMS =3000 ;

	m_port =port ;

	//Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	tcp::resolver resolver( m_ioSvc );
	tcp::resolver::query query( tcp::v4(), m_port );
	tcp::endpoint endpoint = *resolver.resolve( query );
	m_acceptor.open( endpoint.protocol() );
	m_acceptor.set_option( boost::asio::ip::tcp::acceptor::reuse_address( true ) );
	m_acceptor.bind( endpoint );
	m_acceptor.listen();

	// Load server config
	if( !sConfig.LoadOrCopyFormDefaultFile( "etc/", "server.conf", "default/server_default.conf" ) )
	{
		printf( "server config loading fail.....................!" ) ;
		assert( false ) ;
	}

	m_ip =fixIP ;

	if( m_ip.empty() )
	{
	// Get server adderss
#ifdef _WIN32

	tcp::resolver addressResolver( m_ioSvc );
	tcp::resolver::query addressQuery(host_name(), "");
	tcp::resolver::iterator iter = addressResolver.resolve(addressQuery);

	boost::asio::ip::tcp::resolver::iterator end;
	boost::system::error_code error = boost::asio::error::host_not_found;
	boost::asio::ip::tcp::endpoint addressEndpoint;
	while( error && iter != end )
	{
		addressEndpoint = *iter ;
		if( !addressEndpoint.address().is_multicast() && !addressEndpoint.address().is_unspecified() && !addressEndpoint.address().is_loopback() && addressEndpoint.address().is_v4() )
		{
			m_ip = addressEndpoint.address().to_v4().to_string() ;
			break ;
		}
		iter++ ;
	}

#else

	int      sock_get_ip;  
	char     ipaddr[50];  
	struct   sockaddr_in *sin;  
	struct   ifreq ifr_ip;     

	if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{  
		printf("socket create failse...GetLocalIp!\n");  
		return ;
	}  

	typedef std::vector< std::string > NetworkCardNameTable ;
	NetworkCardNameTable networkCardNames ;
	if( !sConfig.GetStringArray( "Linux.network_card_name", ",", networkCardNames ) )
	{
		printf( "get config network name fail use default..........\n" ) ;
		networkCardNames.push_back( "eth0" ) ;
		networkCardNames.push_back( "p2p1" ) ;
	}

	bool isFindNetworkCardName =false ;
	for( NetworkCardNameTable::iterator itr =networkCardNames.begin(); itr != networkCardNames.end(); ++itr )
	{
		memset(&ifr_ip, 0, sizeof(ifr_ip));     
		strncpy(ifr_ip.ifr_name, itr->c_str(), sizeof(ifr_ip.ifr_name) - 1);     

		if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) >= 0 )     
		{     
			isFindNetworkCardName =true ;
			printf( "use network name is %s;\n", itr->c_str() ) ;
			break ;
		}       
	}

	if( !isFindNetworkCardName )
	{
		printf("socket create failse...GetLocalIp!\n");  
		return ;
	}

	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
	strcpy(ipaddr,inet_ntoa(sin->sin_addr));         

	m_ip =ipaddr ;

	close( sock_get_ip );

#endif
	}

	NLOG( "Server ip: %s ;", m_ip.c_str() ) ;
}

TcpAsyncServer::~TcpAsyncServer( void )
{
}

void TcpAsyncServer::Remove( uint64 sessionId )
{
	NLOG( "TcpAsyncServer remove session %d. ", sessionId ) ;
	m_justConnectConns.erase( sessionId ) ;

	ConnectMapItr itr =m_conns.find( sessionId ) ;
	if( itr != m_conns.end() )
	{
		m_waiForCloseConns.insert( std::make_pair( ::time( NULL ), itr->second ) ) ;
		m_conns.erase( itr ) ;
	}

	ASSERT( boost::this_thread::get_id() == sIoPool.GetServerIoThreadId() ) ;
}


void TcpAsyncServer::Start( INetPackHandler* pPackHandler )
{
	m_isOpen =true ;

	m_pPackHandler = pPackHandler;

	StartAccept();
}

void TcpAsyncServer::StartAccept( void )
{
	TcpAsyncConn::Ptr pNewConn = TcpAsyncConn::Create( this, sIoPool.GetIoService(), ++m_idSeed );

	m_acceptor.async_accept
		(
		 pNewConn->Socket(),
		 boost::bind
		 (
		  &TcpAsyncServer::HandleAccept, 
		  this, 
		  pNewConn, 
		  boost::asio::placeholders::error
		 )
		);
}


void TcpAsyncServer::HandleAccept( TcpAsyncConn::Ptr pNewConn, const boost::system::error_code& error )
{
	if( !GetIsOpen() )
	{
		pNewConn.reset() ;
		return ;
	}

	uint64 nowTimeMS =sOS.TimeMS() ;
	if( !error )
	{
		ASSERT( boost::this_thread::get_id() == sIoPool.GetServerIoThreadId() ) ;
		bool isInsert =m_conns.insert( std::make_pair( pNewConn->Id(), pNewConn ) ).second ;
		if( !isInsert )
		{
			ELOG( "TcpAsyncServer::HandleAccept()::m_justConnectConns insert faild, will close session, id is duplication !!!!! " ) ;
			ASSERT( false ) ;
			pNewConn->PostClose() ;
		}
		else
		{
			m_justConnectConns[ pNewConn->Id() ]= pNewConn ;

			pNewConn->Start( m_pPackHandler );
			pNewConn->SetConnectTimeMS( nowTimeMS ) ;
		}
	}

	StartAccept();

	CheckWaitFirstPacketSessions( nowTimeMS ) ;
}

void TcpAsyncServer::Stop()
{
	m_ioSvc.post(boost::bind(&TcpAsyncServer::SyncStop, this));
}


void TcpAsyncServer::SyncStop()
{
	m_acceptor.cancel() ;
	m_acceptor.close() ;

	m_isOpen =false ;
}

//void TcpAsyncServer::HadRecvPacket( int sessionId )
//{
//	ConnectMapItr itr = m_justConnectConns.find( sessionId ) ;
//	if( itr != m_justConnectConns.end() )
//	{
//		bool isInsert =m_conns.insert( *itr ).second ;
//		if( !isInsert )
//		{
//			itr->second->PostClose() ;
//
//			ELOG( "TcpAsyncServer::HadRecvPacket()::m_conns insert faild, will close session, id is duplication !!!!! " ) ;
//			ASSERT( false ) ;
//		}
//
//		m_justConnectConns.erase( itr ) ;
//	}
//	else
//	{
//		ELOG( "TcpAsyncServer::HadRecvPacket() can not find session: %d ", sessionId ) ;
//	}
//}

void TcpAsyncServer::CheckWaitFirstPacketSessions( uint64 nowTimeMS )
{
	ASSERT( nowTimeMS > m_justConnectLimitTimeMS ) ;
	const uint64 checkTime = nowTimeMS - m_justConnectLimitTimeMS ;

	ConnectMapItr tmpItr ;
	for( ConnectMapItr itr =m_justConnectConns.begin(); itr != m_justConnectConns.end(); /*++itr*/ )
	{
		tmpItr =itr++ ;

		TcpAsyncConn::Ptr &ptr =tmpItr->second ;

		if( ptr->GetIsGetPacket() )
		{
			m_justConnectConns.erase( tmpItr ) ;
		}
		else
		{
			if( checkTime < ptr->GetConnectTimeMS() )
			{
				break ;
			}

			NLOG( "Session %d( %s:%d ) be closed, beacuse not first packet after %llu MS !"
					, tmpItr->first, ptr->Ip().c_str(), ptr->Port(), m_justConnectLimitTimeMS ) ;

			ptr->SetPackHandler( NULL ) ; // Not recv packet .

			ptr->PostClose() ;

			m_justConnectConns.erase( tmpItr ) ;
		}
	}


	// check for close
	time_t nowTime =::time( NULL ) ;
	for( MConnectMapItr rItr =m_waiForCloseConns.begin(); rItr != m_waiForCloseConns.end(); /*++itr*/ )
	{
		TcpAsyncConn::Ptr &ptr =rItr->second ;
		if( nowTime - rItr->first > 120 && ptr->GetIsFinishClose() )
		{
			m_waiForCloseConns.erase( rItr++ ) ;
		}
		else
		{
			++rItr ;
		}
	}
}

