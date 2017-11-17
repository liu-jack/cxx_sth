#include "QuestTable.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "utility/Utility.h"
#include "Quest.pb.h"
#include "Logger.h"


void QuestLogTable::SaveTo( pb::GxDB_Quest& msg )
{
    msg.set_quest_id( quest_id);
    msg.set_active_time( active_time);
}

void QuestLogTable::LoadFrom( const pb::GxDB_Quest& msg )
{
    quest_id = msg.quest_id();
    active_time = msg.active_time();
}

void QuestCompleteTable::SetFinishMask( uint32 finishQuestId )
{
    string::size_type size = finish_mask.size();
    if ( finishQuestId >= size)
    {
        Utility::RightShift( finish_mask , finishQuestId + 1 - size, '0');
    }
    finish_mask[ finish_mask.size() - 1 - finishQuestId] = '1';
}

void QuestCompleteTable::SaveTo( pb::FinishMask& msg )
{
    boost::dynamic_bitset<uint32> bits( finish_mask);     
    boost::to_block_range(bits, RepeatedFieldBackInserter( msg.mutable_finish_masks()->mutable_values()));
    msg.set_count( finish_mask.size());
}

void QuestCompleteTable::LoadFrom( const pb::FinishMask& msg )
{
    boost::dynamic_bitset<uint32> bits( msg.count());
    boost::from_block_range(
        msg.finish_masks().values().begin(),
        msg.finish_masks().values().end(),
        bits);
    boost::to_string_helper( bits, finish_mask, false);
}

void QuestCounterTable::SaveTo( pb::GxDB_Behavior& msg )
{
    msg.set_behavior( behavior_type);
    msg.set_count( count);
    msg.set_last_trigger_time( last_trigger_time);
}

void QuestCounterTable::LoadFrom( const pb::GxDB_Behavior& msg )
{
    behavior_type = msg.behavior();
    count = msg.count();
    last_trigger_time = msg.last_trigger_time();
}