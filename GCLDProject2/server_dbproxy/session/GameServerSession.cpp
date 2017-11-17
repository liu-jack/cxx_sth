
#include <sstream>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>

#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "../PlayerData.h"
#include "../DbProxyServer.h"
#include "../data/DbProxyDataMgr.h"
#include "../data/GlobalData.h"
#include "../data/SessionData.h"
#include "data/PayItemTable.h"

#include "Base.h"
#include "def/MmoAssert.h"
#include "def/ObjectDefines.h"
#include "utility/Utility.h"
#include "utility/TimeParser.h"
#include "utility/TimePeriod.h"
#include "memory_buffer/NetPack.h"

#include "Enum.pb.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "Login.pb.h"
#include "Character.pb.h"
#include "InterServer.pb.h"
#include "Quest.pb.h"
#include "Social.pb.h"
#include "Web.pb.h"
#include "PvP.pb.h"
#include "GameServer.pb.h"
#include "Module.pb.h"
#include "Palace.pb.h"
#include "Activity.pb.h"

using namespace pb;


GameServerSession::HandlerArray GameServerSession::s_serverHandlers               =RegisteServerHandlers() ;


#define FIND_PLAYER_AND_DO_STH( MsgType, do_sth ) \
	MsgType upMsg; \
	uint64 playerId = 0; \
	packet >> playerId >> upMsg; \
	PlayerDataPtr plPtr = m_sessionDataPtr->GetOrLoadPlayerData( playerId) ; \
	if(plPtr.get()) \
	{ \
		plPtr->do_sth( upMsg); \
	}


GameServerSession::HandlerArray GameServerSession::RegisteServerHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

#undef Msg_Declare
#define Msg_Declare(e, MsgType) handlers[e] =&GameServerSession::Handle_##e;
#include "../msg/GlobalDataMsg.h"
#include "../msg/PlayerDataMsg.h"

	handlers[ pb::S_PING ]                           =&GameServerSession::HandlePing ;
	handlers[ pb::SG2D_REGISTER ]                    =&GameServerSession::HandleRegister ;
	handlers[ pb::SG2D_GET_PLAYER_LIST ]             =&GameServerSession::HandleGetPlayerList ;
	handlers[ pb::SG2D_CREATE_PLAYER ]               =&GameServerSession::HandleCreatePlayer ;
	handlers[ pb::SG2D_SELSEC_PALYER ]               =&GameServerSession::HandleSelectPlayer ;
	handlers[ pb::SG2D_PLAYER_OFFLINE ]              =&GameServerSession::HandlePlayerOffline ;
    handlers[ pb::SG2D_PLAYER_CHANGE_NAME ]          =&GameServerSession::HandlePlayerChangeName ;

	handlers[ pb::SG2D_UPDATA_STAGELEVEL_INFO ]      =&GameServerSession::HandleUpdateStageLevel ;
    handlers[ pb::SG2D_UPDATE_ENDLESS_LEVEL ]        =&GameServerSession::HandleUpdateStageLevelEndless ;

	handlers[ pb::SG2D_UPDATE_BAG]                   =&GameServerSession::HandleUpdateBag;

	handlers[ pb::SG2D_CREATE_PLAYER_DATA]           =&GameServerSession::HandleCreatePlayerData;
	handlers[ pb::SG2D_UPDATE_PLAYER_VALUE]          =&GameServerSession::HandleUpdatePlayerValue;

	handlers[ pb::SG2D_UPDATE_BATTLE_CHARS]          =&GameServerSession::HandleUpdateBattleChars;
	handlers[ pb::SG2D_CREATE_CHAR]                  =&GameServerSession::HandleCreateCharacter;
	handlers[ pb::SG2D_DELETE_CHARS]                 =&GameServerSession::HandleDestoryCharacters;
	handlers[ pb::SG2D_UPDATE_CHAR]                  =&GameServerSession::HandleUpdateCharacter;

	handlers[ pb::SG2D_CHECK_MAIL_RECIVERS_EXIST]    =&GameServerSession::HandleCheckMailReciVersExist;
	handlers[ pb::SG2D_BROADCAST_OFFLINE_MSG]        =&GameServerSession::HandleOffLineMsg;
	handlers[ pb::SG2D_MAIL_RECEVE_MAIL]             =&GameServerSession::HandleReceveMail;
	handlers[ pb::SG2D_MAIL_UPDATEMAILINFO]          =&GameServerSession::HandleUpdateMail;

	handlers[ pb::SG2D_UPDATE_BEHAVIOR_COUNTER]      =&GameServerSession::HandleUpdateQuestCounter;
	handlers[ pb::SG2D_COMPLETE_QUEST]               =&GameServerSession::HandleUpdateQuestComplete;
	handlers[ pb::SG2D_UPDATE_QUEST]                 =&GameServerSession::HandleUpdateQuestLog;

	handlers[ pb::SG2D_UPDATE_SELL_PROGRAM_INFO]     =&GameServerSession::HandleUpdateSellProgramInfo;
	handlers[ pb::SG2D_UPDATE_SELL_RANDOM_PROGRAME]  =&GameServerSession::HandleUpdateSellRandomProgram;
	handlers[ pb::SG2D_UPDATE_LOOT_RECORD]           =&GameServerSession::HandleUpdateLootRoundRecord;
    handlers[ pb::SG2D_UPDATE_LOTTERY_RECORD]        =&GameServerSession::HandleUpdateLotteryFreeRecord;


	handlers[ pb::SG2D_FRIEND_OPERATE ]              =&GameServerSession::HandleFriendOperate;
	handlers[ pb::SG2D_SOCIAL_SEARCH_PLAYERSINFO ]   =&GameServerSession::HandleFriendSearch;
	handlers[ pb::SG2D_UPDATE_FRIEND_DATA ]          =&GameServerSession::HandleFriendUpdateData;

	handlers[ pb::SG2D_UPDATE_GUILD_DATA ]           =&GameServerSession::HandleUpdateGuildInfo;
	handlers[ pb::SG2D_UPDATE_GUILD_MEMBER_DATA ]    =&GameServerSession::HandleUpdateGuildMemberInfo;
	handlers[ pb::SG2D_GUILD_SEARCH_PLAYERSINFO ]    =&GameServerSession::HandleGuildSearchPlayerInfo;
	handlers[ pb::SG2D_CREAT_GUILD ]                 =&GameServerSession::HandleCreateGuildInfo;

	handlers[ pb::SG2D_WEB_GET_PLAYER_LIST ]         =&GameServerSession::HandleWebGetPlayerList;

	handlers[pb::SG2D_GET_PLAYERINFO ]               =&GameServerSession::HandlerGetPlayerInfo;
	handlers[ pb::SG2D_PVP_CREATE_VIRTUAL_PLAYER ]   =&GameServerSession::HandlePvPCreateVirtualPlayer;

	handlers[ pb::SG2D_PVP_ENTRY_ARENA ]             =&GameServerSession::HandlePvPEntryArena ;
	handlers[ pb::SG2D_PVP_ARENA_PLAYER_INFO ]       =&GameServerSession::HandlePvPGetPlayerInfo ;
	handlers[ pb::SG2D_PVP_ARENA_FIGHT_RESULT_INFO ] =&GameServerSession::HandlePvPFightResultInfo ;
	handlers[ pb::SG2D_PVP_ARENA_FIGLE_RECORD_INFO ] =&GameServerSession::HandlePvPFightRecordInfo ;

	handlers[ pb::SG2D_SERVER_GOBAL_VALUE ]          =&GameServerSession::HandleServerGobalValue ;

	handlers[ pb::SG2D_PVP_GET_REWARD_RANK_LIST ]    =&GameServerSession::HandleGetPvPRewardRankList ;

	handlers[ pb::SG2D_PVP_ARENA_RANKING_LIST ]      =&GameServerSession::HandlePvPGetRankingList ;
	handlers[ pb::SG2D_PVP_MARK_FIGHT ]              =&GameServerSession::HandlePvPMarkFight ;

    handlers[ pb::SG2D_UPDATE_PLAYER_STRING]         =&GameServerSession::HandleUpdatePlayerString ;

	handlers[ pb::SG2D_WEB_PAY ]                     =&GameServerSession::HandleWebPay ;


	handlers[ pb::SG2D_UPDATE_MODULE_RECORD ]        =&GameServerSession::HandleUpdatePlayerModule ;
	handlers[ pb::SG2D_UPDATE_BUILDING_RECORD ]		 =&GameServerSession::HandleUpdatePlayerBuilding ;

	handlers[ pb::SG2D_GET_PLAYER_APPEND_DATA ]		 =&GameServerSession::HandleGetPlayerAppendData ;

	handlers[ pb::SG2D_UNLOCK_FORBIDEN ]		 =&GameServerSession::HandleForbidenPlayer;

	handlers[ pb::SG2D_SAVE_PLAYER_CACHE ]		 =&GameServerSession::HandleSavePlayerCache;
	handlers[ pb::SG2D_SAVE_ALL_PLAYER_CACHE ]		 =&GameServerSession::HandleSaveAllPlayerCache;
	
	handlers[ pb::SG2D_PLAYER_SET_NAME_AND_HEADID ]		 =&GameServerSession::HandleSetNameAndHeadId;
	
	handlers[ pb::SG2D_UPDATE_ACTIIVTY_DATA ]		 =&GameServerSession::HandleUpdateActivityData;
	handlers[ pb::SG2D_ACTIVITY_STAR_BOX_DELETE ]		 =&GameServerSession::HandleDeleteActivityStarBoxTable;
	handlers[ pb::SG2D_ACTIVITY_GENERAL_REWARD_DELETE ]		 =&GameServerSession::HandleDeleteGeneralRewardTable;
	handlers[ pb::SG2D_SELECT_COUNTRY]               = &GameServerSession::HandleSelectCountry;

	handlers[ pb::SG2D_SAVE_DBPROXY]               = &GameServerSession::HandleSaveDbproxy;

	return handlers ;
}



GameServerSession::GameServerSession( void )
{
}

GameServerSession::~GameServerSession( void )
{
	m_sessionDataPtr->SetSession(NULL);
	m_sessionDataPtr.reset();
	m_gloabalDataPtr.reset();
	sDBServer.GameServerUnRegister( this ) ;
}


void GameServerSession::Update( uint64 nowTimeMS, int64 diffMS )
{
	Session::Update( nowTimeMS, diffMS ) ;
	m_sessionDataPtr->Update(nowTimeMS,diffMS);
}

int GameServerSession::GetRegionId( void )
{
	return GetPairId().second ;
}

int GameServerSession::GetServerId( void )
{
	return GetPairId().first;
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

	pb::SG2D_RegisterInfo info ;
	packet >> info ;

	SetPairId( SessionPairId( info.server_id(), info.region_id() ) ) ;
	SetAddress( SessionAddress( info.port(), info.ip() ) ) ;

	//NLOG( "HandleRegister(): %s", info.DebugString().c_str() );

	pb::SG2D_RegisterInfoRet retInfo ;
	DbProxyServer::ServerRegisterResult ret =sDBServer.GameServerRegister( this ) ;
	switch( ret )
	{
	case DbProxyServer::SERVER_REGISTER_OK:
		{
			retInfo.set_result( pb::SERVER_REGISTER_OK ) ;

			NLOG( "%s,id:%llu, pairID(%u,%u), address(%s, %s) register ok"
				, GetSessionTypeName(), GetSessionId(), GetPairId().second, GetPairId().first, GetAddress().second.c_str(), GetAddress().first.c_str() ) ;
			m_gloabalDataPtr = sDataMgr.GetOrLoadGlobalData();
			if (m_gloabalDataPtr.get())
			{
				m_gloabalDataPtr->OnGameServerRegist(this);
			}
			m_sessionDataPtr = sDataMgr.GetOrLoadSessionData(GetPairId());
			if (m_sessionDataPtr.get())
			{
                m_sessionDataPtr->SetSession(this);
			}
			
		} break ;

	case DbProxyServer::SERVER_REGISTER_FAIL_ID:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ID ) ;
		} break ;

	case DbProxyServer::SERVER_REGISTER_FAIL_ADDRESS:
		{
			SetIsNeedClose( true ) ;
			retInfo.set_result( pb::SERVER_REGISTER_FAIL_ADDRESS ) ;
		} break ;

	default:
		ASSERT( false ) ;
	}

	Send( pb::SD2G_REGISTER_RET, retInfo ) ;
}

void GameServerSession::HandleGetPlayerList( NetPack &packet )
{
	pb::SG2D_GetPlayerList info ;
	packet >> info ;

	PlayerTableCacheType tmpTable ;
	sDataMgr.GetPlayerList( tmpTable, info.account_id(), GetPairId().second, GetPairId().first ) ;

	

	// retInfo
	pb::SGetPlayerList retInfo ;
	retInfo.set_result( pb::GET_PALYER_LIST_SUCCESS ) ;

	for( PlayerTableCacheMItr itr =tmpTable.MapBegin(); itr != tmpTable.MapEnd(); ++itr )
	{
		PlayerTable &info =itr->second ;
		pb::PlayerListInfo *pAddElement =retInfo.add_player_infos() ;
		pAddElement->set_name( info.name ) ;
		pAddElement->set_slot( info.slot ) ;
		pAddElement->set_player_id( info.playerId ) ;
		pAddElement->set_head_icon(info.cardHeadIconId);
		MapLogicTableCacheType tempMapTable;
		sDataMgr.GetMapLogicRecord( tempMapTable, info.playerId ) ;
		MapLogicTableCacheMItr itrTemp =tempMapTable.MapBegin();
		if (itrTemp != tempMapTable.MapEnd())
		{
			MapLogicTable &infoMap =itrTemp->second ;
			pAddElement->set_country_id(infoMap.country_id);
		}
		else
		{
			pAddElement->set_country_id(0);
		}
	}

	Send( pb::SD2G_GET_PLAYER_LIST_RET, packet.GetSessionId(), retInfo ) ;
}


void GameServerSession::HandlePlayerChangeName( NetPack &packet )
{
    pb::MSG_ChangeName msg; 
    packet >> msg ;

    if( sDataMgr.GetHadThisPlayerInDB( msg.name() ) )
    {
        msg.set_err_code( pb::ErrNameAlreadyExist ) ;
        Send( pb::SD2G_PLAYER_CHANGE_NAME_RET, packet.GetSessionId(), msg ) ;
        return ;
    }

    PlayerDataPtr ptr = m_sessionDataPtr->GetPlayerData( msg.player_id());
    if ( ptr.get())
    {
        ptr->ChangeNameImm( msg.name());
    }

    msg.set_err_code( pb::ErrCommonSuccess ) ;
    Send( pb::SD2G_PLAYER_CHANGE_NAME_RET, packet.GetSessionId(), msg ) ;

    
    
}

void GameServerSession::HandleCreatePlayer( NetPack &packet )
{
	pb::SG2D_CreatePlayer info ;
	packet >> info ;


	pb::SCreateCharRes retInfo ;

	retInfo.set_channel_id(info.channel_id());
	retInfo.set_name(info.name());
	retInfo.set_sdk_user_id(info.sdk_user_id());
	retInfo.set_platform_id(info.platform_id());

	if( sDataMgr.GetHadThisPlayerInDB( info.name() ) )
	{
		retInfo.mutable_res()->set_result( pb::CREATE_PLAYER_SAME_NAME ) ;
		Send( pb::SD2G_CREATE_PLAYER_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}

	PlayerTable newPlayer ;
	newPlayer.accountId       =info.account_id() ;
	newPlayer.name            =info.name() ;
	newPlayer.regionId        =GetPairId().second ;
	newPlayer.serverId        =GetPairId().first ;
	newPlayer.channelId       =info.channel_id() ;
	newPlayer.cardHeadIconId  =info.card_solution_id() ;
	newPlayer.level           =1;
	newPlayer.sdkUserId       =info.sdk_user_id() ;
	newPlayer.playerId        =Guid(TYPE_PLAYER,GetPairId().second, GetPairId().first, m_sessionDataPtr->GetNextPlayerGuid());
	Utility::SetFlag( newPlayer.flags, pb::PLAYER_FLAG_IS_NEW ) ;
	Utility::SetFlag( newPlayer.flags, pb::PLAYER_FLAG_FIRST_REFRESH_EQUIP_SHOP ) ;



	if( !sDataMgr.AddNewPlayerToTable( newPlayer ) )
	{
		retInfo.mutable_res()->set_result( pb::CREATE_PLAYER_DATABASE_INSERT_FAIL ) ;
		Send( pb::SD2G_CREATE_PLAYER_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}


	retInfo.mutable_res()->set_player_id( newPlayer.playerId ) ;
	retInfo.mutable_res()->set_result( pb::CREATE_PLAYER_SUCCESS ) ;
	Send( pb::SD2G_CREATE_PLAYER_RET, packet.GetSessionId(), retInfo ) ;
}

void GameServerSession::HandleSelectPlayer( NetPack &packet )
{
	pb::SG2D_SelectPlayer info ;
	packet >> info ;
	pb::SSlectPlayer retInfo ;
	retInfo.set_player_id(info.player_id());
	PlayerDataPtr dataPtr = m_sessionDataPtr->GetPlayerData(info.player_id());
	if ( dataPtr.get())
	{
		//账号id与存储的id不同
		if( dataPtr->m_PlayerTable->accountId != info.account_id() )
		{
			retInfo.set_result( pb::SELECT_PLAYER_PLAYER_ID_ERROR ) ;
			Send( pb::SD2G_SELSEC_PALYER_RET, packet.GetSessionId(), retInfo ) ;
			return ;
		}
		//账号被锁定
		if (dataPtr->IsLocked())
		{
			retInfo.set_result( pb::SELECT_PLAYER_PLAYER_ID_LOCKED ) ;
			Send( pb::SD2G_SELSEC_PALYER_RET, packet.GetSessionId(), retInfo ) ;
			return;
		}

		dataPtr->m_PlayerTable->platformId =info.platform_id() ;


		retInfo.set_result( pb::SELECT_PLAYER_SUCCESS ) ;
		dataPtr->WriteAllPlayerData( retInfo.mutable_player_data() ) ;

		retInfo.set_is_quick_login( info.is_quick_login() ) ;
		retInfo.set_country_id(info.country_id());
		retInfo.set_headpic_id(dataPtr->m_PlayerTable->cardHeadIconId);
		retInfo.set_expected_country_id(info.expected_country_id());

		Send( pb::SD2G_SELSEC_PALYER_RET, packet.GetSessionId(), retInfo ) ;
		m_sessionDataPtr->OnPlayerOnline( dataPtr ) ;
	}
	else
	{
		//获取玩家信息
		dataPtr.reset( sDataMgr.LoadPlayerData( info.account_id(), info.player_id()) );	//sDataMgr.LoadPlayerData返回PlayerData指针
		if( !dataPtr )
		{
			if (sDataMgr.GetHadThisPlayerInDBLocked(info.player_id()))
			{
				retInfo.set_result( pb::SELECT_PLAYER_PLAYER_ID_LOCKED ) ;
				Send( pb::SD2G_SELSEC_PALYER_RET, packet.GetSessionId(), retInfo ) ;
			}
			else
			{
				retInfo.set_result( pb::SELECT_PLAYER_PLAYER_ID_ERROR ) ;
				Send( pb::SD2G_SELSEC_PALYER_RET, packet.GetSessionId(), retInfo ) ;
			}
			return;
		}

		bool isInsert = m_sessionDataPtr->AddPlayerDataToMap( dataPtr ) ;
		ASSERT( isInsert ) ;
		if( !isInsert )
		{
			ELOG( "GameServerSession::HandleSelectPlayer() AddPlayerDataToMap() fail palyer datat not cache ....................." ) ;
		}

		dataPtr->m_PlayerTable->platformId =info.platform_id() ;


		m_sessionDataPtr->OnPlayerOnline( dataPtr ) ;
		retInfo.set_result( pb::SELECT_PLAYER_SUCCESS ) ;
		dataPtr->WriteAllPlayerData( retInfo.mutable_player_data() ) ;
		retInfo.set_is_quick_login( info.is_quick_login() ) ;
		retInfo.set_country_id(info.country_id());
		retInfo.set_headpic_id(info.headpic_id());
		retInfo.set_expected_country_id(info.expected_country_id());

		Send( pb::SD2G_SELSEC_PALYER_RET, packet.GetSessionId(), retInfo ) ;
	}


}



void GameServerSession::HandlePlayerOffline( NetPack &packet )
{
	pb::SG2D_Player_Id info ;
	packet >> info ;
	m_sessionDataPtr->OnPlayerOffLine( info.player_id() ) ;
}

void GameServerSession::HandleUpdateStageLevel( NetPack &packet )
{
    //FIND_PLAYER_AND_DO_STH( SG2D_UpdataStageLevel, UpdateStageLevelInfo);
}
void GameServerSession::HandleUpdateStageLevelEndless( NetPack &packet )
{
    //FIND_PLAYER_AND_DO_STH( SG2D_UpdataStageLevel, UpdateEndlessLevel);
}

void GameServerSession::HandleUpdateBag( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_Bag, UpdateBag);
}

void GameServerSession::HandleCreatePlayerData( NetPack &packet )
{
	pb::CreatePlayerData createData;
	packet >> createData;

	if( createData.all_data().full_value().uint64_values().values_size() < pb::PLAYER_FIELD_64_BIT_COUNT )
	{
		ELOG( "Data error no create success!!!!" ) ;
		return ;
	}

	uint64 playerId =createData.all_data().full_value().uint64_values().values( pb::PLAYER_FIELD_PLAYER_ID ) ;
	//ASSERT( playerId != 0 ) ;

	if(playerId == 0)
	{
		ELOG( "create failed id=0 !!!!!!!!" ) ;
		return;
	}

	PlayerDataPtr dataPtr = m_sessionDataPtr->GetPlayerData(playerId);
	if ( dataPtr.get())
	{
		ASSERT( createData.account_id() != 0 ) ;
		dataPtr.reset( sDataMgr.LoadPlayerData( createData.account_id(), playerId) );
		if( !dataPtr )
		{
			ELOG( "GameServerSession::HandleCreatePlayerData() load data error !" ) ;
			return ;
		}
		bool isInsert = m_sessionDataPtr->AddPlayerDataToMap( dataPtr ) ;
		ASSERT( isInsert ) ;
		if( !isInsert )
		{
			ELOG( "GameServerSession::HandleCreatePlayerData() AddPlayerDataToMap() fail palyer datat not cache ....................." ) ;
			return ;
		}

		// update player all data .
		dataPtr->ReadAllPlayerdata( createData.all_data() ) ;
	}

}



void GameServerSession::HandleUpdateBattleChars( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_BattleCharacter, UpdateBattleArray);
}

void GameServerSession::HandleDestoryCharacters( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( Uint32List, DestoryCharacter);
}

void GameServerSession::HandleCreateCharacter( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_CharacterCreate, AddCharacter);
}

void GameServerSession::HandleUpdateCharacter( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_CharacterUpdate, UpdateCharacter);
}

void GameServerSession::HandleUpdatePlayerValue( NetPack &packet )
{
	//FIND_PLAYER_AND_DO_STH( ObjectValueUpdate, UpdatePlayerValue);
	ObjectValueUpdate upMsg; 
	uint64 playerId = 0; 
	packet >> playerId >> upMsg; 
	PlayerDataPtr playerPtr = m_sessionDataPtr->GetPlayerData(playerId);
	if ( playerPtr.get())
	{
		playerPtr->UpdatePlayerValue( upMsg); 
	}
}

void GameServerSession::HandleUpdateQuestCounter( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GxDB_Behavior, UpdateQuestCounter);
}

void GameServerSession::HandleUpdateQuestLog( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GxDB_Quest, UpdateQuestLog);
}

void GameServerSession::HandleUpdateQuestComplete( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GxDB_Quest, UpdateQuestComplete);
}

void GameServerSession::HandleUpdateSellRandomProgram( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_SellRandomProgram, UpdateSellRandomProgram);
}

void GameServerSession::HandleUpdateSellProgramInfo( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_SellProgramInfo, UpdateSellProgramInfo);
}

void GameServerSession::HandleUpdateLootRoundRecord( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_LootRecord, UpdateLootRoundRecord);    
}

void GameServerSession::HandleUpdateLotteryFreeRecord( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( GS2C_LotteryFreeRecord, UpdateLotteryFree);    
}

void GameServerSession::HandleUpdatePlayerString( NetPack& packet )
{
    FIND_PLAYER_AND_DO_STH( GS2C_PlayerString, UpdatePlayerString);    
}




void GameServerSession::HandleCheckMailReciVersExist( NetPack &packet )
{
	enum
	{
		CHECKPLAYEREXIST = 1,
		CHECKPLAYERNOTEXIST,
	};
	SG2D_Check_Mail_Recivers_Exist checkMsg;
	packet >> checkMsg;
	bool isCanSend = true;
	SD2G_Check_Mail_Recivers_Exist_Rspone responMsg;
	SG2G_Mail_SendMail*repMail = responMsg.mutable_mail();
	repMail->CopyFrom(checkMsg.mail());
	repMail->clear_to_players();
	responMsg.set_is_server_mail(checkMsg.is_server_mail());
	responMsg.set_alv(checkMsg.alv());
	responMsg.set_blv(checkMsg.blv());
	responMsg.set_avip(checkMsg.avip());
	responMsg.set_bvip(checkMsg.bvip());
	int nPlayer = checkMsg.mail().to_players_size();
	if (!checkMsg.is_server_mail())
	{
		for (int i=0; i< nPlayer; i++)
		{
			uint64 playerId = checkMsg.mail().to_players(i);
			bool isExitPlayer = false;
			if(m_sessionDataPtr->IsInPlayerCache(playerId))
			{
				isExitPlayer = true;
			}
			else if (sDataMgr.GetHadThisPlayerInDB(playerId))
			{
				isExitPlayer = true;
			}
			if (isExitPlayer)
			{
				repMail->add_to_players(playerId);
			}
		}
		if (repMail->to_players_size() < 1)
		{
			isCanSend = false;
		}
	}
	//事件类型：1，后台邮件, 2系统固定邮件，3后台固定邮件,5 just tips
	if (isCanSend && (responMsg.mail().mail_type() == pb::MAIL_PLATGIFT || responMsg.mail().mail_type() == pb::MAIL_PLATFROM))
	{
		PlatfromMailTable newPlatMail;
		newPlatMail.LoadFrom(responMsg.mail());
		newPlatMail.create_time = ::time(NULL);
		PlatfromMailTable* resultMail = m_sessionDataPtr->AddNewPlatfromMailToTable(newPlatMail);
		if(resultMail)
		{
			repMail->set_mail_id(resultMail->id);
		}
	}
	if (responMsg.mail().mail_id() > 0)
	{
		responMsg.set_is_can_send(isCanSend);
		if (isCanSend)
		{
			m_sessionDataPtr->ReceveMails(responMsg);
		}
	}
}


void GameServerSession::HandleFriendUpdateData( NetPack &packet )
{
}


void GameServerSession::HandleFriendOperate( NetPack &packet )
{
	
}

void GameServerSession::HandleFriendSearch( NetPack &packet )
{
	
}

void GameServerSession::HandleCreateGuildInfo( NetPack &packet )
{
}

void GameServerSession::HandleWebGetPlayerList( NetPack &packet )
{
	pb::WebGetPlayerList info ;
	packet >> info ;

	pb::WebGetPlayerListRet retInfo ;

	std::string sdk_user_id =info.user_id() ;
	if( sdk_user_id.empty() )
	{
		ELOG( "GameServerSession::HandleWebGetPlayerList(), sdk_user_id is empty()" ) ;
		Send( pb::SD2G_WEB_GET_PLAYER_LIST_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}


	CMySqlConnection* pConn = sDatabase.GetConnection() ;
	if( pConn == NULL )
	{
		ASSERT( false ) ;
		return ;
	}

	CMysqlRstPtr pResult =pConn->QueryFormat( "SELECT playerId, name FROM %s WHERE sdkUserId LIKE %s AND regionId =%u AND serverId =%u; "
		, PlayerTable::GetTableName(), info.user_id().c_str(), GetPairId().second, GetPairId().first ) ;

	if( pResult )
	{
		ASSERT( pResult->GetRowCount() >= 1 ) ;

		Field* pField = pResult->Fetch();

		do
		{
			WebPlayerInfo *pInfo =retInfo.add_player_info() ;

			char id[30] ="" ;
			sprintf( id, "%llu", pField[0].GetUInt64()  ) ;

			pInfo->set_role_id( id ) ;
			pInfo->set_role_name( pField[1].GetString() ) ;

		} while( pResult->NextRow() ) ;
	}

	sDatabase.PutConnection( pConn ) ;

	Send( pb::SD2G_WEB_GET_PLAYER_LIST_RET, packet.GetSessionId(), retInfo ) ;
}

void GameServerSession::HandleUpdateGuildMemberInfo( NetPack &packet )
{

}

void GameServerSession::HandleUpdateGuildInfo( NetPack &packet )
{
}

void GameServerSession::HandleGuildSearchPlayerInfo( NetPack &packet )
{

}

void GameServerSession::HandlerGetPlayerInfo( NetPack &packet )
{
	//pb::PairValueUint64 getPlayerInfoMsg;
	//packet >> getPlayerInfoMsg;
	//uint64 playerId = getPlayerInfoMsg.first();
	//uint64 youId = getPlayerInfoMsg.second();
	//pb::GS2C_Get_Player_Info_Respone msgGetInfo;
	//msgGetInfo.set_result(2);
	//msgGetInfo.set_your_guid(youId);
	//bool isGet = GetPlayerBaseData(playerId,*msgGetInfo.mutable_info());
	//if (isGet)
	//{
	//	msgGetInfo.set_result(1);
	//}

	//Send( SD2G_GET_PLAYERINFO_RESPON, packet.GetSessionId(), msgGetInfo );
}

void GameServerSession::HandleUpdateMail( NetPack &packet )
{
	uint64 playerId;
	pb::SG2D_MailsOperate operate;
	packet >> playerId;
	packet >> operate;
	PlayerDataPtr ptrPlayer = m_sessionDataPtr->GetPlayerData(playerId);
	if ( ptrPlayer.get())
	{
		ptrPlayer->UpdateMail(operate);
		pb::GS2C_Mail_MailList mailupdates;
		if(ptrPlayer->GetPlayerMailUpdate(mailupdates))
			Send( pb::SD2G_MAIL_UPDATEMAILINFO_RET, packet.GetSessionId(),mailupdates) ;
	}
}

void GameServerSession::HandleReceveMail( NetPack &packet )
{
	uint64 playerId =0;
	packet >> playerId;
	pb::SG2G_Mail_SendMail platMail;
	packet >> platMail;
	PlayerDataPtr ptrPlayer = m_sessionDataPtr->GetPlayerData(playerId);
	if ( ptrPlayer.get())
	{
		if(ptrPlayer->AddMailAsync( platMail))
		{
			pb::GS2C_Mail_MailList mailupdates;
			if(ptrPlayer->GetPlayerMailUpdate( mailupdates))
				Send( pb::SD2G_MAIL_UPDATEMAILINFO_RET, packet.GetSessionId(), mailupdates) ;
		}
	}
}

void GameServerSession::HandleOffLineMsg( NetPack &packet )
{
	
}


void GameServerSession::HandlePvPCreateVirtualPlayer( NetPack &packet )
{
}


void GameServerSession::HandlePvPEntryArena( NetPack &packet )
{
	//pb::PvPEntryArena info ;
	//packet >> info ;

	//ASSERT( info.player_id() != 0 ) ;

	//int rank =AddPvPRank( info.player_id(), info.account_id(), m_pvpRank.Empty() ? 1 : m_pvpRank.RBegin()->first + 1 ) ;
	//if( rank == 0 )
	//{
	//	ELOG( "GameServerSession::HandlePvPEntryArena() get repeat entry arena?????" ) ;
	//	return ;
	//}

	//pb::PvPEntryArenaRet retInfo ;
	//retInfo.set_result( rank > 0 ? pb::PVP_ARENA_RESULT_SUCCESS : pb::PVP_ARENA_RESULT_OTHER_ERROR ) ;
	//retInfo.set_rank( rank ) ;
	//Send( pb::SD2G_PVP_ENTRY_ARENA_RET, packet.GetSessionId(), retInfo ) ;
}

void GameServerSession::HandlePvPGetPlayerInfo( NetPack &packet )
{
	//pb::PvPGetArenaPlayerInfo info ;
	//packet >> info ;
	//ASSERT( info.rank_size() != 0 ) ;

	//pb::PvPGetArenaPlayerInfoRet retInfo ;

	//for( int i =0; i < info.rank_size(); ++i )
	//{
	//	const int rank =info.rank( i ) ;

	//	PvPRankTable *pElement =m_pvpRank.GetElement( rank ) ;
	//	if( pElement != NULL )
	//	{
	//		pb::ArenaAttackListPlayerInfo *pInfo =retInfo.add_infos() ;

	//		pInfo->set_rank( rank ) ;

	//		if( !GetPlayerBaseData( pElement->playerId, *pInfo->mutable_player_info(), pb::PLAYER_BASE_DATA_JUST_PVP_BASE_INFO ) )
	//		{
	//			pInfo->Clear() ;
	//		}
	//	}
	//}

	//retInfo.set_result( pb::PVP_ARENA_RESULT_SUCCESS ) ;
	//Send( pb::SD2G_PVP_ARENA_PLAYER_INFO_RET, packet.GetSessionId(), retInfo ) ;
}

void GameServerSession::HandlePvPFightResultInfo( NetPack &packet )
{
	//pb::PvPArenaFinghtInfo info ;
	//packet >> info ;

	//pb::PvPArenaFinghtInfoRet retInfo ;
	//retInfo.set_is_win( info.is_win() ) ;

	//// 检测数据正确性
	//PvPRankTable *pTargetRank =sDataMgr.GetTable<PvPRankTable>()->GetEntryByKey( info.target_id() ) ;
	//if( pTargetRank == NULL )
	//{
	//	retInfo.set_result( pb::PVP_ARENA_RESULT_CAN_NOT_FIND_PLAYER ) ;
	//	Send( pb::SD2G_PVP_ARENA_FIGHT_RESULT_INFO_RET, packet.GetSessionId(), retInfo ) ;
	//	return ;
	//}

	//PvPRankTable *pSelfRank =sDataMgr.GetTable<PvPRankTable>()->GetEntryByKey( info.self_id() ) ;
	//if( pSelfRank == NULL )
	//{
	//	retInfo.set_result( pb::PVP_ARENA_RESULT_CAN_NOT_FIND_PLAYER ) ;
	//	Send( pb::SD2G_PVP_ARENA_FIGHT_RESULT_INFO_RET, packet.GetSessionId(), retInfo ) ;
	//	return ;
	//}

	//bool isFighting =pSelfRank->extra.isFighting && pTargetRank->extra.isFighting ;
	////ASSERT( isFighting ) ;
	//if( !isFighting )
	//{
	//	retInfo.set_result( pb::PVP_ARENA_RESULT_OTHER_ERROR ) ;
	//	Send( pb::SD2G_PVP_ARENA_FIGHT_RESULT_INFO_RET, packet.GetSessionId(), retInfo ) ;
	//	return ;
	//}

	//pSelfRank->extra.isFighting     =false ;

	//ASSERT( pSelfRank->extra.targetPlayerId != 0 ) ;
	//pSelfRank->extra.targetPlayerId =0 ;

	//pTargetRank->extra.isFighting =false ;
	//ASSERT( pSelfRank->extra.targetPlayerId == 0 ) ;

	//int targetDiff =0 ;
	//if( info.is_win() && pSelfRank->rank > pTargetRank->rank )
	//{
	//	targetDiff =pTargetRank->rank - pSelfRank->rank ;

	//	std::swap( pSelfRank->rank, pTargetRank->rank ) ;

	//	// 要发给客户端之前的那个
	//	retInfo.set_new_history_rank( pSelfRank->historyRank ) ;

	//	pSelfRank->historyRank =(std::min)( pSelfRank->rank, pSelfRank->historyRank ) ;

	//	// 调整排名的位置
	//	m_pvpRank.SwapValue( pSelfRank->rank, pTargetRank->rank ) ;

	//	// 保存数据库
	//	m_pvpRank.SaveOneMod( pSelfRank ) ;
	//}
	//else
	//{
	//	retInfo.set_new_history_rank( pSelfRank->historyRank ) ;
	//}

	//// 记录战报
	//std::stringstream ss ;
	//ss << !info.is_win() << ',' << info.self_id() << ',' << info.self_rank() << ','
	//	<< info.self_level() << ',' << info.self_name() << ',' << info.time() <<  ','
	//	<< info.self_fight_value() << ',' << info.self_char_proto_id() << ',' << targetDiff << ',' << info.self_vip_level() ;

	//pTargetRank->AddOneFightRecord( ss.str() ) ;

	//// 保存数据库
	//m_pvpRank.SaveOneMod( pTargetRank ) ;

	//// 返回结果
	//retInfo.set_new_rank( pSelfRank->rank ) ;

	//retInfo.set_target_id( pTargetRank->playerId ) ;
	//retInfo.set_target_rank( pTargetRank->rank ) ;

	//retInfo.set_result( pb::PVP_ARENA_RESULT_SUCCESS ) ;
	//Send( pb::SD2G_PVP_ARENA_FIGHT_RESULT_INFO_RET, packet.GetSessionId(), retInfo ) ;
}


void GameServerSession::HandlePvPFightRecordInfo( NetPack &packet )
{
	//pb::PvPArenaFightRecordInfo info ;
	//packet >> info ;

	//pb::PvPArenaFightRecordInfoRet retInfo ;

	//// 检测数据正确性
	//PvPRankTable *pSelfRank =m_pvpRank.GetElement( info.rank() ) ;
	//if( pSelfRank == NULL || pSelfRank->playerId != info.palyer_id() )
	//{
	//	retInfo.set_result( pb::PVP_ARENA_RESULT_CAN_NOT_FIND_PLAYER ) ;
	//	Send( pb::SD2G_PVP_ARENA_FIGLE_RECORD_INFO_RET, packet.GetSessionId(), retInfo ) ;
	//	return ;
	//}

	//retInfo.set_result( pb::PVP_ARENA_RESULT_SUCCESS ) ;
	//for( int i =0; i < PvPFightRecordMaxCount; ++i )
	//{
	//	std::string &str =pSelfRank->fightRecord[i] ;
	//	if( str.empty() )
	//	{
	//		break ;
	//	}

	//	retInfo.add_records( str ) ;
	//}

	//Send( pb::SD2G_PVP_ARENA_FIGLE_RECORD_INFO_RET, packet.GetSessionId(), retInfo ) ;
}


void GameServerSession::HandleServerGobalValue( NetPack &packet )
{
	//pb::ServerGobalValue info ;
	//packet >> info ;

	//ServerGobalValueMap& gobalValues =GetServerGobalValueMap() ;

	//if( info.values_size() > 0 )
	//{
	//	for( int i =0; i < info.values_size(); ++i )
	//	{
	//		const pb::GobalValue &value =info.values( i ) ;

	//		ServerGobalValueTable *pValue =gobalValues.GetElement( value.index() ) ;
	//		if( pValue != NULL )
	//		{
	//			// update
	//			selfSetGobalValue( *pValue, value ) ;

	//			// 保存数据库
	//			gobalValues.SaveOneMod( pValue ) ;
	//		}
	//		else
	//		{
	//			// add
	//			ServerGobalValueTable    newValue ;
	//			newValue.regionId        =GetPairId().second ;
	//			newValue.serverId        =GetPairId().first ;
	//			newValue.index           =value.index() ;
	//			newValue.type            =value.type() ;
	//			selfSetGobalValue( newValue, value ) ;

	//			gobalValues.AddAndAddToCache( &newValue ) ;
	//		}
	//	}
	//}
	//else
	//{
	//	// get
	//	if( gobalValues.Empty() )
	//	{
	//		InitServerGobalValue() ;
	//	}

	//	for( ServerGobalValueMapItr itr =gobalValues.Begin(); itr != gobalValues.End(); ++itr )
	//	{
	//		pb::GobalValue *pValue =info.add_values() ;
	//		pValue->set_index( itr->second->index ) ;
	//		pValue->set_i64_value( itr->second->int64Value ) ;
	//	}

	//	Send( pb::SD2G_SERVER_GOBAL_VALUE_RET, info ) ;
	//}
}


void GameServerSession::HandleGetPvPRewardRankList( NetPack &packet )
{
	/*pb::SG2D_PvPGetRankList info ;
	packet >> info ;

	pb::SG2D_PvPGetRankListRet retInfo ;
	for( int i =0; i < info.ranks_size(); ++i )
	{
		const SG2D_PvPGetRank &rank = info.ranks( i ) ;

		if( rank.beg_rank() > rank.end_rank() )
		{
			break ;
		}

		PvPRankMapItr bItr =m_pvpRank.GetLowBoundItr( rank.beg_rank() ) ;
		PvPRankMapItr eItr =m_pvpRank.GetUpperBoundItr( rank.end_rank() ) ;
		if( bItr == eItr )
		{
			break ;
		}

		pb::SG2D_PvPGetRankRet *pRetRank =retInfo.add_ranks() ;

		pRetRank->set_mail_id( rank.mail_id() ) ;

		for( PvPRankMapItr itr =bItr; itr != eItr; ++itr )
		{
			if( itr->second->accountId != 0 )
			{
				pRetRank->add_player_ids( itr->second->playerId ) ;
			}
		}
	}

	Send( pb::SD2G_PVP_GET_REWARD_RANK_LIST_RET, retInfo ) ;*/
}


void GameServerSession::HandlePvPGetRankingList( NetPack &packet )
{
	//pb::PvPGetRankingList info ;
	//packet >> info ;

	//pb::PvPGetRankingListRet retInfo ;

	//// 检测数据正确性
	//if( info.beg_rank() > m_pvpRank.Size() || info.beg_rank() > info.end_rank() )
	//{
	//	ELOG( " GameServerSession::HandlePvPGetRankingList() Get error data. return " ) ;
	//	return ;
	//}

	//PvPRankMapItr begItr =m_pvpRank.GetLowBoundItr( info.beg_rank() ) ;
	//PvPRankMapItr endItr =m_pvpRank.GetUpperBoundItr( info.end_rank() ) ;

	//retInfo.set_result( pb::PVP_ARENA_RESULT_SUCCESS ) ;
	//for( PvPRankMapItr itr =begItr; itr != endItr; ++itr )
	//{
	//	pb::RankingInfo *pInfo =retInfo.add_infos() ;

	//	pInfo->set_rank( itr->second->rank ) ;
	//	if( !GetPlayerBaseData( itr->second->playerId, *pInfo->mutable_info(), pb::PLAYER_BASE_DATA_JUST_PVP_BASE_INFO ) )
	//	{
	//		pInfo->Clear() ;
	//	}
	//}

	//Send( pb::SD2G_PVP_ARENA_RANKING_LIST_RET, packet.GetSessionId(), retInfo ) ;
}

void GameServerSession::HandlePvPMarkFight( NetPack &packet )
{
	/*pb::PvpMarkFight info ;
	uint64 self_player_id =0 ;
	packet >> info ;
	packet >> self_player_id ;

	pb::PvpMarkFightRet retInfo ;

	PvPRankTable *pSelfRank   =sDataMgr.GetTable<PvPRankTable>()->GetEntryByKey( self_player_id ) ;
	PvPRankTable *pTargetRank =m_pvpRank.GetElement( info.rank() ) ;
	if( pTargetRank == NULL || pSelfRank == NULL )
	{
		retInfo.set_result( pb::PVP_ARENA_RESULT_CAN_NOT_FIND_PLAYER ) ;
		Send( pb::SG2D_PVP_MARK_FIGHT_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}

	if( pTargetRank->extra.isFighting )
	{
		retInfo.set_result( pb::PVP_ARENA_RESULT_TARGET_IS_FIGHTING ) ;
		Send( pb::SG2D_PVP_MARK_FIGHT_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}

	if( pSelfRank->extra.isFighting )
	{
		retInfo.set_result( pb::PVP_ARENA_RESULT_SELF_IS_FIGHTING ) ;
		Send( pb::SG2D_PVP_MARK_FIGHT_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}

	if( pTargetRank->playerId != info.player_id() )
	{
		retInfo.set_result( pb::PVP_ARENA_RESULT_FIGHT_MARK_ERROR ) ;
		Send( pb::SG2D_PVP_MARK_FIGHT_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}

	retInfo.mutable_infos()->set_rank( info.rank() ) ;
	if( !GetPlayerBaseData( info.player_id(), *retInfo.mutable_infos()->mutable_player_info() ) )
	{
		retInfo.mutable_infos()->Clear() ;
		retInfo.set_result( pb::PVP_ARENA_RESULT_OTHER_ERROR ) ;
		Send( pb::SG2D_PVP_MARK_FIGHT_RET, packet.GetSessionId(), retInfo ) ;
		return ;
	}

	pSelfRank->extra.isFighting     =true ;
	pSelfRank->extra.targetPlayerId =info.player_id() ;

	pTargetRank->extra.isFighting   =true ;

	retInfo.set_result( pb::PVP_ARENA_RESULT_SUCCESS ) ;
	Send( pb::SG2D_PVP_MARK_FIGHT_RET, packet.GetSessionId(), retInfo ) ;*/
}


void GameServerSession::HandleWebPay( NetPack &packet )
{

	pb::WebPayToDatabase info ;
	packet >> info ;

	uint64 playerId =info.player_id() ;

	pb::WebPayToDatabaseRet retInfo ;
	retInfo.set_is_ok( true ) ;
	retInfo.set_order_id( info.order_id() ) ;
	retInfo.set_player_id( info.player_id() ) ; 
	retInfo.set_channel(info.channel());
	retInfo.set_money(info.money());
	retInfo.set_platform(info.platform());
	bool isOnline  =false ;

    PlayerDataPtr playerDataPtr = m_sessionDataPtr->GetPlayerData( playerId);
    if( playerDataPtr.get())  // Get Cache
	{
        isOnline = ( playerDataPtr->GetIsOffline() == false) ;

		// SDK 账号检查
        if( playerDataPtr->m_PlayerTable->sdkUserId != info.sdk_account() )
		{
			retInfo.set_result(pb::CAN_NOT_FIND_ACCOUNT);
			retInfo.set_is_ok( false ) ;
			retInfo.set_error_msg( "sdk account error!" ) ;
			Send( pb::SG2D_WEB_PAY_RET, packet.GetSessionId(), retInfo ) ;
			return ;
		}
	}
	else
	{
		playerDataPtr = m_sessionDataPtr->GetOrLoadPlayerData(playerId);
		if (playerDataPtr)
		{
			isOnline = ( playerDataPtr->GetIsOffline() == false) ;
			// SDK 账号检查
			if( playerDataPtr->m_PlayerTable->sdkUserId != info.sdk_account() )
			{
				retInfo.set_result(pb::CAN_NOT_FIND_ACCOUNT);
				retInfo.set_is_ok( false ) ;
				retInfo.set_error_msg( "sdk account error!" ) ;
				Send( pb::SG2D_WEB_PAY_RET, packet.GetSessionId(), retInfo ) ;
				return ;
			}
		}
		else
		{
			retInfo.set_is_ok( false ) ;
			retInfo.set_result(pb::CAN_NOT_FIND_ROLE);
			retInfo.set_error_msg( "role id error!" ) ;
			Send( pb::SG2D_WEB_PAY_RET, packet.GetSessionId(), retInfo ) ;
			return ;
		}
	}

	// 1：获取购买的商品
    PayItemIntValue payItemVal ;
    packet.ReadStruct( payItemVal ) ;
    // 4
    uint32 vip_exp = 0; // 充值所得经验
    uint32 final_crystal =0 ;  // 充值所得水晶
	vip_exp = payItemVal.reward_diamond;
	final_crystal = payItemVal.reward_diamond;
    //
    retInfo.set_is_online( isOnline ) ; 
	retInfo.mutable_order_record()->set_is_new(true);
	retInfo.mutable_order_record()->set_vip_xp(vip_exp);
	retInfo.set_result(pb::WEB_PAY_RERSULT_OK);
	playerDataPtr->m_PlayerTable->vipExp += vip_exp;
	bool hadFirstPay = Utility::HasFlag(playerDataPtr->m_PlayerTable->flags,(uint32)pb::PLAYER_FLAG_HAD_FIRST_PAY);
	if(hadFirstPay)
	{
		playerDataPtr->m_PlayerTable->cashGold += final_crystal;
		retInfo.mutable_order_record()->set_crystal_final(final_crystal);
	}
	else
	{
		if (std::strcmp(payItemVal.product_id.c_str(),"com.yxjq.m30") == 0)
		{
			playerDataPtr->m_PlayerTable->buy_month_card_normal = sOS.TimeSeconds();
			playerDataPtr->m_PlayerTable->cashGold += final_crystal;
			retInfo.mutable_order_record()->set_crystal_final(final_crystal);
		}
		else if (std::strcmp(payItemVal.product_id.c_str(),"com.yxjq.m50") == 0)
		{
			playerDataPtr->m_PlayerTable->buy_month_card_advance = sOS.TimeSeconds();
			playerDataPtr->m_PlayerTable->cashGold += final_crystal;
			retInfo.mutable_order_record()->set_crystal_final(final_crystal);
		}
		else
		{
			playerDataPtr->m_PlayerTable->cashGold += (2*final_crystal);
			retInfo.mutable_order_record()->set_crystal_final(2*final_crystal);
		}
		Utility::SetFlag(playerDataPtr->m_PlayerTable->flags,(uint32)pb::PLAYER_FLAG_HAD_FIRST_PAY);
	}
	playerDataPtr->m_PlayerTable.SaveMod();
    // 发到游戏服务器
    Send( pb::SG2D_WEB_PAY_RET, packet.GetSessionId(), retInfo ) ;
}


void GameServerSession::HandleUpdatePlayerModule( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( pb::GxDB_Module, UpdateModuleInfo);
}

void GameServerSession::HandleUpdatePlayerBuilding( NetPack &packet )
{
	FIND_PLAYER_AND_DO_STH( pb::GxDB_Building, UpdateBuildInfo);
}

void GameServerSession::HandleGetPlayerAppendData( NetPack &packet )
{
	pb::SG2D_Player_Id info ;
	packet >> info ;
	PlayerDataPtr dataPtr = m_sessionDataPtr->GetPlayerData( info.player_id());
	if( dataPtr.get())  // Get Cache
	{
		pb::PlayerAppendData AppendData;
		dataPtr->WriteBuildInfo(*AppendData.mutable_building());
		dataPtr->WriteModuleInfo(*AppendData.mutable_module());
		Send( pb::SD2G_GET_PLAYER_APPEND_DATA_RET, packet.GetSessionId(), AppendData) ;
	}
}

void GameServerSession::HandleForbidenPlayer( NetPack &packet )
{
	pb::SG2D_Forbiden_Player info ;
	packet >> info ;
	PlayerDataPtr dataPtr = m_sessionDataPtr->GetPlayerData( info.player_id());
	if( dataPtr.get())  // Get Cache
	{
		if(m_sessionDataPtr->UpdateForbidenInfo(dataPtr->m_PlayerTable.GetContent() ,info))
		{
			dataPtr->m_PlayerTable.SaveMod();
			if (info.forbidid() == pb::ACCOUNT_OP_KICK_OFF_LINE)
			{
				m_sessionDataPtr->RemovePlayerDataFromMap(info.player_id());
			}
			else
			{
				pb::SD2G_Forbiden_Player_Ret forbidenRet;
				forbidenRet.set_acountid(dataPtr->m_PlayerTable->accountId);
				forbidenRet.set_player_name(dataPtr->m_PlayerTable->name);
				forbidenRet.set_forbidid(info.forbidid());
				forbidenRet.set_time(info.time());
				Send(pb::SD2G_UNLOCK_FORBIDEN_RET,forbidenRet);
			}
		}
	}
	else
	{
		if (info.forbidid() == pb::ACCOUNT_OP_KICK_OFF_LINE)
		{
			return;
		}
		PlayerTableCacheType    temp_PlayerTable;
		PlayerTable *pPlayerTable = sDataMgr.GetPlayerTable(temp_PlayerTable,info.player_id());
		if(m_sessionDataPtr->UpdateForbidenInfo(pPlayerTable,info))
		{
			temp_PlayerTable.WriteEntry(pPlayerTable);

			pb::SD2G_Forbiden_Player_Ret forbidenRet;
			forbidenRet.set_acountid(pPlayerTable->accountId);
			forbidenRet.set_player_name(pPlayerTable->name);
			forbidenRet.set_forbidid(info.forbidid());
			forbidenRet.set_time(info.time());
			Send(pb::SD2G_UNLOCK_FORBIDEN_RET,forbidenRet);
		}
	}
}

void GameServerSession::HandleSavePlayerCache( NetPack &packet )
{
	pb::SG2D_Player_Id info;
	packet >> info ;
	m_sessionDataPtr->SavePlayerCache(info.player_id());
}

void GameServerSession::HandleSaveAllPlayerCache(NetPack& packet)
{
	m_sessionDataPtr->SaveAllPlayerCache();
}

void GameServerSession::HandleSetNameAndHeadId( NetPack& packet )
{
	pb::MSG_SetNameAndHeadId msg; 
	packet >> msg ;

	if( sDataMgr.GetHadThisPlayerInDB( msg.name() ) )
	{
		msg.set_err_code( pb::ErrNameAlreadyExist ) ;
		Send( pb::SD2G_PLAYER_SET_NAME_AND_HEADID_RET, packet.GetSessionId(), msg ) ;
		return ;
	}

	PlayerDataPtr ptr = m_sessionDataPtr->GetPlayerData( msg.player_id());
	if ( ptr.get())
	{
		ptr->SetNameAndHeadId( msg.name(),msg.head_id());
	}

	msg.set_err_code( pb::ErrCommonSuccess ) ;
	Send( pb::SD2G_PLAYER_SET_NAME_AND_HEADID_RET, packet.GetSessionId(), msg ) ;
}

void GameServerSession::HandleUpdateActivityData( NetPack& packet )
{
	pb::SC2G_activity_package_Update msg;
	packet >> msg;
	pb::SC2G_activity_package sendmsg;
	if(msg.update())
	{
		m_gloabalDataPtr->UpdateActivityAllInfo(msg.all_info());
	}
	for(std::map<uint64,bool>::iterator it = m_gloabalDataPtr->vec_modify[MODIFY_TYPE_PLAN].begin();it !=m_gloabalDataPtr->vec_modify[MODIFY_TYPE_PLAN].end();++it)
	{
		ActivityPlanTable* ac_plan = m_gloabalDataPtr->m_ActivityPlanTable.GetElement(it->first);
		if(it->second && ac_plan)
		{
			pb::SC2G_activity_plan * info = sendmsg.add_activity_plan();
			if(ac_plan->open_way == REGIST_WAY)
			{
				info->set_table_id(ac_plan->id);
				info->set_activity_id(ac_plan->activity_id);
				info->set_open_way(REGIST_WAY);
				info->set_start_time(ac_plan->start_time);
				info->set_last_time(ac_plan->last_time);
				info->set_show_time(ac_plan->show_time);
				info->set_stage_area(ac_plan->stage_area);
				info->set_need_greater_than_open(ac_plan->need_greater_than_open);
				info->set_need_less_than_open(ac_plan->need_less_than_open);
			}
			else if(ac_plan->open_way == MANAL_AN_TIME_WAY)
			{
				info->set_table_id(ac_plan->id);
				info->set_activity_id(ac_plan->activity_id);
				info->set_open_way(MANAL_AN_TIME_WAY);
				info->set_start_date(ac_plan->start_date);
				info->set_end_date(ac_plan->end_date);
				info->set_end_show_date(ac_plan->end_show_date);
				info->set_stage_area(ac_plan->stage_area);
				info->set_need_greater_than_open(ac_plan->need_greater_than_open);
				info->set_need_less_than_open(ac_plan->need_less_than_open);
			}
		}
	}
	for(std::map<uint64,bool>::iterator it = m_gloabalDataPtr->vec_modify[MODIFY_TYPE_PROTO].begin();it !=m_gloabalDataPtr->vec_modify[MODIFY_TYPE_PROTO].end();++it)
	{
		ActivityProtoTable* ac_proto = m_gloabalDataPtr->m_ActivityProtoTable.GetElement(it->first);
		if(it->second && ac_proto)
		{
			pb::SC2G_activity_proto * info = sendmsg.add_activity_proto();
			info->set_table_id(ac_proto->id);
			info->set_act_type_id(ac_proto->act_enum_type);
			info->set_reward_type(ac_proto->reward_type);
		}
	}
	for(std::map<uint64,bool>::iterator it = m_gloabalDataPtr->vec_modify[MODIFY_TYPE_REWARD].begin();it !=m_gloabalDataPtr->vec_modify[MODIFY_TYPE_REWARD].end();++it)
	{
		ActivityRewardTable* ac_reward = m_gloabalDataPtr->m_ActivityRewardTable.GetElement(it->first);
		if(it->second && ac_reward)
		{
			pb::SC2G_activity_reward * info = sendmsg.add_activity_reward();
			info->set_table_id(ac_reward->id);
			info->set_cost_price(ac_reward->cost_price);
			info->set_activity_id(ac_reward->activity_id);
			info->set_conditon(ac_reward->condition);
			info->set_cost_discount(ac_reward->cost_discount);
			info->set_reward_list(ac_reward->reward_list);
			info->set_is_has_discount(ac_reward->is_has_discount);
		}
	}
	for(std::map<uint64,bool>::iterator it = m_gloabalDataPtr->vec_modify[MODIFY_TYPE_OPEN_TIME].begin();it !=m_gloabalDataPtr->vec_modify[MODIFY_TYPE_OPEN_TIME].end();++it)
	{
		ServerOpenTimeTable* server_open_time = m_gloabalDataPtr->m_ServerOpenTimeTable.GetElement(it->first);
		if(it->second && server_open_time)
		{
			pb::SC2G_server_open_time * info = sendmsg.add_server_open_time();
			info->set_table_id(server_open_time->id);
			info->set_server_id(server_open_time->server_id);
			info->set_open_time(server_open_time->open_time);
		}
	}
	Send(pb::SD2G_UPDATE_ACTIVITY_DATA_RET,sendmsg);
}

void GameServerSession::HandleDeleteActivityStarBoxTable(NetPack& packet)
{
	pb::GxDB_Hero_Reward_delete msg;packet >>msg;
	if(msg.operate_type() == pb::DATA_DEL)
	{
		m_sessionDataPtr->ClearActivityStartBoxTable();
	}
}

void GameServerSession::HandleDeleteGeneralRewardTable(NetPack& packet)
{
	pb::GxDB_Hero_Reward_delete msg;packet >> msg;
	if(msg.operate_type() == pb::DATA_DEL)
	{
		m_sessionDataPtr->ClearGeneralRewardTable();
	}
}

void GameServerSession::HandleSelectCountry(NetPack& packet)
{
	pb::GS2C_Select_Country_Rsp msg;
	IntPairVec vec;
	uint32 country_id = 0;
	sDataMgr.GetSameCountryNum(vec);
	if(vec.size() < 3)
	{
		if(vec.size() == 1)
		{
RAND:
			country_id =System::Rand(1,4);
			if(country_id == vec[0].first)//choose one from [1,4) 
			{
				goto RAND;
			}
		}
		else if(vec.size() == 0)
		{
			country_id = System::Rand(1,4);//choose one from [1,4) 
		}
		else if(vec.size() == 2)
		{
			if(vec[0].first == 1)
			{
				if(vec[1].first == 2) country_id = 3;
				if(vec[1].first == 3) country_id = 2;
			}
			if(vec[0].first == 2)
			{
				if(vec[1].first == 1) country_id = 3;
				if(vec[1].first == 3) country_id = 1;
			}
			if(vec[0].first == 3)
			{
				if(vec[1].first == 1) country_id = 2;
				if(vec[1].first == 2) country_id = 1;
			}
		}
		msg.set_country_id(country_id);
		Send(pb::SD2G_SELECT_COUNTRY_RET,packet.GetSessionId(),msg);
		return;
	}
#ifndef MIN3
#define MIN3(x,y,z)						MIN(MIN(x,y),z)
#endif
#ifndef MAX3
#define MAX3(x,y,z)						MAX(MAX(x,y),z)
#endif
	int min = MIN3(vec[0].second,vec[1].second,vec[2].second);
	if(min == vec[0].second) country_id = vec[0].first;
	else if(min == vec[1].second) country_id = vec[1].first;
	else if(min == vec[2].second) country_id = vec[2].first;
	msg.set_country_id(country_id);
	Send(pb::SD2G_SELECT_COUNTRY_RET,packet.GetSessionId(),msg);
	return;
}

void GameServerSession::HandleSaveDbproxy( NetPack& packet )
{
	m_sessionDataPtr->SaveAndRemoveAllPlayerCache();
}


#undef Msg_Declare
#define Msg_Declare(e, MsgType) void GameServerSession::Handle_##e(NetPack &packet) { \
            FIND_PLAYER_AND_DO_STH(MsgType, Player_##e); \
        }
#include "../msg/PlayerDataMsg.h"


#undef Msg_Realize
#define Msg_Realize(e) void GameServerSession::Handle_##e(NetPack &packet)
Msg_Realize(SG2D_COUNTRY_BASE)
{
    pb::CountyInfo msg; packet >> msg;
    if (CountryBaseTable* pCell = m_sessionDataPtr->m_CountryBaseTable.GetElement(msg.id()))
    {
        pCell->LoadFrom(msg);
        m_sessionDataPtr->m_CountryBaseTable.SaveOneMod(pCell);
    }
    else
    {
        CountryBaseTable cell;
        cell.region_id = GetRegionId();
        cell.server_id = GetServerId();
        cell.LoadFrom(msg);
        m_sessionDataPtr->m_CountryBaseTable.AddAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_COUNTRY_ADD_CITY)
{
    pb::Struct_City_Info msg; packet >> msg;
    CachePtrMap< uint32, CountryCityTable>& table = m_sessionDataPtr->m_CountryCityTable[msg.country()-1];

    CountryCityTable cell;
    cell.region_id = GetRegionId();
    cell.server_id = GetServerId();
    cell.LoadFrom(msg);
	if (!table.Contain(cell.GetLogicalKey()))
	{
		table.AddAndAddToCache(&cell);
	}
}
Msg_Realize(SG2D_COUNTRY_DEL_CITY)
{
    pb::Struct_City_Info msg; packet >> msg;
    CachePtrMap< uint32, CountryCityTable>& table = m_sessionDataPtr->m_CountryCityTable[msg.country()-1];
    table.DeleteEntry(msg.city_id());
}
Msg_Realize(SG2D_COUNTRY_ALL_INFO)
{
    pb::SD2G_Country_AllInfo msg; packet >> msg;

    for (int i = 0; i < msg.base_info_size(); ++i)
    {
        const pb::CountyInfo& baseInfo = msg.base_info(i);
        if (CountryBaseTable* pCell = m_sessionDataPtr->m_CountryBaseTable.GetElement(baseInfo.id()))
        {
            pCell->LoadFrom(baseInfo);
            m_sessionDataPtr->m_CountryBaseTable.SaveOneMod(pCell);
        }
        else
        {
            CountryBaseTable cell;
            cell.region_id = GetRegionId();
            cell.server_id = GetServerId();
            cell.LoadFrom(baseInfo);
            m_sessionDataPtr->m_CountryBaseTable.AddAndAddToCache(&cell);
        }
    }

    for (int i = 0; i < MONSTER_COUNTRY_ID; ++i)
    {
        m_sessionDataPtr->m_CountryCityTable[i].RemoveSelfEntryFromCacheAndDb();
    }
    for (int i = 0; i < msg.city_lst_size(); ++i)
    {
        const pb::Struct_City_Info& info = msg.city_lst(i);

        CachePtrMap< uint32, CountryCityTable>& tableLst = m_sessionDataPtr->m_CountryCityTable[info.country()-1];

        CountryCityTable cell;
        cell.region_id = GetRegionId();
        cell.server_id = GetServerId();
        cell.LoadFrom(info);
        tableLst.AddAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_COUNTRY_LEVLE_UP_TASK_RANK)
{
    pb::Country_Levle_Up_Task_Rank msg; packet >> msg;

    CachePtrMap< uint64, CountryLevelUpTaskPlayerTable>& tableLst = m_sessionDataPtr->m_CountryLevelUpTaskPlayerTable[msg.country() - 1];

    tableLst.RemoveAllEntry();

    for (int i = 0; i < msg.player_lst_size(); ++i)
    {
        CountryLevelUpTaskPlayerTable cell;
        cell.region_id = GetRegionId();
        cell.server_id = GetServerId();
        cell.LoadFrom(msg.player_lst(i));
        tableLst.AddAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_PALACE_ALL_INFO)
{
    pb::GS2C_Palace_All_Info req; packet >> req;

    for (int i = 0; i < req.office_lst_size(); ++i)
    {
        const pb::OfficeInfo& info = req.office_lst(i);

        CachePtrMap< int, PalaceOfficeTable>& tableMap = m_sessionDataPtr->m_PalaceOfficeTable[info.country_id() - 1];

        if (PalaceOfficeTable* pCell = tableMap.GetElement(info.office_id()))
        {
            pCell->LoadFrom(info);
            tableMap.SaveOneMod(pCell);
        }
        else
        {
            PalaceOfficeTable cell;
            cell.region_id = GetRegionId();
            cell.server_id = GetServerId();
            cell.LoadFrom(info);
            tableMap.AddAndAddToCache(&cell);
        }
    }
}
Msg_Realize(SG2D_ACTIVITY_COUNTRY_OCCUPY_CITY_UPDATE)
{
	pb::Activity_Occupy_City_Num msg; packet >> msg;
	//CachePtrMap< uint64, ActivityCountryOccupyCityTable>& tableLst = m_gloabalDataPtr->m_ActivityCountryOccupyCityTable;
	if(ActivityCountryOccupyCityTable* pcell = m_gloabalDataPtr->m_ActivityCountryOccupyCityTable.GetElement(msg.country_id()))
	{
		pcell->LoadFrom(msg);
		m_gloabalDataPtr->m_ActivityCountryOccupyCityTable.SaveOneMod(pcell);
	}
	else
	{
		ActivityCountryOccupyCityTable cell;
		cell.LoadFrom(msg);
		m_gloabalDataPtr->m_ActivityCountryOccupyCityTable.AddAndAddToCache(&cell);
	}
}

