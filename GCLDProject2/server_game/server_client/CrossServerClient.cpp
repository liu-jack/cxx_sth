#include <fstream>
#include <string>

#include "CrossServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../GameServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"

#include "DbTool.h"

#include "object/Player.h"
#include "session/PlayerPool.h"

#include "CrossWar.pb.h"
#include "Map/WorldMap.h"
#include "Map/country/country.h"
#include "character/Character.h"
#include "character/CharacterStorage.h"
#include "Map/city/CityHandler.cpp"
#include "GameServer.h"
#include "Character.pb.h"
#include "utility/MsgTool.h"
#include "CrossLogic/ItemCharacterLogic.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "reward/reward.h"
#include "utility/Utility.h"
#include "SysEvent/SysEventMgr.h"

CrossServerClient::HandlerArray CrossServerClient::s_handlers =RegisteHandlers() ;


CrossServerClient::HandlerArray CrossServerClient::RegisteHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]                =&CrossServerClient::HandlePong ;
	handlers[ pb::SG2R_REGISTER_RET ]     =&CrossServerClient::HandleRegisterRet ;
	handlers[ pb::SG2R_UPDATE_PLAYER_VALUE] =&CrossServerClient::HandleUpdatePlayerValue;
	handlers[ pb::SR2G_QUIT_CROSS_SERVER ]		=&CrossServerClient::HandleQuitCrossWar;
	handlers[ pb::SR2G_Cross_Result_Country ]	=&CrossServerClient::HandleCrossResultCountry;
	handlers[ pb::SG2R_UPDATE_PLAYER_CHAR_VALUE] =&CrossServerClient::HandleUpdatePlayerCharValue;
	handlers[ pb::SR2G_NOTICE_CLOSE_CROSS]		=&CrossServerClient::HandleEndCrossWar;
	handlers[ pb::SG2R_Check_Can_Change_Hero ] = &CrossServerClient::CanChangeHeros;
	handlers[ pb::SR2G_Award_Take_Player ] = &CrossServerClient::GetAwardFromCross;
	return handlers ;
}

#ifdef _MMO_SERVER_
void CrossServerClient::OnRecv( TcpAsyncConn *pConn, NetPack *pPack )
{
	if (pPack->IsClientPacket())
	{
		if( NetPack::IsNoticeRecvAndSend( *pPack ) )
		{
			NLOG( "[Recv] 3 %s to %s, %s( %u )", GetServerName(), GetClientName(), pPack->GetOpcodeName(), pPack->GetOpcode() );
		}
		if(pPack->IsCrossToClientPacket())
		{
			if(Player* player = sPlayerPool.GetByPlayerId(pPack->GetSessionId()))
			{
				if(player->IsInCrossWar())
				{
					std::auto_ptr<NetPack> guard( pPack );
					sGameSvr.SendMsgToPlayer(pPack->GetSessionId(),*pPack);
				}
			}
		}
		else if (pPack->IsClientToCrossPacket())
		{
			sGameSvr.PostPlayerNetPack(pPack->GetSessionId(),pPack);
		}
		else
		{
			delete pPack;
			pPack = NULL;
		}
	}
	else
	{
		ServerClient::OnRecv(pConn,pPack);
	}
}

void CrossServerClient::OnSend( uint64 sessionId, NetPack *pPack )
{
	if (pPack)
	{
		if (pPack->IsCrossToClientPacket())
		{
			if( NetPack::IsNoticeRecvAndSend( *pPack ) )
			{
				NLOG( "[Send] %s to %s, %s( %u )", GetClientName(), GetServerName(), pPack->GetOpcodeName(), pPack->GetOpcode() );
			}
		}
		else if (pPack->IsClientToCrossPacket())
		{
			if( NetPack::IsNoticeRecvAndSend( *pPack ) )
			{
				NLOG( "[Send] %s to %s, %s( %u )", GetClientName(), GetServerName(), pPack->GetOpcodeName(), pPack->GetOpcode() );
			}
		}
		else
		{
			ServerClient::OnSend(sessionId,pPack);
		}
	}
}
#endif

CrossServerClient::CrossServerClient( void )
{
	LLOG("[1002] Create One Cross Server Client");

	SetClientType( PACKET_TYPE_SERVER_GAME ) ;
	SetDstServerType( PACKET_TYPE_SERVER_CROSS ) ;

	SetPingDiffMS( sGameSvr.Config().GetInt( "CrossServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sGameSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	SetConnectNow() ;

	//SetMaxConnectCount( 6 ) ;
}

CrossServerClient::~CrossServerClient( void )
{
}



void CrossServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{

}

bool CrossServerClient::HandlePacket( NetPack &packet )
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

void CrossServerClient::ClientConnect( void )
{
	SendRegisterInfo() ;
}

void CrossServerClient::SendPing( void )
{
	//LLOG("[1002] CrossServerClient::SendPing");
	ServerClient::SendPing( pb::S_PING ) ;
}

void CrossServerClient::HandlePong( NetPack &packet )
{
	ServerClient::HandlePong( packet ) ;
}

void CrossServerClient::SendRegisterInfo( void )
{
	pb::SG2R_RegisterInfo msg ;
	msg.set_server_id( sGameSvr.GetServerId() ) ;
	msg.set_region_id(sGameSvr.GetRegionId());
	msg.set_ip( sGameSvr.GetIP() ) ;
	msg.set_port( sGameSvr.GetPort() ) ;
	const String_Server *pNames =DbTool::Get<String_Server>( sGameSvr.GetServerId() ) ;
	if( pNames != NULL )
	{
		for( int i =LANG_SC; i < LANG_COUNT; ++i )
		{
			msg.add_names( pNames->str[i] ) ;
		}
	}
	else
	{
		ELOG( "GameServer id %u can not find name info ?????????????????? ", sGameSvr.GetServerId()  ) ;
	}
	Send( pb::SG2R_REGISTER, msg ) ;
}

void CrossServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SG2R_RegisterInfoRet retInfo ;
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

void CrossServerClient::HandleUpdatePlayerValue( NetPack &packet )
{
	LLOG("[1002] Get PlayerValue Update CrossServerClient::HandleUpdatePlayerValue");
	uint64 playerId = packet.GetSessionId();
	PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);
	pb::GS2R_Update_Player_Value retInfo ; packet >> retInfo;
	if(curplay)
	{
		if(curplay->GetAttrUint(retInfo.index()) != retInfo.val())
		{
			curplay->SetValueInt((int)retInfo.index(),(int)retInfo.val(),false);
		}
	}
}

void CrossServerClient::HandleUpdatePlayerCharValue( NetPack &packet )
{
	LLOG("[1002] Get PlayerValue Char Update CrossServerClient::HandleUpdatePlayerCharValue");
	uint64 playerId = packet.GetSessionId();
	PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);
	pb::GS2R_Update_Player_Character_Exp retInfo ; packet >> retInfo;
	if(curplay)
	{
		Character* charone = curplay->m_characterStorage->MutableCharacter(retInfo.charid());
		if(charone)
			charone->TryAddXP((int)retInfo.addexp(),true);
			
	}
}

//收到Cross发来的SMSG_QUIT_CROSS_WAR
void CrossServerClient::HandleQuitCrossWar( NetPack &packet )
{
	LLOG("[1002] CrossServerClient::HandleQuitCrossWar");

	uint64 playerId = packet.GetSessionId();
	PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);
	if(curplay)
	{
		LLOG("[1002] Set Is In Cross War False");
		curplay->SetInCrossWar(false);

		pb::GS2C_Enter_Cross_War msg;
		msg.set_error_code(pb::ErrCommonSuccess);
		curplay->m_characterStorage->SaveTo(*msg.mutable_character_info());
		sWorldMap.ForEachCity(boost::bind(&SaveCityInfo, _1, msg.mutable_cityinfo()));
		curplay->Send(pb::SMSG_QUIT_CROSS_WAR,msg);
	}
}

void CrossServerClient::HandleCrossResultCountry( NetPack &packet )
{
	pb::CR2G_Country_Awards msg; packet >> msg;

	uint32 uniqueid = msg.unique_index_id();
	
	if(Country* country = sWorldMap.GetCountryByUniqueIdInCross(uniqueid))
	{
		country->AddCountryExp(msg.defanceawardtoexp());
		country->AddCountryExp(msg.attackawardtoexp());

		//守城失败，扣除30点繁荣度
		if(msg.defanceawardtoexp() == 0)
			country->ModifyProsDegree(-GET_BASE_DEF_INT(pb::BD_PERISH_FAULT_RESULT));

		if(msg.attackawardtoexp()/2 > 0)
		{
			country->ModifyProsDegree((msg.attackawardtoexp()/2)*GET_BASE_DEF_INT(pb::BD_PERISH_ONE_STAR_REWARD));
		}
	}
}

void CrossServerClient::HandleEndCrossWar( NetPack &packet )
{
	//断开与cross的连接
	LLOG("[1002] CrossServerClient::HandleEndCrossWar");
	sGameSvr.CloseAllCrossServerClient();
}

void CrossServerClient::CanChangeHeros(NetPack &packet)
{
	uint64 playerId = packet.GetSessionId();
	Player* curplay =	sPlayerPool.GetByPlayerId(playerId);
	if(curplay)
	{
		if ( !curplay->m_characterStorage.get())
			return;
		const TecTable* tec = NULL;
		GS2C_ChangeBattleCharacter msg;
		packet >> msg;

		if(!msg.crossret())
		{
			curplay->SendErrorCode(ErrCrossHeroBusy);
			return;
		}

		Character* forCharctor = curplay->m_characterStorage->MutableCharacter(msg.card_id());
		if (msg.is_zhaomu())
		{
			if (forCharctor)
			{
				bool isEnoughtCoin = false;
				int costZhaomu = forCharctor->GetZhaoMuNeedCost();
				if (curplay->EnoughCurrency(eCoin,costZhaomu))
				{
					isEnoughtCoin = curplay->TryDeductCurrency(IR_PLAYER_ZHAOMU_CHARACTER_COST,eCoin,costZhaomu);
				}

				if (!isEnoughtCoin)
				{
					curplay->SendErrorCode(ErrNotEnoughCoin);
					return;
				}
			}
			else
			{
				curplay->SendErrorCode(ErrNotUnlock);
				return;
			}
		}

		std::vector< CharacterId> characterGuids;
		MsgTool::SaveMsgToVec( msg.battle_array(), characterGuids);

		ItemCharacterLogic::SetBattleArray( curplay, characterGuids);
	}
}

void CrossServerClient::GetAwardFromCross(NetPack &packet)
{
	SR2G_AwardTakePlayer msg; packet >> msg;
	uint64 playerid = msg.playerid();
	if(Player* playerone = sPlayerPool.GetByPlayerId(playerid))
	{
		IntPairVec pVec;
		pVec.clear();
		for(int i=0; i<msg.rewards_size(); i++)
		{
			PairValue* pvalue = msg.mutable_rewards(i);
			pVec.push_back(IntPair(pvalue->first(), pvalue->second()));
		}
		sReward.Change(*playerone,pVec);
		string items;
		Utility::PacketItemsToString(items,pVec);
#ifdef _MMO_SERVER_

		sSysEventMgr.SendJustTipsMail(playerone->GetGuid(),EVENT_TYPE_CROSS_SERVER,items);
#endif
	}
}