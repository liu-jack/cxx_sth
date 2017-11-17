
#include "EventLogServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../CenterServer.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


EventLogServerSession::HandlerArray EventLogServerSession::s_serverHandlers =RegisteServerHandlers() ;


EventLogServerSession::HandlerArray EventLogServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]              =&EventLogServerSession::HandlePing ;
	handlers[ pb::SE2C_REGISTER ]       =&EventLogServerSession::HandleRegister ;

	return handlers ;
}



EventLogServerSession::EventLogServerSession( void )
{
}

EventLogServerSession::~EventLogServerSession( void )
{
	sCenterSvr.EventLogServerUnRegister( this ) ;
}


void EventLogServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool EventLogServerSession::HandleNetPack( NetPack &pack )
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


void EventLogServerSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::S_PONG ) ;
}



void EventLogServerSession::HandleRegister( NetPack &packet )
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
	CenterServer::ServerRegisterResult ret =sCenterSvr.EventLogServerRegister( this ) ;
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

	Send( pb::SE2C_REGISTER_RET, retInfo ) ;
}


