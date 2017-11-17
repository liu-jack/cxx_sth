
#include "EventLogServer.h"

//#include "EventLogServerLogicalTaker.h"

#include "session/GameServerSession.h"
#include "session/LoginServerSession.h"
#include "session/CrossServerSession.h"

#include "server_client/CenterServerClient.h"
#include "net_framework/IoServicePool.h"

#include "Logger.h"
#include "OS.h"


#include "def/MmoAssert.h"

#include "Opcode.pb.h"


EventLogServer::EventLogServer(void)
{
	//SetLogicalTaker( new EventLogServerLogicalTaker() ) ;
}

EventLogServer::~EventLogServer(void)
{
}



bool EventLogServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
{
	if( !Server::InitGameConf( filePath, strConfigFile, strDefaultConfigFile ) )
	{
		return false ;
	}

	if( GetLogicalTaker() )
	{
		GetLogicalTaker()->SetThreadUpdateLimitTimeMS( Config().GetInt( "Self.thread_update_limit_time_ms", 100 ) ) ;
		GetLogicalTaker()->SetNotRecvPackLimitTimeMS( Config().GetInt( "Self.not_recv_packet_limit_time_ms", 3000 ) ) ;
	}

	return true ;
}


Session* EventLogServer::CreateSesion( int type )
{
	Session *pSession =NULL ;
	switch( type )
	{
	case PACKET_TYPE_SERVER_GAME:
		{
			pSession =new GameServerSession() ;
		} break ;

	case PACKET_TYPE_SERVER_LOGIN:
		{
			pSession =new LoginServerSession() ;
		} break ;
	case PACKET_TYPE_SERVER_CROSS:
		{
			pSession =new CrossServerSession() ;
		} break ;
	case PACKET_TYPE_SERVER_LOBBY:
		{
			pSession =new CrossServerSession() ;
		} break ;
	case PACKET_TYPE_CLIENT:
	case PACKET_TYPE_SERVER_GATEWAY:
	case PACKET_TYPE_SERVER_DBPROXY:
	case PACKET_TYPE_SERVER_CENTER:
	case PACKET_TYPE_SERVER_WEB:
	default:
		{
			ASSERT( false ) ;
		}
	}

	return pSession ;
}


void EventLogServer::OnBeforeStart( void )
{
}


void EventLogServer::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	sCenterClient.Update( nowTimeMS, diffMS ) ;
}

void EventLogServer::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game server relative

void EventLogServer::GameServerUnRegister( Session *pSession )
{
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


EventLogServer::ServerRegisterResult EventLogServer::GameServerRegister( Session *pSession )
{
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cross server relative

void EventLogServer::CrossServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_CROSS ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int aCount =m_crossSessions.erase( pSession->GetPairId() ) ;
	ASSERT( aCount == 1 ) ;
	if( aCount != 1 )
	{
		ELOG( "m_gameSessions.erase() return number not equal 1 is error !" ) ;
	}

	int bCount =m_crossSessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;
	if( bCount != 1 )
	{
		ELOG( "m_gameSessionAddresses.erase() return number not equal 1 is error !" ) ;
	}

	pSession->SetIsVerification( false ) ;
}


EventLogServer::ServerRegisterResult EventLogServer::CrossServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_CROSS ) ;

	if( m_crossSessions.find( pSession->GetPairId() ) != m_crossSessions.end() )
	{
		return SERVER_REGISTER_FAIL_ID ;
	}

	if( m_crossSessionAddresses.find( pSession->GetAddress() ) != m_crossSessionAddresses.end() )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_crossSessions.insert( std::make_pair( pSession->GetPairId(), pSession ) ) ;
	m_crossSessionAddresses.insert( pSession->GetAddress() ) ;

	pSession->SetIsVerification( true ) ;

	return SERVER_REGISTER_OK ;
}

void EventLogServer::LobbyServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_LOBBY ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int aCount =m_lobbySessions.erase( pSession->GetPairId() ) ;
	ASSERT( aCount == 1 ) ;
	if( aCount != 1 )
	{
		ELOG( "m_gameSessions.erase() return number not equal 1 is error !" ) ;
	}

	int bCount =m_lobbySessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;
	if( bCount != 1 )
	{
		ELOG( "m_gameSessionAddresses.erase() return number not equal 1 is error !" ) ;
	}

	pSession->SetIsVerification( false ) ;
}

EventLogServer::ServerRegisterResult EventLogServer::LobbyServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_LOBBY ) ;

	if( m_lobbySessions.find( pSession->GetPairId() ) != m_lobbySessions.end() )
	{
		return SERVER_REGISTER_FAIL_ID ;
	}

	if( m_lobbySessionAddresses.find( pSession->GetAddress() ) != m_lobbySessionAddresses.end() )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_lobbySessions.insert( std::make_pair( pSession->GetPairId(), pSession ) ) ;
	m_lobbySessionAddresses.insert( pSession->GetAddress() ) ;

	pSession->SetIsVerification( true ) ;

	return SERVER_REGISTER_OK ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Loign server relative
void EventLogServer::LoginServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_LOGIN ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int bCount =m_loginSessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;

	RemoveSessionFromSessionMap( m_loginSessions, pSession ) ;

	pSession->SetIsVerification( false ) ;
}

EventLogServer::ServerRegisterResult EventLogServer::LoginServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_LOGIN ) ;

	if( m_loginSessionAddresses.find( pSession->GetAddress() ) != m_loginSessionAddresses.end() )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_loginSessionAddresses.insert( pSession->GetAddress() ) ;

	m_newLoginSessions.push_back( pSession ) ;

	bool isOk =AddSeesionToSeerionMap( m_loginSessions, pSession ) ;
	ASSERT( isOk ) ;
	if( isOk )
	{
		pSession->SetIsVerification( true ) ;
	}

	return SERVER_REGISTER_OK ;
}
