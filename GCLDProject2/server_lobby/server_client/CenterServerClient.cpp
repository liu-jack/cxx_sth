#include <fstream>
#include <string>

#include "CenterServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../LobbyServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


CenterServerClient::HandlerArray CenterServerClient::s_handlers =RegisteHandlers() ;


CenterServerClient::HandlerArray CenterServerClient::RegisteHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]            =&CenterServerClient::HandlePong ;
	handlers[ pb::SB2C_REGISTER_RET ] =&CenterServerClient::HandleRegisterRet ;
	handlers[ pb::SC2B_ADD_EVENT_LOG_SERVER ]          =&CenterServerClient::HandleGetEventLogServersRet;
	handlers[ pb::SB2C_GET_EVENT_LOG_SERVER_RET ]      =&CenterServerClient::HandleGetEventLogServersRet;

	

	return handlers ;
}


CenterServerClient::CenterServerClient( void )
{
	SetClientType( PACKET_TYPE_SERVER_LOBBY ) ;
	SetDstServerType( PACKET_TYPE_SERVER_CENTER ) ;

	SetPingDiffMS( sLobbySvr.Config().GetInt( "CenterServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sLobbySvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
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
	pb::SB2C_RegisterInfo msg ;

	msg.set_ip( sLobbySvr.GetIP() ) ;
	msg.set_port( sLobbySvr.GetPort() ) ;

	Send( pb::SB2C_REGISTER, msg ) ;
}



void CenterServerClient::HandlePong( NetPack &packet )
{
	ServerClient::HandlePong( packet ) ;
}



void CenterServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SB2C_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			//sLobbySvr.SetRegionId( retInfo.region_id() ) ;
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
	pb::GetEventLogServersRet info ;
	packet >> info ;

	const pb::Address &address =info.addresses() ;

	if( !sLobbySvr.AddOneEventLogServer( address.ip(), address.port() ) )
	{
		WLOG( "Event server had connect ?  %s, %s setting faild ??", address.ip().c_str(), address.port().c_str() ) ;
	}
}

