#include "PlayerStageLogic.h"

#include "Base.h"
#include "utility/Utility.h"

#include "server_client/DbProxyServerClient.h"

#include "object/Player.h"
#include "object/PlayerDataManager.h"

#include "stage/StageDefine.h"
#include "datastruct/struct_stage.h"
#include "stage/StageLevelLog.h"
#include "stage/StageLog.h"
#include "stage/StageLogger.h"
#include "stage/StageManager.h"

#include "character/CharacterStorage.h"
#include "character/Character.h"

#include "Loot/LootDefine.h"
#include "CrossLogic/PlayerLootLogic.h"


#include "quest/behavior/PlayerBehaviorCounter.h"


#include "Enum.pb.h"
#include "InterServer.pb.h"
#include "ServerOpcode.pb.h"
#include "google/protobuf/repeated_field.h"


namespace 
{
    void GenBattleArrayProtoStr( Player& player, std::string& out_str)
    {
        CharacterStorage* charStor = player.m_characterStorage.get();
        const CharacterIds& ids = charStor->GetBattleArray();
        for (CharacterIds::const_iterator it = ids.begin();
            it != ids.end(); ++it)
        {
            
            if ( *it == 0)
                continue;
            const Character* ch = charStor->GetCharacter(*it);
            if ( ch == NULL)
                continue;

            char tmp[32] = {};
            snprintf( tmp, 32, "%d", ch->ProtoID());
            if ( !out_str.empty())
            {
                out_str.append(" ");
            }
            out_str.append( tmp);
        }
    }

    unsigned GetQuestType(const DB_StageLevel& proto)
    {
        //switch( proto.level_type_main())
        //{
        //case STAGE_LEVEL_TYPE_MAIN_NORMAL_MODE:
        //    {
        //        switch( proto.GetStageType())
        //        {
        //        case STAGE_NORMAL:   return QC2_NORMAL_LEVEL;// 普通模式
        //        case STAGE_ELITE:    return QC2_ELITE_LEVEL;// 精英模式
        //        case STAGE_NIGHTMARE:return QC2_NIGHTMARE_LEVEL; //恶梦模式
        //        } 
        //    }
        //case STAGE_LEVEL_TYPE_MAIN_ENDLESS_MODE:    return QC2_ENDLESS_MOD;
        //case STAGE_LEVEL_TYPE_MAIN_BOSS_MODE:       return QC2_BOSS_MOD;
        //case STAGE_LEVEL_TYPE_MAIN_GOLD_MODE:       return QC2_GOLD_MOD;
        //default:
        //    break;
        //}
        return 999;
    }



    uint32 GetStarFromDieHero( uint32 dieHero)
    {
        if (dieHero == 0)
        {
            return 3;
        }
        else if (dieHero == 1)
        {
            return 2;
        }
        else if ( dieHero >= 2)
        {
            return 1;
        }
        else
            return 0;
    }


    typedef google::protobuf::RepeatedPtrField< ::pb::PairValue > RepeatedPair;
    void MergePairValue( RepeatedPair& to, const RepeatedPair& from, bool isOverLap)
    {
        if (isOverLap)
        {
            for( RepeatedPair::const_iterator fi = from.begin(); fi != from.end(); ++fi)
            {
                bool exist = false;
                for ( RepeatedPair::iterator ti = to.begin(); ti != to.end(); ++ti)
                {
                    if (fi->first() == ti->first())
                    {
                        ti->set_second( ti->second() +  fi->second());
                        exist = true;
                        break;
                    }             
                }
                if ( !exist)
                {
                    ::pb::PairValue * pa = to.Add();
                    if ( pa)
                    {
                        pa->set_first(fi->first());
                        pa->set_second(fi->second());
                    }
                }
            }
        }
        else
        {
            std::copy( from.begin(), from.end(), RepeatedFieldBackInserter( &to));
        }
    }


}


void PlayerStageLogic::GiveRewardToPlayer( Player& player, const DB_StageLevel& sl, pb::GS2C_Reward* reward , bool bGiveItem, float factor )
{
    pb::PairValue*  rewardCoin = reward->add_curency_type_count();
    rewardCoin->set_first( sl.level_reward_curr_type);

    uint32 curr = (uint32)( sl.level_reward_curr_value * factor);
    uint32 exp =  (uint32)( sl.level_reward_exp * factor);

    rewardCoin->set_second( curr);
    reward->set_exp( exp);

    player.AddCurrency( pb::IR_STAGE_LEVEL_LOOT_GET, sl.level_reward_curr_type, curr);
    player.AddXP( exp);

    if( !bGiveItem )
    {
        return ;
    }

    // Loot
    pb::GS2C_LootList tempList;
    LootParam param;
    param.provider           = player.m_lootRecord.get();
    param.victim_type        = LOOTTYPE_STAGELEVEL;
    param.victim_id          = sl.id;
    param.victim_lv          = sl.level_unlock_level;
    param.player_vip_lv      = player.GetVipLevel();

    bool isHasLoot = PlayerLootLogic::PlayerLoot(player, tempList, param, pb::IR_STAGE_LEVEL_LOOT_GET);
    if (isHasLoot)
    {
        MergePairValue( *reward->mutable_item_id_count(), tempList.item_id_count(), false);
        MergePairValue( *reward->mutable_curency_type_count(), tempList.curency_type_count(), false);
    }
}
void PlayerStageLogic::CalculateBattleResult( Player& player, const pb::C2GS_BattleData& fromData, pb::GS2C_BattleResult& outResult )
{
    StageLogger* SL = player.m_stageLogger.get();

    const DB_StageLevel* theStageLevel = sStageManager.GetStageLevelById( fromData.stage_level_id());
    if ( theStageLevel == NULL)
        return;

    bool matchPreBattleID = player.IsMatchPreBattleId( fromData.pre_battle_id());
    player.ResetPrebattleId();
    if ( !matchPreBattleID)
    {
        outResult.set_err_code( pb::ErrPrebattleIdNotMatch);
        return;
    }
    outResult.set_is_battle_win( true) ;

    switch( theStageLevel->stage_type )
    {
    case STAGE_LEVEL_TYPE_MAIN_NORMAL_MODE:
        {
            if( outResult.is_battle_win())
            {
                if ( !TryCost( player, *theStageLevel, 1))
                {
                    outResult.set_err_code( pb::ErrNotEnoughCurr);
                    return;
                }

                StageLevelLog* levelLog = AddOrGetStageLevelLog( player, fromData.stage_level_id());
                if ( levelLog)
                {
                    PlayerStageLevelLogic::CalculateBattleResult( player, *SL, *levelLog,  fromData, outResult) ;
                }
            }
        } break;
    default:
        break;
    }
}

StageLevelLog* PlayerStageLogic::AddOrGetStageLevelLog( Player& player, uint32 level_id )
{
    StageLogger* logger = player.m_stageLogger.get();
    if ( logger == NULL)
        return NULL;

    bool newStage = false, newLevel = false;
    StageLevelLog* stage_level_log = logger->AddOrGetStageLevelLog( level_id, newStage, newLevel);
    if ( newLevel && stage_level_log)
    {
        PlayerStageLevelLogic::SendStageLevelLogToDb( player, *stage_level_log);
    }
    
    return stage_level_log;
}

void PlayerStageLogic::GmClearLevelById( Player& player, uint32 level_id )
{
    StageLogger* logger = player.m_stageLogger.get();
    if ( logger == NULL)
        return;


    pb::GS2C_BattleResult battleResult;
    const std::set<uint32>&  levelids = sStageManager.GetAllLevelIds();
	for (std::set<uint32>::const_iterator iter = levelids.begin(); iter != levelids.end();iter++)
	{
		uint32 id = *iter;
		if (level_id == 0 || id <= level_id)
		{
			pb::C2GS_BattleData battle;
			battle.set_stage_level_id(id);
			StageLevelLog* levelLog = AddOrGetStageLevelLog( player, id);
			if ( levelLog)
			{
				battleResult.set_is_battle_win(true);
				PlayerStageLevelLogic::CalculateBattleResult( player, *logger, *levelLog, battle, battleResult );
			}
		}
		else
		{               
			continue;
		}
	}
}

bool PlayerStageLogic::TryCost( Player& player, const DB_StageLevel& level, uint32 count )
{
    switch( level.level_cost_type)
    {
    case CONSU_NONE:
        return true ;
    case CONSU_GOLDEN:
        return player.TryDeductCurrency(
			pb::IR_STAGE_LEVEL_MODE_COST,
            eSysGold,
            level.level_cost_value * count);
    default:
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////
void PlayerStageLevelLogic::SendStageLevelLogToDb( Player& player, const StageLevelLog& log )
{
    pb::SG2D_UpdataStageLevel  stageLevelUpdateMsg;
    log.SaveTo(*stageLevelUpdateMsg.mutable_info());

    stageLevelUpdateMsg.set_op_type( pb::DATA_UPDATE);
    sDbProxyClient.SendPlayerPack( player, pb::SG2D_UPDATA_STAGELEVEL_INFO, stageLevelUpdateMsg ) ;
}

void PlayerStageLevelLogic::TriggerQuestBranchSuccess( Player &player, const DB_StageLevel& proto)
{
    //if ( PlayerBehaviorCounter * counter = player.m_behaviorCounter.get())
    //{
    //    unsigned cat2 = GetQuestType(proto);
    //    counter->TriggerBehavior(&player, QC1_PVE, QC2_SPECIFIED_LEVEL, QC3_BRANCH_SUCCESS, proto.id());
    //    counter->TriggerBehavior(&player, QC1_PVE, cat2, QC3_BRANCH_SUCCESS);
    //}      
}

void PlayerStageLevelLogic::TriggerQuestLevelFinish( Player &player, const DB_StageLevel& proto, int count /*= 1*/ )
{
 /*   unsigned cat2 = GetQuestType(proto);
    if ( PlayerBehaviorCounter * counter = player.m_behaviorCounter.get())
    {
        if (proto.IsArenaMode())
        {
            counter->TriggerBehavior(&player, QC1_PVP, QC2_UNDEFINED, QC3_DO_PVP, 0 ,count);
            counter->TriggerBehavior(&player, QC1_PVP, QC2_UNDEFINED, QC3_PVP_SUCCESS, 0 ,count);
        }
        else
        {
            counter->TriggerBehavior( &player, QC1_PVE, QC2_SPECIFIED_LEVEL, QC3_SUCCESS_TIME, proto.id() , count);
            counter->TriggerBehavior(
                &player,
                QC1_PVE, 
                cat2,
                QC3_SUCCESS_TIME , 0, count);
        }


    }
    
    IQuestLogger* logger = player.m_questLogger.get();
    if ( logger)
    {
        logger->UpdateQuest( QC1_PVE, QC2_SPECIFIED_LEVEL, QC3_GET_STAR_COUNT, proto.id() );
        logger->UpdateQuest(
            QC1_PVE,
            cat2,
            QC3_GET_STAR_COUNT );
    }*/
}

void PlayerStageLevelLogic::OnFinishLevel( Player &player, StageLevelLog& log, pb::GS2C_BattleResult &outResult )
{
    pb::GS2C_Reward* reward = outResult.mutable_battle_reward();
    PlayerStageLogic::GiveRewardToPlayer( player, log.Proto(), reward );
    log.AddFightCountCurPeriod( 1);
    SendStageLevelLogToDb(player, log);
    TriggerQuestLevelFinish( player, log.Proto() ) ;


}

void PlayerStageLevelLogic::CalculateBattleResult(
    Player& player, StageLogger& stageloger,  StageLevelLog& log,
    const pb::C2GS_BattleData& fromData, pb::GS2C_BattleResult& outResult   )
{
    
    if ( outResult.is_battle_win())
    {
        log.SetClearance();
        OnFinishLevel( player, log, outResult);
        TriggerQuestBranchSuccess( player, log.Proto());
        string battleArrayStr;
        battleArrayStr.reserve( 20);
        GenBattleArrayProtoStr(player, battleArrayStr); 

		player.AddEventLogFMT( pb::EVENT_PLAYER_BEHAVIOR_STAGE,
            4,
            "i", player.GetLevel(),
            "i", player.GetCombatPower(),
            "s", battleArrayStr.c_str(),
            "i", log.Proto().id
            );
    }

    log.SaveTo(*outResult.mutable_stage_level_info());
}
