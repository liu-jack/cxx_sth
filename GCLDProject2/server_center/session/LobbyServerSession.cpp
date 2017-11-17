
#include "LobbyServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../CenterServer.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


LobbyServerSession::HandlerArray LobbyServerSession::s_serverHandlers =RegisteServerHandlers() ;


LobbyServerSession::HandlerArray LobbyServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                           =&LobbyServerSession::HandlePing ;
	handlers[ pb::SB2C_REGISTER ]                   =&LobbyServerSession::HandleRegister ;
	handlers[ pb::SB2C_GET_EVENT_LOG_SERVER ]       =&LobbyServerSession::HandleGetEventLogServers ;

	return handlers ;
}



LobbyServerSession::LobbyServerSession( void )
{
}

LobbyServerSession::~LobbyServerSession( void )
{
	sCenterSvr.LobbyServerUnRegister( this ) ;
}


void LobbyServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool LobbyServerSession::HandleNetPack( NetPack &pack )
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


void LobbyServerSession::HandlePing( NetPack &packet )
{
	Session::HandlePing( packet, pb::S_PONG ) ;
}



void LobbyServerSession::HandleRegister( NetPack &packet )
{
	if( GetIsVerification() )
	{
		WLOG( "Had register, can not register again ........................" ) ;
		return ;
	}

	pb::SB2C_RegisterInfo info ;
	packet >> info ;
	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	pb::SB2C_RegisterInfoRet retInfo ;
	CenterServer::ServerRegisterResult ret =sCenterSvr.LobbyServerRegister( this ) ;
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

	Send( pb::SB2C_REGISTER_RET, retInfo ) ;
}


void LobbyServerSession::HandleGetEventLogServers( NetPack &packet )
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

	Send( pb::SC2B_ADD_EVENT_LOG_SERVER, retInfo ) ;
}
