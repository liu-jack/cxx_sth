
#include "Session.h"

#include "Logger.h"
#include "OS.h"

#include "SessionTypeDef.h"
#include "Server.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"
#include "detail/TcpAsyncConn.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"


uint32 Session::s_takePacketLimit       =10;
uint64 Session::s_delayNoticLimitTimeMS =10;


Session::Session( void )
{
	Init() ;
}

Session::~Session( void )
{
	ASSERT( m_spConn == NULL ) ;
	Clear() ;
}


void Session::Init( void )
{
	m_isClosed          =false ;
	m_isNeedClose       =false ;
	m_isServerSession   =false ;
	m_isVerification    =false ;
	m_isInWorldThread   =true ;
	m_sessionType       =PACKET_TYPE_END;
	m_sessionId         =0;
	m_pServer           =NULL;
	m_delayTime         =0;
	m_preRecvPacketTime =0;
	m_waitVerfyTimeMS   =0 ;
	m_spConn            =NULL;
}

void Session::Clear( void )
{
	DeleteAllPtrInLFQueue<NetPack*>( m_packetTable ) ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// virtual
void Session::OnConnect( TcpAsyncConn *pConn )
{
	NLOG( "Session::OnConnect() be invoked !!!!!!" ) ;
	ASSERT( false ) ;
}

void Session::OnRecv( TcpAsyncConn *pConn, NetPack *pPack )
{
	if( m_spConn == NULL )
	{
		ASSERT( GetIsHadClosed() ) ;
		WLOG( "Session::OnRecv( id: %llu) is get recv opCode (%s)%d packet by is post close, so discard!", GetSessionId(), pPack->GetOpcodeName(), pPack->GetOpcode() ) ;
		return ;
	}

	if( GetSessionType() != pPack->GetPacketType() )
	{
		ELOG( "Session Id: %llu, recv opCode (%s)%d packet type:%s not match seesion type %s, will be abandoned."
			, GetSessionId(), pPack->GetOpcodeName(), pPack->GetOpcode(), pPack->GetPacketTypeName(), GetSessionTypeName() ) ;

		delete pPack ;
		pPack =NULL ;

		CloseSession() ;
		return ;
	}

	GetServer()->NoticeRecv( pConn->Id(), pPack ) ;

	PostInputPacket( pPack ) ; 
}

void Session::OnSend( uint64 sessionId, NetPack *pPack )
{
	pPack->SetPacketType( GetSessionType() ) ;
	GetServer()->NoticeSend( sessionId, pPack ) ;
}

void Session::OnClose( uint64 sessionId )
{
	m_spConn =NULL ;

	GetServer()->OnCloseSession( sessionId ) ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
const char* Session::GetSessionTypeName( void )
{
	return NetPack::GetPacketTypeName( GetSessionType() ) ;
}


bool Session::GetIsSetSessionType( void )
{
	return m_sessionType != PACKET_TYPE_END ;
}

void Session::Send( NetPack &packet,const uint64 sessionId, char clientType) const
{
	packet.SetPacketType(clientType) ;
	packet.SetSessionId(sessionId);
	if( m_spConn != NULL )
	{
		m_spConn->AsSendStackPacket( packet ) ;
	}
	else
	{
		ELOG( "Session is %llu send %s(%u) faild because m_spConn is empty!!", GetSessionId(), packet.GetOpcodeName(), packet.GetOpcode() ) ;
	}
}

void Session::Send( NetPack *pPacket,const uint64 sessionId, char clientType) const
{
	pPacket->SetPacketType(clientType) ;
	pPacket->SetSessionId(sessionId);
	if( m_spConn != NULL )
	{
		m_spConn->AsSendNewPacket( pPacket ) ;
	}
	else
	{
		ELOG( "Session is %llu send %s(%u) faild because m_spConn is empty!!", GetSessionId(), pPacket->GetOpcodeName(), pPacket->GetOpcode() ) ;

		delete pPacket ;
		pPacket =NULL ;
	}
}

void Session::Send( NetPack &packet ) const
{
	packet.SetPacketType( GetServer()->GetServerType() ) ;

	if( m_spConn != NULL )
	{
		m_spConn->AsSendStackPacket( packet ) ;
	}
	else
	{
		ELOG( "Session is %llu send %s(%u) faild because m_spConn is empty!!", GetSessionId(), packet.GetOpcodeName(), packet.GetOpcode() ) ;
	}
}

void Session::Send( NetPack *pPacket ) const
{
	pPacket->SetPacketType( GetServer()->GetServerType() ) ;

	if( m_spConn != NULL )
	{
		m_spConn->AsSendNewPacket( pPacket ) ;
	}
	else
	{
		ELOG( "Session is %llu send %s(%u) faild because m_spConn is empty!!", GetSessionId(), pPacket->GetOpcodeName(), pPacket->GetOpcode() ) ;

		delete pPacket ;
		pPacket =NULL ;
	}
}

void Session::Send( int opCode, const ::google::protobuf::Message &msg ) const
{
	NetPack *pPacket =new NetPack( opCode, msg.ByteSize() + 2 ) ; // + 2, beacuse packet << msg will add a uint16 sign the msg size .
	*pPacket << msg;
	Send( pPacket );
}

void Session::Send( const int opCode, const uint64 sessionId, const ::google::protobuf::Message &msg ) const
{
	NetPack *pPacket =new NetPack( opCode, msg.ByteSize() + 2 ) ; // + 2, beacuse pPacket << msg will add a uint16 sign the msg size .
	pPacket->SetSessionId( sessionId ) ;
	*pPacket << msg;
	Send( pPacket );
}

void Session::Send( const int opCode, const uint64 sessionId,char clientType, const ::google::protobuf::Message &msg ) const
{
	NetPack *pPacket =new NetPack( opCode, msg.ByteSize() + 2 ) ; // + 2, beacuse pPacket << msg will add a uint16 sign the msg size .
	*pPacket << msg;
	Send( pPacket,sessionId,clientType );
}

void Session::CloseSession( void )
{
	//ASSERT( m_spConn ) ;
	ASSERT( GetIsHadClosed() == false ) ;

	SetIsHadClosed( true ) ;
	if( m_spConn != NULL )
	{
		m_spConn->PostClose() ;
		m_spConn =NULL ;
	}
}

void Session::Kick( int32 errorCode )
{
	NetPack pack( pb::SMSG_KICK ) ;
	pack << errorCode;
	Send( pack ) ;
}


void Session::SetConn( TcpAsyncConn *pConn )
{
	ASSERT( pConn ) ;
	m_spConn =pConn ;
	m_spConn->SetPackHandler( this ) ;

	// 9223372036854775807 * 2 : max uint64
	char port[20] ="" ;
	sprintf( port, "%d", m_spConn->Port() ) ;
	SetAddress( SessionAddress( port, m_spConn->Ip() ) ) ;
}

void Session::Update( uint64 nowTimeMS, int64 diffMS )
{
	NetPack *pPacket         =NULL ;

	uint32 takeCount = GetTakePacketLimit();
	while( takeCount > 0 && GetOnePacket( pPacket ) )
	{
		takeCount--;
		SetPreRecvTime( nowTimeMS ) ;

		// Smart pointer.
		std::auto_ptr<NetPack> guard( pPacket );

		if( !HandleNetPack( *pPacket ) )
		{
			ELOG ("%s session receive not handle packet from %s, session id:%llu, %s(%d) will be close"
					, GetSessionTypeName(), pPacket->GetPacketTypeName(), GetSessionId(), pPacket->GetOpcodeName(), pPacket->GetOpcode() );

			SetIsNeedClose( true ) ;
		}
	}


}


void Session::HandlePing( NetPack &packet, int pongOpCode )
{
	uint64 pingTimeMS      =0 ;
	uint64 pingDelayTimeMS =0 ;

	packet >> pingTimeMS     ; ASSERT( pingTimeMS > 0 ) ;
	packet >> pingDelayTimeMS;

	SetDelayTime( pingDelayTimeMS ) ;

	NetPack pong( pongOpCode, sizeof( uint64 ) * 2 ) ;
	pong << pingTimeMS ;
	Send( pong ) ;

	// Delay notice 
	if( GetDelayTime() > GetDelayNoticeLimitTimeMS() )
	{
		WLOG( "Id:%llu, type:%s, delay %llu MS !!!!", GetSessionId(), GetSessionTypeName(), GetDelayTime() ) ;
	}
}

