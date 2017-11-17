
#include "WebServerSession.h"
#include <boost/foreach.hpp>
#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../CenterServer.h"
#include "../session/GameServerSession.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "Web.pb.h"
#include "utility/Utility.h"
#include "multi_language/multi_lang_def.h"
#include "../table/GiftBagTable.h"
#include "../table/GiftCodeTable.h"
#include "../CenterDataMgr.h"

WebServerSession::HandlerArray WebServerSession::s_serverHandlers =RegisteServerHandlers() ;


WebServerSession::HandlerArray WebServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                          =&WebServerSession::HandlePing ;
	handlers[ pb::SW2C_GET_SERVER_LIST ]            =&WebServerSession::HandleGetServerList ;
	handlers[ pb::SW2C_GET_PLAYER_LIST ]            =&WebServerSession::HandleGetPayerList ;
	handlers[ pb::SW2C_PAY ]                        =&WebServerSession::HandleWebPay ;
	handlers[ pb::SW2C_SEND_SERVERS_MAIL ]                        =&WebServerSession::HandleSendServersMail ;
	handlers[ pb::SW2C_SEND_PLAYER_MAIL ]                        =&WebServerSession::HandleSendPlayerMail ;
	handlers[ pb::SW2C_PLAYER_FORBID ]                        =&WebServerSession::HandlePlayerForbid;
	handlers[ pb::SW2C_SET_PLAYER_VIP ]                        =&WebServerSession::HandleSetPlayerVip ;
	handlers[ pb::SW2C_RECREATE_PAY_ORDER ]                        =&WebServerSession::HandleRecreatePayOrder ;
	handlers[ pb::SW2C_SEND_SERVERS_NOTICE ]                        =&WebServerSession::HandleSendServersNotice ;
	handlers[ pb::SW2C_SERVERS_HOT_RES_RELOAD ]                        =&WebServerSession::HandleServersHotResReload;
	handlers[ pb::SW2C_CREATE_GIFT_CODE ]                        =&WebServerSession::HandleCreatGiftCode;
	handlers[ pb::SW2C_GET_ONLINE_PLAYER_COUNT ]                        =&WebServerSession::HandleGetPlayerOnlineCount;
	handlers[ pb::SW2C_GET_SERVER_INFO ]                        =&WebServerSession::HandleGetServerInfo;

	

	

	return handlers ;
}



WebServerSession::WebServerSession( void )
{
}

WebServerSession::~WebServerSession( void )
{
}


void WebServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool WebServerSession::HandleNetPack( NetPack &pack )
{
	// All server packet !

	int opCode               =pack.GetOpcode() ;

	HandlerArray &funTable   =s_serverHandlers ;

	ASSERT( static_cast<size_t>( opCode ) < funTable.size() ) ;
	NetPackHandler packetFun = funTable[ opCode ] ;

	if( packetFun )
	{
		(this->*packetFun)( pack ) ;
		return true ;
	}

	return false ;
}


void WebServerSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::S_PONG ) ;
}



void WebServerSession::HandleRegister( NetPack &packet )
{
	if( GetIsVerification() )
	{
		WLOG( "Had register, can not register again ........................" ) ;
		return ;
	}

	pb::SL2C_RegisterInfo info ;
	packet >> info ;
	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	pb::SL2C_RegisterInfoRet retInfo ;
	CenterServer::ServerRegisterResult ret =sCenterSvr.LoginServerRegister( this ) ;
	switch( ret )
	{
	case CenterServer::SERVER_REGISTER_OK:
		{
			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;
			retInfo.set_region_id( sCenterSvr.GetRegionId() ) ;
			NLOG( "%s register %s,SessionId:%llu,(reg:%u,ser:%u), (%s:%s) register ok"
				, GetSessionTypeName(),sCenterSvr.GetServerTypeName(), GetSessionId(), GetPairId().second, GetPairId().first, GetAddress().second.c_str(), GetAddress().first.c_str() ) ;
		} break ;

	case CenterServer::SERVER_REGISTER_FAIL_ID:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ID ) ;
		} break ;

	case CenterServer::SERVER_REGISTER_FAIL_ADDRESS:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ADDRESS ) ;
		} break ;

	default:
		ASSERT( false ) ;
	}

	Send( pb::SL2C_REGISTER_RET, retInfo ) ;
}



void WebServerSession::HandleGetServerList( NetPack &pack )
{
	pb::GetGameServerList info ;
	pack >> info ;

	size_t lang =LANG_EN ;
	switch( info.lang() )
	{
	case pb::sc:
		{
			lang =LANG_SC ;
		} break ;

	case pb::th:
		{
			lang =LANG_TH ;
		} break ;

	case pb::ve:
		{
			lang =LANG_VE ;
		} break ;

	case pb::en:
		{
			lang =LANG_EN ;
		} break ;
	case pb::tc:
		{
			lang =LANG_TC ;
		} break ;
	default:
		WLOG( "WebServerSession::HandleGetServerList() recv langType %u, not define ???? )", info.lang() ) ;
	}

	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;

	pb::GameServerList retInfo ;
	for( SessionByServerIdMapItr itr =games.begin(); itr != games.end(); ++itr )
	{
		GameServerSession *pGame =static_cast<GameServerSession*>( itr->second ) ;
		ASSERT( pGame->GetSessionType() == PACKET_TYPE_SERVER_GAME ) ;
		if(  pGame->GetSessionType() != PACKET_TYPE_SERVER_GAME )
		{
			continue ;
		}
	
		pb::GameServerInfo *pInfo =retInfo.add_infos() ;
		pInfo->set_id( itr->first ) ;

		GameServerSession::ServerNameTable &nams =pGame->GetServerNames() ;
		ASSERT( lang < nams.size() ) ;
		pInfo->set_name( nams[lang] ) ;
	}

	Send( pb::SW2C_GET_SERVER_LIST_RET, retInfo ) ;

}


void WebServerSession::HandleGetPayerList( NetPack &packet )
{
	pb::WebGetPlayerList info ;
	packet >> info ;

	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find( info.server_id() ) ;
	if( itr == games.end() )
	{
		ELOG( "WebServerSession::HandleGetPayerList() can not find server %u ...", info.server_id() ) ;
		return ;
	}

	SetIsVerification( true ) ;

	itr->second->Send( pb::SC2G_WEB_GET_PLAYER_LIST, GetSessionId(), info ) ;
}


void WebServerSession::HandleWebPay( NetPack &packet )
{
	pb::WebPay info ;
	packet >> info ;

	// 发往游戏服务器
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find( info.server_id() ) ;
	if( itr == games.end() )
	{
		pb::WebPayRet retInfo ;
		retInfo.set_result( pb::CAN_NOT_FIND_SERVER ) ;
		retInfo.set_error_msg( "server not found" ) ;
		Send( pb::SW2C_PAY_RET, retInfo ) ;

		ELOG( "WebServerSession::HandleGetPayerList() can not find server %u ...", info.server_id() ) ;
		return ;
	}

	SetIsVerification( true ) ;

	itr->second->Send( pb::SC2G_WEB_PAY, GetSessionId(), info ) ;
}

void WebServerSession::HandleSendServersMail( NetPack &packet )
{
	pb::ServersMail info ;
	packet >> info ;
	std::vector<int> servertsVec;
	Utility::SplitStr(info.sid(),servertsVec,'|');
	info.clear_sid();
	SessionPairIdSet resultSend;
	sCenterSvr.BroadcastMsgToGameServer(pb::SC2G_SEND_SERVERS_MAIL,servertsVec,info,resultSend);
	SessionPairIdSet::iterator iterRes = resultSend.begin();
	pb::ServerResponse ResMsg;
	ResMsg.set_code(200);
	for (; iterRes != resultSend.end(); ++iterRes)
	{
		const SessionPairId& curPair = (SessionPairId)(*iterRes);
		if (curPair.second != 200)
		{
			ResMsg.set_code(curPair.second);
		}
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(curPair.first);
		serSt->set_code(curPair.second);
	}
	Send(pb::SW2C_SEND_SERVERS_MAIL_RET,ResMsg);
}

void WebServerSession::HandleSendPlayerMail( NetPack &packet )
{
	pb::PlayerMail info ;
	packet >> info ;
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find(info.sid()) ;
	if( itr != games.end() )
	{
		Session* curSession = itr->second;
		curSession->Send( pb::SC2G_SEND_PLAYER_MAIL, GetSessionId(), info ) ;
	}
	else
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(0);
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(info.sid());
		serSt->set_code(0);
		Send(pb::SW2C_SEND_PLAYER_MAIL_RET,ResMsg);
	}

}

void WebServerSession::HandlePlayerForbid( NetPack &packet )
{
	pb::PlayerForbid info ;
	packet >> info ;
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find(info.sid());
	if( itr != games.end() )
	{
		itr->second->Send( pb::SC2G_PLAYER_FORBID, GetSessionId(), info ) ;
	}
	else
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(0);
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(info.sid());
		serSt->set_code(0);
		Send(pb::SW2C_PLAYER_FORBID_RET,ResMsg);
	}
}

void WebServerSession::HandleSetPlayerVip( NetPack &packet )
{
	pb::SetPlayerVip info ;
	packet >> info ;
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find(info.sid());
	if( itr != games.end() )
	{
		itr->second->Send( pb::SC2G_SET_PLAYER_VIP, GetSessionId(), info ) ;
	}
	else
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(0);
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(info.sid());
		serSt->set_code(0);
		Send(pb::SW2C_SET_PLAYER_VIP_RET,ResMsg);
	}

}

void WebServerSession::HandleRecreatePayOrder( NetPack &packet )
{
	pb::ReCreateOder info ;
	packet >> info ;
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find(info.sid());
	if( itr != games.end() )
	{
		itr->second->Send( pb::SC2G_RECREATE_PAY_ORDER, GetSessionId(), info ) ;
	}
	else
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(0);
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(info.sid());
		serSt->set_code(0);
		Send(pb::SW2C_RECREATE_PAY_ORDER_RET,ResMsg);
	}
}

void WebServerSession::HandleSendServersNotice( NetPack &packet )
{
	pb::ServersNotice info ;
	packet >> info ;
	std::vector<int> servertsVec;
	Utility::SplitStr(info.sid(),servertsVec,'|');
	SessionPairIdSet resultSend;
	sCenterSvr.BroadcastMsgToGameServer(pb::SC2G_SEND_SERVERS_NOTICE,servertsVec,info,resultSend);
	SessionPairIdSet::iterator iterRes = resultSend.begin();
	pb::ServerResponse ResMsg;
	ResMsg.set_code(200);
	for (; iterRes != resultSend.end(); ++iterRes)
	{
		const SessionPairId& curPair = (SessionPairId)(*iterRes);
		if (curPair.second != 200)
		{
			ResMsg.set_code(curPair.second);
		}
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(curPair.first);
		serSt->set_code(curPair.second);
	}
	Send(pb::SW2C_SEND_SERVERS_NOTICE_RET,ResMsg);

}

void WebServerSession::HandleServersHotResReload( NetPack &packet )
{
	pb::HotResReload info ;
	packet >> info ;
	std::vector<int> servertsVec;
	Utility::SplitStr(info.sid(),servertsVec,'|');
	int type = atoi(info.type().c_str());
	pb::ServerResponse ResMsg;
	ResMsg.set_code(0);
	if(type == 1)
	{
		SessionPairIdSet resultSend;
		pb::SC2G_activity_package_Update sendmsg;
		sendmsg.set_update(1);
		if(sCenterDataMgr.reloadActivityTables(*sendmsg.mutable_all_info()))
		{
			sCenterSvr.BroadcastMsgToGameServer(pb::SC2G_SERVERS_HOT_RES_RELOAD,servertsVec,sendmsg,resultSend);
			ResMsg.set_code(200);
			SessionPairIdSet::iterator iterRes = resultSend.begin();
			for (; iterRes != resultSend.end(); ++iterRes)
			{
				const SessionPairId& curPair = (SessionPairId)(*iterRes);
				if (curPair.second != 200)
				{
					ResMsg.set_code(curPair.second);
				}
				pb::ServerCode* serSt = ResMsg.add_reg();
				serSt->set_id(curPair.first);
				serSt->set_code(curPair.second);
			}
		}
	}
	else if (type == 2)
	{
		ResMsg.set_code(200);
		pb::SW2C2L_UpdateServerInfo slinfo;
		slinfo.set_region_id(sCenterSvr.GetRegionId());
		std::vector<int>::iterator retIter = servertsVec.begin();
		for(; retIter != servertsVec.end(); ++retIter)
		{
			int serverid = (int)(*retIter);
			slinfo.add_server_id(serverid);
			pb::ServerCode* serSt = ResMsg.add_reg();
			serSt->set_id(serverid);
			serSt->set_code(200);
		}
		pb::BroadcastHeader header ;
		header.set_type( pb::UPDATE_GAME_SERVER_TESTER_ACCOUNT_IDS ) ;
		NetPack packet( pb::SC2L_BROADCAST, header.ByteSize() + slinfo.ByteSize() + 4 ) ;
		packet << header ;
		packet << slinfo ;
		sCenterSvr.BroadcastToLoginServer(packet);
	}
	else if (type == 3)
	{
		ResMsg.set_code(200);
		pb::SW2C2L_UpdateServerInfo slinfo;
		slinfo.set_region_id(sCenterSvr.GetRegionId());
		std::vector<int>::iterator retIter = servertsVec.begin();
		for(; retIter != servertsVec.end(); ++retIter)
		{
			int serverid = (int)(*retIter);
			slinfo.add_server_id(serverid);
			pb::ServerCode* serSt = ResMsg.add_reg();
			serSt->set_id(serverid);
			serSt->set_code(200);
		}
		pb::BroadcastHeader header ;
		header.set_type( pb::UPDATE_GAME_SERVER_LIST_INFO ) ;
		NetPack packet( pb::SC2L_BROADCAST, header.ByteSize() + slinfo.ByteSize() + 4 ) ;
		packet << header ;
		packet << slinfo ;
		sCenterSvr.BroadcastToLoginServer(packet);
	}
	else if (type == 4)
	{
		ResMsg.set_code(200);
		pb::SW2C2L_UpdateServerInfo slinfo;
		slinfo.set_region_id(sCenterSvr.GetRegionId());
		std::vector<int>::iterator retIter = servertsVec.begin();
		for(; retIter != servertsVec.end(); ++retIter)
		{
			int serverid = (int)(*retIter);
			slinfo.add_server_id(serverid);
			pb::ServerCode* serSt = ResMsg.add_reg();
			serSt->set_id(serverid);
			serSt->set_code(200);
		}
		pb::BroadcastHeader header ;
		header.set_type( pb::UPDATE_GAME_SERVER_VERSION_INFO ) ;
		NetPack packet( pb::SC2L_BROADCAST, header.ByteSize() + slinfo.ByteSize() + 4 ) ;
		packet << header ;
		packet << slinfo ;
		sCenterSvr.BroadcastToLoginServer(packet);
	}
	else if (type == 5)
	{
		pb::SC2G_ReloadPayItem sendmsg;
		sendmsg.set_region_id(sCenterSvr.GetRegionId());
		SessionPairIdSet resultSend;
		sCenterSvr.BroadcastMsgToGameServer(pb::SC2G_SERVERS_HOT_PAY_ITEM_RELOAD,servertsVec,sendmsg,resultSend);
		ResMsg.set_code(200);
		SessionPairIdSet::iterator iterRes = resultSend.begin();
		for (; iterRes != resultSend.end(); ++iterRes)
		{
			const SessionPairId& curPair = (SessionPairId)(*iterRes);
			if (curPair.second != 200)
			{
				ResMsg.set_code(curPair.second);
			}
			pb::ServerCode* serSt = ResMsg.add_reg();
			serSt->set_id(curPair.first);
			serSt->set_code(curPair.second);
		}
	}
	else if (type == 6)
	{
		pb::SC2G_ReloadPayItem sendmsg;
		sendmsg.set_region_id(sCenterSvr.GetRegionId());
		SessionPairIdSet resultSend;
		sCenterSvr.BroadcastMsgToGameServer(pb::SC2G_SERVERS_SAVE_DBPROXY,servertsVec,sendmsg,resultSend);
		ResMsg.set_code(200);
		SessionPairIdSet::iterator iterRes = resultSend.begin();
		for (; iterRes != resultSend.end(); ++iterRes)
		{
			const SessionPairId& curPair = (SessionPairId)(*iterRes);
			if (curPair.second != 200)
			{
				ResMsg.set_code(curPair.second);
			}
			pb::ServerCode* serSt = ResMsg.add_reg();
			serSt->set_id(curPair.first);
			serSt->set_code(curPair.second);
		}
	}
	Send(pb::SW2C_SERVERS_HOT_RES_RELOAD_RET,ResMsg);
}

void WebServerSession::HandleCreatGiftCode( NetPack &packet )
{
	//operate  1.新增礼包类型 2，新增兑换码，每次批次号需要不同，数量最好不要超过10000 ，3.新增礼包类型，同时新增兑换码，每次批次号需要不同
	//reuse  0.每个码只能被使用一次,1.同一个类型可以被同一个玩家多次使用，同一个码只能使用一次,2.同一个兑换码可以被多个玩家使用，每个玩家只能使用一次
	pb::CreateGiftCode info ;
	packet >> info ;
	if (info.operate() == 1)
	{
		GiftBagTable giftBag;
		giftBag.LoadFrom(info);
		GiftBagTable* curBag = sCenterDataMgr.AddNewGiftBagTable(giftBag);
		if (curBag)
		{
			pb::ServerResponse ResMsg;
			ResMsg.set_code(200);
			pb::ServerCode* serSt = ResMsg.add_reg();
			serSt->set_id(curBag->giftId);
			serSt->set_code(200);
			Send(pb::SW2C_CREATE_GIFT_CODE_RET,ResMsg);
		}
	}
	else if (info.operate() == 2)
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(200);
		StringVectorPtr ptrCode = sCenterDataMgr.getGiftCode(info.count());
		BOOST_FOREACH(std::string& rCode,*ptrCode)
		{
			GiftCodeTable giftCode;
			giftCode.LoadFrom(info);
			giftCode.giftCode = rCode;
			sCenterDataMgr.AddNewGiftCodeTable(giftCode);
		}
		Send(pb::SW2C_CREATE_GIFT_CODE_RET,ResMsg);
	}
	else if (info.operate() == 3)
	{
		GiftBagTable giftBag;
		giftBag.LoadFrom(info);
		pb::ServerResponse ResMsg;
		ResMsg.set_code(200);
		GiftBagTable* curBag = sCenterDataMgr.AddNewGiftBagTable(giftBag);
		if (curBag)
		{
			StringVectorPtr ptrCode = sCenterDataMgr.getGiftCode(info.count());
			BOOST_FOREACH(std::string& rCode,*ptrCode)
			{
				GiftCodeTable giftCode;
				giftCode.LoadFrom(info);
				giftCode.giftCode = rCode;
				sCenterDataMgr.AddNewGiftCodeTable(giftCode);
			}
		}
		Send(pb::SW2C_CREATE_GIFT_CODE_RET,ResMsg);
	}
}

void WebServerSession::HandleGetPlayerOnlineCount( NetPack &packet )
{
	pb::OnlinePlayerCount info ;
	packet >> info ;
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find(info.sid());
	if( itr != games.end() )
	{
		itr->second->Send( pb::SC2G_GET_ONLINE_PLAYER_COUNT, GetSessionId(), info ) ;
	}
	else
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(0);
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(info.sid());
		serSt->set_code(0);
		Send(pb::SW2C_GET_ONLINE_PLAYER_COUNT_RET,ResMsg);
	}
	

}

void WebServerSession::HandleGetServerInfo( NetPack &packet )
{
	pb::OnlinePlayerCount info ;
	packet >> info ;
	SessionByServerIdMap &games =sCenterSvr.GetGameSessions() ;
	SessionByServerIdMapItr itr =games.find(info.sid());
	if( itr != games.end() )
	{
		itr->second->Send( pb::SC2G_GET_SERVER_INFO, GetSessionId(), info ) ;
	}
	else
	{
		pb::ServerResponse ResMsg;
		ResMsg.set_code(0);
		pb::ServerCode* serSt = ResMsg.add_reg();
		serSt->set_id(info.sid());
		serSt->set_code(0);
		Send(pb::SW2C_GET_SERVER_INFO_RET,ResMsg);
	}
}
