
#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../EventLogServer.h"
#include "../data/EventLogDataMgr.h"


#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


GameServerSession::HandlerArray GameServerSession::s_serverHandlers =RegisteServerHandlers() ;


GameServerSession::HandlerArray GameServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]              =&GameServerSession::HandlePing ;
	handlers[ pb::SG2E_REGISTER ]       =&GameServerSession::HandleRegister ;
	handlers[ pb::SG2E_EVENT ]          =&GameServerSession::HandleServerEvent ;

	return handlers ;
}



GameServerSession::GameServerSession( void )
{
}

GameServerSession::~GameServerSession( void )
{
	sEventLogSvr.GameServerUnRegister( this ) ;
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

	pb::SG2E_RegisterInfo info ;
	packet >> info ;
	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	if( info.region_id() != sEventLogSvr.GetRegionId() )
	{
		SetIsNeedClose( true ) ;
		NLOG( "%s, address(%s, %s) register center id is %d not %d will be close"
			, GetSessionTypeName(), GetAddress().second.c_str(), GetAddress().first.c_str(), info.region_id(), sEventLogSvr.GetRegionId() ) ;

		pb::SG2L_RegisterInfoRet retInfo ;
		retInfo.set_result( pb::SERVER_REGISTER_FAIL_DIFF_CENTER_ID ) ;
		Send( pb::SG2L_REGISTER_RET, retInfo ) ;

		return ;
	}

	SetPairId( SessionPairId( info.server_id(), info.region_id() ) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	pb::SG2E_RegisterInfoRet retInfo ;
	EventLogServer::ServerRegisterResult ret =sEventLogSvr.GameServerRegister( this ) ;
	switch( ret )
	{
	case EventLogServer::SERVER_REGISTER_OK:
		{
			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;

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

	Send( pb::SG2E_REGISTER_RET, retInfo ) ;
}



void GameServerSession::HandleServerEvent( NetPack &packet )
{
	pb::Server_Event evtMsg;
	packet >> evtMsg ;

	EventLog eventLog ;
	eventLog.ReadFrom( evtMsg, GetPairId().second, GetPairId().first ) ;

	sDataMgr.SaveNewEventlog( eventLog ) ;
}
