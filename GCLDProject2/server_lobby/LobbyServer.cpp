
#include "LobbyServer.h"

//#include "session/ClientSession.h"
#include "session/GameServerSession.h"
#include "session/CrossServerSession.h"
#include "server_client/CenterServerClient.h"
#include "server_client/EventLogServerClient.h"
#include "net_framework/IoServicePool.h"
#include "json/JsonTool.h"
#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"

#include "Opcode.pb.h"
//#include "Login.pb.h"


#include "AutoUpdateLobby.h"
#include "logic/country/war/CrossWarDestServerMgr.h"


LobbyServer::LobbyServer(void)
{
	//SetLogicalTaker( new LoginServerLogicalTaker() ) ;
}

LobbyServer::~LobbyServer(void)
{
}



bool LobbyServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
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


Session* LobbyServer::CreateSesion( int type )
{
	Session *pSession =NULL ;
	switch( type )
	{
	case PACKET_TYPE_SERVER_CROSS:
		{
			pSession =new CrossServerSession() ;
		} break ;
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


void LobbyServer::OnBeforeStart( void )
{

}


void LobbyServer::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	sCenterClient.Update( nowTimeMS, diffMS ) ;
	sEventLogClient.Update( nowTimeMS, diffMS ) ;
	sAutoUpdateLobby.Update(nowTimeMS, diffMS);
	sCrossWarDestSerMgr.Update(nowTimeMS, diffMS);
}

void LobbyServer::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game server relative

void LobbyServer::GameServerUnRegister( Session *pSession )
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


LobbyServer::ServerRegisterResult LobbyServer::GameServerRegister( Session *pSession )
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

LobbyServer::ServerRegisterResult LobbyServer::CrossServerRegister( Session *pSession )
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


bool LobbyServer::AddOneEventLogServer( const std::string &ip, const std::string &port )
{
	if( sEventLogClient.IsSettingAddress() )
	{
		return false ;
	}

	sEventLogClient.SetDstInfo( ip, port ) ;
	sEventLogClient.SetConnectNow() ;
	return true ;
}

SessionPairId LobbyServer::GetServerPairId( uint64 index )
{
	uint32 serverId = index & 0x00000000ffffffff ;
	uint32 regionId = index >> 32 ;
	
	//NLOG( " %u and %u get value : %llu .", serverId, regionId, index ) ;
	return SessionPairId( serverId, regionId ) ;
}

uint64 LobbyServer::GetServerIndex( const SessionPairId &pairId )
{
	uint64 serverId = pairId.first ;
	uint64 regionId = pairId.second ;

	uint64 retValue = serverId | regionId << 32 ;
	//NLOG( " %u and %u get value : %llu .", pairId.first, pairId.second, retValue ) ;
	return retValue ;
}

void LobbyServer::CrossServerUnRegister( Session *pSession )
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

	int bCount = m_crossSessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;
	if( bCount != 1 )
	{
		ELOG( "m_gameSessionAddresses.erase() return number not equal 1 is error !" ) ;
	}

	pSession->SetIsVerification( false ) ;
}
