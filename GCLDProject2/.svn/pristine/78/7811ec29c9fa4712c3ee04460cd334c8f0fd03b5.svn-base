#include "PlayerData.h"


// PlayerData.cpp 部分实现

//////////////////////////////////////////////////////////////////////////
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"

#include "Item.pb.h"

void PlayerData::UpdateLootRoundRecord( const pb::GS2C_LootRecord& msg )
{
   /* LootRecordTable * cell = m_LootRecordTable.GetElement( msg.loot_type_id());
    if ( cell)
    {
        SetIsDirty( DIRTY_LOOT_RECORD);
        cell->LoadFrom(msg);
        m_LootRecordTable.SetModifiedID( msg.loot_type_id());
    }
    else
    {
        LootRecordTable aLootRecord;
        aLootRecord.player_id = GetPlayerId();
        aLootRecord.LoadFrom(msg);

        m_LootRecordTable.AddAndAddToCache( &aLootRecord);
    }*/
}


void PlayerData::UpdateLotteryFree( const pb::GS2C_LotteryFreeRecord& msg )
{
   /* LotteryFreeTable * cell = m_LotteryFreeTable.GetElement( msg.lottery_type());
    if ( cell)
    {
        SetIsDirty( DIRTY_LOTTERY_RECORD);
        cell->LoadFrom(msg);
        m_LotteryFreeTable.SetModifiedID( msg.lottery_type());
    }
    else
    {
        LotteryFreeTable aLotteryRecord;
        aLotteryRecord.player_id = GetPlayerId();
        aLotteryRecord.LoadFrom(msg);

        m_LotteryFreeTable.AddAndAddToCache( &aLotteryRecord);
    }*/
}


bool PlayerData::save_mod_loot_record()
{
   /* m_LootRecordTable.SaveMod();*/
    return true;
}


bool PlayerData::save_mod_lottery_record()
{
    //m_LotteryFreeTable.SaveMod();
    return true;
}


void PlayerData::WriteLootData( pb::GS2C_AllLootRecord& allLootData )
{
 /*   for ( BOOST_AUTO( it , m_LootRecordTable.Begin());
        it != m_LootRecordTable.End(); ++it )
    {
        MsgTool::SaveToRepeatField( it->second, allLootData.mutable_records());
    }*/
}


void PlayerData::WriteLotteryData( pb::GS2C_AllLotteryInfo& allLotteryData )
{
 /*   for ( BOOST_AUTO(it, m_LotteryFreeTable.Begin());
        it != m_LotteryFreeTable.End(); ++it )
    {
        MsgTool::SaveToRepeatField( it->second, allLotteryData.mutable_free_records());
    }*/

}

