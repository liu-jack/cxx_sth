
#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "../LobbyServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "../logic/country/war/CrossWarDestServerMgr.h"
#include "Country.pb.h"

GameServerSession::HandlerArray GameServerSession::s_serverHandlers =RegisteServerHandlers() ;


GameServerSession::HandlerArray GameServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                         =&GameServerSession::HandlePing ;
	handlers[ pb::SG2B_REGISTER ]                  =&GameServerSession::HandleRegister ;
	handlers[ pb::SG2L_COLLECT_COUNTRY_INFO_RET ]  =&GameServerSession::HandleCollectCountryInfo;
	handlers[ pb::SG2B_Ask_Cross_Info ]			   =&GameServerSession::HandleCrossInfo;

	return handlers ;
}



GameServerSession::GameServerSession( void )
{
}

GameServerSession::~GameServerSession( void )
{
	sLobbySvr.GameServerUnRegister( this ) ;
}


void GameServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
}


bool GameServerSession::HandleNetPack( NetPack &pack )
{
	// All server packet !

	int opCode               =pack.GetOpcode() ;
	//LLOG("HandleNetPack opcode=%d",opCode);
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

	pb::SG2B_RegisterInfo info ;
	packet >> info ;

	LLOG("game serverid = %d, region_id = %d",info.server_id(),info.region_id());
	SetPairId( SessionPairId( info.server_id(), info.region_id() ) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	pb::SG2B_RegisterInfoRet retInfo ;
	LobbyServer::ServerRegisterResult ret =sLobbySvr.GameServerRegister( this ) ;
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

	Send( pb::SG2B_REGISTER_RET, retInfo ) ;
}


void GameServerSession::HandleCollectCountryInfo( NetPack &packet )
{
	LLOG("[1002] Get Collect Country Info");
	pb::SG2B_Country_Info info;
	packet >> info ;
	
	sCrossWarDestSerMgr.HandleServerInfo(info,GetPairId());
}

void GameServerSession::HandleCrossInfo(NetPack &packet)
{
	LLOG("[1002] Get Handle Cross Info");
	uint32 serverid;
	packet >> serverid;
	
	if(sCrossWarDestSerMgr.m_CrossWarState != pb::State_Null)
	{
		pb::SL2G_DevideCrossServers sendmsg;
		sCrossWarDestSerMgr.GetCrossInfoInOneSession(serverid,sendmsg);
		Send( pb::SB2G_Ask_Cross_Info_Ret, sendmsg);
	}
}