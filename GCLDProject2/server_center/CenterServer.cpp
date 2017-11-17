
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
	m_pEventLogSession       =NULL ;	//�¼���־�Ự
	m_pNewEventLogSession    =NULL ;	//��ʱ�Ự

	// ���ķ�����Ҳ��ʱ�����߼��߳�
	//SetLogicalTaker( new CenterServerLogicalTaker() ) ;
}

CenterServer::~CenterServer(void)
{
}


//��ʼ�������ļ�
bool CenterServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
{
	//�����ʼ�������ļ�
	if( !Server::InitGameConf( filePath, strConfigFile, strDefaultConfigFile ) )
	{
		return false ;
	}

	//���ò��ID
	SetRegionId( Config().GetInt( "Self.region_id", 95560 ) ) ;
	ASSERT( GetRegionId() != 0 ) ;

	//���ķ�����û���߼��߳�
	if( GetLogicalTaker() )
	{
		GetLogicalTaker()->SetThreadUpdateLimitTimeMS( Config().GetInt( "Self.thread_update_limit_time_ms", 100 ) ) ;
		GetLogicalTaker()->SetNotRecvPackLimitTimeMS( Config().GetInt( "Self.not_recv_packet_limit_time_ms", 3000 ) ) ;
	}

	return true ;
}

//�����Ự
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
//ж����Ϸ�������Ự
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

//ע����Ϸ�������Ự
CenterServer::ServerRegisterResult CenterServer::GameServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;	//�Ự����Ϊ��Ϸ�������Ự

	if( m_gameSessionIds.find( pSession->GetPairId() ) != m_gameSessionIds.end() )	//�Ự��serverid&regionid�Ѿ���ע����
	{
		return SERVER_REGISTER_FAIL_ID ;
	}

	if( m_gameSessionAddresses.find( pSession->GetAddress() ) != m_gameSessionAddresses.end() )	//�Ự��IP��port�Ѿ���ע����
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	//������Ϸ�Ự�б�һ���Ự
	m_gameSessionIds.insert( pSession->GetPairId() ) ;
	m_gameSessionAddresses.insert( pSession->GetAddress() ) ;

	bool isInsert =m_gameSessions.insert( std::make_pair( pSession->GetPairId().first, pSession ) ).second ;
	ASSERT( isInsert ) ;
	if( isInsert )
	{
		pSession->SetIsVerification( true ) ;	//ע��ɹ��������Ự����Ϊ��ʶ��
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
//ж�ص�¼�������Ự
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

//ע���¼�������Ի�
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

//����µ�¼����Ϣת������Ϸ������
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
	if( !m_newLoginSessions.empty() )	//�µ�¼�Ự��Ϊ��
	{
		if( !m_gameSessions.empty() )	//��Ϸ�������Ự����
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
			BroadcastToGameServer( packet ) ;	//��������Ϸ�������Ự�㲥��Ϣ
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
//���web�������Ự
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
//ж���¼���־�������Ự
void CenterServer::EventLogServerUnRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_EVENT ) ;
	if( !pSession->GetIsVerification() )	//�Ựδ��֤���˳�
	{ 
		return ;
	}

	ASSERT( m_pEventLogSession == pSession ) ;
	m_pEventLogSession    =NULL ;
	m_pNewEventLogSession =NULL ;

	pSession->SetIsVerification( false ) ;
}

//ע���¼���־�������Ự
CenterServer::ServerRegisterResult CenterServer::EventLogServerRegister( Session *pSession )
{
	ASSERT( pSession->GetSessionType() == PACKET_TYPE_SERVER_EVENT ) ;

	if( m_pEventLogSession != NULL )
	{
		return SERVER_REGISTER_FAIL_ADDRESS ;
	}

	m_pEventLogSession    =pSession ;

	// ��Ϸ�������ǿգ���Ҫ�㲥
	if( !m_gameSessions.empty() )
	{
		m_pNewEventLogSession =pSession ;
	}

	pSession->SetIsVerification( true ) ;	//�Ự����֤

	return SERVER_REGISTER_OK ;
}

//����Ƿ�����־�Ự��Ҫ֪ͨ��Ϸ���������¼������
void CenterServer::CheckNoticeNewEventLogServerToGameAndLogin( void )
{
	// Notice new login after pb::SG2C_GET_EVENT_LOG_SERVER message
	if( m_pNewEventLogSession != NULL )	//���µ��¼���־�Ự
	{
		if( !m_gameSessions.empty() )	//��Ϸ�Ự��Ϊ��
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


