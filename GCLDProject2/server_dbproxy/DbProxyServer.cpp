
#include "DbProxyServer.h"

#include "DbProxyServerLogicalTaker.h"

#include "session/GameServerSession.h"


#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"


#include "Opcode.pb.h"

uint32							DbProxyServer::s_maxCachePlayerBaseDataCount  =0 ;
uint32                          DbProxyServer::s_maxCachePlayerDays           =0 ;
uint32                          DbProxyServer::s_maxCachePlayerCount          =0 ;
time_t                          DbProxyServer::s_maxCacheOfflinePlayerSeconds =0 ;
time_t                          DbProxyServer::s_playerDataSaveDiffMS         =0 ;

DbProxyServer::DbProxyServer(void)
{
	// ��ʱ�ر��߼��߳�
	//SetLogicalTaker( new DbProxyServerLogicalTaker() ) ;
}

DbProxyServer::~DbProxyServer(void)
{
}


void DbProxyServer::OnBeforeStart( void )
{
	NLOG( "max_cache_player_count: %u", GetMaxCachePlayerCount() ) ;
	NLOG( "max_cache_offline_player_seconds: %llu", GetMaxCacheOfflinePlayerSeconds() ) ;

}

bool DbProxyServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
{
	if( !Server::InitGameConf( filePath, strConfigFile, strDefaultConfigFile ) )
	{
		return false ;
	}

	
	SetMaxCachePlayerDays( Config().GetInt( "Self.max_cache_player_days", 7 ) ) ;

	SetMaxCachePlayerCount( Config().GetInt( "Self.max_cache_player_count", 1000 ) ) ;

	SetMaxCacheOfflinePlayerSeconds( Config().GetInt( "Self.max_cache_offline_player_seconds", 600 ) ) ;

	SetPlayerDataSaveDiffMS( Config().GetInt( "Self.player_data_save_diff_ms", 300000 ) ) ;

	SetMaxCachePlayerBaseDataCount( Config().GetInt( "Self.max_cache_base_player_data_count", 1000 ) ) ;

	if( GetLogicalTaker() )
	{
		GetLogicalTaker()->SetThreadUpdateLimitTimeMS( Config().GetInt( "Self.thread_update_limit_time_ms", 100 ) ) ;
		GetLogicalTaker()->SetNotRecvPackLimitTimeMS( Config().GetInt( "Self.not_recv_packet_limit_time_ms", 3000 ) ) ;
	}
	
	return true ;
}


Session* DbProxyServer::CreateSesion( int type )
{
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



void DbProxyServer::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}

void DbProxyServer::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game server relative

void DbProxyServer::GameServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int aCount =m_gameSessionIds.erase( pSession->GetPairId() ) ;
	ASSERT( aCount == 1 ) ;

	//int bCount =m_gameSessionAddresses.erase( pSession->GetAddress() ) ;
	//if( pSession->GetIsVerification() )
	//{
	//	ASSERT( aCount == 1 ) ;
	//}

	RemoveSessionFromSessionMap( m_gameSessions, pSession ) ;

	pSession->SetIsVerification( false ) ;
}

DbProxyServer::ServerRegisterResult DbProxyServer::GameServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;

	if( m_gameSessionIds.find( pSession->GetPairId() ) != m_gameSessionIds.end() )
	{
		return SERVER_REGISTER_FAIL_ID ;
	}

	//if( m_gameSessionAddresses.find( pSession->GetAddress() ) != m_gameSessionAddresses.end() )
	//{
	//	return SERVER_REGISTER_FAIL_ADDRESS ;
	//}

	m_gameSessionIds.insert( pSession->GetPairId() ) ;
	//m_gameSessionAddresses.insert( pSession->GetAddress() ) ;

	bool isOk =AddSeesionToSeerionMap( m_gameSessions, pSession ) ;
	ASSERT( isOk ) ;
	if( isOk )
	{
		pSession->SetIsVerification( true ) ;
	}

	return SERVER_REGISTER_OK ;
}

