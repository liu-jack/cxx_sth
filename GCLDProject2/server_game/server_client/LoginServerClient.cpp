#include <fstream>
#include <string>

#include "LoginServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../GameServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"

#include "DbTool.h"


LoginServerClient::HandlerArray LoginServerClient::s_handlers =RegisteHandlers() ;


LoginServerClient::HandlerArray LoginServerClient::RegisteHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]                =&LoginServerClient::HandlePong ;
	handlers[ pb::SG2L_REGISTER_RET ]     =&LoginServerClient::HandleRegisterRet ;

	return handlers ;
}


LoginServerClient::LoginServerClient( void )
{
	SetClientType( PACKET_TYPE_SERVER_GAME ) ;
	SetDstServerType( PACKET_TYPE_SERVER_LOGIN ) ;

	SetPingDiffMS( sGameSvr.Config().GetInt( "LoginServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sGameSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	SetConnectNow() ;

	SetMaxConnectCount( 6 ) ;
}

LoginServerClient::~LoginServerClient( void )
{
}



void LoginServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{

}

bool LoginServerClient::HandlePacket( NetPack &packet )
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

void LoginServerClient::ClientConnect( void )
{
	SendRegisterInfo() ;
}

void LoginServerClient::SendPing( void )
{
	ServerClient::SendPing( pb::S_PING ) ;
}

void LoginServerClient::HandlePong( NetPack &packet )
{
	ServerClient::HandlePong( packet ) ;
}

void LoginServerClient::SendRegisterInfo( void )
{
	pb::SG2L_RegisterInfo msg ;
	msg.set_server_id( sGameSvr.GetServerId() ) ;
	msg.set_region_id( sGameSvr.GetRegionId() ) ;
	msg.set_ip( sGameSvr.GetIP() ) ;
	msg.set_port( sGameSvr.GetPort() ) ;
	msg.set_player_count( sGameSvr.GetLogicalTaker()->GetSessionCount() ) ;
	msg.set_logical_update_ms( sGameSvr.GetLogicalTaker()->GetUpdateUseTimeMS() ) ;
	Send( pb::SG2L_REGISTER, msg ) ;
}

void LoginServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SG2L_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;
		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s)  fail !: %s .", GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

