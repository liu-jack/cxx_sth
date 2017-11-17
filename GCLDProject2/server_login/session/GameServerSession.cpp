
#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "../LoginServer.h"

#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "../data/Account.h"
#include "../data/LoginDataMgr.h"

GameServerSession::HandlerArray GameServerSession::s_serverHandlers =RegisteServerHandlers() ;


GameServerSession::HandlerArray GameServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]              =&GameServerSession::HandlePing ;
	handlers[ pb::SG2L_REGISTER ]       =&GameServerSession::HandleRegister ;
	handlers[ pb::SG2L_BROADCAST ]       =&GameServerSession::HandleGameServerBroadCast ;

	return handlers ;
}



GameServerSession::GameServerSession( void )
{
	SetPlayerCount( 0 ) ;
	SetLogicalUpdateMS( 0 ) ;
}

GameServerSession::~GameServerSession( void )
{
	sLoginSvr.GameServerUnRegister( this ) ;
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

	pb::SG2L_RegisterInfo info ;
	packet >> info ;
	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	if( info.region_id() != sLoginSvr.GetRegionId() )
	{
		SetIsNeedClose( true ) ;
		NLOG( "%s, address(%s, %s) register center id is %d not %d will be close"
			, GetSessionTypeName(), GetAddress().second.c_str(), GetAddress().first.c_str(), info.region_id(), sLoginSvr.GetRegionId() ) ;

		pb::SG2L_RegisterInfoRet retInfo ;
		retInfo.set_result( pb::SERVER_REGISTER_FAIL_DIFF_CENTER_ID ) ;
		Send( pb::SG2L_REGISTER_RET, retInfo ) ;

		return ;
	}

	SetPlayerCount( info.player_count() ) ;
	SetLogicalUpdateMS( info.logical_update_ms() ) ;

	SetPairId( SessionPairId( info.server_id(), info.region_id() ) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	pb::SG2L_RegisterInfoRet retInfo ;
	LoginServer::ServerRegisterResult ret =sLoginSvr.GameServerRegister( this ) ;
	switch( ret )
	{
	case LoginServer::SERVER_REGISTER_OK:
		{
			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;

			NLOG( "%s register %s,SessionId:%llu, (reg:%u,ser:%u), (%s:%s) register ok"
				, GetSessionTypeName(),sLoginSvr.GetServerTypeName(), GetSessionId(), GetPairId().second, GetPairId().first, GetAddress().second.c_str(), GetAddress().first.c_str() ) ;
			
		} break ;

	case LoginServer::SERVER_REGISTER_FAIL_ID:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ID ) ;
		} break ;

	case LoginServer::SERVER_REGISTER_FAIL_ADDRESS:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ADDRESS ) ;
		} break ;

	default:
		ASSERT( false ) ;
	}

	Send( pb::SG2L_REGISTER_RET, retInfo ) ;
}

void GameServerSession::HandleGameServerBroadCast( NetPack &packet )
{
	pb::G2LBroadcastHeader header ;
	packet >> header ;

	switch( header.type() )
	{
	case pb::UPDATE_GAME_SERVER_PERFORMANCE_INFO:
		{
			pb::SG2C2L_UpdatePerformanceInfo body ;
			packet >> body ;

			GameServerSession *pGameSession =sLoginSvr.GetGameServerSessoin( SessionPairId( body.server_id(), body.region_id() ) ) ;
			if( NULL == pGameSession )
			{
				ELOG( "CenterServerClient::HandleCenterBroadcast() can not find %d, %d", body.server_id(), body.region_id() ) ;
				break ;
			}

			pGameSession->SetPlayerCount( body.player_count() ) ;
			pGameSession->SetLogicalUpdateMS( body.logical_update_ms() ) ;

			ConfigServerInfo &info =sLoginSvr.GetOrCreateConfigServerInfo( pGameSession->GetPairId() ) ;
			ASSERT( info.m_pGameSession != NULL && info.m_pGameSession == pGameSession ) ;
			if( info.m_pGameSession == pGameSession )
			{
				info.UpdateState() ;

				SessionPairIdSet& pairSet = sLoginSvr.GetCombinServerSet(pGameSession->GetPairId());
				for (SessionPairIdSet::iterator iterSet = pairSet.begin(); iterSet != pairSet.end(); ++iterSet)
				{
					const SessionPairId& curPair = (*iterSet);
					if(sLoginSvr.IsInComfigMap(curPair))
					{
						ConfigServerInfo& curSconfig = sLoginSvr.GetOrCreateConfigServerInfo(curPair);
						if(curSconfig.m_pGameSession == NULL)
						{
							curSconfig.m_pGameSession = pGameSession;
						}
						curSconfig.UpdateState();
					}
				}
			}

			pb::SG2L_Update_Account_Login bodyRole ;
			packet >> bodyRole ;
			AccountRoleInfo* pRoleInfo = sDataMgr.GetRoleInfoByPlayerId(bodyRole.player_id());
			if (pRoleInfo)
			{
				pRoleInfo->ReadFrom(bodyRole);
				pRoleInfo->SetModify();
			}
			else
			{
				AccountRoleInfo roleInfo;
				roleInfo.ReadFrom(bodyRole);
				sDataMgr.AddAndSaveRoleInfo(roleInfo);
			}
		} break ;
	default:
		break;
	}
}
