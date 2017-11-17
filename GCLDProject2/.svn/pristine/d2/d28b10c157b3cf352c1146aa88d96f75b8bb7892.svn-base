#include <fstream>
#include <string>

#include "LobbyServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../CrossServer.h"
#include "DbTool.h"
#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "cross_wall/CrossWorldMap.h"
#include "CrossWar.pb.h"
#include "cross_wall/CrossWarManager/CrossWarMgr.h"
#include "Map/country/country.h"

LobbyServerClient::HandlerArray LobbyServerClient::s_handlers =RegisteHandlers() ;


LobbyServerClient::HandlerArray LobbyServerClient::RegisteHandlers( void )
{
	//LLOG("{CrossServer}LobbyServerClient::RegisteHandlers");
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]                =&LobbyServerClient::HandlePong ;
	handlers[ pb::SR2B_REGISTER_RET ]     =&LobbyServerClient::HandleRegisterRet ;
	handlers[ pb::SL2R_NOTICE_WAR_START ]     =&LobbyServerClient::HandleWarStart ;
	handlers[ pb::SL2S_NOTICE_WAR_STATE ]	=&LobbyServerClient::HandleWarState;
	handlers[ pb::SL2R_DEVIDE_CROSS_SERVER ]	=&LobbyServerClient::HandleDevides;

	return handlers ;
}


LobbyServerClient::LobbyServerClient( void )
{
	//LLOG("{CrossServer}LobbyServerClient::LobbyServerClient");
	SetClientType( PACKET_TYPE_SERVER_CROSS ) ;
	SetDstServerType( PACKET_TYPE_SERVER_LOBBY ) ;

	SetPingDiffMS( sCrossSvr.Config().GetInt( "LobbyServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sCrossSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	SetConnectNow() ;

	//SetMaxConnectCount( 6 ) ;
}

LobbyServerClient::~LobbyServerClient( void )
{
}



void LobbyServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("{CrossServer}LobbyServerClient::ClientUpdate");

}

bool LobbyServerClient::HandlePacket( NetPack &packet )
{
	//LLOG("{CrossServer}LobbyServerClient::HandlePacket");
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
	LLOG("{CrossServer}LobbyServerClient::ClientConnect");
	SendRegisterInfo() ;
}

void LobbyServerClient::SendPing( void )
{
	//LLOG("{CrossServer}LobbyServerClient::SendPing");
	ServerClient::SendPing( pb::S_PING ) ;
}

void LobbyServerClient::HandlePong( NetPack &packet )
{
	//LLOG("{CrossServer}LobbyServerClient::HandlePong");
	ServerClient::HandlePong( packet ) ;
}

void LobbyServerClient::SendRegisterInfo( void )
{
	LLOG("{CrossServer}LobbyServerClient::SendRegisterInfo");
	pb::SR2B_RegisterInfo msg ;
	msg.set_server_id( sCrossSvr.GetServerId() ) ;
	msg.set_region_id(sCrossSvr.GetRegionId());
	msg.set_ip( sCrossSvr.GetIP() ) ;
	msg.set_port( sCrossSvr.GetPort() ) ;
	/*const String_Server *pNames =DbTool::Get<String_Server>( sCrossSvr.GetServerId() ) ;
	if( pNames != NULL )
	{
		for( int i =LANG_SC; i < LANG_COUNT; ++i )
		{
			msg.add_names( pNames->str[i] ) ;
		}
	}
	else
	{
		ELOG( "CrossServer id %u can not find name info ?????????????????? ", sCrossSvr.GetServerId()  ) ;
	}*/
	Send( pb::SR2B_REGISTER, msg ) ;
}

void LobbyServerClient::HandleRegisterRet( NetPack &packet )
{
	LLOG("{CrossServer}LobbyServerClient::HandleRegisterRet");
	pb::SR2B_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;
		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s) fail ! : %s .",GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

void LobbyServerClient::HandleWarState( NetPack &packet )
{
	pb::GS2C_Notice_CrossWar_State msg;
	packet >> msg;

	LLOG("[1002] HandleWarState,state = %d",msg.curstate());

	sCrossWarMgr.SetCrossState(msg.curstate(),msg.stateendtime());

}

void LobbyServerClient::HandleWarStart( NetPack &packet )
{
	LLOG("[1002] LobbyServerClient::HandleWarStart");
	sCrossWorldMap.Init();
	//sCrossWarMgr.m_cState = State_Collect_Info;	
}

void LobbyServerClient::HandleDevides(NetPack &packet)
{
	pb::GS2L_Devide_Country_Infos msg; packet >> msg;

	LLOG("HandleDevides");

	sCrossWorldMap.m_uniqueToCountry.clear();

	for(int i=0; i<msg.countrys_size(); i++)
	{
		pb::Devide_Country_Info* countryone = msg.mutable_countrys(i);
		if(countryone)
		{
			uint32 oriCountryId = countryone->orig_country_id();
			uint32 devideCountryId = countryone->devide_country_id();
			uint32 uniqueid = countryone->unique_index_id();
			uint32 pros_degree = countryone->pros_degree();
			uint32 serverid = countryone->server_id();
			Country* country = sCrossWorldMap.GetCountry(devideCountryId);
			if(country)
			{
				sCrossWorldMap.m_uniqueToCountry.insert(std::make_pair(uniqueid,devideCountryId));
				
				country->m_Prosperity_Degree = pros_degree;
				country->OriGameCountryId = oriCountryId;
				country->OriGameServerId = serverid;
				country->SetCrossUniqueID(uniqueid);
			}
		}
	}
}
			