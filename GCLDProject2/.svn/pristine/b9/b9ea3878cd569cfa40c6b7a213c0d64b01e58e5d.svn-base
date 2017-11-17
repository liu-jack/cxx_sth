#ifdef _WIN32
    #include <stddef.h>
#else
    #include <cstdint>
#endif
#include "PlayerTable.h"

#include "memory_buffer/MultiValueArray.h"
#include "Enum.pb.h"
#include "CommomDef.pb.h"
#include "utility/MsgTool.h"
#include "boost/bind.hpp"
#include "Social.pb.h"

#include "DbProxyDataMgr.h"

#define MEM_OFFSET(structure, member) ( reinterpret_cast<intptr_t>( &((structure*)0)->member ) )
#define GET_VAL_FROM_OFFSET(valtype, offset) (*((valtype*)((char*)(this) + offset)))
#define GET_PTR_FROM_OFFSET(valtype, offset) (valtype*)((char*)(this) + offset)


void PlayerTable::SaveTo( pb::ObjectFullValue& msg ) const
{
    MultiValueArray values( pb::PLAYER_FIELD_32_BIT_COUNT, pb::PLAYER_FIELD_64_BIT_COUNT, pb::PLAYER_FIELD_FLOAT_COUNT, pb::PLAYER_FIELD_STR_COUNT ) ;

    for ( uint32 i = 0 ; i < pb::PLAYER_FIELD_32_BIT_COUNT; ++i)
    {
        if ( const int32* valPtr = GetIndexInt32CPtr(i))
        {
            values.SetInt32( i  , *valPtr ) ; 
        }            
    }

    //PvPRankTable *pRankInfo =sDataMgr.GetPlayerPvPRank( playerId ) ; 
    //values.SetInt32( pb::PLAYER_FIELD_PVP_RANK            , pRankInfo != NULL ? pRankInfo->rank : 0 ) ;
    //values.SetInt32( pb::PLAYER_FIELD_PVP_HISTORY_RANK    , pRankInfo != NULL ? pRankInfo->historyRank : 0 ) ;

    // uint64 
    for ( uint32 i = 0 ; i < pb::PLAYER_FIELD_64_BIT_COUNT; ++i)
    {
        if ( const uint64* valPtr = GetIndexUint64CPtr(i))
        {
            values.SetUint64( i  , *valPtr ) ;
            //DLOG( "######### %s  =  %llu", pb::PLAYER_64_BIT_FIELD_Name(( pb::PLAYER_64_BIT_FIELD)(i)).c_str(), *valPtr);
        }            
    }

    // string
    for ( uint32 i = 0 ; i < pb::PLAYER_FIELD_STR_COUNT; ++i)
    {
        if ( const string* valPtr = GetIndexStringCPtr(i))
        {
            values.SetString( i  , *valPtr ) ; 
        }            
    }

    // string
    for ( uint32 i = 0 ; i < pb::PLAYER_FIELD_FLOAT_COUNT; ++i)
    {
        if ( const float* valPtr = GetIndexFloatCPtr(i))
        {
            values.SetFloat( i  , *valPtr ) ; 
        }            
    }

    values.BuildFullUpdateMsg( msg ) ;
}

void PlayerTable::LoadFrom( const pb::ObjectFullValue& msg )
{
    if ( msg.has_int32_values())
    {
        for( int i = 0; i < msg.int32_values().values_size(); ++i)
        {
            if( int32* valPtr = GetIndexInt32Ptr(i))
            {
                *valPtr = msg.int32_values().values( i);
            }
        }
    }
    if ( msg.has_uint64_values())
    {
        for( int i = 0; i < msg.uint64_values().values_size(); ++i)
        {
            if( uint64* valPtr = GetIndexUint64Ptr(i))
            {
                *valPtr = msg.uint64_values().values( i);
            }
        }
    }

	ASSERT( playerId == msg.uint64_values().values( pb::PLAYER_FIELD_PLAYER_ID )  );

    if ( msg.has_float_values())
    {
        for( int i = 0; i < msg.float_values().values_size(); ++i)
        {
            if( float* valPtr = GetIndexFloatPtr(i))
            {
                *valPtr = msg.float_values().values( i);
            }
        }
    }

    if ( msg.has_string_values())
    {
        for( int i = 0; i < msg.string_values().values_size(); ++i)
        {
            if( string* valPtr = GetIndexStringPtr(i))
            {
                *valPtr = msg.string_values().values( i);
            }
        }
    }
}

bool PlayerTable::LoadUpdateFrom( const pb::ObjectValueUpdate& msg )
{
	bool needSyncSave = false;
    if ( msg.has_update_int32())
    {
       needSyncSave = MsgTool::SaveAndCheckUpdateMsg<int32>(
            msg.update_int32(),
            boost::bind(&PlayerTable::GetIndexInt32Ptr, this, _1),
			boost::bind(&PlayerTable::CheckNeedSyncSaveIndexInt32, this, _1),
            pb::PLAYER_32_BIT_FIELD_ARRAYSIZE);
    }
    if ( msg.has_update_uint64())
    {
       needSyncSave = MsgTool::SaveAndCheckUpdateMsg<uint64>(
            msg.update_uint64(),
            boost::bind(&PlayerTable::GetIndexUint64Ptr, this, _1),
			boost::bind(&PlayerTable::CheckNeedSyncSaveIndexUint64, this, _1),
            pb::PLAYER_64_BIT_FIELD_ARRAYSIZE);
    }
    if ( msg.has_update_float())
    {
       needSyncSave = MsgTool::SaveAndCheckUpdateMsg<float>(
            msg.update_float(),
            boost::bind(&PlayerTable::GetIndexFloatPtr, this, _1),
			boost::bind(&PlayerTable::CheckNeedSyncSaveIndexFloat, this, _1),
            pb::PLAYER_FLOAT_FIELD_ARRAYSIZE);
    }
    if ( msg.has_update_string())
    {
        needSyncSave = MsgTool::SaveAndCheckUpdateMsg<string>(
            msg.update_string(),
            boost::bind(&PlayerTable::GetIndexStringPtr, this, _1),
			boost::bind(&PlayerTable::CheckNeedSyncSaveIndexString, this, _1),
            pb::PLAYER_STR_FIELD_ARRAYSIZE);
    }
	return needSyncSave;
}

int32* PlayerTable::GetIndexInt32Ptr( uint32 index )
{
    static intptr_t offsets[] =
    {
		MEM_OFFSET(PlayerTable, continLoginDay),      // PLAYER_FIELD_CONTINUOUSLY_LOGIN_DAY 
		MEM_OFFSET(PlayerTable, flags),               // PLAYER_FIELD_FLAGS  
		MEM_OFFSET(PlayerTable, cardHeadIconId),      // PLAYER_FIELD_HEAD_ICON_ID 
		MEM_OFFSET(PlayerTable, guideProcess),        // PLAYER_FIELD_GUIDE_PROCESS
		MEM_OFFSET(PlayerTable, lockReason),      // PLAYER_FIELD_LOCKREASON 
        MEM_OFFSET(PlayerTable, level),               // PLAYER_FIELD_LEVEL                
        MEM_OFFSET(PlayerTable, vipLv),               // PLAYER_FIELD_VIP_LEVEL  
		MEM_OFFSET(PlayerTable, coin),				  // PLAYER_FIELD_COIN 
        MEM_OFFSET(PlayerTable, sysGold),                  // PLAYER_FIELD_SYS_GOLD                    
        MEM_OFFSET(PlayerTable, iron),               // PLAYER_FIELD_IRON                
        MEM_OFFSET(PlayerTable, food),           // PLAYER_FIELD_FOOD            
        MEM_OFFSET(PlayerTable, wood),                  // PLAYER_FIELD_WOOD                    
        MEM_OFFSET(PlayerTable, honor),               // PLAYER_FIELD_HORNOR               
        MEM_OFFSET(PlayerTable, buildingHonor),       // PLAYER_FIELD_BUILDING_HORNOR 
        MEM_OFFSET(PlayerTable, training),                // PLAYER_FIELD_TRAINING                  
		MEM_OFFSET(PlayerTable, troopsNum),                // PLAYER_FIELD_TROOPSNUM
		MEM_OFFSET(PlayerTable, freeCollectNum),                // PLAYER_FIELD_FREECOLLECTNUM  
		MEM_OFFSET(PlayerTable, superCollectNum),                // PLAYER_FIELD_SUPERCOLLECTNUM  
        MEM_OFFSET(PlayerTable, troopsOder),               // PLAYER_FIELD_TROOPSODER 
		MEM_OFFSET(PlayerTable, goldHammer),       // PLAYER_FIELD_GOLDHAMMER
        MEM_OFFSET(PlayerTable, coupon),                // PLAYER_FIELD_COUPON                  
        MEM_OFFSET(PlayerTable, marketBuyNum),                // PLAYER_FIELD_MARKETBUYNUM                  
        MEM_OFFSET(PlayerTable, freePurify),              // PLAYER_FIELD_FREEPURIFY                
        MEM_OFFSET(PlayerTable, extrmePurify),             // PLAYER_FIELD_EXTRMEPURIFY              
        MEM_OFFSET(PlayerTable, godStonePurify),           // PLAYER_FIELD_GODSTONEPURIFY   
		MEM_OFFSET(PlayerTable, cashGold),           // PLAYER_FIELD_CASH_GOLD   
		MEM_OFFSET(PlayerTable, curExp),                 // PLAYER_FIELD_CUR_XP                
		MEM_OFFSET(PlayerTable, vipExp),              // PLAYER_FIELD_CUR_VIP_XP   
		MEM_OFFSET(PlayerTable, dailyScore),           // PLAYER_FIELD_DAILY_QUEST_SCORE   
		MEM_OFFSET(PlayerTable, autoHammer),                        // PlAYER_FIELD_AUTO_HAMMER
		MEM_OFFSET(PlayerTable, login7DayRewardTakeCount),           // PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_COUNT      
		MEM_OFFSET(PlayerTable, loginMonthlyRewardTakeCount),        // PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_COUNT
		MEM_OFFSET(PlayerTable, preBattleId),                        // PlAYER_FIELD_PRE_BATTLE_ID
        MEM_OFFSET(PlayerTable, palaceLowOfficeId),                  // PlAYER_FIELD_PALACE_LOW_OFFICE_ID
		MEM_OFFSET(PlayerTable, panzerId),                  // PLAYER_FIELD_PANZER_ID
		MEM_OFFSET(PlayerTable, bagCapacity),                  // PLAYER_FIELD_BAG_CAPACITY
		MEM_OFFSET(PlayerTable, isRegistCrossServer),                  // PLAYER_FIELD_IS_REGIST_CROSS_SERVER
		MEM_OFFSET(PlayerTable, isInCrossServer),                   //PLAYER_FIELD_IS_IN_CROSS_SERVER		
		MEM_OFFSET(PlayerTable, isAutoIncreaseTroops),                  // PlAYER_FIELD_IS_AUTO_INCREASE_TROOPS
		MEM_OFFSET(PlayerTable, buyLabourTimesCurDay),                  // PLAYER_FIELD_BUY_LABOUR_TIMES
		MEM_OFFSET(PlayerTable, curLabourNum),                  // PLAYER_FIELD_LABOUR_NUM
		MEM_OFFSET(PlayerTable, curRebuildId),                  // PLAYER_FIELD_CUR_REBUILD_ID
		MEM_OFFSET(PlayerTable, isBeginRebuild),                  // PLAYER_FIELD_IS_BEGIN_REBUILD
		MEM_OFFSET(PlayerTable, curPalaceRebuildRound),                  // PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND
		MEM_OFFSET(PlayerTable, curRoundInvest1),                  // PLAYER_FIELD_CUR_ROUND_INVEST1
		MEM_OFFSET(PlayerTable, curRoundInvest2),                  // PLAYER_FIELD_CUR_ROUND_INVEST2
		MEM_OFFSET(PlayerTable, curRoundInvest3),                  // PLAYER_FIELD_CUR_ROUND_INVEST3
		MEM_OFFSET(PlayerTable, herosExpUpPercent),			//PLYAER_FIELD_HEROS_EXPUP_PERCENT
		MEM_OFFSET(PlayerTable, exploitRankValue),			//PLAYER_FIELD_EXPLOIT_RANK_VALUE
		MEM_OFFSET(PlayerTable, militarypower),				//PLAYER_FIELD_MILITARY_POWER_VALUE
		MEM_OFFSET(PlayerTable, exploitRankNormalbox),			//PLAYER_FIELD_EXPLOIT_RANK_NORMAL_BOX
		MEM_OFFSET(PlayerTable, exploitRankAdvanceBox),			//PLAYER_FIELD_EXPLOIT_RANK_ADVANCE_BOX
		MEM_OFFSET(PlayerTable, exploitRankBoxLevel),			//PLAYER_FIELD_EXPLOIT_RANK_BOX_LEVEL
		MEM_OFFSET(PlayerTable, trickRankValue),			//PLAYER_FIELD_TRICK_RANK_VALUE
		MEM_OFFSET(PlayerTable, occupyRankValue),			//PLAYER_FIELD_OCCUPY_RANK_VALUE
		MEM_OFFSET(PlayerTable,SumDinnerNumber),			//PLAYER_FIELD_SUM_DINNER_NUMBER
		MEM_OFFSET(PlayerTable,DinnerNumber),				//PLAYER_FIELD_DINNER_NUMBER
		MEM_OFFSET(PlayerTable, trainingFight),				//PLAYER_FIELD_TRAINING_FIGHT
		MEM_OFFSET(PlayerTable, trainingDouble),			//PLAYER_FIELD_TRAINING_DOUBLE
		MEM_OFFSET(PlayerTable, trainingReduce),			//PLAYER_FIELD_TRAINING_REDUCE
		MEM_OFFSET(PlayerTable, normalExploitboxLimit),			//PLAYER_FIELD_NORMAL_EXPLOIT_BOX_LIMIT
		MEM_OFFSET(PlayerTable, advanceExploitboxLimit),			//PLAYER_FIELD_ADVANCE_EXPLOT_BOX_LIMIT
		MEM_OFFSET(PlayerTable, draftOrderLimit),			//PLAYER_FIELD_DRAFT_ORDER_LIMIT
		MEM_OFFSET(PlayerTable, troopsOrderLimit),			//PLAYER_FLIELD_TROOPS_ORDER_LIMIT
		MEM_OFFSET(PlayerTable, trickrewardstep),			//PLAYER_FIELD_TRICKREWARD_STEP
		MEM_OFFSET(PlayerTable,occupyrewardstep),			//PLAYER_FIELD_OCCUPYREWARD_STEP
		MEM_OFFSET(PlayerTable,is_advancebox),				//PLAYER_FIELD_IS_ADVANCE_BOX
		MEM_OFFSET(PlayerTable,characterSlotnum),			//PLAYER_FIELD_CARD_SLOT_COUNT
		MEM_OFFSET(PlayerTable,createShadownum),			//PLAYER_FIELD_CREATESHADOW_NUM
		MEM_OFFSET(PlayerTable,today_add_xp),				//PLAYER_FIELD_TODAY_ADD_XP
		MEM_OFFSET(PlayerTable,vip_reward_take_flag),	//PLAYER_FIELD_TAKE_VIP_REWARD_FLAG
		MEM_OFFSET(PlayerTable,exploit_reward_step_id),		//PLAYER_FIELD_TAKE_EXPLOIT_BOX_STEP
		MEM_OFFSET(PlayerTable,daily_quest_reward_take_flag),//PLAYER_FIELD_TAKE_DAILY_QUEST_REWARD_FLAG
		MEM_OFFSET(PlayerTable,grow_up_quest_exploit_value),//PLAYER_FIELD_GROW_UP_QUEST_EXPLOIT_VALUE
		MEM_OFFSET(PlayerTable,xilian_count),				//PLAYER_FIELD_XILIAN_COUNT
		MEM_OFFSET(PlayerTable,char_row_num),				//PLAYER_FIELD_CHAR_ROW_NUM
		MEM_OFFSET(PlayerTable,sign_up_days),				//PLAYER_FIELD_CONTUINE_SIGNUP_DAY
		MEM_OFFSET(PlayerTable,retroactive_days),			//PLAYER_FIELD_RETROACTIVE_DAY
		MEM_OFFSET(PlayerTable,daily_sign_take_extra_reward_flag),//PLAYER_FIELD_SIGN_EXTRA_REWARD_FLAG
		MEM_OFFSET(PlayerTable,activity_star_box_level),//PLAYER_FIELD_ACTIVITY_STAR_BOX_LEVEL
		MEM_OFFSET(PlayerTable,use_free_quick_combat_times),//PLAYER_FIELD_USE_FREE_QUICK_COMBAT_TIMES
		MEM_OFFSET(PlayerTable,open_fog_count),	//PLAYER_FIELD_OPEN_FOG_COUNT
		MEM_OFFSET(PlayerTable,is_fog_take_reward),//PLAYER_FIELD_TAKE_FOG_REWARD_FLAG
		MEM_OFFSET(PlayerTable,activity_xilian_count),//PLAYER_FIELD_ACTIVITY_XILIAN_COUNT
		MEM_OFFSET(PlayerTable,is_xilian_take_reward),//PLAYER_FIELD_TAKE_XILIAN_REWARD_FLAG
		MEM_OFFSET(PlayerTable,iron_collect_times),//PLAYER_FIELD_IRON_COLLECT_TIMES
		MEM_OFFSET(PlayerTable,iron_collect_take_flag),//PLAYER_FIELD_IRON_COLLECT_REWARD_FLAG
		MEM_OFFSET(PlayerTable,iron_collect_time_ten),//PLAYER_FIELD_IRON_COLLECT_TEN
		MEM_OFFSET(PlayerTable,activity_xilian_ten),//PLAYER_FIELD_XILIAN_TEN_TIMES
		MEM_OFFSET(PlayerTable,people_loyal),		//PLAYER_FIELD_PEOPLE_LOYAL
		MEM_OFFSET(PlayerTable,zhengwu_times),		//PLAYER_FIELD_ZHENGWU_TIMES
		MEM_OFFSET(PlayerTable,renown),				//PLAYER_FIELD_RENOWN
		MEM_OFFSET(PlayerTable,star_crystal),				//PLAYER_FIELD_STARCRYSTAL
		MEM_OFFSET(PlayerTable,board),				//PLAYER_FIELD_BOARD
		MEM_OFFSET(PlayerTable,gene_code),				//PLAYER_FIELD_GENE_CODE
		MEM_OFFSET(PlayerTable,visit_famous_take_reward_state),				//PLAYER_FIELD_VISIT_FAMOUS
		MEM_OFFSET(PlayerTable,limited_shop_flag),				//PLAYER_FIELD_LIMITED_SHOP_FLAG
		MEM_OFFSET(PlayerTable,beef),				//PLAYER_FIELD_BEEF
		MEM_OFFSET(PlayerTable,wine),				//PLAYER_FIELD_WINE
		MEM_OFFSET(PlayerTable,reward_box_num),			//PLAYER_FIELD_HERO_REWARD_BOX
		MEM_OFFSET(PlayerTable,activity_gold_num),//	PLAYER_FIELD_ACTIVITY_GOLD_NUM
		MEM_OFFSET(PlayerTable,activity_recharge_take_flag),//	PLAYER_FIELD_ACTIVITY_GOLD_TAKE_FLAG
		MEM_OFFSET(PlayerTable,attack_city_num),//	PLAYER_FIELD_BRANCH_QUEST_EXPLOIT
		MEM_OFFSET(PlayerTable,world_fight_take_reward_flag),//	PLAYER_FIELD_WORLD_FIGHT_TAKE_REWARD_FLAG
		MEM_OFFSET(PlayerTable,cur_general_reward_char_id),//PLAYER_FIELD_CURR_REWARD_CHAR_ID
		MEM_OFFSET(PlayerTable,login_days),			//PLAYER_FIELD_LOGIN_DAYS
		MEM_OFFSET(PlayerTable,occupy_city_num),	//PLAYER_FIELD_OCCUPY_CITY_NUM
		MEM_OFFSET(PlayerTable,main_quest_exploit_value),	//PLAYER_FIELD_MAIN_QUEST_GET_EXPLOIT
		MEM_OFFSET(PlayerTable,today_occupy_city),	//PLAYER_FIELD_TODAY_OCCUPY_CITY
		MEM_OFFSET(PlayerTable,today_zhuzheng),	//PLAYER_FIELD_TODAY_ZHUZHENG
		MEM_OFFSET(PlayerTable,today_zhugong),	//PLAYER_FIELD_TODAY_ZHUGONG
		MEM_OFFSET(PlayerTable,today_continue_occpuy_city),	//PLAYER_FIELD_TODAY_CONTINUE_OCCPUY_CITY
		MEM_OFFSET(PlayerTable,activity_personal_occupy_city),	//PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM
		MEM_OFFSET(PlayerTable,activity_personal_add_soldier),	//PLAYER_FIELD_ACTIVITY_ADD_SOLDIER
        //////////////////////////////////////////////////////////////////////////
        //PLAYER_FIELD_PVP_RANK                            
        //PLAYER_FIELD_PVP_HISTORY_RANK                    
    };

    if ( index < sizeof( offsets) / sizeof(intptr_t) )
    {
        return GET_PTR_FROM_OFFSET( int32, offsets[index]);
    }
    else
    {
        return NULL;
    }
}

const int32* PlayerTable::GetIndexInt32CPtr( uint32 index ) const
{
    return const_cast<PlayerTable&>(*this).GetIndexInt32Ptr( index); 
}

uint64* PlayerTable::GetIndexUint64Ptr( uint32 index )
{
    static intptr_t offsets[] =
    {
        MEM_OFFSET(PlayerTable, playerId),                    // PLAYER_FIELD_PLAYER_ID
        MEM_OFFSET(PlayerTable, guildId),                     // PLAYER_FIELD_GUILD_ID 
        MEM_OFFSET(PlayerTable, lastLogin),                   // PLAYER_FIELD_LAST_LOGIN_TIME 
        MEM_OFFSET(PlayerTable, registTime),           // PLAYER_FIELD_REGIST_TIME 
        MEM_OFFSET(PlayerTable, resetHornorTime),           // PLAYER_FIELD_NEXT_RECOVER_HORNOR_TIME 
        MEM_OFFSET(PlayerTable, resetMaketBuyNumTime),              // PLAYER_FIELD_NEXT_RECOVER_MARKET_BUY_NUM_TIME 
        MEM_OFFSET(PlayerTable, resetFreePurifyTime),               // PLAYER_FIELD_NEXT_RECOVER_FREE_PURIFY_TIME 
        MEM_OFFSET(PlayerTable, resetDailyScoreTime),             // PLAYER_FIELD_NEXT_RECOVER_DAILY_QUEST_SCORE_TIME 
		MEM_OFFSET(PlayerTable, resetSmithyBuyNumTime),               // PLAYER_FIELD_NEXT_RECOVER_SMITHY_BUY_NUM_TIME 
        MEM_OFFSET(PlayerTable, login7DayRewardTakeTime),     // PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_TIME 
        MEM_OFFSET(PlayerTable, loginMonthlyRewardTakeTime),  // PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_TIME
		MEM_OFFSET(PlayerTable, curRebuildEndTime),  // PLAYER_FIELD_CUR_REBUILD_END_TIME
		MEM_OFFSET(PlayerTable, palaceTakeSalaryTime),  // PLAYER_FIELD_PALACE_TAKE_SALARY_TIME
		MEM_OFFSET(PlayerTable, lockTime),  // PLAYER_FIELD_LOCK_TIME
		MEM_OFFSET(PlayerTable,killNum),					//PLAYER_FIELD_KILL_NUM
		MEM_OFFSET(PlayerTable,enter_world_time),  //PLAYER_FIELD_ENTER_WORLD_TIME
		MEM_OFFSET(PlayerTable,buy_month_card_normal),  //PLAYER_FIELD_BUY_MONTH_CARD_NORMAL
		MEM_OFFSET(PlayerTable,buy_month_card_advance),  //PLAYER_FIELD_BUY_MONTH_CARD_ADVANCE
		MEM_OFFSET(PlayerTable,take_month_card_normal),  //PLAYER_FIELD_TAKE_MONTH_CARD_NORMAL
		MEM_OFFSET(PlayerTable,take_month_card_advance),  //PLAYER_FIELD_TAKE_MONTH_CARD_ADVANCE
		MEM_OFFSET(PlayerTable,congratulate_active_time),  //PLAYER_FIELD_CONGRATULATE_ACTIVE_TIME
	};

    if ( index < sizeof( offsets)/ sizeof(intptr_t))
    {
        return GET_PTR_FROM_OFFSET( uint64, offsets[index]);
    }
    else
    {
        return NULL;
    }
}

const uint64* PlayerTable::GetIndexUint64CPtr( uint32 index ) const
{
    return const_cast<PlayerTable&>(*this).GetIndexUint64Ptr( index); 
}

std::string* PlayerTable::GetIndexStringPtr( uint32 index )
{
    static intptr_t offsets[] =
    {
        MEM_OFFSET(PlayerTable, name),                    // PLAYER_FIELD_NAME = 0,
        MEM_OFFSET(PlayerTable, guildName),                     // PALYER_FIELD_GUILD_NAME,
		MEM_OFFSET(PlayerTable, sdkUserId),                     // PLAYER_FIELD_SDK_NAME,
    };
    if ( index < sizeof( offsets)/ sizeof(intptr_t))
    {
        return GET_PTR_FROM_OFFSET( string, offsets[index]);
    }
    else
    {
        return NULL;
    }
}

const std::string* PlayerTable::GetIndexStringCPtr( uint32 index ) const
{
    return const_cast<PlayerTable&>(*this).GetIndexStringPtr( index); 
}

float* PlayerTable::GetIndexFloatPtr( uint32 index )
{
    return NULL;
}

const float* PlayerTable::GetIndexFloatCPtr( uint32 index ) const
{
    return const_cast<PlayerTable&>(*this).GetIndexFloatPtr( index); 
}

void PlayerStringTable::SaveTo( pb::GS2C_PlayerString& msg )
{
    msg.set_str_type( str_type);
    msg.set_str( str);
}

void PlayerStringTable::LoadFrom( const pb::GS2C_PlayerString& msg )
{
    str_type = msg.str_type();
    str = msg.str();
}


bool PlayerTable::CheckNeedSyncSaveIndexUint64( uint32 index ) const
{
	switch(index)
	{
	case pb::PLAYER_FIELD_BUY_MONTH_CARD_NORMAL:
	case pb::PLAYER_FIELD_BUY_MONTH_CARD_ADVANCE:
	case pb::PLAYER_FIELD_TAKE_MONTH_CARD_NORMAL:
	case pb::PLAYER_FIELD_TAKE_MONTH_CARD_ADVANCE:
		{
			return true;
		}
	}
	return false;
}

bool PlayerTable::CheckNeedSyncSaveIndexInt32( uint32 index ) const
{
	switch(index)
	{
	case pb::PLAYER_FIELD_CUR_VIP_XP:
	case pb::PLAYER_FIELD_VIP_LEVEL:
	case pb::PLAYER_FIELD_SYS_GOLD:
	case pb::PLAYER_FIELD_CASH_GOLD:
	case pb::PLAYER_FIELD_HEAD_ICON_ID:
	case pb::PLAYER_FIELD_FLAGS:
		{
			return true;
		}
	}
	return false;
}


bool PlayerTable::CheckNeedSyncSaveIndexString( uint32 index ) const
{

	return false;
}

bool PlayerTable::CheckNeedSyncSaveIndexFloat( uint32 index ) const
{
	return false;
}
