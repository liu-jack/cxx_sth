
#include "CrossServer.h"

//#include "session/ClientSession.h"
#include "session/GameServerSession.h"

#include "server_client/CenterServerClient.h"
#include "server_client/EventLogServerClient.h"
#include "server_client/LobbyServerClient.h"
#include "net_framework/IoServicePool.h"
//#include "json/JsonTool.h"
#include "Logger.h"
#include "OS.h"
#include "object/Player.h"

#include "def/MmoAssert.h"

#include "Opcode.pb.h"
#include "AutomaticUpdate/AutoUpdateCross.h"
#include "CrossServerLogicalTaker.h"
//#include "Login.pb.h"
#include "session/PlayerPool.h"





CrossServer::CrossServer(void)
:m_regionId(0),m_serverId(1)
{
	LLOG("{CrossServer}CrossServer::CrossServer");
	Player::RegisteAllHandlers() ;
	SetLogicalTaker( new CrossServerLogicalTaker() ) ;
}

CrossServer::~CrossServer(void)
{
}


//初始化跨服服务器配置
bool CrossServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
{
	//LLOG("{CrossServer}CrossServer::InitGameConf");
	if( !Server::InitGameConf( filePath, strConfigFile, strDefaultConfigFile ) )		//配置消息时间等
	{
		return false ;
	}
	m_serverId          = Config().GetInt( "Self.server_id", 1 ) ;

	if( GetLogicalTaker() )	//逻辑处理模块
	{
		GetLogicalTaker()->SetThreadUpdateLimitTimeMS( Config().GetInt( "Self.thread_update_limit_time_ms", 100 ) ) ;
		GetLogicalTaker()->SetNotRecvPackLimitTimeMS( Config().GetInt( "Self.not_recv_packet_limit_time_ms", 3000 ) ) ;
	}

	return true ;
}


Session* CrossServer::CreateSesion( int type )
{
	LLOG("{CrossServer}CrossServer::CreateSesion");
	Session *pSession =NULL ;
	switch( type )
	{
	case PACKET_TYPE_SERVER_GAME:
		{
			pSession =new GameServerSession() ;
		} break ;
	case PACKET_TYPE_CLIENT:
	case PACKET_TYPE_SERVER_LOGIN:
	case PACKET_TYPE_SERVER_GATEWAY:
	case PACKET_TYPE_SERVER_DBPROXY:
	case PACKET_TYPE_SERVER_CENTER:
	default:
		{
			ASSERT( false ) ;
		}
	}

	return pSession ;
}


void CrossServer::OnBeforeStart( void )
{
	//LLOG("{CrossServer}CrossServer::OnBeforeStart");
//	sHttpClients.Start( sIoPool.GetServerIoService() ) ;

	// Test
	//const int count =3 ;
	//for( int i =0; i < count; ++i )
	//{
	//	MCRequest *pR =new MCRequest( "www.baidu.com" ) ;
	//	sHttpClients.GetMultiCurl().Request( pR ) ;
	//}
	//sHttpClients.GetMultiCurl().PostRequest() ;
	//std::vector<MCRequest*> mf ;
	//while( mf.size() < count )
	//{
	//	MCRequest *pR =sHttpClients.GetMultiCurl().GetFinishRequest() ;
	//	if( pR != NULL )
	//	{
	//		mf.push_back( pR ) ;
	//	}

	//	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	//}

	//mf.size() ;
}


void CrossServer::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("{CrossServer}CrossServer::BeforeSeesionUpdate");
	sCenterClient.Update( nowTimeMS, diffMS ) ;
	sEventLogClient.Update( nowTimeMS, diffMS ) ;
	//sAutoUpdateCross.Update(nowTimeMS, diffMS);
	sLobbyClient.Update(nowTimeMS,diffMS);
}

void CrossServer::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("{CrossServer}CrossServer::AfterSessionUpdate");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game server relative

void CrossServer::GameServerUnRegister( Session *pSession )
{
	LLOG("{CrossServer}CrossServer::GameServerUnRegister");
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int aCount =m_gameSessions.erase( pSession->GetPairId() ) ;
	ASSERT( aCount == 1 ) ;
	if( aCount != 1 )
	{
		ELOG( "m_gameSessions.erase() return number not equal 1 is error !" ) ;
	}

	int bCount =m_gameSessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;
	if( bCount != 1 )
	{
		ELOG( "m_gameSessionAddresses.erase() return number not equal 1 is error !" ) ;
	}

	pSession->SetIsVerification( false ) ;


}


CrossServer::ServerRegisterResult CrossServer::GameServerRegister( Session *pSession )
{
	LLOG("{CrossServer}CrossServer::GameServerRegister");
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;

	if( m_gameSessions.find( pSession->GetPairId() ) != m_gameSessions.end() )
	{
		return SERVER_REGISTER_FAIL_ID ;
	}

	if( m_gameSessionAddresses.find( pSession->GetAddress() ) != m_gameSessionAddresses.end() )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_gameSessions.insert( std::make_pair( pSession->GetPairId(), pSession ) ) ;
	m_gameSessionAddresses.insert( pSession->GetAddress() ) ;

	pSession->SetIsVerification( true ) ;



	return SERVER_REGISTER_OK ;
}

SessionPairId CrossServer::GetGamePairId( uint64 index )
{
	LLOG("{CrossServer}CrossServer::GetGamePairId");
	uint32 serverId = index & 0x00000000ffffffff ;
	uint32 regionId = index >> 32 ;
	
	//NLOG( " %u and %u get value : %llu .", serverId, regionId, index ) ;
	return SessionPairId( serverId, regionId ) ;
}

uint64 CrossServer::GetGameIndex( const SessionPairId &pairId )
{
	LLOG("{CrossServer}CrossServer::GetGameIndex");
	uint64 serverId = pairId.first ;
	uint64 regionId = pairId.second ;

	uint64 retValue = serverId | regionId << 32 ;
	//NLOG( " %u and %u get value : %llu .", pairId.first, pairId.second, retValue ) ;
	return retValue ;
}

bool CrossServer::AddOneEventLogServer( const std::string &ip, const std::string &port )
{
	LLOG("{CrossServer}CrossServer::AddOneEventLogServer");
	if( sEventLogClient.IsSettingAddress() )
	{
		return false ;
	}

	sEventLogClient.SetDstInfo( ip, port ) ;
	sEventLogClient.SetConnectNow() ;
	return true ;
}