#include <fstream>
#include <string>

#include "CenterServerClient.h"
#include "DbProxyServerClient.h"
#include "GameServerHttpClients.h"
#include "EventLogServerClient.h"
#include "LobbyServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../GameServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"
#include "data/PayItemTable.h"

#include "DbTool.h"
#include "datastruct/struct_chatmenu.h"

#include "Enum.pb.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "GameServer.pb.h"
#include "Web.pb.h"
#include "utility/Utility.h"
#include "../session/PlayerPool.h"
#include "../SysEvent/SysEventMgr.h"

CenterServerClient::HandlerArray CenterServerClient::s_handlers =RegisteHandlers();


CenterServerClient::HandlerArray CenterServerClient::RegisteHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]                             =&CenterServerClient::HandlePong ;
	handlers[ pb::SG2C_REGISTER_RET ]                  =&CenterServerClient::HandleRegisterRet ;
	handlers[ pb::SC2G_ADD_LOGIN_SERVERS ]             =&CenterServerClient::HandleGetLoginServersRet;
	handlers[ pb::SG2C_GET_LOGIN_SERVERS_RET ]         =&CenterServerClient::HandleGetLoginServersRet;
	handlers[ pb::SC2G_ADD_CROSS_SERVERS ]			   =&CenterServerClient::HandleGetCrossServersRet;
	handlers[ pb::SG2C_GET_CROSS_SERVERS_RET ]         =&CenterServerClient::HandleGetCrossServersRet;

	handlers[ pb::SC2G_WEB_GET_PLAYER_LIST ]           =&CenterServerClient::HandleWebGetPlayerList;

	handlers[ pb::SC2G_ADD_EVENT_LOG_SERVER ]          =&CenterServerClient::HandleGetEventLogServersRet;
	handlers[ pb::SG2C_GET_EVENT_LOG_SERVER_RET ]      =&CenterServerClient::HandleGetEventLogServersRet;

	handlers[ pb::SC2G_WEB_PAY ]                       =&CenterServerClient::HandleWebPay ;

	handlers[ pb::SC2G_SEND_SERVERS_MAIL ]                       =&CenterServerClient::HandleSendServersMail ;
	handlers[ pb::SC2G_SEND_PLAYER_MAIL ]                       =&CenterServerClient::HandleSendPlayerMail ;
	handlers[ pb::SC2G_PLAYER_FORBID ]                       =&CenterServerClient::HandlePlayerForbid ;
	handlers[ pb::SC2G_SET_PLAYER_VIP ]                       =&CenterServerClient::HandleSetPlayerVip;
	handlers[ pb::SC2G_RECREATE_PAY_ORDER ]                       =&CenterServerClient::HandleReCreatePayOrder;
	handlers[ pb::SC2G_SEND_SERVERS_NOTICE ]                       =&CenterServerClient::HandleSendServersNotice;
	handlers[ pb::SC2G_SERVERS_HOT_RES_RELOAD ]                       =&CenterServerClient::HandleServerHotResReload;
	handlers[ pb::SG2C_EXCHANGE_GIFT_CODE_RET ]                       =&CenterServerClient::HandleExchangeGiftCodeRes;
	handlers[ pb::SG2C_FEEDBACK_RET ]                       =&CenterServerClient::HandleFeedBackRes;
	handlers[ pb::SC2G_GET_ONLINE_PLAYER_COUNT ]                       =&CenterServerClient::HandleGetPlayerOnlineCount;
	handlers[ pb::SC2G_GET_SERVER_INFO ]                       =&CenterServerClient::HandleGetGameserverInfo;
	handlers[ pb::SC2G_SERVERS_HOT_PAY_ITEM_RELOAD ]                       =&CenterServerClient::HandleServerPayItemHotReload;
	handlers[ pb::SC2G_SERVERS_SAVE_DBPROXY ]                       =&CenterServerClient::HandleSaveDbproxyCache;

	
	
	
	return handlers ;
}


CenterServerClient::CenterServerClient( void )
{
	SetClientType( PACKET_TYPE_SERVER_GAME ) ;
	SetDstServerType( PACKET_TYPE_SERVER_CENTER ) ;

	SetPingDiffMS( sGameSvr.Config().GetInt( "CenterServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sGameSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	SetConnectNow() ;
}

CenterServerClient::~CenterServerClient( void )
{
}



void CenterServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{

}

bool CenterServerClient::HandlePacket( NetPack &packet )
{
	size_t opCode = packet.GetOpcode() ;

	if( opCode >= s_handlers.size() )
	{
		ASSERT( false ) ;
		return false ;
	}

	NetPackHandler packetFun = s_handlers[ opCode ] ;
	if( packetFun )
	{
		(this->*packetFun)( packet ) ;
		return true ;
	}

	return false ;
}

void CenterServerClient::ClientConnect( void )
{
	SendRegisterInfo() ;
}

void CenterServerClient::SendPing( void )
{
	ServerClient::SendPing( pb::S_PING ) ;
}


void CenterServerClient::SendRegisterInfo( void )
{
	pb::SG2C_RegisterInfo msg ;

	msg.set_server_id( sGameSvr.GetServerId() ) ;

	msg.set_ip( sGameSvr.GetIP() ) ;
	msg.set_port( sGameSvr.GetPort() ) ;

	const String_Server *pNames =DbTool::Get<String_Server>( sGameSvr.GetServerId() ) ;
	if( pNames != NULL )
	{
		for( int i =LANG_SC; i < LANG_COUNT; ++i )
		{
			msg.add_names( pNames->str[i] ) ;
		}
	}
	else
	{
		ELOG( "GameServer id %u can not find name info ?????????????????? ", sGameSvr.GetServerId()  ) ;
	}

	Send( pb::SG2C_REGISTER, msg ) ;
}

void CenterServerClient::SendLockedAccount( uint64 accountId, const string& accountName, int locktype, int lockTime)
{
	pb::BroadcastHeader header ;
	header.set_type( pb::UPDATE_GAME_SERVER_LOCKED_ACCOUNT_INFO ) ;

	pb::SG2L_UpdatePlayerLockType body ;
	body.set_account_id( accountId) ;
	body.set_account_name(accountName);
	body.set_lock_time((uint64)lockTime);
	body.set_locktype((pb::ACCOUNT_LOCK_OP_TYPE)locktype);
	NetPack packet( pb::SG2C_BROADCAST_TO_LOGIN_SERVER, header.ByteSize() + body.ByteSize() + 4 ) ;
	packet << header ;
	packet << body ;
	Send( packet ) ;
}

void CenterServerClient::HandlePong( NetPack &packet )
{
	ServerClient::HandlePong( packet ) ;
}

void CenterServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SG2C_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			sGameSvr.SetRegionId( retInfo.region_id() ) ;

			// Set dbproxy address
			const ConfigMgr &config =sGameSvr.Config() ;
			std::string dbServerIp   =config.GetString( "DatabaseServer.ip", "127.0.0.1" ) ;
			std::string dbServerPort =config.GetString( "DatabaseServer.port", "55600" ) ;
			NLOG( "Setting database server client %s(%s) .", dbServerIp.c_str(), dbServerPort.c_str() ) ;
			sDbProxyClient.SetDstInfo( dbServerIp, dbServerPort ) ;

			// Set lobby address
			std::string lobbyIp   =config.GetString( "LobbyServer.ip", "127.0.0.1" ) ;
			std::string lobbyPort =config.GetString( "LobbyServer.port", "55300" ) ;
			NLOG( "Setting lobby server client %s(%s) .", lobbyIp.c_str(), lobbyPort.c_str() ) ;
			sLobbyClient.SetDstInfo( lobbyIp, lobbyPort ) ;

			// Get all login servers
			NetPack getLogins( pb::SG2C_GET_LOGIN_SERVERS ) ;
			Send( getLogins ) ;

			// Get event log server
			NetPack getEventServer( pb::SG2C_GET_EVENT_LOG_SERVER ) ;
			Send( getEventServer ) ;

			// Get all cross servers		liup
			//NetPack getCrosses( pb::SG2C_GET_CROSS_SERVERS ) ;
			//Send( getCrosses ) ;

			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;


			// 连接 web 服务器
			sHttpClients.SetWebUrl( retInfo.web_url() ) ;

            for ( int i = 0; i < REQUEST_OP_COUNT; ++i )
            {
                if( sGameSvr.m_curlRequest.IsNeedToSend(i) )
                {
                    sGameSvr.m_curlRequest.SendCURLRequest(i) ;
                }
            }

		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s) fail ! %s .", GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

void CenterServerClient::HandleGetLoginServersRet( NetPack &packet )
{
	pb::SG2C_GetLoinServersRet retInfo ;
	packet >> retInfo ;
	LLOG("HandleGetLoginServersRet");
	const int count =retInfo.addresses_size() ;
	for( int i=0; i < count; ++i )
	{
		const pb::Address &address =retInfo.addresses( i ) ;
		LLOG("ip=%s,port=%s",address.ip().c_str(), address.port().c_str() );
		if( !sGameSvr.AddOneLoginServer( address.ip(), address.port() ) )
		{
			WLOG( "Loign server %s, %s is had connected in game server ??", address.ip().c_str(), address.port().c_str() ) ;
		}
	}
}

void CenterServerClient::HandleGetCrossServersRet( NetPack &packet )
{
	pb::SG2C_GetCrossServersRet retInfo ;
	packet >> retInfo ;

	const int count =retInfo.addresses_size() ;
	for( int i=0; i < count; ++i )
	{
		const pb::Address &address =retInfo.addresses( i ) ;
		LLOG("ip=%s,port=%s",address.ip().c_str(), address.port().c_str() );
		if( !sGameSvr.AddOneCrossServer( address.ip(), address.port() ) )
		{
			WLOG( "Cross server %s, %s is had connected in game server ??", address.ip().c_str(), address.port().c_str() ) ;
		}
	}
}


void CenterServerClient::HandleWebGetPlayerList( NetPack &packet )
{
	packet.SetOpCode( pb::SG2D_WEB_GET_PLAYER_LIST ) ;
	sDbProxyClient.Send( packet ) ;
}


void CenterServerClient::HandleGetEventLogServersRet( NetPack &packet )
{
	pb::GetEventLogServersRet info ;
	packet >> info ;

	const pb::Address &address =info.addresses() ;

	if( !sGameSvr.AddOneEventLogServer( address.ip(), address.port() ) )
	{
		WLOG( "Event server had connect ?  %s, %s setting faild ??", address.ip().c_str(), address.port().c_str() ) ;
	}
}

void CenterServerClient::DoWebPay( const pb::WebPay& info, uint64 sessionId)
{
	const uint64 playerId =atoll( info.role_id().c_str() ) ;

	NLOG( "pb::WebPayParm----:oid:%s,channel:%d, platform:%d,curency:%d,money:%0.2f,account:%s,roid:%s,serId:%u,pre:%0.2f,proId:%s", info.order_id().c_str(),info.channel(),info.platform(),info.currency(),info.money(),info.sdk_account().c_str(),info.role_id().c_str(),info.server_id(),info.present(),info.product_id().c_str()) ;

	pb::Sdk_PayResult payResult;
	payResult.mutable_payinfo()->CopyFrom(info);
	// server log
	sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_START, playerId, 0, 5,"u", info.channel(),"s", info.order_id().c_str(),  "f", info.money(),"s",info.sdk_account().c_str(),"u", info.platform()) ;
	PayItem*  pItem = sGameSvr.m_payItemsInfo.GetPayItem(info.product_id());
	if (pItem)
	{
		pb::GXDB_PayOrder payOder;
		payOder.set_is_new(true);
		payOder.set_product_id(info.product_id());
		float realNeedMoney = pItem->iVal.need_rmb * info.present();
		pb::WebPayRet retInfo ;
		NLOG( "pb::WebPayParm--------:money:%0.2f,needMoney:%0.2f", info.money(),realNeedMoney) ;
		bool hasError = false;
		if (info.currency() != pb::WEB_PAY_RMB)
		{
			retInfo.set_result( pb::CAN_NOT_FIND_MONEY_TYPE ) ;
			retInfo.set_error_msg( "not find money type!!!" ) ;
			retInfo.set_order_id( info.order_id() ) ;
			Send( pb::SC2G_WEB_PAY_RET, sessionId, retInfo ) ;
			// server log
			sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_FAILED, playerId,0, 5,
				"u", info.channel(),"s", info.order_id().c_str(),  "f", info.money(), "u", retInfo.result(), "s", retInfo.error_msg().c_str() ) ;
			payResult.mutable_result()->CopyFrom(retInfo);
			hasError = true;
		}
		else if (info.money() < realNeedMoney)
		{
			NLOG( "pb::WebPayParm--------:money:%0.2f,needMoney:%0.2f", info.money(),realNeedMoney) ;
			retInfo.set_result(pb::CAN_MONEY_NOT_ENOUGH);
			retInfo.set_order_id( info.order_id() ) ;
			retInfo.set_error_msg( "pay money not enough !!" ) ;
			Send( pb::SC2G_WEB_PAY_RET, sessionId, retInfo ) ;
			sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_FAILED, playerId,0, 5,
				"u", info.channel(),"s", info.order_id().c_str(),  "f", info.money(), "u", retInfo.result(), "s", retInfo.error_msg().c_str() ) ;
			payResult.mutable_result()->CopyFrom(retInfo);
			hasError = true;
		}
		else
		{
			payOder.set_crystal_final(pItem->iVal.reward_diamond);
			payOder.set_vip_xp(pItem->iVal.reward_diamond);
		}
		PlayerPtr pPlayer = sPlayerPool.GetPtrByPlayerId(playerId);
		if (pPlayer.get())
		{
			if (hasError)
			{
				pPlayer->Send(pb::SMSG_SDK_PAY_RET,payResult);
			}
			else
			{
				pPlayer->TakePayOrderResult(payOder);
				pb::WebPayRet retOkInfo ;
				retOkInfo.set_result( pb::WEB_PAY_RERSULT_OK ) ;
				retOkInfo.set_order_id( info.order_id() ) ;
				Send( pb::SC2G_WEB_PAY_RET, sessionId, retOkInfo ) ;
				sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_OK, playerId,0, 5, "u", info.channel(), "s", info.order_id().c_str(), "f", info.money(),  "u", payOder.crystal_final(),"u", info.platform() ) ;
				payResult.mutable_result()->CopyFrom(retOkInfo);
				pPlayer->Send(pb::SMSG_SDK_PAY_RET,payResult);
			}
		}
		else
		{
			pb::WebPayToDatabase payToDatabase ;
			payToDatabase.set_order_id( info.order_id() ) ;
			payToDatabase.set_currency( info.currency() ) ;
			payToDatabase.set_present( info.present() ) ;
			payToDatabase.set_player_id( playerId ) ;
			payToDatabase.set_money( info.money() ) ;
			payToDatabase.set_sdk_account( info.sdk_account() ) ;
			payToDatabase.set_platform(info.platform());
			NetPack sendPack( pb::SG2D_WEB_PAY, sizeof( pItem->iVal ) + payToDatabase.ByteSize() + 4 ) ;
			sendPack << payToDatabase ;
			sendPack.AppendStruct( pItem->iVal ) ;
			sendPack.SetSessionId( sessionId ) ;
			if( !sDbProxyClient.Send( sendPack ) )
			{
				pb::WebPayRet retOffInfo ;
				retOffInfo.set_result( pb::CAN_NOT_FIND_SERVER ) ;
				retOffInfo.set_order_id( info.order_id() ) ;
				retOffInfo.set_error_msg( "database can not use!!!" ) ;
				Send( pb::SC2G_WEB_PAY_RET, sessionId, retOffInfo ) ;

				// server log
				sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_FAILED, playerId,0,5,
					"u", info.channel(),"s", info.order_id().c_str(),  "f", info.money(), "u", retOffInfo.result(), "s", retOffInfo.error_msg().c_str() ) ;
			}

			NLOG( "DoWebPay Send To dbproxy:crystal:%u,vix_xp:%u", payOder.crystal_final(),payOder.vip_xp()) ;
		}
	}
	else
	{
		pb::WebPayRet retInfo ;
		retInfo.set_result( pb::CAN_NOT_FIND_ITEM_TYPE ) ;
		retInfo.set_error_msg( "can not find sell product" ) ;
		Send( pb::SC2G_WEB_PAY_RET, sessionId, retInfo ) ;
		// server log
		sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_FAILED, playerId,0, 5,
			"u", info.channel(),"s", info.order_id().c_str(),  "f", info.money(), "u", retInfo.result(), "s", retInfo.error_msg().c_str() ) ;
		payResult.mutable_result()->CopyFrom(retInfo);
		PlayerPtr pPlayer = sPlayerPool.GetPtrByPlayerId(playerId);
		if (pPlayer.get())
		{
			pPlayer->Send(pb::SMSG_SDK_PAY_RET,payResult);
		}
	}
}

void CenterServerClient::HandleWebPay( NetPack &packet )
{
	pb::WebPay info ;
	packet >> info ;
	DoWebPay(info,packet.GetSessionId());
}

void CenterServerClient::HandleSendServersMail( NetPack &packet )
{
	pb::ServersMail info ;
	packet >> info ;
	std::set<uint64> playerIds;
	pb::SG2D_Check_Mail_Recivers_Exist checkMsg;
	checkMsg.set_alv(info.alv());
	checkMsg.set_blv(info.blv());
	checkMsg.set_avip(info.avip());
	checkMsg.set_bvip(info.bvip());
	checkMsg.set_is_server_mail(true);
	checkMsg.mutable_mail()->set_send_type(info.online() > 0 ? pb::MAIL_SEND_ONLINE:pb::MAIL_SEND_ALL);
	checkMsg.mutable_mail()->set_title(info.title());
	checkMsg.mutable_mail()->set_content(info.content());
	checkMsg.mutable_mail()->set_channel(info.channel());
	checkMsg.mutable_mail()->set_items(info.mail_award());
	checkMsg.mutable_mail()->set_mail_type(info.moldid() == 1 ? pb::MAIL_PLATGIFT : pb::MAIL_PLATFROM);
	sSysEventMgr.SendPlatFormMail(checkMsg,playerIds);
}

void CenterServerClient::HandleSendPlayerMail( NetPack &packet )
{
	pb::PlayerMail info ;
	packet >> info ;
	std::set<uint64> playerIds;
	Utility::SplitStr(info.to(),playerIds,'|');
	pb::SG2D_Check_Mail_Recivers_Exist checkMsg;
	checkMsg.set_is_server_mail(false);
	checkMsg.mutable_mail()->set_send_type(pb::MAIL_SEND_ALL);
	checkMsg.mutable_mail()->set_title(info.title());
	checkMsg.mutable_mail()->set_content(info.content());
	checkMsg.mutable_mail()->set_items(info.mail_award());
	checkMsg.mutable_mail()->set_mail_type(info.moldid() == 1 ? pb::MAIL_PLATGIFT : pb::MAIL_PLATFROM);
	sSysEventMgr.SendPlatFormMail(checkMsg,playerIds);
	pb::ServerCode resCode;
	resCode.set_id(info.sid());
	resCode.set_code(200);
	Send( pb::SC2G_SEND_PLAYER_MAIL_RET, packet.GetSessionId(), resCode ) ;
}

void CenterServerClient::HandlePlayerForbid( NetPack &packet )
{
	pb::PlayerForbid info ;
	packet >> info ;
	std::set<uint64> playerVec;
	Utility::SplitStr(info.lordid(),playerVec,'|');
	bool isSet = true;
	for (std::set<uint64>::iterator iter = playerVec.begin(); iter != playerVec.end(); ++iter)
	{
		uint64 curPlayerId = (*iter);
		PlayerPtr pPlayer = sPlayerPool.GetPtrByPlayerId(curPlayerId);
		if (pPlayer.get())
		{
			switch(info.forbidid())
			{
			case  pb::ACCOUNT_OP_LOCK_SPEAK:
				{
					pPlayer->SetShutUpState(true);
					if (info.time() > 0)
					{
						if (info.time() == 1)
						{
							pPlayer->SetLockTime(1);
						}
						else
						{
							pPlayer->SetLockTime(sOS.GetRealTime() + (uint64)info.time());
						}
					}
					else
					{
						pPlayer->SetLockTime(0);
					}
				}
				break;
			case  pb::ACCOUNT_OP_UNLOCK_SPEAK:
				{
					pPlayer->SetShutUpState(false);
				}
				break;
			case  pb::ACCOUNT_OP_LOCK:
				{
					pPlayer->SetLockState(true);
					if (info.time() > 0)
					{
						if (info.time() == 1)
						{
							pPlayer->SetLockTime(1);
						}
						else
						{
							pPlayer->SetLockTime(sOS.GetRealTime() + (uint64)info.time());
						}
					}
					else
					{
						pPlayer->SetLockTime(0);
					}
				}
				break;
			case  pb::ACCOUNT_OP_UNLOCK:
				{
					pPlayer->SetLockState(false);
				}
				break;
			case  pb::ACCOUNT_OP_KICK_OFF_LINE:
				{
					pPlayer->CloseSession();

					pb::SG2D_Forbiden_Player forbitInfo;
					forbitInfo.set_player_id(curPlayerId);
					forbitInfo.set_forbidid(info.forbidid());
					forbitInfo.set_time(info.time());
					sDbProxyClient.Send(pb::SG2D_UNLOCK_FORBIDEN,forbitInfo);
				}
				break;
			default:
				{
					isSet = false;
				}
				break;
			}

			if (info.forbidid() == pb::ACCOUNT_OP_LOCK || info.forbidid() == pb::ACCOUNT_OP_UNLOCK )
			{
				SendLockedAccount(pPlayer->AccountId(),pPlayer->CharName(),info.forbidid(),info.time());
			}
		}
		else
		{
			pb::SG2D_Forbiden_Player forbitInfo;
			forbitInfo.set_player_id(curPlayerId);
			forbitInfo.set_forbidid(info.forbidid());
			forbitInfo.set_time(info.time());
			sDbProxyClient.Send(pb::SG2D_UNLOCK_FORBIDEN,forbitInfo);
		}
	}
	pb::ServerCode resCode;
	resCode.set_id(info.sid());
	resCode.set_code(isSet ? 200 : 0);
	Send( pb::SC2G_PLAYER_FORBID_RET, packet.GetSessionId(), resCode ) ;
}

void CenterServerClient::HandleSetPlayerVip( NetPack &packet )
{
	pb::SetPlayerVip info ;
	packet >> info ;

	std::set<uint64> playerVec;
	Utility::SplitStr(info.lordid(),playerVec,'|');
	pb::ServerCode resCode;
	resCode.set_id(info.sid());
	resCode.set_code(200);
	Send( pb::SC2G_SET_PLAYER_VIP_RET, packet.GetSessionId(), resCode ) ;
}

void CenterServerClient::HandleReCreatePayOrder( NetPack &packet )
{
	pb::ReCreateOder info ;
	packet >> info ;
	std::set<uint64> playerVec;
	Utility::SplitStr(info.lordid(),playerVec,'|');
	pb::ServerCode resCode;
	resCode.set_id(info.sid());
	resCode.set_code(200);
	Send( pb::SC2G_RECREATE_PAY_ORDER_RET, packet.GetSessionId(), resCode ) ;
}

void CenterServerClient::HandleSendServersNotice( NetPack &packet )
{
	pb::ServersNotice info ;
	packet >> info ;

}

void CenterServerClient::HandleServerHotResReload( NetPack &packet )
{
	pb::SC2G_activity_package_Update msg;
	packet >> msg;
	if(msg.update())
	{
		sDbProxyClient.Send(pb::SG2D_UPDATE_ACTIIVTY_DATA,msg);
	}

}

void CenterServerClient::HandleExchangeGiftCodeRes( NetPack &packet )
{
	pb::SG2C_Exchange_Gift_Code_Ret info ;
	packet >> info ;
	if (info.is_success())
	{
		std::set<uint64> playerIds;
		playerIds.insert(info.player_id());
		pb::SG2D_Check_Mail_Recivers_Exist checkMsg;
		checkMsg.set_is_server_mail(false);
		checkMsg.mutable_mail()->set_send_type(pb::MAIL_SEND_ALL);
		checkMsg.mutable_mail()->set_title(info.giftname());
		checkMsg.mutable_mail()->set_content("Back Stage Gifts");
		checkMsg.mutable_mail()->set_items(info.gifts());
		checkMsg.mutable_mail()->set_mail_type( pb::MAIL_PLATGIFT);
		sSysEventMgr.SendPlatFormMail(checkMsg,playerIds);
	}
	else
	{
		PlayerPtr pPlayer = sPlayerPool.GetPtrByPlayerId(info.player_id());
		if (pPlayer.get())
		{
			switch(info.error_code())
			{
			case 1:
				pPlayer->SendErrorCode(pb::ErrChannel);
				break;
			case 2:
				pPlayer->SendErrorCode(pb::ErrGiftCodeNotExist);
				break;
			case 3:
				pPlayer->SendErrorCode(pb::ErrGiftCodeExpier);
				break;
			case 4:
				pPlayer->SendErrorCode(pb::ErrGiftCodeNoActive);
				break;
			case 5:
				pPlayer->SendErrorCode(pb::ErrGiftCodeInValid);
				break;
			}
		}
	}
}

void CenterServerClient::HandleFeedBackRes( NetPack &packet )
{
	pb::SG2C_FeedBack_Ret info ;
	packet >> info ;
	pb::FeedbackRet infRect;
	infRect.set_is_success(false);
	PlayerPtr pPlayer = sPlayerPool.GetPtrByPlayerId(info.player_id());
	if (pPlayer.get())
	{
		infRect.set_is_success(info.is_success());
		infRect.set_error_code(info.error_code());
		pPlayer->Send(pb::SMSG_FEEDBACK,infRect);
	}
}

void CenterServerClient::HandleGetPlayerOnlineCount( NetPack &packet )
{
	pb::OnlinePlayerCount info;
	packet >> info;
	int curOnlines = sGameSvr.GetOnlinePlayerCount();
	pb::ServerCode resCode;
	resCode.set_id(info.sid());
	resCode.set_code(curOnlines);
	Send( pb::SG2C_GET_ONLINE_PLAYER_COUNT_RET, packet.GetSessionId(), resCode ) ;
}

void CenterServerClient::HandleGetGameserverInfo( NetPack &packet )
{
	pb::OnlinePlayerCount info;
	packet >> info;
	std::string serinfos;
	if(sGameSvr.GetLogicTheardUpdateTimeInfo(serinfos))
	{
		pb::serverInfo serInfo;
		serInfo.set_sid(info.sid());
		serInfo.set_info(serinfos);
		Send( pb::SG2C_GET_SERVER_INFO_RET, packet.GetSessionId(), serInfo ) ;
	}
}

void CenterServerClient::HandleServerPayItemHotReload( NetPack &packet )
{
		sGameSvr.m_curlRequest.SendCURLRequest(REQUEST_OP_PAY_ITEM_INFO) ;
}

void CenterServerClient::HandleSaveDbproxyCache( NetPack &packet )
{
	pb::SC2G_ReloadPayItem sendmsg;
	packet >> sendmsg;
	sDbProxyClient.Send(pb::SG2D_SAVE_DBPROXY,sendmsg);
}
