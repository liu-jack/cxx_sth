
#include "LoginServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../EventLogServer.h"
#include "../data/EventLogDataMgr.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


LoginServerSession::HandlerArray LoginServerSession::s_serverHandlers =RegisteServerHandlers() ;


LoginServerSession::HandlerArray LoginServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]              =&LoginServerSession::HandlePing ;
	handlers[ pb::SL2E_REGISTER ]       =&LoginServerSession::HandleRegister ;
	handlers[ pb::SL2E_EVENT ]          =&LoginServerSession::HandleServerEvent ;

	return handlers ;
}



LoginServerSession::LoginServerSession( void )
{
}

LoginServerSession::~LoginServerSession( void )
{
	sEventLogSvr.LoginServerUnRegister( this ) ;
}


void LoginServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool LoginServerSession::HandleNetPack( NetPack &pack )
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


void LoginServerSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::S_PONG ) ;
}



void LoginServerSession::HandleRegister( NetPack &packet )
{
	if( GetIsVerification() )
	{
		WLOG( "Had register, can not register again ........................" ) ;
		return ;
	}

	pb::SL2E_RegisterInfo info ;
	packet >> info ;
	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	SetPairId( SessionPairId( info.server_id(), info.region_id() ) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	pb::SL2E_RegisterInfoRet retInfo ;
	EventLogServer::ServerRegisterResult ret =sEventLogSvr.LoginServerRegister( this ) ;
	switch( ret )
	{
	case EventLogServer::SERVER_REGISTER_OK:
		{
			NLOG( "%s register %s,SessionId:%llu, pairID(r:%u,s:%u), address(ip:%s, p:%s) register ok"
				, GetSessionTypeName(),sEventLogSvr.GetServerTypeName(), GetSessionId(), GetPairId().second, GetPairId().first, GetAddress().second.c_str(), GetAddress().first.c_str() ) ;
		} break ;

	case EventLogServer::SERVER_REGISTER_FAIL_ID:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ID ) ;
		} break ;

	case EventLogServer::SERVER_REGISTER_FAIL_ADDRESS:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ADDRESS ) ;
		} break ;

	default:
		ASSERT( false ) ;
	}

	Send( pb::SL2E_REGISTER_RET, retInfo ) ;
}


void LoginServerSession::HandleServerEvent( NetPack &packet )
{
	pb::Server_Event evtMsg;
	packet >> evtMsg ;

	EventLog eventLog ;
	eventLog.ReadFrom( evtMsg, GetPairId().second, GetPairId().first ) ;

	sDataMgr.SaveNewEventlog( eventLog ) ;
}