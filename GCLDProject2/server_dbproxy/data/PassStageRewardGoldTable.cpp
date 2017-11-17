

#include "PassStageRewardGoldTable.h"
#include "Quest.pb.h"
void PassStageRewardRecordTable::SaveTo( pb::pass_stage_reward_info& msg )
{
	msg.set_id(table_id);
	msg.set_is_take_reward(is_take_reward);
	msg.set_stage_is_passed(stage_is_passed);
	msg.set_stage_level(stage_level);
}

void PassStageRewardRecordTable::LoadFrom( const pb::pass_stage_reward_info& msg )
{
	table_id = msg.id();
	is_take_reward = msg.is_take_reward();
	stage_is_passed = msg.stage_is_passed();
	stage_level = msg.stage_level();
}