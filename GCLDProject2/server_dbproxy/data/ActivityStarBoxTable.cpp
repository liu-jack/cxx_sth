

#include "ActivityStarBoxTable.h"
#include "Activity.pb.h"

void ActivityStarBoxTable::SaveTo( pb::GxDB_ActivityStarBoxInfo& msg )
{
	msg.set_table_id(table_id);
	msg.set_is_take_reward(is_take_reward);
	msg.set_npc_is_opened(npc_is_opened);
	msg.set_left_times(npc_left_reward_times);
	msg.set_last_take_reward_time(last_take_rewrd_time);
}

void ActivityStarBoxTable::LoadFrom( const pb::GxDB_ActivityStarBoxInfo& msg )
{
	table_id = msg.table_id();
	is_take_reward = msg.is_take_reward();
	npc_is_opened = msg.npc_is_opened();
	npc_left_reward_times = msg.left_times();
	last_take_rewrd_time = msg.last_take_reward_time();
}