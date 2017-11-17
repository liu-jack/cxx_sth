
#include "CrossServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../LobbyServer.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


CrossServerSession::HandlerArray CrossServerSession::s_serverHandlers =RegisteServerHandlers() ;


CrossServerSession::HandlerArray CrossServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                           =&CrossServerSession::HandlePing ;
	handlers[ pb::SR2B_REGISTER ]                   =&CrossServerSession::HandleRegister ;

	return handlers ;
}



CrossServerSession::CrossServerSession( void )
{
}

CrossServerSession::~CrossServerSession( void )
{
	sLobbySvr.CrossServerUnRegister( this ) ;
}


void CrossServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool CrossServerSession::HandleNetPack( NetPack &pack )
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


void CrossServerSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::S_PONG ) ;
}



void CrossServerSession::HandleRegister( NetPack &packet )
{
	if( GetIsVerification() )
	{
		WLOG( "Had register, can not register again ........................" ) ;
		return ;
	}

	pb::SR2B_RegisterInfo info ;
	packet >> info ;
	LLOG("cross serverid = %d, region_id = %d",info.server_id(),info.region_id());
	SetPairId( SessionPairId( info.server_id(), info.region_id()) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	pb::SR2B_RegisterInfoRet retInfo ;
	LobbyServer::ServerRegisterResult ret =sLobbySvr.CrossServerRegister( this ) ;
	switch( ret )
	{
	case LobbyServer::SERVER_REGISTER_OK:
		{
			std::vector< std::string > &vec =GetServerNames() ;
			vec.reserve( info.names_size() ) ;
			for( int i =0; i < info.names_size(); ++i )
			{
				vec.push_back( info.names( i ) ) ;
			}

			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;
			NLOG( "%s register %s,SessionId:%llu,(reg:%u,ser:%u), (%s:%s) register ok"
				, GetSessionTypeName(),sLobbySvr.GetServerTypeName(), GetSessionId(), GetPairId().second, GetPairId().first, GetAddress().second.c_str(), GetAddress().first.c_str() ) ;
		} break ;

	case LobbyServer::SERVER_REGISTER_FAIL_ID:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ID ) ;
		} break ;

	case LobbyServer::SERVER_REGISTER_FAIL_ADDRESS:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ADDRESS ) ;
		} break ;

	default:
		ASSERT( false ) ;
	}

	Send( pb::SR2B_REGISTER_RET, retInfo ) ;
}

