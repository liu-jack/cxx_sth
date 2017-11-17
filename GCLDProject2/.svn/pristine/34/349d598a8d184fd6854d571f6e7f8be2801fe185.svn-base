#include <fstream>
#include <string>

#include "CenterServerClient.h"
#include "LobbyServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../CrossServer.h"
#include "DbTool.h"
#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


CenterServerClient::HandlerArray CenterServerClient::s_handlers =RegisteHandlers() ;


CenterServerClient::HandlerArray CenterServerClient::RegisteHandlers( void )
{
	//LLOG("{CrossServer}CenterServerClient::RegisteHandlers");
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]            =&CenterServerClient::HandlePong ;
	handlers[ pb::SR2C_REGISTER_RET ] =&CenterServerClient::HandleRegisterRet ;
	handlers[ pb::SC2R_ADD_EVENT_LOG_SERVER ]     =&CenterServerClient::HandleGetEventLogServersRet ;
	handlers[ pb::SR2C_GET_EVENT_LOG_SERVER_RET ] =&CenterServerClient::HandleGetEventLogServersRet ;

	return handlers ;
}


CenterServerClient::CenterServerClient( void )
{
	//LLOG("{CrossServer}CenterServerClient::CenterServerClient");
	SetClientType( PACKET_TYPE_SERVER_CROSS ) ;
	SetDstServerType( PACKET_TYPE_SERVER_CENTER ) ;

	SetPingDiffMS( sCrossSvr.Config().GetInt( "CenterServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sCrossSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	SetConnectNow() ;
}

CenterServerClient::~CenterServerClient( void )
{
}



void CenterServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("{CrossServer}CenterServerClient::ClientUpdate");

}

bool CenterServerClient::HandlePacket( NetPack &packet )
{
	//LLOG("{CrossServer}CenterServerClient::HandlePacket");
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
	//LLOG("{CrossServer}CenterServerClient::ClientConnect");
	SendRegisterInfo() ;
}

void CenterServerClient::SendPing( void )
{
	//LLOG("{CrossServer}CenterServerClient::SendPing");
	ServerClient::SendPing( pb::S_PING ) ;
}


void CenterServerClient::SendRegisterInfo( void )
{
	LLOG("{CrossServer}CenterServerClient::SendRegisterInfo");
	pb::SR2C_RegisterInfo msg ;
	msg.set_server_id( sCrossSvr.GetServerId() ) ;
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
	Send( pb::SR2C_REGISTER, msg ) ;
}



void CenterServerClient::HandlePong( NetPack &packet )
{
	//LLOG("{CrossServer}CenterServerClient::HandlePong");
	ServerClient::HandlePong( packet ) ;
}



void CenterServerClient::HandleRegisterRet( NetPack &packet )
{
	LLOG("{CrossServer}CenterServerClient::HandleRegisterRet");
	pb::SR2C_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			sCrossSvr.SetRegionId( retInfo.region_id() ) ;

			// Set lobby address
			const ConfigMgr &config =sCrossSvr.Config() ;
			std::string lobbyIp   =config.GetString( "LobbyServer.ip", "127.0.0.1" ) ;
			std::string lobbyPort =config.GetString( "LobbyServer.port", "55300" ) ;
			NLOG( "Setting lobby server client %s(%s) .", lobbyIp.c_str(), lobbyPort.c_str() ) ;
			sLobbyClient.SetDstInfo( lobbyIp, lobbyPort ) ;

			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;

			

		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s)  fail !: %s .", GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

void CenterServerClient::HandleGetEventLogServersRet( NetPack &packet )
{
	LLOG("{CrossServer}CenterServerClient::HandleGetEventLogServersRet");
	pb::GetEventLogServersRet info ;
	packet >> info ;

	const pb::Address &address =info.addresses() ;

	if( !sCrossSvr.AddOneEventLogServer( address.ip(), address.port() ) )
	{
		WLOG( "Event server had connect ?  %s, %s setting faild ??", address.ip().c_str(), address.port().c_str() ) ;
	}
}

