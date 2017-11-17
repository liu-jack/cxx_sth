
#include "CenterServer.h"

#include "CenterServerLogicalTaker.h"

#include "CenterServerHttpClients.h"

#include "session/GameServerSession.h"
#include "session/LoginServerSession.h"
#include "session/WebServerSession.h"
#include "session/EventLogServerSession.h"
#include "session/CrossServerSession.h"
#include "session/LobbyServerSession.h"

#include "net_framework/IoServicePool.h"


#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "utility/STLUtility.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "Web.pb.h"



CenterServer::CenterServer(void)
{
	m_pEventLogSession       =NULL ;	//事件日志会话
	m_pNewEventLogSession    =NULL ;	//临时会话

	// 中心服务器也暂时不开逻辑线程
	//SetLogicalTaker( new CenterServerLogicalTaker() ) ;
}

CenterServer::~CenterServer(void)
{
}


//初始化配置文件
bool CenterServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
{
	//基类初始化配置文件
	if( !Server::InitGameConf( filePath, strConfigFile, strDefaultConfigFile ) )
	{
		return false ;
	}

	//设置层次ID
	SetRegionId( Config().GetInt( "Self.region_id", 95560 ) ) ;
	ASSERT( GetRegionId() != 0 ) ;

	//中心服务器没有逻辑线程
	if( GetLogicalTaker() )
	{
		GetLogicalTaker()->SetThreadUpdateLimitTimeMS( Config().GetInt( "Self.thread_update_limit_time_ms", 100 ) ) ;
		GetLogicalTaker()->SetNotRecvPackLimitTimeMS( Config().GetInt( "Self.not_recv_packet_limit_time_ms", 3000 ) ) ;
	}

	return true ;
}

//创建会话
Session* CenterServer::CreateSesion( int type )
{
	Session *pSession =NULL ;
	switch( type )
	{
	case PACKET_TYPE_SERVER_GAME:
		{
			pSession = new GameServerSession() ;
		} break ;

	case PACKET_TYPE_SERVER_LOGIN:
		{
			pSession = new LoginServerSession() ;
		} break ;

	case PACKET_TYPE_SERVER_WEB:
		{
			pSession = new WebServerSession() ;
			pSession->SetIsVerification( true ) ;
		} break ;

	case PACKET_TYPE_SERVER_EVENT:
		{
			pSession = new EventLogServerSession() ;
		} break ;
	case PACKET_TYPE_SERVER_CROSS:
		{
			pSession = new CrossServerSession() ;
		} break ;

	case PACKET_TYPE_SERVER_LOBBY:
		{
			pSession = new LobbyServerSession() ;
		} break ;
	case PACKET_TYPE_CLIENT:
	case PACKET_TYPE_SERVER_GATEWAY:
	case PACKET_TYPE_SERVER_DBPROXY:
	case PACKET_TYPE_SERVER_CENTER:
		{
		} break ;

	default:
		{
			ASSERT( false ) ;
		}
	}

	return pSession ;
}

void CenterServer::OnBeforeStart( void )
{
}


void CenterServer::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}

void CenterServer::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	CheckNoticeNewLoginServerToGame() ;
	CheckNoticeNewCrossServerToGame();
	CheckNoticeNewEventLogServerToGameAndLogin() ;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game server relative
//卸载游戏服务器会话
void CenterServer::GameServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int aCount =m_gameSessionIds.erase( pSession->GetPairId() ) ;
	ASSERT( aCount == 1 ) ;

	int bCount =m_gameSessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;

	m_gameSessions.erase( pSession->GetPairId().first ) ;

	pSession->SetIsVerification( false ) ;
}

//注册游戏服务器会话
CenterServer::ServerRegisterResult CenterServer::GameServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;	//会话类型为游戏服务器会话

	if( m_gameSessionIds.find( pSession->GetPairId() ) != m_gameSessionIds.end() )	//会话的serverid&regionid已经被注册了
	{
		return SERVER_REGISTER_FAIL_ID ;
	}

	if( m_gameSessionAddresses.find( pSession->GetAddress() ) != m_gameSessionAddresses.end() )	//会话的IP，port已经被注册了
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	//插入游戏会话列表一条会话
	m_gameSessionIds.insert( pSession->GetPairId() ) ;
	m_gameSessionAddresses.insert( pSession->GetAddress() ) ;

	bool isInsert =m_gameSessions.insert( std::make_pair( pSession->GetPairId().first, pSession ) ).second ;
	ASSERT( isInsert ) ;
	if( isInsert )
	{
		pSession->SetIsVerification( true ) ;	//注册成功，该条会话设置为已识别
	}

	return SERVER_REGISTER_OK ;
}



void CenterServer::BroadcastMsgToGameServer( const int opCode, std::vector<int>& retVec, const ::google::protobuf::Message &msg, SessionPairIdSet& sendResult)
{
	std::vector<int>::iterator retIter = retVec.begin();
	for(; retIter != retVec.end(); ++retIter)
	{
	  int serverid = (int)(*retIter);
	  SessionByServerIdMapItr itr = m_gameSessions.find( serverid) ;
	  if( itr != m_gameSessions.end() )
	  {
		 itr->second->Send( opCode,msg) ;
		 sendResult.insert( SessionPairId( serverid, 200 )) ;
	  }
	  else
	  {
		 sendResult.insert( SessionPairId( serverid, 0 )) ;
	  }
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Loign server relative
//卸载登录服务器会话
void CenterServer::LoginServerUnRegister( Session *pSession )
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

//注册登录服务器对话
void CenterServer::CrossServerUnRegister(Session *pSession)
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_CROSS ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int bCount = m_crossSessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;

	RemoveSessionFromSessionMap( m_crossSessions, pSession ) ;

	pSession->SetIsVerification( false ) ;
  
}

void CenterServer::LobbyServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_LOBBY ) ;
	if( !pSession->GetIsVerification() )
	{
		return ;
	}

	int bCount = m_lobbySessionAddresses.erase( pSession->GetAddress() ) ;
	ASSERT( bCount == 1 ) ;

	RemoveSessionFromSessionMap( m_lobbySessions, pSession ) ;

	pSession->SetIsVerification( false ) ;

}

CenterServer::ServerRegisterResult CenterServer::LoginServerRegister( Session *pSession )
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

//检查新登录的消息转发给游戏服务器
CenterServer::ServerRegisterResult CenterServer::CrossServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_CROSS ) ;

	if( m_crossSessionAddresses.find( pSession->GetAddress() ) != m_crossSessionAddresses.end() )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_crossSessionAddresses.insert( pSession->GetAddress() ) ;

	m_newCrossSessions.push_back( pSession ) ;

	bool isOk =AddSeesionToSeerionMap( m_crossSessions, pSession ) ;
	ASSERT( isOk ) ;
	if( isOk )
	{
		pSession->SetIsVerification( true ) ;
	}

	return SERVER_REGISTER_OK ;
}


CenterServer::ServerRegisterResult CenterServer::LobbyServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_LOBBY ) ;

	if( m_lobbySessionAddresses.find( pSession->GetAddress() ) != m_lobbySessionAddresses.end() )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_lobbySessionAddresses.insert( pSession->GetAddress() ) ;

	bool isOk =AddSeesionToSeerionMap( m_lobbySessions, pSession ) ;
	ASSERT( isOk ) ;
	if( isOk )
	{
		pSession->SetIsVerification( true ) ;
	}

	return SERVER_REGISTER_OK ;
}

void CenterServer::CheckNoticeNewLoginServerToGame( void )
{
	// Notice new login after pb::SG2C_GET_LOGIN_SERVERS message
	if( !m_newLoginSessions.empty() )	//新登录会话不为空
	{
		if( !m_gameSessions.empty() )	//游戏服务器会话存在
		{
			pb::SG2C_GetLoinServersRet retInfo ;

			for( SessionVec::iterator vItr =m_newLoginSessions.begin(); vItr != m_newLoginSessions.end(); ++vItr )
			{
				pb::Address *pAddress =retInfo.add_addresses() ;

				Session *pSession =*vItr ;
				const SessionAddress &sAddress =pSession->GetAddress() ;
				pAddress->set_ip( sAddress.second ) ;
				pAddress->set_port( sAddress.first ) ;
			}

			NetPack packet( pb::SC2G_ADD_LOGIN_SERVERS, retInfo.ByteSize() + 2 ) ;
			packet << retInfo ;
			BroadcastToGameServer( packet ) ;	//向所有游戏服务器会话广播消息
		}

		m_newLoginSessions.clear() ;
	}
}


void CenterServer::CheckNoticeNewCrossServerToGame( void )
{
	//if( !m_newCrossSessions.empty() )
	//{
	//	if( !m_gameSessions.empty() )
	//	{
	//		pb::SG2C_GetCrossServersRet retInfo ;

	//		for( SessionVec::iterator vItr =m_newCrossSessions.begin(); vItr != m_newCrossSessions.end(); ++vItr )
	//		{
	//			pb::Address *pAddress =retInfo.add_addresses() ;

	//			Session *pSession =*vItr ;
	//			const SessionAddress &sAddress =pSession->GetAddress() ;
	//			pAddress->set_ip( sAddress.second ) ;
	//			pAddress->set_port( sAddress.first ) ;
	//		}

	//		NetPack packet( pb::SC2G_ADD_CROSS_SERVERS, retInfo.ByteSize() + 2 ) ;
	//		packet << retInfo ;
	//		BroadcastToGameServer( packet ) ;
	//	}

	//	m_newCrossSessions.clear() ;
	//}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Web server relative
//获得web服务器会话
WebServerSession* CenterServer::GetWebServerSession( SessionId sesionId )
{
	Session *pSession =GetSession( sesionId ) ;
	if( pSession != NULL && pSession->GetSessionType() == PACKET_TYPE_SERVER_WEB )
	{
		return static_cast<WebServerSession*>( pSession ) ;
	}

	return NULL ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event log server relative
//卸载事件日志服务器会话
void CenterServer::EventLogServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_EVENT ) ;
	if( !pSession->GetIsVerification() )	//会话未验证，退出
	{ 
		return ;
	}

	ASSERT( m_pEventLogSession == pSession ) ;
	m_pEventLogSession    =NULL ;
	m_pNewEventLogSession =NULL ;

	pSession->SetIsVerification( false ) ;
}

//注册事件日志服务器会话
CenterServer::ServerRegisterResult CenterServer::EventLogServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_EVENT ) ;

	if( m_pEventLogSession != NULL )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_pEventLogSession    =pSession ;

	// 游戏服务器非空，需要广播
	if( !m_gameSessions.empty() )
	{
		m_pNewEventLogSession =pSession ;
	}

	pSession->SetIsVerification( true ) ;	//会话被验证

	return SERVER_REGISTER_OK ;
}

//检查是否有日志会话需要通知游戏服务器与登录服务器
void CenterServer::CheckNoticeNewEventLogServerToGameAndLogin( void )
{
	// Notice new login after pb::SG2C_GET_EVENT_LOG_SERVER message
	if( m_pNewEventLogSession != NULL )	//有新的事件日志会话
	{
		if( !m_gameSessions.empty() )	//游戏会话不为空
		{
			pb::GetEventLogServersRet retInfo ;

			pb::Address *pAddress =retInfo.mutable_addresses() ;

			const SessionAddress &sAddress =m_pNewEventLogSession->GetAddress() ;
			pAddress->set_ip( sAddress.second ) ;
			pAddress->set_port( sAddress.first ) ;

			NetPack packet( pb::SC2G_ADD_EVENT_LOG_SERVER, retInfo.ByteSize() + 2 ) ;
			packet << retInfo ;
			BroadcastToGameServer( packet ) ;

			packet.SetOpCode( pb::SC2L_ADD_EVENT_LOG_SERVER ) ;
			BroadcastToLoginServer( packet ) ;
		}

		m_pNewEventLogSession =NULL ;
	}
}


