
#include "LoginServer.h"

#include "LoginServerLogicalTaker.h"

#include "LoginServerHttpClients.h"

#include "session/ClientSession.h"
#include "session/GameServerSession.h"

#include "server_client/CenterServerClient.h"
#include "server_client/EventLogServerClient.h"

#include "net_framework/IoServicePool.h"
#include "json/JsonTool.h"
#include "Logger.h"
#include "OS.h"


#include "def/MmoAssert.h"

#include "Opcode.pb.h"
#include "Login.pb.h"
#include "utility/Utility.h"


ConfigServerInfo::ConfigServerInfo( void )
{
	m_region_id =1;
	m_group_id =1;
	m_server_id =1;
	m_to_server_id=0;
	m_test_state =0;
	m_is_new = 0;
	m_is_default = 0;
	m_pGameSession = NULL ;
	m_player_limit = 20000;
	m_delay_limit = 2000;
	m_state = pb::SERVER_STATE_MAINTAIN ;
}

void ConfigServerInfo::UpdateState( void )
{
	//ASSERT( m_pGameSession != NULL ) ;
	if( m_pGameSession == NULL )
	{
		m_state =pb::SERVER_STATE_MAINTAIN ;
		return ;
	}

	if (m_test_state > 0)
	{
		m_state =pb::SERVER_STATE_MAINTAIN ;
		return;
	}

	if( m_pGameSession->GetLogicalUpdateMS() < m_delay_limit)
	{
		if (m_pGameSession->GetPlayerCount() > m_player_limit)
		{
			m_state =pb::SERVER_STATE_BUSY ;
		}
		else if( m_pGameSession->GetLogicalUpdateMS() <= 200 )
		{
			m_state =pb::SERVER_STATE_GOOD ;
		}
		else
		{
			m_state =pb::SERVER_STATE_BUSY ;
		}
	}
	else
	{
		m_state = pb::SERVER_STATE_LIMIT;
	}

}

bool ConfigServerInfo::IsMaintainState()
{
	return ((m_state == pb::SERVER_STATE_MAINTAIN) || (m_state == pb::SERVER_STATE_LIMIT));
}

bool ConfigServerInfo::IsCloseChannel( int channel )
{
   return m_set_close_channel.count(channel) == 1;
}

LoginServer::LoginServer(void)
{
	m_isOpenSdkCheck =false ;
	SetRegionId( 0 ) ; 
	m_configServerMap.clear();
	m_combinServerMap.clear();
	m_defautServer.first = 0;
	m_defautServer.second = 0;
	
	SetLogicalTaker( new LoginServerLogicalTaker() ) ;
}

LoginServer::~LoginServer(void)
{
}



bool LoginServer::InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
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


	m_isOpenSdkCheck =Config().GetInt( "Self.open_sdk_account_check", 0 ) > 0 ;

	return true ;
}


Session* LoginServer::CreateSesion( int type )
{
	Session *pSession =NULL ;
	switch( type )
	{
	case PACKET_TYPE_CLIENT:
		{
			pSession =new ClientSession() ;
		} break ;

	case PACKET_TYPE_SERVER_GAME:
		{
			pSession =new GameServerSession() ;
		} break ;

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


void LoginServer::OnBeforeStart( void )
{
	sHttpClients.Start( sIoPool.GetServerIoService() ) ;

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


void LoginServer::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	MCRequest *pRequest =NULL ;
	while( ( pRequest =sHttps.GetFinishRequest() ) != NULL )
	{
		switch( pRequest->m_opType )
		{
		case HTTP_REQUEST_SDK_ACCOUNT_CHECK:	
			{
				Session *pSession =GetSession( pRequest->m_id ) ;
				if( pSession != NULL )
				{
					NetPack *pPack =new NetPack( pb::CMSG_SDK_SERVER_ACCOUNT_CHECK, sizeof( pRequest->m_response ) ) ;

					*pPack << pRequest->m_response.m_result ;
					*pPack << pRequest->m_response.m_responseCode ;
					*pPack << pRequest->m_response.m_responseReason ;
					*pPack << pRequest->m_response.m_data ;

					if( pRequest->m_response.m_result != CURLE_OK )
					{
						ELOG( "Session (%u) %s => (%d) %s", pRequest->m_id, pRequest->m_url.c_str(), pRequest->m_response.m_result, pRequest->m_response.m_error);
					}

					pSession->PostInputPacket( pPack ) ;
				}
				else
				{
					ELOG( "Finish http request can not find session id: %u. ", pRequest->m_id ) ;
				}

			} break ;

		case HTTP_REQUEST_GET_CONFIG_SERVER_LIST:
			{
				ASSERT( pRequest->m_id == GetRegionId() ) ;

				if( pRequest->m_response.m_result != CURLE_OK )
				{
					ELOG( " Get config server list fail, %s => (%d) %s", pRequest->m_url.c_str(), pRequest->m_response.m_result, pRequest->m_response.m_error);
				}
				else
				{
					InitConfiServerList( pRequest->m_response.m_data ) ;
				}

			} break ;
		case HTTP_REQUEST_GET_VERSION_CHANNEL:
			{
				ASSERT( pRequest->m_id == GetRegionId() ) ;
				if( pRequest->m_response.m_result != CURLE_OK )
				{
					ELOG( " Get version channel fail, %s => (%d) %s", pRequest->m_url.c_str(), pRequest->m_response.m_result, pRequest->m_response.m_error);
				}
				else
				{
					InitVersionChannel( pRequest->m_response.m_data ) ;
				}

			} break ;
		case HTTP_REQUEST_GET_TESTER_ACCOUNT_IDS:
			{
				ASSERT( pRequest->m_id == GetRegionId() ) ;
				if( pRequest->m_response.m_result != CURLE_OK )
				{
					ELOG( " Get tester account ids fail, %s => (%d) %s", pRequest->m_url.c_str(), pRequest->m_response.m_result, pRequest->m_response.m_error);
				}
				else
				{
					InitTesterAccountIds( pRequest->m_response.m_data ) ;
				}

			} break ;
		default:
			MASSERT( false, "MCRequest unknow m_opType " ) ;
		}


		delete pRequest ;
		pRequest =NULL ;
	}
}

void LoginServer::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game server relative

void LoginServer::GameServerUnRegister( Session *pSession )
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

	bool isInConfig = IsInComfigMap(pSession->GetPairId());
	// 添加服务器列表，更新为维护
	ConfigServerInfo &info =GetOrCreateConfigServerInfo( pSession->GetPairId() ) ;
	ASSERT( info.m_pGameSession != NULL && info.m_pGameSession == pSession ) ;
	info.m_region_id       =pSession->GetPairId().second ;
	info.m_server_id       =pSession->GetPairId().first ;
	if (!isInConfig)
	{
		info.m_group_id = info.m_server_id/1000;
	}
	info.m_state           =pb::SERVER_STATE_MAINTAIN ;
	info.m_pGameSession    =NULL ;
}


LoginServer::ServerRegisterResult LoginServer::GameServerRegister( Session *pSession )
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


	bool isInConfig = IsInComfigMap(pSession->GetPairId());
	// 添加服务器列表，更新为开启
	ConfigServerInfo &info =GetOrCreateConfigServerInfo( pSession->GetPairId() ) ;

	GameServerSession *pGameSession =static_cast<GameServerSession*>( pSession ) ;
	info.m_region_id =pSession->GetPairId().second ;
	info.m_server_id =pSession->GetPairId().first ;
	if (!isInConfig)
	{
		info.m_group_id = info.m_server_id/1000;
	}
	info.m_pGameSession =pGameSession ;
	info.UpdateState() ;
	return SERVER_REGISTER_OK ;
}

SessionPairId LoginServer::GetGamePairId( uint64 index )
{
	uint32 serverId = index & 0x00000000ffffffff ;
	uint32 regionId = index >> 32 ;
	
	//NLOG( " %u and %u get value : %llu .", serverId, regionId, index ) ;
	return SessionPairId( serverId, regionId ) ;
}

uint64 LoginServer::GetGameIndex( const SessionPairId &pairId )
{
	uint64 serverId = pairId.first ;
	uint64 regionId = pairId.second ;

	uint64 retValue = serverId | regionId << 32 ;
	//NLOG( " %u and %u get value : %llu .", pairId.first, pairId.second, retValue ) ;
	return retValue ;
}

void LoginServer::GetConfigServerList( void )
{
	MCRequest *pRequest =new MCRequest( sHttpClients.GetWebUrl() + std::string( "/get_table_info.php" ) ) ;

	pRequest->m_id     =GetRegionId() ;
	pRequest->m_opType =HTTP_REQUEST_GET_CONFIG_SERVER_LIST ;


	// 设置参数
	sHttps.AppendParams( pRequest->m_url, "?table_name=", "server_list" ) ;

	sHttps.Request( pRequest ) ;

	sHttps.PostRequest() ;
}

void LoginServer::GetVersionChannel( void )
{
	MCRequest *pRequest =new MCRequest( sHttpClients.GetWebUrl() + std::string( "/get_table_info.php" ) ) ;

	pRequest->m_id     =GetRegionId() ;
	pRequest->m_opType =HTTP_REQUEST_GET_VERSION_CHANNEL;


	// 设置参数
	sHttps.AppendParams( pRequest->m_url, "?table_name=", "version_channel" ) ;

	sHttps.Request( pRequest ) ;

	sHttps.PostRequest() ;
}


void LoginServer::GetTesterAccountIds( void )
{
	MCRequest *pRequest =new MCRequest( sHttpClients.GetWebUrl() + std::string( "/get_table_info.php" ) ) ;

	pRequest->m_id     =GetRegionId() ;
	pRequest->m_opType =HTTP_REQUEST_GET_TESTER_ACCOUNT_IDS;


	// 设置参数
	sHttps.AppendParams( pRequest->m_url, "?table_name=", "tester_account" ) ;

	sHttps.Request( pRequest ) ;

	sHttps.PostRequest() ;
}

void LoginServer::InitConfiServerList( const std::string &data )
{
	JsonTool jt ;
	if( !jt.LoadFromStr( data ) )
	{
		ELOG( "LoginServer::InitConfiServerList() json data load faild ~~~~~!!!!" ) ;
		return ;
	}

	if( !jt.GetIsHadElement() )
	{
		return ;
	}

	JsonTool element =jt.GetElement() ;
	while( element.GetIsOk() )
	{
		uint32 region_id =element.GetInt( "region_id" ) ;
		uint32 group_id = element.GetInt( "group_id" ) ;
		uint32 server_id =element.GetInt( "server_id" ) ;
		uint32 to_server_id =element.GetInt( "to_server_id" ) ;
		uint32  player_limit =element.GetInt( "player_limit" ) ;
		uint32  delay_limit =element.GetInt( "delay_limit" ) ;
		uint32  is_new =element.GetInt( "is_new" ) ;
		uint32  is_default =element.GetInt( "is_default" ) ;
		uint32  test_state =element.GetInt( "test_state" ) ;
		std::string  closeChannel;
		element.GetString( "close_channel", closeChannel) ;

		SessionPairId id( server_id, region_id ) ;

		if( m_configServerMap.find( id ) == m_configServerMap.end() )
		{
			ConfigServerInfo &info =GetOrCreateConfigServerInfo( id ) ;
			info.m_region_id =region_id ;
			info.m_server_id =server_id ;
			info.m_group_id =group_id ;
			info.m_to_server_id =to_server_id ;
			info.m_player_limit =player_limit ;
			info.m_is_new = is_new;
			info.m_is_default = is_default;
			info.m_delay_limit =delay_limit ;
			info.m_test_state =test_state ;
			Utility::SplitStr(closeChannel,info.m_set_close_channel,',');
			info.m_state     =pb::SERVER_STATE_MAINTAIN ;

			if (to_server_id > 0  && region_id > 0)
			{
				GetCombinServerSet(SessionPairId(to_server_id,region_id)).insert(id);
			}
			if (is_default > 0)
			{
				m_defautServer.first = server_id;
				m_defautServer.second = region_id;
			}
		}
		else
		{
			ConfigServerInfo &info =GetOrCreateConfigServerInfo( id ) ;
			info.m_region_id =region_id ;
			info.m_server_id =server_id ;
			info.m_group_id =group_id ;
			info.m_to_server_id =to_server_id ;
			info.m_player_limit =player_limit ;
			info.m_delay_limit =delay_limit ;
			info.m_test_state =test_state ;
			info.m_is_new = is_new;
			info.m_is_default = is_default;
			Utility::SplitStr(closeChannel,info.m_set_close_channel,',');
			info.UpdateState();

			if (to_server_id > 0 && region_id > 0)
			{
				GetCombinServerSet(SessionPairId(to_server_id,region_id)).insert(id);
			}
			if (is_default > 0)
			{
				m_defautServer.first = server_id;
				m_defautServer.second = region_id;
			}
		}

		element =jt.GetElement() ;
	}
}


void LoginServer::InitVersionChannel( const std::string &data )
{
	JsonTool jt ;
	if( !jt.LoadFromStr( data ) )
	{
		ELOG( "LoginServer::InitVersionChannel() json data load faild ~~~~~!!!!" ) ;
		return ;
	}

	if( !jt.GetIsHadElement() )
	{
		return ;
	}

	m_versonTale.clear();

	JsonTool element =jt.GetElement() ;
	while( element.GetIsOk() )
	{
		uint32 vid =element.GetInt( "id" ) ;
		uint32 channelId = element.GetInt( "channelId" ) ;
		uint32 platformId =element.GetInt( "platformId" ) ;
		uint32  curVersionId =element.GetInt( "curVersionId" ) ;
		float  apkSize =element.GetFloat( "versionSize" ) ;
		uint32  isHotUpdate =element.GetInt( "isHotUpdate" ) ;
		uint32  isVerify =element.GetInt( "isVerify" ) ;
		uint32  resourceId =element.GetInt( "resourceId" ) ;
		std::string  noticeAddress;
		element.GetString( "noticeAddress", noticeAddress) ;
		std::string  resourceAddress;
		element.GetString( "resourceAddress", resourceAddress) ;
		std::string  maxVersionAddress;
		element.GetString( "maxVersionAddress", maxVersionAddress) ;

		OrignInfoId id( channelId, platformId ) ;

		if( m_versonTale.find( id ) != m_versonTale.end() )
		{
			NLOG( "LoginServer::InitApkVersionInfo,  version info channel: %u, platform: %u is repeat",channelId, platformId ) ;
			element =jt.GetElement() ;
			continue ;
		}
		ClientVersion &clientVersion    =m_versonTale[ id ] ;
		clientVersion.id                =vid;
		clientVersion.channelId         =channelId ;
		clientVersion.platformId        =platformId ;
		clientVersion.curVersionId      =curVersionId ;
		clientVersion.apkSize           =apkSize;
		clientVersion.isHotUpdate       =isHotUpdate;
		clientVersion.isVerify			=isVerify;
		clientVersion.maxVersionAddress =maxVersionAddress;
		clientVersion.resourceId      =resourceId;
		clientVersion.noticeAddress   =noticeAddress ;
		clientVersion.resourceAddress =resourceAddress ;
		element =jt.GetElement() ;
	}
}

void LoginServer::InitTesterAccountIds( const std::string &data )
{
	JsonTool jt ;
	if( !jt.LoadFromStr( data ) )
	{
		ELOG( "LoginServer::InitTesterAccountIds() json data load faild ~~~~~!!!!" ) ;
		return ;
	}
	if( !jt.GetIsHadElement() )
	{
		return ;
	}
	m_testerAccountIds.clear();
	JsonTool element =jt.GetElement() ;
	while( element.GetIsOk() )
	{
		uint64 accountId =element.GetInt64( "id") ;
		m_testerAccountIds.insert(accountId);
		element =jt.GetElement() ;
	}
}

bool LoginServer::IsInComfigMap( const SessionPairId &id )
{
	return m_configServerMap.find(id) != m_configServerMap.end();
}

bool LoginServer::IsCloseChannel( const SessionPairId &id, int channelId )
{
	ConfigServerMap::iterator iterC = m_configServerMap.find(id);
	if (iterC != m_configServerMap.end())
	{
		ConfigServerInfo& sInfo = iterC->second;
		return sInfo.IsCloseChannel(channelId);
	}
	return false;
}

bool LoginServer::IsTesterAccountId( uint64 accountId )
{
   return m_testerAccountIds.count(accountId) == 1;
}

bool LoginServer::IsTesterServer( const SessionPairId &id )
{
	ConfigServerMap::iterator iterC = m_configServerMap.find(id);
	if (iterC != m_configServerMap.end())
	{
		ConfigServerInfo& sInfo = iterC->second;
		return sInfo.m_test_state > 0;
	}
	return false;
}

bool LoginServer::IfIsCombinServerGetOther( SessionPairId &id )
{
	ConfigServerMap::iterator iterC = m_configServerMap.find(id);
	if (iterC != m_configServerMap.end())
	{
		ConfigServerInfo& sInfo = iterC->second;
		if(sInfo.m_to_server_id > 0)
		{
			id.first = sInfo.m_to_server_id;
			return true;
		}
	}
	return false;
}

bool LoginServer::AddOneEventLogServer( const std::string &ip, const std::string &port )
{
	if( sEventLogClient.IsSettingAddress() )
	{
		return false ;
	}

	sEventLogClient.SetDstInfo( ip, port ) ;
	sEventLogClient.SetConnectNow() ;
	return true ;

}


const ClientVersion* LoginServer::GetVersions( uint32 channelId, uint32 platformId )
{
	const VersionInfoTable &table =GetVersonInfoTable() ;

	OrignInfoId orignId( channelId, platformId ) ;

	VersionInfoTableCItr itr =table.find( orignId ) ;
	if( itr == table.end() )
	{
		return false ;
	}
	return &itr->second;
}

bool LoginServer::CanConnectByThisVersion( uint32 channelId, uint32 platformId, uint32 versionId )
{
	const VersionInfoTable &table =GetVersonInfoTable() ;

	OrignInfoId orignId( channelId, platformId ) ;

	VersionInfoTableCItr itr =table.find( orignId ) ;
	if( itr == table.end() )
	{
		return false ;
	}
	return itr->second.IsVersionMatch( versionId ) ;
}