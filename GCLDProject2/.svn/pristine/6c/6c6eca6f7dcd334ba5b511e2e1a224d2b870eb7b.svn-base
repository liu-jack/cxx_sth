
#include "LoginServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../CenterServer.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"


LoginServerSession::HandlerArray LoginServerSession::s_serverHandlers =RegisteServerHandlers() ;


LoginServerSession::HandlerArray LoginServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                           =&LoginServerSession::HandlePing ;
	handlers[ pb::SL2C_REGISTER ]                   =&LoginServerSession::HandleRegister ;
	handlers[ pb::SL2C_GET_EVENT_LOG_SERVER ]       =&LoginServerSession::HandleGetEventLogServers ;

	return handlers ;
}



LoginServerSession::LoginServerSession( void )
{
}

LoginServerSession::~LoginServerSession( void )
{
	sCenterSvr.LoginServerUnRegister( this ) ;
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

	pb::SL2C_RegisterInfo info ;
	packet >> info ;
	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	pb::SL2C_RegisterInfoRet retInfo ;
	CenterServer::ServerRegisterResult ret =sCenterSvr.LoginServerRegister( this ) ;
	switch( ret )
	{
	case CenterServer::SERVER_REGISTER_OK:
		{
			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;
			retInfo.set_region_id( sCenterSvr.GetRegionId() ) ;
			retInfo.set_web_url( sCenterSvr.Config().GetString( "Self.web_url", "127.0.0.1/" ) ) ;
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

	Send( pb::SL2C_REGISTER_RET, retInfo ) ;
}


void LoginServerSession::HandleGetEventLogServers( NetPack &packet )
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

	Send( pb::SC2L_ADD_EVENT_LOG_SERVER, retInfo ) ;
}
