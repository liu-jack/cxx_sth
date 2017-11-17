

#include "GrowUpQuestTable.h"
#include "Quest.pb.h"


void GrowUpQuestTable::SaveTo( pb::GxDB_Grow_Up_Quest_Info& msg )
{
	msg.set_quest_id(quest_id);
	msg.set_cur_value(cur_value);
	msg.set_group_id(group_id);
	msg.set_is_finished(is_finished);
	msg.set_quest_type(quest_type);
	msg.set_is_take_reward(is_take_reward);
}

void GrowUpQuestTable::LoadFrom( const pb::GxDB_Grow_Up_Quest_Info& msg )
{
	quest_id = msg.quest_id();
	cur_value = msg.cur_value();
	group_id = msg.group_id();
	is_finished = msg.is_finished();
	quest_type = msg.quest_type();
	is_take_reward = msg.is_take_reward();
}

void GrowUpQuestGroupTable::SaveTo( pb::GxDB_Grow_Up_Quest_Group_Info& msg )
{
	msg.set_group_id(group_id);
	msg.set_group_is_finished(is_finished);
	msg.set_is_take_group_reward(is_take_group_reward);
}

void GrowUpQuestGroupTable::LoadFrom( const pb::GxDB_Grow_Up_Quest_Group_Info& msg )
{
	group_id = msg.group_id();
	is_finished = msg.group_is_finished();
	is_take_group_reward = msg.is_take_group_reward();
}