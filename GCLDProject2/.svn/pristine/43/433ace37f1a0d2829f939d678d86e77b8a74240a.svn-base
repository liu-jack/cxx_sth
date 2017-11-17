
#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "../CenterServer.h"
#include "WebServerSession.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"
#include "../CenterDataMgr.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "Web.pb.h"


GameServerSession::HandlerArray GameServerSession::s_serverHandlers =RegisteServerHandlers() ;


GameServerSession::HandlerArray GameServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                         =&GameServerSession::HandlePing ;
	handlers[ pb::SG2C_REGISTER ]                  =&GameServerSession::HandleRegister ;
	handlers[ pb::SG2C_GET_LOGIN_SERVERS ]         =&GameServerSession::HandleGetLoginServers ;
	handlers[ pb::SG2C_GET_CROSS_SERVERS ]         =&GameServerSession::HandleGetCrossServers ;
	handlers[ pb::SG2C_BROADCAST_TO_LOGIN_SERVER ] =&GameServerSession::HandleBroadcastToLoginServer;
	handlers[ pb::SC2G_WEB_GET_PLAYER_LIST_RET ]   =&GameServerSession::HandleC2GWebGetPayerListRet ;
	handlers[ pb::SG2C_GET_EVENT_LOG_SERVER ]      =&GameServerSession::HandleGetEventLogServers ;
	handlers[ pb::SC2G_WEB_PAY_RET ]               =&GameServerSession::HandleWebPayRet ;
	handlers[ pb::SC2G_SEND_PLAYER_MAIL_RET ]               =&GameServerSession::HandleSendPlayerMailRet;
	handlers[ pb::SC2G_PLAYER_FORBID_RET ]               =&GameServerSession::HandlePlayerForbidenRet;
	handlers[ pb::SC2G_SET_PLAYER_VIP_RET ]               =&GameServerSession::HandleSelectPlayerVipRet;
	handlers[ pb::SC2G_RECREATE_PAY_ORDER_RET ]               =&GameServerSession::HandleReCreatePayOrderRet;
	handlers[ pb::SG2C_EXCHANGE_GIFT_CODE ]               =&GameServerSession::HandleExchangeGiftCode;
	handlers[ pb::SG2C_FEEDBACK ]               =&GameServerSession::HandleFeedBack;
	handlers[ pb::SG2C_GET_ONLINE_PLAYER_COUNT_RET ]               =&GameServerSession::HandleGetOnlinePlayerCountRet;
	handlers[ pb::SG2C_GET_SERVER_INFO_RET ]               =&GameServerSession::HandleGetServerInfoRet;
	
	


	

	return handlers ;
}



GameServerSession::GameServerSession( void )
{
}

GameServerSession::~GameServerSession( void )
{
	sCenterSvr.GameServerUnRegister( this ) ;
}


void GameServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool GameServerSession::HandleNetPack( NetPack &pack )
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

void GameServerSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::S_PONG ) ;
}


void GameServerSession::HandleRegister( NetPack &packet )
{
	if( GetIsVerification() )
	{
		WLOG( "Had register, can not register again ........................" ) ;
		return ;
	}

	pb::SG2C_RegisterInfo info ;
	packet >> info ;

	SetPairId( SessionPairId( info.server_id(), sCenterSvr.GetRegionId() ) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	pb::SG2C_RegisterInfoRet retInfo ;
	CenterServer::ServerRegisterResult ret =sCenterSvr.GameServerRegister( this ) ;
	switch( ret )
	{
	case CenterServer::SERVER_REGISTER_OK:
		{
			std::vector< std::string > &vec =GetServerNames() ;
			vec.reserve( info.names_size() ) ;
			for( int i =0; i < info.names_size(); ++i )
			{
				vec.push_back( info.names( i ) ) ;
			}

			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;
			retInfo.set_region_id( sCenterSvr.GetRegionId() ) ;
			retInfo.set_web_url( sCenterSvr.Config().GetString( "Self.web_url", "127.0.0.1/" ) ) ;

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

	Send( pb::SG2C_REGISTER_RET, retInfo ) ;
}


void GameServerSession::HandleGetLoginServers( NetPack &packet )
{
	ASSERT( packet.DynamicBodySize() == 0 ) ;

	pb::SG2C_GetLoinServersRet retInfo ;

	SessionAddressSet &table =sCenterSvr.GetLoginAddressTable() ;
	for( SessionAddressSet::iterator itr =table.begin(); itr != table.end(); ++itr )
	{
		pb::Address *pAddress =retInfo.add_addresses() ;
		pAddress->set_ip( itr->second ) ;
		pAddress->set_port( itr->first ) ;
	}

	Send( pb::SG2C_GET_LOGIN_SERVERS_RET, retInfo ) ;
}

void GameServerSession::HandleGetCrossServers( NetPack &packet )
{
	ASSERT( packet.DynamicBodySize() == 0 ) ;

	pb::SG2C_GetCrossServersRet retInfo ;

	SessionAddressSet &table =sCenterSvr.GetCrossAddressTable() ;
	for( SessionAddressSet::iterator itr =table.begin(); itr != table.end(); ++itr )
	{
		pb::Address *pAddress =retInfo.add_addresses() ;
		pAddress->set_ip( itr->second ) ;
		pAddress->set_port( itr->first ) ;
	}

	Send( pb::SG2C_GET_CROSS_SERVERS_RET, retInfo ) ;
}

void GameServerSession::HandleBroadcastToLoginServer( NetPack &packet )
{
	packet.SetOpCode( pb::SC2L_BROADCAST ) ;
	sCenterSvr.BroadcastToLoginServer( packet ) ;
}

void GameServerSession::HandleC2GWebGetPayerListRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleC2GWebGetPayerListRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}

	packet.SetOpCode( pb::SW2C_GET_PLAYER_LIST_RET ) ;
	pSession->Send( packet ) ;
}


void GameServerSession::HandleGetEventLogServers( NetPack &packet )
{
	Session *pSesson =sCenterSvr.GetEventLogSession() ;
	if( pSesson == NULL )
	{
		return ;
	}

	pb::GetEventLogServersRet retInfo ;

	pb::Address *pAddress =retInfo.mutable_addresses() ;

	const SessionAddress &sAddress =pSesson->GetAddress() ;

	pAddress->set_ip( sAddress.second ) ;
	pAddress->set_port( sAddress.first ) ;

	Send( pb::SC2G_ADD_EVENT_LOG_SERVER, retInfo ) ;
}


void GameServerSession::HandleWebPayRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleWebPayRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}

	pb::WebPayRet retInfo ;
	packet >> retInfo ;
	
	ELOG( "Web pay return %s", retInfo.DebugString().c_str() ) ;

	packet.SetOpCode( pb::SW2C_PAY_RET ) ;
	pSession->Send( packet ) ;
}

void GameServerSession::HandleSendPlayerMailRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleSendPlayerMailRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}
	pb::ServerCode retInfo ;
	packet >> retInfo ;
	pb::ServerResponse ResMsg;
	ResMsg.set_code(retInfo.code());
	pb::ServerCode* serSt = ResMsg.add_reg();
	serSt->set_id(retInfo.id());
	serSt->set_code(retInfo.code());
	pSession->Send(pb::SW2C_SEND_PLAYER_MAIL_RET,ResMsg);
}

void GameServerSession::HandlePlayerForbidenRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandlePlayerForbidenRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}
	pb::ServerCode retInfo ;
	packet >> retInfo ;
	pb::ServerResponse ResMsg;
	ResMsg.set_code(retInfo.code());
	pb::ServerCode* serSt = ResMsg.add_reg();
	serSt->set_id(retInfo.id());
	serSt->set_code(retInfo.code());
	pSession->Send(pb::SW2C_PLAYER_FORBID_RET,ResMsg);
}

void GameServerSession::HandleSelectPlayerVipRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleSelectPlayerVipRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}
	pb::ServerCode retInfo ;
	packet >> retInfo ;
	pb::ServerResponse ResMsg;
	ResMsg.set_code(retInfo.code());
	pb::ServerCode* serSt = ResMsg.add_reg();
	serSt->set_id(retInfo.id());
	serSt->set_code(retInfo.code());
	pSession->Send(pb::SW2C_SET_PLAYER_VIP_RET,ResMsg);
}

void GameServerSession::HandleReCreatePayOrderRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleReCreatePayOrderRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}
	pb::ServerCode retInfo ;
	packet >> retInfo ;
	pb::ServerResponse ResMsg;
	ResMsg.set_code(retInfo.code());
	pb::ServerCode* serSt = ResMsg.add_reg();
	serSt->set_id(retInfo.id());
	serSt->set_code(retInfo.code());
	pSession->Send(pb::SW2C_RECREATE_PAY_ORDER_RET,ResMsg);
}


void GameServerSession::HandleExchangeGiftCode( NetPack &packet )
{
	//1,渠道不对，2,兑换码不存在，3,已过期，4，未生效，5,已失效。
	pb::SG2C_Exchange_Gift_Code msg;
	packet >> msg;
	pb::SG2C_Exchange_Gift_Code_Ret msgRet;
	msgRet.set_is_success(false);
	msgRet.set_player_id(msg.player_id());
	GiftCodeTable* forCode = sCenterDataMgr.GetGiftCodeTable(msg.gift_code());
	if (forCode)
	{
		if(forCode->CheckChannel(msg.channel()))
		{
			GiftBagTable* giftBag =  sCenterDataMgr.GetGiftBagTable(forCode->giftId);
			if (giftBag)
			{
				time_t curTime = ::time(NULL);
				bool isCanUse = true;
				if (giftBag->reuse > 0)
				{
					if (sCenterDataMgr.GiftBagCodeIsUsedByPlayerId(msg.player_id(),forCode->giftId))
					{
						msgRet.set_error_code(5);
						isCanUse = false;
					}
					else if (sCenterDataMgr.GiftCodeIsUsed(forCode->giftCode))
					{
						msgRet.set_error_code(5);
						isCanUse = false;
					}
				}
				else if (forCode->playerId > 0)
				{
					msgRet.set_error_code(5);
					isCanUse = false;
				}

				if (giftBag->valid != 1)
				{
					msgRet.set_error_code(5);
					isCanUse = false;
				}
				else if (curTime < giftBag->beginTime)
				{
					msgRet.set_error_code(4);
					isCanUse = false;
				}
				else if (curTime > giftBag->endTime)
				{
					msgRet.set_error_code(3);
					isCanUse = false;
				}
				if (isCanUse)
				{
					msgRet.set_is_success(true);
					msgRet.set_giftname(giftBag->giftName);
					msgRet.set_gifts(giftBag->gifts);
					if (giftBag->reuse > 0)
					{
						GiftCodeTableUse tempRecord;
						tempRecord.LoadFrom(*forCode);
						tempRecord.playerId = msg.player_id();
						tempRecord.serverId = GetPairId().first;
						tempRecord.useTime = ::time(NULL);
						sCenterDataMgr.AddNewGiftCodeUseTable(tempRecord);
					}
					else
					{
						forCode->playerId = msg.player_id();
						forCode->serverId = GetPairId().first;
						forCode->useTime = ::time(NULL);
						sCenterDataMgr.SaveGiftCodeTable(forCode);
					}
				}
			}
		}
		else
		{
			msgRet.set_error_code(1);
		}
	}
	else
	{
		msgRet.set_error_code(2);
	}
	Send(pb::SG2C_EXCHANGE_GIFT_CODE_RET,msgRet);
}

void GameServerSession::HandleFeedBack( NetPack &packet )
{
	pb::SG2C_FeedBack msg;
	packet >> msg;

	FeedbackTable fedTable;
	fedTable.LoadFrom(msg);
	pb::SG2C_FeedBack_Ret  fedBack;
	fedBack.set_player_id(msg.playerid());
	if(sCenterDataMgr.AddNewFeedBackTable(fedTable))
	{
		fedBack.set_is_success(true);
	}
	else
	{
		fedBack.set_is_success(false);
	}
	Send(pb::SG2C_FEEDBACK_RET,fedBack);
}


void GameServerSession::HandleGetOnlinePlayerCountRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleGetOnlinePlayerCountRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}
	pb::ServerCode retInfo ;
	packet >> retInfo ;
	pb::ServerResponse ResMsg;
	ResMsg.set_code(200);
	pb::ServerCode* serSt = ResMsg.add_reg();
	serSt->set_id(retInfo.id());
	serSt->set_code(retInfo.code());
	pSession->Send(pb::SW2C_GET_ONLINE_PLAYER_COUNT_RET,ResMsg);
}

void GameServerSession::HandleGetServerInfoRet( NetPack &packet )
{
	WebServerSession *pSession =sCenterSvr.GetWebServerSession( packet.GetSessionId() ) ;
	if( pSession == NULL )
	{
		ELOG( "GameServerSession::HandleGetOnlinePlayerCountRet() can not find sesion %llu", packet.GetSessionId()  ) ;
		return ;
	}
	pb::serverInfo retInfo ;
	packet >> retInfo ;
	pSession->Send(pb::SW2C_GET_SERVER_INFO_RET,retInfo);
}
