#include <fstream>
#include <string>

#include "LobbyServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../GameServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"

#include "DbTool.h"
#include "../Map/WorldMap.h"
#include "../Map/country/country.h"
#include "CrossWar.pb.h"
#include "../session/PlayerPool.h"
#include "Palace/PalaceMgr.h"
#include "Palace.pb.h"
#include "Map/city/city.h"
#include "Activity/ActMgr.h"
#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"

LobbyServerClient::HandlerArray LobbyServerClient::s_handlers =RegisteHandlers() ;

static void ClearCallUp(City* city)
{
	city->ClearCallUp();
}

LobbyServerClient::HandlerArray LobbyServerClient::RegisteHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]                =&LobbyServerClient::HandlePong ;
	handlers[ pb::SG2B_REGISTER_RET ]     =&LobbyServerClient::HandleRegisterRet ;
	handlers[ pb::SL2G_COLLECT_COUNTRY_INFO] = &LobbyServerClient::HandleGetCountryInfo ;
	handlers[ pb::SB2G_UPDATE_CROSS_WAR_INFO] = &LobbyServerClient::HandleUpdateCrossWarInfo ;
	handlers[ pb::SL2G_NOTICE_WAR_START ]     = &LobbyServerClient::HandleWarStart ;
	handlers[ pb::SL2G_DEVIDE_CROSS_SERVER ] = &LobbyServerClient::HandleDevideCrossServer;
	handlers[ pb::SL2S_NOTICE_WAR_STATE ]	=&LobbyServerClient::HandleWarState;
	handlers[ pb::SB2G_Ask_Cross_Info_Ret ] = &LobbyServerClient::HandleAskCrossInfoRet;

	return handlers ;
}


LobbyServerClient::LobbyServerClient( void )
{
	SetClientType( PACKET_TYPE_SERVER_GAME ) ;
	SetDstServerType( PACKET_TYPE_SERVER_LOBBY ) ;

	SetPingDiffMS( sGameSvr.Config().GetInt( "LobbyServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sGameSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	SetConnectNow() ;
}

LobbyServerClient::~LobbyServerClient( void )
{
}



void LobbyServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{

}

bool LobbyServerClient::HandlePacket( NetPack &packet )
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

void LobbyServerClient::ClientConnect( void )
{
	SendRegisterInfo() ;
}

void LobbyServerClient::SendPing( void )
{
	ServerClient::SendPing( pb::S_PING ) ;
}

void LobbyServerClient::HandlePong( NetPack &packet )
{
	ServerClient::HandlePong( packet ) ;
}

void LobbyServerClient::SendRegisterInfo( void )
{
	pb::SG2B_RegisterInfo msg ;
	msg.set_server_id( sGameSvr.GetServerId() ) ;
	msg.set_region_id(sGameSvr.GetRegionId());
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
	Send( pb::SG2B_REGISTER, msg ) ;
}

void LobbyServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SG2B_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;
			//请求跨服信息
			AskCrossWarInfoFromLobby();
		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s)  fail !: %s .", GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

void LobbyServerClient::HandleGetCountryInfo( NetPack &packet )
{
	LLOG("[1002] get collect country info msg");
	pb::SG2B_Country_Info countryMsg;
	countryMsg.set_is_can_cross_war(false);
	if (sWorldMap.AnyCountryCanCrossWar())	//判断是否能进行跨服战
	{
		LLOG("[1002] can enter cross war");
		countryMsg.set_is_can_cross_war(true);
		sWorldMap.SaveCountryDevideinfo(countryMsg);
		sActMgr.isCrossWarCover = true;
	}
	Send(pb::SG2L_COLLECT_COUNTRY_INFO_RET,countryMsg);
}

void LobbyServerClient::HandleUpdateCrossWarInfo( NetPack &packet )
{
	LLOG("LobbyServerClient::HandleUpdateCrossWarInfo");
	pb::SB2G_Country_War_Info retInfo ;
	packet >> retInfo ;
	for (int i = 0; i< retInfo.war_info_size(); i++)
	{
		sWorldMap.UpdateCountryWarInfo(retInfo.war_info(i));
	}
}

void LobbyServerClient::HandleWarState( NetPack &packet )
{
	pb::GS2C_Notice_CrossWar_State msg;
	packet >> msg;
	if(sWorldMap.AnyCountryCanCrossWar())
	{
		for(int i=1; i<=3; i++)
		{
			if (Country* country = sWorldMap.GetCountry(i))
			{
				country->SetCrossState(msg.curstate());
				country->cross_war_state_end_time = msg.stateendtime();

				pb::GS2C_Notice_CrossWar_State sendmsg;
				sendmsg.set_curstate(country->GetCrossState());
				sendmsg.set_stateendtime(msg.stateendtime());
				sPlayerPool.BroadcastMsgToAllPlayerByCountry(pb::SMSG_NOTICE_CROSS_UPDATE,sendmsg,i);
				LLOG("[1002] Notice All Client,current stage = %d, nexttime = %llu",(int)msg.curstate(),msg.stateendtime());
			}
		}

		switch(msg.curstate())
		{
		case pb::State_Null:
			//不主动处理，等待cross处理完数据返回后再断开连接
			break;
		case pb::State_Collect_Info:
			break;
		case pb::State_Devide:
			break;
		case pb::State_Prepare:
			break;
		case pb::State_Preview:
			//发送官员信息到跨服
			SendOfficeInfoToCross();
			break;
		case pb::State_Start:
			//清理地图征召令
			sWorldMap.ForEachCity(boost::bind(&ClearCallUp, _1));
			break;
		case pb::State_Award:
			break;
		default:
			break;
		}

		if(msg.curstate() == pb::State_Start)
		{
			FOREACH_PLAYERPTR(playerId, playerPtr)
			{
				if(playerPtr->m_AutoCountryWarLog->GetAutoCountryWar())	//有自动国战
				{
					playerPtr->m_AutoCountryWarLog->Stop(true);	//停止自动国战
				}
			}
		}

	}
}

void LobbyServerClient::HandleWarStart( NetPack &packet )
{
	if(sWorldMap.AnyCountryCanCrossWar())
	{
		for(int i=1; i<=3; i++)
		{
			if (Country* country = sWorldMap.GetCountry(i))
			{
				country->isOpenedCrossWar = true;
				//country->SetCrossState(pb::CROSS_STATE_END);
			}
		}
	}
}

void LobbyServerClient::HandleDevideCrossServer( NetPack &packet )
{
	LLOG("[1002] LobbyServerClient::HandleDevideCrossServer");
	pb::CountryWithAddress msginfo;
	packet >> msginfo;

	const pb::Address &address =msginfo.addresses();
	if( !sGameSvr.AddOneCrossServer( address.ip(), address.port() ) )
	{
		WLOG( "[1002] Loign server %s, %s is had connected in game server ??", address.ip().c_str(), address.port().c_str() ) ;
		
		LLOG("[1002] Server Is Already Connected Just Use, ServerIp = %s, Port = %s, CountryID = %d",address.ip().c_str(),address.port().c_str(),msginfo.countryid());
		if (Country* country = sWorldMap.GetCountry(msginfo.countryid()))
		{
			country->SetCrossConnectSession(SessionAddress( address.port(), address.ip() ));
			//country->SetCrossState(pb::State_Prepare);
			country->cross_war_state_end_time = msginfo.prepareendtime();
			country->SetCrossUniqueID(msginfo.uniqueid());
		}
	}
	else
	{
		LLOG("[1002] Get One Cross Server Ip From Lobby Server, ServerIp = %s, Port = %s, CountryID = %d",address.ip().c_str(),address.port().c_str(),msginfo.countryid());
		if (Country* country = sWorldMap.GetCountry(msginfo.countryid()))
		{
			country->SetCrossConnectSession(SessionAddress( address.port(), address.ip() ));
			//country->SetCrossState(pb::State_Prepare);	//进入筹备阶段
			country->cross_war_state_end_time = msginfo.prepareendtime();
			country->SetCrossUniqueID(msginfo.uniqueid());
		}
	}

	pb::GS2C_Notice_CrossWar_State sendmsg;
	sendmsg.set_curstate(pb::State_Prepare);
	sendmsg.set_stateendtime(msginfo.prepareendtime());
	//sendmsg.set_crosscountryid(msginfo.uniqueid());
	sPlayerPool.BroadcastMsgToAllPlayerByCountry(pb::SMSG_NOTICE_CROSS_START, sendmsg,msginfo.countryid());
	LLOG("[1002] Notice All Client, countryid = %d",msginfo.countryid());

	//FOREACH_PLAYERPTR(playerid,playerptr)
	//{
	//	if(playerptr->GetCountryId() == msginfo.countryid())
	//		playerptr->SetCrossUniqueID();
	//}
}

void LobbyServerClient::SendOfficeInfoToCross()
{
	for(int i=1; i<=3; i++)
	{
		if (Country* country = sWorldMap.GetCountry(i))
		{
			pb::GS2C_Palace_All_Info msg;

			sPalaceMgr(i)->SaveTo(i,msg);
			msg.set_unique_country_id(country->cross_uniqueID);
			country->SendCrossServerMsg(pb::SG2R_OFFICE_INFO,msg,PACKET_TYPE_SERVER_GAME);
		}
	}
}

void LobbyServerClient::AskCrossWarInfoFromLobby()
{
	LLOG("[1002] Send Ask Cross Info");
	NetPack packet( pb::SG2B_Ask_Cross_Info, sizeof(uint32) + 2 ) ;
	packet << sGameSvr.GetServerId();
	Send( packet ) ;
}

void LobbyServerClient::HandleAskCrossInfoRet( NetPack &packet )
{
	LLOG("[1002] LobbyServerClient::HandleAskCrossInfoRet");
	pb::SL2G_DevideCrossServers msg;
	packet >> msg;
	for(int i = 0; i < msg.country_addresses_size(); i++)
	{
		LLOG("[1002] country %d",i);
		const pb::CountryWithAddress& countryinfoone = msg.country_addresses(i);
		uint32 countryid = countryinfoone.countryid();
		LLOG("[1002] countryid=%d",countryid);
		if(Country* countryone = sWorldMap.GetCountry(countryid))
		{
			LLOG("[1002] uniqueid1=%d, uniqueid2=%d",countryone->GetCrossUniqueID(),countryinfoone.uniqueid());
			if(countryone->GetCrossUniqueID() != countryinfoone.uniqueid())
			{
				sGameSvr.AddOneCrossServer(countryinfoone.addresses().ip(), countryinfoone.addresses().port());
				countryone->SetCrossUniqueID(countryinfoone.uniqueid());
				countryone->SetCrossState(msg.curstate());
				countryone->cross_war_state_end_time = countryinfoone.prepareendtime();
				countryone->isOpenedCrossWar = true;
				countryone->SetCrossConnectSession(SessionAddress( countryinfoone.addresses().port(),countryinfoone.addresses().ip()));
				sActMgr.isCrossWarCover = true;
			}
		}
	}
}