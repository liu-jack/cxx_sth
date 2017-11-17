
#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "../CrossServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "GameServer.pb.h"
#include "session/PlayerPool.h"
#include "object/Player.h"
#include "cross_wall/CrossWorldMap.h"
#include "cross_wall/CrossWarManager/CrossWarMgr.h"

#include "CrossWar.pb.h"
#include "Palace.pb.h"
#include "Palace/PalaceMgr.h"
#include "Map/country/country.h"
#include "character/Character.h"
#include "character/CharacterStorage.h"
#include "AutomaticUpdate/AutoUpdateCross.h"
//#include "object/Player.h"
//#include "session/PlayerPool.h"


using namespace pb;

typedef boost::shared_ptr<Player> PlayerPtr ;

GameServerSession::HandlerArray GameServerSession::s_serverHandlers =RegisteServerHandlers() ;


GameServerSession::HandlerArray GameServerSession::RegisteServerHandlers( void )
{
	//LLOG("{CrossServer}GameServerSession::RegisteServerHandlers");
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PING ]                         =&GameServerSession::HandlePing ;
	handlers[ pb::SG2R_REGISTER ]                  =&GameServerSession::HandleRegister ;
	handlers[ pb::SG2R_REGIST_CROSS_PLAYER_INFO ]  =&GameServerSession::HandleRegisterCrossPlayer ;
	handlers[ pb::SG2R_UPDATE_PLAYER_VALUE ] =&GameServerSession::HandleUpdatePlayerValue;
	handlers[ pb::SG2R_OFFICE_INFO ] =&GameServerSession::HandleOfficeInfo;
	handlers[ pb::SG2R_UPDATE_PLAYER_CHAR_VALUE ] =&GameServerSession::HandleUpdatePlayerCharValue;
	handlers[ pb::SG2R_Check_Can_Change_Hero ] =&GameServerSession::CheckCanChangeHeros;

	return handlers ;
}

GameServerSession::GameServerSession( void )
{
	LLOG("{CrossServer}GameServerSession::GameServerSession");
}

GameServerSession::~GameServerSession( void )
{
	LLOG("{CrossServer}GameServerSession::~GameServerSession");
	sCrossSvr.GameServerUnRegister( this ) ;
}


void GameServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("{CrossServer}GameServerSession::Update");
	Session::Update( nowTimeMS, diffMS ) ;
}

#ifdef _SERVER_CROSS_
void GameServerSession::OnRecv( TcpAsyncConn *pConn, NetPack *pPack )
{
	//LLOG("{CrossServer}GameServerSession::OnRecv");
	if (pPack->IsClientToCrossPacket())
	{
		uint64 playerId = pPack->GetSessionId();
		if (Player* curplay =	sPlayerPool.GetByPlayerId(playerId))
		{
			curplay->PostInputPacket(pPack);
		}
	}
	else
	{
		Session::OnRecv(pConn,pPack);
	}

}

void GameServerSession::OnSend( uint64 sessionId, NetPack *pPack )
{
	//LLOG("{CrossServer}GameServerSession::OnSend");
	if (pPack)
	{
		if (pPack->IsCrossToClientPacket() || pPack->IsClientToCrossPacket())
		{
			GetServer()->NoticeSend( sessionId, pPack ) ;
		}
		else
		{
			Session::OnSend(sessionId,pPack);
		}
	}
}
#endif
bool GameServerSession::HandleNetPack( NetPack &pack )
{
	//LLOG("{CrossServer}GameServerSession::HandleNetPack");
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
	//LLOG("{CrossServer}GameServerSession::HandlePing");
	Session::HandlePing( packet, pb::S_PONG ) ;
}

//添加跨服玩家
void GameServerSession::HandleRegisterCrossPlayer( NetPack &packet )
{
	LLOG("[1002] GameServerSession::HandleRegisterCrossPlayer");

	pb::SG2R_RegisterPlayerInfoRet retInfo ;
	retInfo.set_is_success(false);
	uint64 playerId = packet.GetSessionId();

	//只有在分区好了之后的筹备、预览和开战阶段能够注册进入跨服战场
	if( sCrossWarMgr.GetCrossState() == State_Prepare || sCrossWarMgr.GetCrossState() == State_Preview || sCrossWarMgr.GetCrossState() == State_Start || sCrossWarMgr.GetCrossState() == State_Award)
	{
		pb::RegistCrossPlayerData info ;
		packet >> info ;
		PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);

		LLOG("[1002] 1,cross unique id=%d",info.cross_unique_id());

		std::map<uint32, uint32>::iterator it = sCrossWorldMap.m_uniqueToCountry.find(info.cross_unique_id());
		if(it!= sCrossWorldMap.m_uniqueToCountry.end())
		{
			LLOG("[1002] 2");
			int countryid = it->second;

			LLOG("[1002] orgcountryid = %d, crosscountryid = %d, uniqueid = %d",info.country_id(),countryid,info.cross_unique_id());

			if(countryid <= 3)
			{
				if (curplay.get())
				{
					LLOG("[1002] get player playerId=%lld",playerId);
					//curplay->LoadCrossPlayerData(info.player_data());	如果已经注册了，则不处理，没有注册则
					retInfo.set_is_success(true);
					curplay->UpdateCrossPlayerData(info.player_data());	//同步数据
				} 
				else
				{
					LLOG("[1002] not get player playerId=%lld, countryid=%d",playerId,countryid);
					curplay.reset(new Player());
					curplay->_SetCountryId(countryid);		//新分配的countryid
					curplay->LoadCrossPlayerData( info.player_data() );
					curplay->Attach(&sAutoUpdateCross.EventTimer());
					curplay->LoadPlayerAppendData(info.append() );
					curplay->SetAccountId(info.account_id());
					curplay->SetSession(this);
					curplay->SetAddress(GetAddress());
					//curplay->SetCrossUniqueID();
					sPlayerPool.Put(curplay);	//加入玩家池中
					retInfo.set_is_success(true);
				}
			}
		}
	}
	retInfo.set_player_id(playerId);
	Send(pb::RMSG_ENTER_CROSS_WAR_RET,playerId,PACKET_TYPE_CLIENT_CROSS,retInfo);
}


void GameServerSession::HandleRegister( NetPack &packet )
{
	LLOG("{CrossServer}GameServerSession::HandleRegister");
	if( GetIsVerification() )
	{
		WLOG( "Had register, can not register again ........................" ) ;
		return ;
	}

	pb::SG2R_RegisterInfo info ;
	packet >> info ;
	SetPairId( SessionPairId( info.server_id(), info.region_id()) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	pb::SG2R_RegisterInfoRet retInfo ;
	CrossServer::ServerRegisterResult ret =sCrossSvr.GameServerRegister( this ) ;
	switch( ret )
	{
	case CrossServer::SERVER_REGISTER_OK:
		{
			std::vector< std::string > &vec =GetServerNames() ;
			vec.reserve( info.names_size() ) ;
			for( int i =0; i < info.names_size(); ++i )
			{
				vec.push_back( info.names( i ) ) ;
			}

			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;
			NLOG( "%s register %s,SessionId:%llu,(reg:%u,ser:%u), (%s:%s) register ok"
				, GetSessionTypeName(),sCrossSvr.GetServerTypeName(), GetSessionId(), GetPairId().second, GetPairId().first, GetAddress().second.c_str(), GetAddress().first.c_str() ) ;
		
			//重新确定连接session
			if(sCrossWarMgr.GetCrossState() != State_Null)	//国战开启状态
			{
				for(int i=0; i<MAX_PLAYER_COUNTRY_ID; i++)
				{
					if(Country* country = sCrossWorldMap.GetCountry(i))
					{
						if(country->GetGameSessionAddress() == GetAddress())
						{
							LLOG("{crossserver} reset connect to game server country");
							country->SetGameSessionFromCross(this);
						}
					}
				}

				FOREACH_PLAYERPTR(PlayerId, playerPtr)
				{
					if(playerPtr->GetAddress() == GetAddress())
					{
						LLOG("{crossserver} reset connect to game server player");
						playerPtr->SetSession(this);
					}
				}
			}
		} break ;

	case CrossServer::SERVER_REGISTER_FAIL_ID:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ID ) ;
		} break ;

	case CrossServer::SERVER_REGISTER_FAIL_ADDRESS:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ADDRESS ) ;
		} break ;

	default:
		ASSERT( false ) ;
	}

	Send( pb::SG2R_REGISTER_RET, retInfo ) ;
}


void GameServerSession::HandleUpdatePlayerValue( NetPack &packet )
{
	LLOG("[1002] Get PlayerValue Update GameServerSession::HandleUpdatePlayerValue");
	uint64 playerId = packet.GetSessionId();
	PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);
	pb::GS2R_Update_Player_Value retInfo ;
	if(curplay && curplay->GetAttrUint(retInfo.index()) != retInfo.val())
	{
		curplay->SetValueInt((int)retInfo.index(),(int)retInfo.val(),false);
	}
}

void GameServerSession::HandleUpdatePlayerCharValue( NetPack &packet )
{
	LLOG("[1002] Get PlayerCharValue Update GameServerSession::HandleUpdatePlayerCharValue");
	uint64 playerId = packet.GetSessionId();
	PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);
	pb::GS2R_Update_Player_Character_Exp retInfo ;
	if(curplay)
	{
		Character* charone = curplay->m_characterStorage->MutableCharacter(retInfo.charid());
		if(charone)
			charone->TryAddXP(retInfo.addexp(),true);
	}
}

void GameServerSession::CheckCanChangeHeros( NetPack &packet )
{
	LLOG("[1002] Get GameServerSession::CheckCanChangeHeros");
	uint64 playerId = packet.GetSessionId();
	PlayerPtr curplay =	sPlayerPool.GetPtrByPlayerId(playerId);
	GS2C_ChangeBattleCharacter askNullMsg;
	packet >> askNullMsg;
	if(curplay)
	{
		askNullMsg.set_crossret(true);
		CharacterIds& charlst = curplay->m_characterStorage->GetCharIDLst();
		if(Country* country = sCrossWorldMap.GetCountry(curplay->GetCountryId()))
		{
			for (CharacterIds::const_iterator it = charlst.begin(); it != charlst.end(); ++it)
			{
				if (Character* character = curplay->m_characterStorage->MutableCharacter(*it))
				{
					if(character->isInCombat || character->GetCurCity() != country->m_capitalCityId)
					{
						askNullMsg.set_crossret(false);
					}
				}
			}
		}
	}
	else
	{
		askNullMsg.set_crossret(false);
	}
	Send( pb::SG2R_Check_Can_Change_Hero, askNullMsg ) ;
}

void GameServerSession::HandleOfficeInfo( NetPack &packet )
{
	pb::GS2C_Palace_All_Info info ;
	packet >> info ;

	uint32 countryid = sCrossWorldMap.GetCountryidByUnique(info.unique_country_id());
	if(Country* country = sCrossWorldMap.GetCountry(countryid))
	{
		sPalaceMgr(countryid)->LoadFrom(countryid,info);
		country->SetGameSessionFromCross(this);
		country->SetGameSessionAddress(GetAddress());
	}
}