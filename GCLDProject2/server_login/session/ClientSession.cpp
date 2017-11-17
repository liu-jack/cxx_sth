

#include "ClientSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "encrypt/Md5Tool.h"
#include "encrypt/SHA1Tool.h"
#include "memory_buffer/NetPack.h"

#include "GameServerSession.h"

#include "../LoginServer.h"
#include "../LoginServerHttpClients.h"
#include "../data/Account.h"
#include "../data/LoginDataMgr.h"
#include "../server_client/EventLogServerClient.h"

// Tips: 这个头文件要放在 LoginServerHttpClients.h 的后面，不然会有冲突，显示网络层接口问题，因为 LoginServerHttpClients.h 包含了asio.hpp 
#include "json/JsonTool.h"

#include "Enum.pb.h"
#include "Opcode.pb.h"
#include "Login.pb.h"
#include "InterServer.pb.h"
#include "Web.pb.h"


ClientSession::HandlerArray ClientSession::s_handlers =RegisteHandlers() ;


ClientSession::HandlerArray ClientSession::RegisteHandlers( void )
{
	HandlerArray handlers( pb::CSMSG_COUNT ) ;

	handlers[ pb::CMSG_PING ]                     =&ClientSession::HandlePing ;

	handlers[ pb::CMSG_VERSION_CHECK ]            =&ClientSession::HandleVersionCheck ;

	handlers[ pb::CMSG_VSTAR_GAME_ACCOUNT_CHECK ] =&ClientSession::HandleVstarGameAccountCheck ;
	handlers[ pb::CMSG_SDK_SERVER_ACCOUNT_CHECK ] =&ClientSession::HandleSdkServerAccountCheck ;
	
	handlers[ pb::CMSG_ACCOUNT_CHECK ]            =&ClientSession::HandleAccountCheck ;
	handlers[ pb::CMSG_REGISTER_ACCOUNT ]         =&ClientSession::HandleRegisterAccount ;
	handlers[ pb::CMSG_CHANGE_ACCOUNT_INFO ]      =&ClientSession::HandleChaneAccountInfo ;
	handlers[ pb::CMSG_GET_SERVER_LIST ]          =&ClientSession::HandleGetServerList ;
	handlers[ pb::CMSG_SELECT_SERVER ]            =&ClientSession::HandleSelectServer ;

	return handlers ;
}

//uint64         "l"
//int64          "b"
//uint32         "u"
//int32          "i"
//const char*    "s"
static void SendEventInfo( uint32 eventId, ClientSession &clientSession, int paramCount, ... )
{
	pb::Server_Event serverEvent ;

	const AccountInfo *pAccount =clientSession.GetAccount() ;

	serverEvent.set_event_id( eventId ) ;
	serverEvent.set_account_id( pAccount != NULL ? (uint64)pAccount->id : 0 ) ;

	switch(eventId)
	{
	case pb::EVENT_USER_ACCT_LOGIN_START:
		{
			//EventLogServerClient::AddUInt32Value( serverEvent, pAccount != NULL ? pAccount->channelId : 0 ) ;
		}
		break;
	default:
		{
			EventLogServerClient::AddStringValue( serverEvent, clientSession.GetAddress().second ) ;
			EventLogServerClient::AddStringValue( serverEvent, clientSession.GetAddress().first ) ;
			EventLogServerClient::AddStringValue( serverEvent, clientSession.GetDyDeviceId() ) ;
			EventLogServerClient::AddUInt32Value( serverEvent, pAccount != NULL ? pAccount->channelId : 0 ) ;
		}
		break;
	}

	va_list vlist;
	va_start(vlist, paramCount);

	sEventLogClient.SendEvent( serverEvent, paramCount, vlist, true);

	va_end(vlist);
}

ClientSession::ClientSession( void )
{
	m_isPassSdkVerfy = !sLoginSvr.IsOpenSdkCheck() ;
	m_pAccount       = NULL ;
}

ClientSession::~ClientSession( void )
{
	if( GetAccount() != NULL )
	{
		sDataMgr.ReleaseAccount( GetAccount() ) ;
		SetAccount( NULL ) ;
	}
}


void ClientSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool ClientSession::HandleNetPack( NetPack &pack )
{
	// All server packet !

	int opCode               =pack.GetOpcode() ;

	HandlerArray &funTable   =s_handlers ;

	ASSERT( static_cast<size_t>( opCode ) < funTable.size() ) ;
	NetPackHandler packetFun = funTable[ opCode ] ;

	if( packetFun )
	{
		(this->*packetFun)( pack ) ;
		return true ;
	}

	return false ;
}

void ClientSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::SMSG_PONG ) ;
}

void ClientSession::HandleVersionCheck( NetPack &packet )
{
	pb::CVersionCheck info ;
	packet >> info ;

	// 每次登录的设备id，未必和注册时一样，给服务器事件使用
	m_dyDeviceId =info.device_id() ;

	pb::SVersionCheck retInfo ;

	const ClientVersion *pClientVserion = sLoginSvr.GetVersions( info.channel_id(), info.platform_id() ) ;
	if( pClientVserion == NULL )
	{
		retInfo.set_result( pb::VERSION_CHECK_CAN_NOT_FIND_PLATFORM ) ;
		Send( pb::SMSG_VERSION_CHECK, retInfo ) ;
		return ;
	}

	if (pClientVserion->isVerify > 0)
	{
		if(!pClientVserion->IsVersionMatch( info.version_id()) )
		{
			retInfo.set_result( pb::VERSION_CHECK_INVALID_VERSION ) ;
			retInfo.set_apk_size(pClientVserion->apkSize);
			retInfo.set_is_hot_update(pClientVserion->isHotUpdate);
			retInfo.set_newest_version_address( pClientVserion->maxVersionAddress ) ;
		}
		else
		{
			bool isMachRes =pClientVserion->IsResourceMatch(info.resource_id()) ;

			if(!isMachRes )
			{
				if (info.resource_id() == 0)
				{
					retInfo.set_result( pb::VERSION_CHECK_RESOURCE_NOT_SET ) ;
				}
				else
				{
					retInfo.set_apk_size(pClientVserion->apkSize);
					retInfo.set_result( pb::VERSION_CHECK_INVALID_RESOURCE_VESION ) ;
					retInfo.set_resource_id( pClientVserion->resourceId  ) ;
					retInfo.set_resource_address( pClientVserion->resourceAddress ) ;
					retInfo.set_notice_address( pClientVserion->noticeAddress ) ;
				}
			}
			else
			{
				retInfo.set_result( pb::VERSION_CHECK_NEWEST_VERION_SUCCESS ) ;  
				retInfo.set_resource_id( pClientVserion->resourceId  ) ;
				retInfo.set_resource_address( pClientVserion->resourceAddress ) ;
				retInfo.set_notice_address( pClientVserion->noticeAddress ) ;
			}
		}
	}
	else
	{
		retInfo.set_result( pb::VERSION_CHECK_NEWEST_VERION_SUCCESS ) ;  
		retInfo.set_resource_id( info.resource_id()  ) ;
		retInfo.set_is_hot_update(0);
		retInfo.set_notice_address( "yxgm.caohua.com/web/client_announcement/announcement.php" ) ;
	}
	if( retInfo.result() != pb::VERSION_CHECK_NEWEST_VERION_SUCCESS )
	{
		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 3, "u", pb::EVENT_VERSION_CHECK_FAIL, "s","EVENT_VERSION_CHECK_FAIL","u", retInfo.result() ) ;
	}

	Send( pb::SMSG_VERSION_CHECK, retInfo ) ;
}

void ClientSession::HandleVstarGameAccountCheck( NetPack &packet )
{
	m_isPassSdkVerfy = false;
	pb::CVstarGameAccountCheck info ;
	packet >> info ;
	m_sdkCheckAccountInfo.ReadFrom( info.register_info() ) ;

	//std::ostringstream streamtemp;
	//info.SerializeToOstream(&streamtemp);
	//std::string varifyinfo = streamtemp.str();
	//NLOG( " VstarGameAccountCheck varifyinfo:%s", varifyinfo.c_str());

	uint32 channelId = m_sdkCheckAccountInfo.channelId;
	if (m_isPassSdkVerfy || channelId ==  (uint32)pb::SDK_NONE || channelId ==  (uint32)pb::SDK_ALL || channelId == (uint32)pb::SDK_HUAWEI)
	{
		pb::SVstarGameAccountCheck retInfo ;
		retInfo.set_status( true ) ;
		retInfo.set_error_code( 0) ;
		retInfo.set_error_msg( "success" ) ;
		retInfo.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
		Send( pb::SMSG_VSTAR_GAME_ACCOUNT_CHECK, retInfo ) ;
		m_isPassSdkVerfy =true ;
		return;
	}
	string postData,requestUrl;
	if (SdkVerifyFromWeb(info))
	{
		return;
	}
	else if(GetRequestPostData(requestUrl,postData,info))
	{
		MCRequest *pRequest =new MCRequest( requestUrl) ;
		pRequest->m_id     = (int)GetSessionId() ;
		pRequest->m_opType =HTTP_REQUEST_SDK_ACCOUNT_CHECK ;
		pRequest->m_data.assign(postData);
		sHttps.Request( pRequest );
		sHttps.PostRequest() ;
		//NLOG( " VstarGameAccountCheck post url:%s, data:%s", pRequest->m_url.c_str(), pRequest->m_data.c_str());
	}
	else
	{
		pb::SVstarGameAccountCheck retInfo ;
		retInfo.set_status( false ) ;
		retInfo.set_error_code( 0) ;
		retInfo.set_error_msg( "not support this sdk !" ) ;
		Send( pb::SMSG_VSTAR_GAME_ACCOUNT_CHECK, retInfo ) ;
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_REGISTER_ACCOUTN_IS_EMPTY,"s","EVENT_SELF_ACCOUNT_REGISTER_ACCOUTN_IS_EMPTY" ) ;
	}

}
bool ClientSession::SdkVerifyFromWeb(const pb::CVstarGameAccountCheck& info)
{
	uint32 channelId = info.register_info().channel_id();
	uint32 platformId = info.register_info().platform_id();
	if (platformId == (uint32)pb::IOS )
	{
		return false;
	}
	if (channelId == (uint32)pb::SDK_JINLI)
	{
		MCRequest *pRequest =new MCRequest( sHttpClients.GetWebUrl() + std::string( "/sdkjinliverify.php" ) ) ;
		pRequest->m_id     =(int)GetSessionId() ;
		pRequest->m_opType =HTTP_REQUEST_SDK_ACCOUNT_CHECK ;
		sHttps.AppendParams( pRequest->m_url, "?playId=", info.sdk_user_id());
		sHttps.AppendParams( pRequest->m_url, "&token=", info.token());
		sHttps.Request( pRequest ) ;
		sHttps.PostRequest() ;
		//NLOG( " VstarGameAccountCheck requst SDK_JINLI url:%s", pRequest->m_url.c_str());
		return true;
	}
	else if (channelId == (uint32)pb::SDK_XIAOMI)
	{
		m_sdkCheckAccountInfo.account = info.sdk_user_id();
		MCRequest *pRequest =new MCRequest( sHttpClients.GetWebUrl() + std::string( "/sdkxiaomiverify.php" ) ) ;
		pRequest->m_id     =(int)GetSessionId() ;
		pRequest->m_opType =HTTP_REQUEST_SDK_ACCOUNT_CHECK ;
		sHttps.AppendParams( pRequest->m_url, "?userId=", info.sdk_user_id());
		sHttps.AppendParams( pRequest->m_url, "&session=", info.token());
		sHttps.Request( pRequest ) ;
		sHttps.PostRequest() ;
		//NLOG( " VstarGameAccountCheck requst SDK_XIAOMI url:%s", pRequest->m_url.c_str());
		return true;
	}
	else if (channelId == (uint32)pb::SDK_OPPO)
	{
		MCRequest *pRequest =new MCRequest( sHttpClients.GetWebUrl() + std::string( "/sdkverifyoppo.php" ) ) ;
		pRequest->m_id     =(int)GetSessionId() ;
		pRequest->m_opType =HTTP_REQUEST_SDK_ACCOUNT_CHECK ;
		sHttps.AppendParams( pRequest->m_url, "?ssoid=", info.sdk_user_id());
		sHttps.AppendParams( pRequest->m_url, "&token=", info.token());
		sHttps.Request( pRequest ) ;
		sHttps.PostRequest() ;
		//NLOG( " VstarGameAccountCheck SDK_OPPO requst url:%s", pRequest->m_url.c_str());
		return true;
	}
	return false;
}

bool ClientSession::GetRequestPostData( std::string &reqUrl,std::string &psData, const pb::CVstarGameAccountCheck& info )
{
	uint32 channelId = info.register_info().channel_id();
	uint32 platformId = info.register_info().platform_id();
	if (platformId == (uint32)pb::IOS )
	{
		if(channelId ==  (uint32)pb::SDK_CAOHUA)//草花
		{
			if (info.sdk_user_id().length() == 0 || info.token().length() == 0) {
				return false; //参数错误
			}
			reqUrl = "http://passport.ios.caohua.com/api/verifyToken";
			char tmp[25] ="" ;
			sprintf( tmp, "%llu", sOS.TimeSeconds() ) ;
			std::string postData;
			sHttps.AppendParams( postData, "appid=", "1000112" ) ;
			sHttps.AppendParams( postData, "&times=",    tmp ) ;
			sHttps.AppendParams( postData, "&token=",   info.token() ) ;
			sHttps.AppendParams( postData, "&userid=",  info.sdk_user_id() ) ;
			// md5 计算
			std::string md5Key,strArgs;
			sHttps.AppendParams( strArgs, postData, "b860a06e00b2e915479b5e4efdf101c4") ;
			Md5Tool::CalculateKey( md5Key, strArgs ) ;
			std::transform( md5Key.begin(), md5Key.end(), md5Key.begin(), ToUpper() );
			sHttps.AppendParams( postData, "&sign=",    md5Key) ;
			psData.append(postData);
			return true;
		}
	}
	else if (platformId == (uint32)pb::ANDROID )
	{

		if(channelId ==  (uint32)pb::SDK_CAOHUA)//草花
		{
			if (info.sdk_user_id().length() == 0 || info.token().length() == 0) {
				return false; //参数错误
			}

			reqUrl = "http://passport.sdk.caohua.com/api/verifyToken";
			char tmp[25] ="" ;
			sprintf( tmp, "%llu", sOS.TimeSeconds() ) ;
			std::string postData;
			sHttps.AppendParams( postData, "appid=", "1000100" ) ;
			sHttps.AppendParams( postData, "&times=",    tmp ) ;
			sHttps.AppendParams( postData, "&token=",   info.token() ) ;
			sHttps.AppendParams( postData, "&userid=",  info.sdk_user_id() ) ;
			// md5 计算
			std::string md5Key,strArgs;
			sHttps.AppendParams( strArgs, postData, "5be6205511a1038b90d137b9b3c29470") ;
			Md5Tool::CalculateKey( md5Key, strArgs ) ;
			std::transform( md5Key.begin(), md5Key.end(), md5Key.begin(), ToUpper() );
			sHttps.AppendParams( postData, "&sign=",    md5Key) ;
			psData.append(postData);
			return true;
		}
		else if(channelId == (uint32)pb::SDK_UC)  //uc
		{
			if (info.token().length() == 0) {
				return false;
			}
			reqUrl = "http://sdk.9game.cn/cp/account.verifySession";
			JsonTool jt,clietc,gamec,datac;
			jt.WriteUint64("id",sOS.TimeSeconds());
			jt.WriteString("service","account.verifySession");
			jt.WriteString("encrypt","md5");
			string strArgs;
			sHttps.AppendParams( strArgs, "sid=",  info.token()) ;
			strArgs.append("b2b859c798ae6812fca1b03b0e2bd7bd");
			std::string md5Key;
			Md5Tool::CalculateKey( md5Key, strArgs );
			std::transform( md5Key.begin(), md5Key.end(), md5Key.begin(), ToLower() );
			jt.WriteString("sign",md5Key);
			gamec.WriteInt("gameId",816845);
			jt.PutChild("game",gamec);
			datac.WriteString("sid",info.token());
			jt.PutChild("data",datac);
			clietc.WriteString("ex","language:c++|version:1.3.0");
			jt.PutChild("client",clietc);
			jt.WriteToSting(psData);
			return true;
		}
		else if(channelId == (uint32)pb::SDK_HUAWEI)
		{
		}
		else if(channelId == (uint32)pb::SDK_JINLI)
		{
		
		}
		else if(channelId == (uint32)pb::SDK_360)
		{
			if (info.token().length() == 0) {
				return false;
			}
			reqUrl = "https://openapi.360.cn/user/me.json";
			sHttps.AppendParams( psData, "access_token=", info.token()) ;
			return true;
		}
	}
	return false;
}

bool ClientSession::PasserResponseData( const MCResopnse &psData, pb::SVstarGameAccountCheck& info )
{
	//NLOG( "VstarGameAccountCheck  PasserResponseData:%s", psData.m_data.c_str());

	JsonTool jt;
	if (!jt.LoadFromStr( psData.m_data ))
	{
		return false;
	}
	info.set_error_msg( "account empty!" ) ;
	uint32 channelId = m_sdkCheckAccountInfo.channelId;
	uint32 platformId = m_sdkCheckAccountInfo.platformId;
	if (platformId == (uint32)pb::IOS )
	{
		if(channelId ==  (uint32)pb::SDK_CAOHUA)//草花
		{
			int curCode = jt.GetInt("code");
			info.set_status( curCode == 200) ;
			info.set_sdk_user_id( m_sdkCheckAccountInfo.account ) ;
			if( info.status() ) // 成功
			{
				m_isPassSdkVerfy =true ;
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
			}
			else
			{
				info.set_error_code( curCode) ;
				std::string strValue ;
				jt.GetString( "msg", strValue ) ;
				info.set_error_msg( strValue ) ;
			}
		}
	}
	else if (platformId == (uint32)pb::ANDROID )
	{
		if(channelId ==  (uint32)pb::SDK_CAOHUA)//草花
		{
			int curCode = jt.GetInt("code");
			info.set_status( curCode == 200) ;
			/*	std::string strValue ;
			jt.GetString( "data.userid", strValue ) ;*/
			info.set_sdk_user_id( m_sdkCheckAccountInfo.account ) ;

			if( info.status() ) // 成功
			{
				m_isPassSdkVerfy =true ;
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
			}
			else
			{
				info.set_error_code( curCode) ;
				std::string strValue ;
				jt.GetString( "msg", strValue ) ;
				info.set_error_msg( strValue ) ;
			}
		}
		else if(channelId == (uint32)pb::SDK_UC)  //uc
		{
			JsonTool rspState(jt.GetChild("state"));
			if (!rspState.GetIsOk())
			{
				return false;
			}

			JsonTool resData(jt.GetChild("data"));
			if (!resData.GetIsOk())
			{
				return false;
			}

			resData.GetString("accountId",m_sdkCheckAccountInfo.account);
			resData.GetString("nickName",m_sdkCheckAccountInfo.password);
			int curCode = rspState.GetInt("code");
			info.set_status( curCode == 1) ;
			info.set_sdk_user_id( m_sdkCheckAccountInfo.account) ;
			if( info.status() ) // 成功
			{
				m_isPassSdkVerfy =true ;
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;

				/*string jsionstr;
				jt.WriteToSting(jsionstr);*/

			}
			else
			{
				info.set_error_code( curCode) ;
				std::string strValue ;
				rspState.GetString( "msg", strValue ) ;
				info.set_error_msg( strValue ) ;
			}
		}
		else if(channelId == (uint32)pb::SDK_HUAWEI)
		{
		}
		else if(channelId == (uint32)pb::SDK_JINLI)
		{
			int curCode = jt.GetInt("status");
			info.set_sdk_user_id( m_sdkCheckAccountInfo.account) ;
			info.set_status(curCode == 0) ;
			if (info.status())
			{
				m_isPassSdkVerfy =true ;
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
			}
			else
			{
				info.set_error_code( curCode) ;
				info.set_error_msg( "verify fail!" ) ;
			}
		}
		else if(channelId == (uint32)pb::SDK_360)
		{
			std::string curId;
			jt.GetString("id",curId);
			if (strcmp(curId.c_str(),"0") != 0)
			{
				jt.GetString("name",m_sdkCheckAccountInfo.account);
				info.set_sdk_user_id( curId) ;
				m_isPassSdkVerfy =true ;
				info.set_status(true);
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
			}
			else
			{
				int curCode = jt.GetInt("error_code");
				info.set_status(false) ;
				info.set_error_code( curCode) ;
				std::string strValue ;
				jt.GetString( "error", strValue ) ;
				info.set_error_msg( strValue ) ;
			}
		}
		else if(channelId == (uint32)pb::SDK_XIAOMI)
		{
			int curId = jt.GetInt("errcode");
			if (curId == 200)
			{
				info.set_sdk_user_id( m_sdkCheckAccountInfo.account) ;
				m_isPassSdkVerfy =true ;
				info.set_status(true);
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
			}
			else
			{
				info.set_status(false) ;
				info.set_error_code( curId) ;
				std::string strValue ;
				jt.GetString( "errMsg", strValue ) ;
				info.set_error_msg( strValue ) ;
			}
		}
		else if(channelId == (uint32)pb::SDK_OPPO)
		{
			int curId = jt.GetInt("resultCode");
			if (curId == 200)
			{
				jt.GetString("ssoid",m_sdkCheckAccountInfo.account);
				jt.GetString("userName",m_sdkCheckAccountInfo.password);
				info.set_sdk_user_id( m_sdkCheckAccountInfo.account) ;
				m_isPassSdkVerfy =true ;
				info.set_status(true);
				info.set_account_id( SdkAccountInfoInit( m_sdkCheckAccountInfo ) ) ;
			}
			else
			{
				info.set_status(false) ;
				info.set_error_code( curId) ;
				std::string strValue ;
				jt.GetString( "resultMsg", strValue ) ;
				info.set_error_msg( strValue ) ;
			}
		}
	}
	//std::ostringstream streamtemp;
	//info.SerializeToOstream(&streamtemp);
	//std::string resultpaser = streamtemp.str();
	//NLOG( "VstarGameAccountCheck resultpaser :%s", resultpaser.c_str());

	if (info.account_id() == 0)
	{
		m_isPassSdkVerfy = false;
		info.set_status(false) ;
		info.set_error_code( 201) ;
		info.set_error_msg( "account empty!" ) ;
	}

	//if (info.status())
	//{
	//	NLOG( "VstarGameAccountCheck status:true");
	//}
	//else
	//{
	//	NLOG( "VstarGameAccountCheck status:false,errcode:%d,msg:%s",info.error_code(), info.error_msg().c_str());
	//}

	if (!m_isPassSdkVerfy)
	{
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_REGISTER_ACCOUTN_IS_EMPTY,  "s",info.error_msg().c_str()) ;
		return false;
	}
	return true;
}

void ClientSession::HandleSdkServerAccountCheck( NetPack &packet )
{
	MCResopnse response ;
	packet >> response.m_result ;
	packet >> response.m_responseCode ;
	packet >> response.m_responseReason ;
	packet >> response.m_data ;

	
	pb::SVstarGameAccountCheck retInfo ;
	retInfo.set_account_id(0);
	// for test
	//response.m_data ="{ \"status\":1,\"data\":{\"userid\":\"153963\"} }" ;
	//response.m_data ="{ \"status\":0, \"errorCode\":103, \"msg\":\"User not found or invalid token\"}" ;
	if( response.m_responseCode == 200 && response.m_result == CURLE_OK )
	{
	     if (!PasserResponseData(response,retInfo))
	     {
			 retInfo.set_status( false ) ;
			 retInfo.set_error_code( response.m_responseCode ) ;
			 retInfo.set_error_msg( response.m_responseReason ) ;
	     }
	}
	else
	{
		retInfo.set_status( false ) ;
		retInfo.set_error_code( response.m_responseCode ) ;
		retInfo.set_error_msg( response.m_responseReason ) ;
	}

	if( retInfo.status() == false )
	{
		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 4, "u", pb::EVENT_SDK_ACCOUNT_CHECK_FAIL, "s","EVENT_SDK_ACCOUNT_CHECK_FAIL", "u", retInfo.error_code(), "s",  retInfo.error_msg().c_str() ) ;
	}

	Send( pb::SMSG_VSTAR_GAME_ACCOUNT_CHECK, retInfo ) ;
}

uint64 ClientSession::SdkAccountInfoInit( AccountInfo &accountInfo )
{
	AccountInfo *pAccount = NULL;
	if (accountInfo.id > 0)
	{
		pAccount =sDataMgr.GetAccount( accountInfo.id) ;
	}
	else
	{
		pAccount =sDataMgr.GetAccount( accountInfo.channelId,accountInfo.platformId,accountInfo.account);
	}
	if( pAccount != NULL )
	{
		// Save account info
		SetAccount( pAccount ) ;

		// Record last ip and last login time
		pAccount->lastIp        =GetAddress().second ;
		pAccount->lastLoginTime =sOS.TimeSeconds();
		sDataMgr.SaveModifyAccount(pAccount);
	}
	else
	{
		if (accountInfo.account.empty())
		{
			return 0;
		}
		accountInfo.regTime       =sOS.TimeSeconds() ;
		accountInfo.lastLoginTime =accountInfo.regTime;
		accountInfo.lastIp        =GetAddress().second ;
		// Insert into table and save to database .
		pAccount =sDataMgr.AddNewAccount( accountInfo ) ;
		ASSERT( pAccount ) ;
		if( pAccount == NULL )
		{
			return 0 ;
		}

		// Save account info
		SetAccount( pAccount ) ;
		sDataMgr.AddAcciontInfoUse( pAccount->id ) ;

		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_START, *this, 4, "u", m_sdkCheckAccountInfo.channelId, "u", m_sdkCheckAccountInfo.platformId,"s", m_sdkCheckAccountInfo.account.c_str(), "s", GetDyDeviceId().c_str()) ;
	}

	SetIsVerification( true ) ;
	sDataMgr.AddAcciontInfoUse( pAccount->id ) ;

	return pAccount->id ;
}


void ClientSession::HandleAccountCheck( NetPack &packet )
{
	if( !m_isPassSdkVerfy )
	{
		ELOG( "ClientSession::HandleAccountCheck() Session(%llu) not passed sdk verfy cannot take this....", GetSessionId() ) ;
		return ;
	}

	pb::CAccountCheck info ;
	packet >> info ;

	pb::SAccountCheck retInfo ;

	AccountInfo *pAccount =NULL ;
	switch( info.method() )
	{
	case pb::ACCOUNT_CHECK_ACCOUNT:
		{
			pAccount =sDataMgr.GetAccount(info.channel_id(),info.platform_id(),info.account() ) ;
			if( pAccount == NULL )
			{
				retInfo.set_result( pb::ACCOUNT_CHECK_NAME_ERROR ) ;
				Send( pb::SMSG_ACCOUNT_CHECK, retInfo ) ;
			}
		} break ;

	default:
		{
			ASSERT( false ) ;

			retInfo.set_result( pb::ACCOUNT_CHECK_METHOD_UNKNOW ) ;
			Send( pb::SMSG_ACCOUNT_CHECK, retInfo ) ;
		}
	}

	if( pAccount == NULL )
	{
		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_CHECK_NOT_ACCOUNT_INFO,  "s","EVENT_SELF_ACCOUNT_REGISTER_ACCOUTN_IS_EMPTY" ) ;
		return ;
	}

	if (pAccount->IsForbiden())
	{
		retInfo.set_result( pb::ACCOUNT_CHECK_FORBIDEN ) ;
		Send( pb::SMSG_ACCOUNT_CHECK, retInfo ) ;
		return;
	}

	if( pAccount->password != info.password() )
	{
		retInfo.set_result( pb::ACCOUNT_CHECK_PWD_ERROR ) ;
		Send( pb::SMSG_ACCOUNT_CHECK, retInfo ) ;

		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_CHECK_PASSWORD_ERROR,  "s","EVENT_SELF_ACCOUNT_CHECK_PASSWORD_ERROR" ) ;
		return ;
	}

	retInfo.set_result( pb::ACCOUNT_CHECK_SUCCESS ) ;
	retInfo.set_account_id( pAccount->id ) ;
	retInfo.set_account_type( static_cast<pb::ACCOUNT_TYPE>( pAccount->accountType ) ) ;

	Send( pb::SMSG_ACCOUNT_CHECK, retInfo ) ;

	// Save account info
	SetAccount( pAccount ) ;
	sDataMgr.AddAcciontInfoUse( pAccount->id ) ;

	SetIsVerification( true ) ;

	// Record last ip and last login time
	pAccount->lastLoginTime       =sOS.TimeSeconds() ;
	pAccount->lastIp        =GetAddress().second ;
	sDataMgr.SaveModifyAccount(pAccount);
}

void ClientSession::HandleRegisterAccount( NetPack &packet )
{
	if( !m_isPassSdkVerfy )
	{
		ELOG( "ClientSession::HandleAccountCheck() Session(%llu) not passed sdk verfy cannot take this....", GetSessionId() ) ;
		return ;
	}

	pb::CRegisterAccount info ;
	packet >> info ;

	pb::SRegisterAccount retInfo ;

	switch( info.type() )
	{
	case pb::REGISTER_ACCOUNT_ACCOUNT:
		{
			if( !info.has_account() || info.account().empty() )
			{
				retInfo.set_result( pb::REGISTER_ACCOUNT_NAME_ERROR ) ;
				Send( pb::SMSG_REGISTER_ACCOUNT, retInfo ) ;

				// Send event log
				SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_REGISTER_ACCOUTN_IS_EMPTY,  "s","EVENT_SELF_ACCOUNT_REGISTER_ACCOUTN_IS_EMPTY" ) ;
				return ;
			}

			AccountInfo *pAccount =sDataMgr.GetAccount(info.channel_id(),info.platform_id(), info.account() ) ;
			if( pAccount != NULL )
			{
				retInfo.set_result( pb::REGISTER_ACCOUNT_NAME_EXIST ) ;
				Send( pb::SMSG_REGISTER_ACCOUNT, retInfo ) ;

				// Send event log
				SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_REGISTER_IS_HAD_REGISTER,  "s","EVENT_SELF_ACCOUNT_REGISTER_IS_HAD_REGISTER" ) ;
				return ;
			}

			if( !info.has_account() || info.password().empty() )
			{
				retInfo.set_result( pb::REGISTER_ACCOUTN_PWD_ERROR ) ;
				Send( pb::SMSG_REGISTER_ACCOUNT, retInfo ) ;

				// Send event log
				SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_REGISTER_PWD_IS_EMPTY,  "s","EVENT_SELF_ACCOUNT_REGISTER_PWD_IS_EMPTY" ) ;
				return ;
			}
		} break ;

	case pb::REGISTER_ACCOUNT_GUEST:
		{
			if( info.has_account() )
			{
				retInfo.set_result( pb::REGISTER_ACCOUNT_NAME_ERROR ) ;
				Send( pb::SMSG_REGISTER_ACCOUNT, retInfo ) ;
				return ;
			}

			// create account and password for user .
			char buffer[512] ="" ;
			sprintf( buffer, "%lld%llu", sOS.TimeMS(), GetSessionId() ) ;

			info.set_account( buffer ) ;
			info.set_password( buffer ) ;
		} break ;

	default:
		{
			ASSERT( false ) ;
			return ;
		}
	}

	AccountInfo newAccount ;
	newAccount.ReadFrom( info ) ;
	newAccount.regTime = sOS.GetRealTime();
	newAccount.lastLoginTime = newAccount.regTime;
	m_dyDeviceId =info.device_id() ;

	// Insert into table and save to database .
	AccountInfo *pAccount =sDataMgr.AddNewAccount( newAccount ) ;
	ASSERT( pAccount ) ;
	if( pAccount == NULL )
	{
		retInfo.set_result( pb::REGISTER_ACCOUNT_NAME_EXIST ) ;
		Send( pb::SMSG_REGISTER_ACCOUNT, retInfo ) ;

		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_SELF_ACCOUNT_REGISTER_WRITE_DB_ERROR,  "s","EVENT_SELF_ACCOUNT_REGISTER_WRITE_DB_ERROR" ) ;
		return ;
	}

	retInfo.set_result( pb::REGISTER_ACCOUNT_SUCCESS ) ;
	retInfo.set_account_id( pAccount->id ) ;
	retInfo.set_account( newAccount.account ) ;
	retInfo.set_password( newAccount.password ) ;

	Send( pb::SMSG_REGISTER_ACCOUNT, retInfo ) ;

	// Save account info
	SetAccount( pAccount ) ;
	sDataMgr.AddAcciontInfoUse( pAccount->id ) ;

	SetIsVerification( true ) ;

	SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_START, *this, 4, "u", newAccount.channelId,"u", newAccount.platformId, "s", newAccount.account.c_str(), "s", GetDyDeviceId().c_str()) ;
}


void ClientSession::HandleChaneAccountInfo( NetPack &packet )
{
	pb::CChangeAccountAndPassord info ;
	packet >> info ;

	pb::SChangeAccountAndPassord retInfo ;

	if( !info.has_old_account() || ! info.has_old_password() || !info.has_new_account() || !info.has_new_password() )
	{
		SetIsNeedClose( true ) ;
		ELOG( "ClientSession::HandleChaneAccountInfo() recv error infomation ! " ) ;
		return ;
	}

	AccountInfo *pAccount =sDataMgr.GetAccount(info.channel_id(),info.platform_id(),info.old_account() ) ;
	if( pAccount == NULL )
	{
		retInfo.set_result( pb::CHANGE_ACCOUNT_PWD_ACCOUNT_ERROR ) ;
		Send( pb::SMSG_CHANGE_ACCOUNT_INFO, retInfo ) ;
		SetIsNeedClose( true ) ;
		return ;
	}

	if( info.old_password() != pAccount->password )
	{
		retInfo.set_result( pb::CHANGE_ACCOUNT_PWD_PASSWORD_ERROR ) ;
		Send( pb::SMSG_CHANGE_ACCOUNT_INFO, retInfo ) ;
		SetIsNeedClose( true ) ;
		return ;
	}

	pAccount->account =info.new_account() ;
	pAccount->password=info.new_password() ;

	sDataMgr.GetAccontTable()->WriteEntry( pAccount ) ;

	retInfo.set_result( pb::CHANGE_ACCOUNT_PWD_ACCOUNT_SUCCESS ) ;
	Send( pb::SMSG_CHANGE_ACCOUNT_INFO, retInfo ) ;
	SetIsNeedClose( true ) ;
}

void ClientSession::HandleGetServerList( NetPack &packet )
{
	if( !m_isPassSdkVerfy )
	{
		ELOG( "ClientSession::HandleGetServerList() Session(%llu) not passed sdk verfy cannot take this....", GetSessionId() ) ;
		return ;
	}

	pb::CGetServerList info ;
	packet >> info ;

	AccountInfo *pAccount =GetAccount() ;
	if (pAccount == NULL)
	{
		if (info.account_id() > 0)
		{
			pAccount =sDataMgr.GetAccount(info.account_id());
		}
	}
	if( pAccount == NULL || pAccount->id != info.account_id() )
	{
		if( pAccount == NULL )
		{
			ELOG( "ClientSession::HandleGetServerList() GetAccount() == NULL " ) ;
		}
		else
		{
			ELOG( "ClientSession::HandleGetServerList() GetAccount()->id != info.account_id() " ) ;
		}

		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_GET_SERVER_LIST_NOT_ACCOUNT_INFO,  "s","EVENT_GET_SERVER_LIST_NOT_ACCOUNT_INFO" ) ;

		return ;
	}

	pb::SGetServerList retInfo ;

	uint32             def_region_id  = 0;
	uint32             def_group_id  = 0;
	uint32             def_server_id = 0;
	uint32             last_region_id  = 0;
	uint32             last_group_id  = 0;
	uint32             last_server_id = 0;
	uint32             last_online_region_id  = 0;
	uint32             last_online_group_id  = 0;
	uint32             last_online_server_id = 0;
	uint32             last_new_region_id  = 0;
	uint32             last_new_group_id  = 0;
	uint32             last_new_server_id = 0;
	bool isDefServerCanUse = false;
	bool isCurLastSerCanUse = false;
	bool hasLasNewSer = false;
	bool hasOnlineSer = false;
	AccoutRoleInfoMap roleMap;
	sDataMgr.GetRoleInfosByAccountId(info.account_id(),roleMap);
	SessionPairId&  defser = sLoginSvr.GetCurDefauteServer();
	LoginServer::ConfigServerMap &configServers =sLoginSvr.GetConfigServerTable() ;
	for( LoginServer::ConfigServerMap::iterator itr =configServers.begin(); itr != configServers.end(); ++itr )
	{
		ConfigServerInfo& curSerInfo = itr->second;
		if( curSerInfo.m_pGameSession != NULL )
		{
			if( !sLoginSvr.CanConnectByThisVersion( info.channel_id(), info.platform_id(), info.version_id() ) )
			{
				continue ;
			}
		}
		curSerInfo.UpdateState();
		if (curSerInfo.m_test_state > 1)
		{
			if (!sLoginSvr.IsTesterAccountId(pAccount->id))
			{
				continue;
			}
		}
		if (curSerInfo.IsCloseChannel(info.channel_id()))
		{
			continue;
		}

		last_region_id  = curSerInfo.m_region_id;
		last_group_id  = curSerInfo.m_group_id;
		last_server_id = curSerInfo.m_server_id;
		if (!curSerInfo.IsMaintainState())
		{
			last_online_region_id  = last_region_id;
			last_online_group_id  = last_group_id;
			last_online_server_id = last_server_id;
			hasOnlineSer = true;

			if (curSerInfo.m_is_new > 0)
			{
				last_new_region_id  = last_region_id;
				last_new_group_id  = last_group_id;
				last_new_server_id = last_server_id;
				hasLasNewSer = true;
			}
	
			if (!isCurLastSerCanUse)
			{
				if (pAccount->lastRegionId > 0 && pAccount->lastServerId > 0)
				{
					if ((pAccount->lastRegionId == last_region_id) &&(pAccount->lastServerId == last_server_id))
					{
						def_region_id  = last_region_id;
						def_server_id = last_server_id;
						def_group_id  = last_group_id;
						isCurLastSerCanUse = true;
					}
				}
				
			}

			if (!isDefServerCanUse)
			{
				if (defser.first > 0 && defser.second > 0)
				{
					if ((defser.first == last_server_id) && (defser.second == last_region_id))
					{
						isDefServerCanUse = true;
					}
				}
			}
		}
		pb::ServerInfo *pInfo =retInfo.add_server_info() ;
		pInfo->set_region_id(last_region_id) ;
		pInfo->set_server_id(last_server_id) ;
		pInfo->set_group_id(last_group_id);
		pInfo->set_is_new(curSerInfo.m_is_new > 0);
		pb::ServerState curSState = static_cast<pb::ServerState>( curSerInfo.m_state );
		pInfo->set_server_state( curSState ) ;

		AccoutRoleInfoMapItr iterRole = roleMap.find(itr->first);
		if (iterRole != roleMap.end())
		{
			pb::RoleInfo* pRoleInfo = retInfo.add_roles_info();
			AccountRoleInfo* curRoleInfo = iterRole->second;
			if (curRoleInfo)
			{
				curRoleInfo->Saveto(*pRoleInfo);
				pRoleInfo->set_server_state(curSState);
			}
		}
	}
	
	if (!isCurLastSerCanUse)
	{
		if (isDefServerCanUse)
		{
			def_region_id  = defser.second;
			def_server_id = defser.first;
		}
		else if (hasLasNewSer)
		{
			def_region_id  = last_new_region_id;
			def_group_id  = last_new_group_id;
			def_server_id = last_new_server_id;
		}
		else if (hasOnlineSer)
		{
			def_region_id  = last_online_region_id;
			def_group_id  = last_online_group_id;
			def_server_id = last_online_server_id;
		}
		else
		{
			def_region_id  = last_region_id;
			def_group_id  = last_group_id;
			def_server_id = last_server_id;
		}
	}

	if (!isDefServerCanUse)
	{
		defser.first = def_server_id;
		defser.second = def_region_id;
	}
	pAccount->lastRegionId = def_region_id;
	pAccount->lastServerId = def_server_id;
	retInfo.set_region_id( def_region_id ) ;
	retInfo.set_server_id( def_server_id) ;
	retInfo.set_group_id(def_group_id);
	

	Send( pb::SMSG_GET_SERVER_LIST, retInfo ) ;
}

void ClientSession::HandleSelectServer( NetPack &packet )
{
	if( !m_isPassSdkVerfy )
	{
		ELOG( "ClientSession::HandleSelectServer() Session(%llu) not passed sdk verfy cannot take this....", GetSessionId() ) ;
		return ;
	}

	pb::CSelectServerId info ;
	packet >> info ;

	AccountInfo *pAccount =GetAccount() ;
	if (pAccount == NULL)
	{
		if (info.account_id() > 0)
		{
			pAccount =sDataMgr.GetAccount(info.account_id());
		}
	}
	if( pAccount == NULL || pAccount->id != info.account_id() )
	{
		if( pAccount == NULL )
		{
			ELOG( "ClientSession::HandleGetServerList() GetAccount() == NULL " ) ;
		}
		else
		{
			ELOG( "ClientSession::HandleGetServerList() GetAccount()->id != info.account_id() " ) ;
		}

		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 2, "u", pb::EVENT_GET_SELECT_SERVER_NOT_ACCOUNT_INFO,  "s","EVENT_GET_SELECT_SERVER_NOT_ACCOUNT_INFO" ) ;

		return ;
	}

	pb::SSelectServerId retInfo ;
	//PairIdSessionMapItr itr =gameSessions.find( sLoginSvr.GetGamePairId( info.id() ) ) ;
	serverId curSerId = info.server_id() > 0 ? info.server_id():1001;
	regionId curRegionId = info.region_id() > 0 ? info.region_id():1001;
	SessionPairId  ssionId( curSerId, curRegionId );
	bool isTesterServer = sLoginSvr.IsTesterServer(ssionId);
	bool isTesterAccount = sLoginSvr.IsTesterAccountId(pAccount->id);
	GameServerSession* curGamSesion = sLoginSvr.GetGameServerSessoin(ssionId);
	if( curGamSesion == NULL)
	{
		if(sLoginSvr.IfIsCombinServerGetOther(ssionId))
		{
			curGamSesion = sLoginSvr.GetGameServerSessoin(ssionId);
		}
	}
	if (curGamSesion == NULL)
	{
		retInfo.set_result( pb::ASK_SELECTSERVERID_INVALID_SERVER ) ;
		Send( pb::SMSG_SELECT_SERVER, retInfo ) ;
		// Send event log
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 3, "u", retInfo.result(),  "s","EVENT_GET_SELECT_SERVER_NOT_FIND_SERVER_ID", "u", info.server_id() ) ;
		return ;
	}

	bool neeNotice = false;
	string failmsg = "";
	if (sLoginSvr.IsInComfigMap(ssionId))
	{
		ConfigServerInfo& curSerConfig = sLoginSvr.GetOrCreateConfigServerInfo(ssionId);
		curSerConfig.UpdateState();
		if (curSerConfig.m_state == pb::SERVER_STATE_MAINTAIN  || curSerConfig.m_state == pb::SERVER_STATE_LIMIT)
		{
			if (curSerConfig.m_state == pb::SERVER_STATE_MAINTAIN)
			{
				retInfo.set_result( pb::ASK_SELECTSERVERID_IN_MAINTAIN ) ;
				failmsg = "ASK_SELECTSERVERID_IN_MAINTAIN";
			}
			else
			{
				retInfo.set_result( pb::ASK_SELECTSERVERID_RECH_SERVER_LIMIT) ;
				failmsg = "ASK_SELECTSERVERID_RECH_SERVER_LIMIT";
			}
			neeNotice = true;
		}
	}
	if (isTesterServer)
	{
		if (!neeNotice)
		{
			neeNotice = true;
			retInfo.set_result( pb::ASK_SELECTSERVERID_IN_MAINTAIN ) ;
			failmsg = "ASK_SELECTSERVERID_IN_MAINTAIN";
		}
		if (isTesterAccount)
		{
			neeNotice = false;
		}
	}
	if (neeNotice)
	{
		Send( pb::SMSG_SELECT_SERVER, retInfo ) ;
		SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_FAILED, *this, 3, "u", retInfo.result(),  "s",failmsg.c_str(), "u", info.server_id() ) ;
		return;
	}

	retInfo.set_result( pb::ASK_SELECTSERVERID_SUCCESS ) ;
	retInfo.set_ip( curGamSesion->GetAddress().second ) ;
	retInfo.set_port( curGamSesion->GetAddress().first ) ;

	if (pAccount->lastRegionId != ssionId.second || pAccount->lastServerId != ssionId.first)
	{
		pAccount->lastRegionId =ssionId.second ;
		pAccount->lastServerId =ssionId.first ;
		sDataMgr.GetAccontTable()->WriteEntry( pAccount );
	}

	// create session key to client .
	std::string key ;
	const uint64 keyValue =sOS.TimeSeconds() ;
	uint64 regionId =ssionId.second ;
	uint64 serverId =ssionId.first ;
	uint64 pairId   =regionId << 32 | serverId ;

	std::vector<uint64> args ;
	args.push_back( pAccount->id ) ;
	args.push_back( pairId ) ;
	args.push_back( keyValue ) ;

	Md5Tool::CalculateKey( key, args ) ;
	retInfo.set_session_key( key ) ;
	retInfo.set_key_value( keyValue ) ;

	Send( pb::SMSG_SELECT_SERVER, retInfo ) ;

	SetIsNeedClose( true ) ;

	

	// Send event log
	SendEventInfo( pb::EVENT_USER_ACCT_LOGIN_OK, *this, 4, "u", pAccount->channelId, "u", pAccount->platformId,"l", pAccount->id, "u", info.server_id() ) ;
}
