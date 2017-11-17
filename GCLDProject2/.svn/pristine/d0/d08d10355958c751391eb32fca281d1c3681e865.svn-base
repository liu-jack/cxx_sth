

#include "GeneralReward.h"
#include "Activity.pb.h"
#include "Quest.pb.h"

void GeneralRewardTable::SaveTo(pb::GxDB_Hero_Reward_info& msg)
{
	msg.set_charid(charId);
	msg.set_has_first_reward(IsHasFirstReward > 0 ? true:false);
}

void GeneralRewardTable::LoadFrom(const pb::GxDB_Hero_Reward_info& msg)
{
	charId  = msg.charid();
	IsHasFirstReward = msg.has_first_reward();
}

void WannaBeStrongerTable::SaveTo(pb::WannaBeStrongerInfo& msg)
{
	msg.set_group_id(group_id);
	msg.set_is_achieve(is_achieve > 0 ? true:false);
	msg.set_is_take_reward(is_take_reward > 0 ? true : false);
// 	for(size_t i = 0;i < ARRAY_SIZE(base_info);++i)
// 	{
// 		pb::WannaBeStrongerBaseInfo* info = msg.add_base_info();
// 		info->set_quest_id(base_info[i].quest_id);
// 		info->set_cur_value(base_info[i].cur_value1_);
// 		info->set_need_value1(base_info[i].need_value1_);
// 		info->set_need_value2(base_info[i].need_value2_);
// 		info->set_quest_type(base_info[i].quest_type);
// 	}
}

void WannaBeStrongerTable::LoadFrom(const pb::WannaBeStrongerInfo& msg)
{
	group_id = msg.group_id();
	is_achieve = msg.is_achieve();
	is_take_reward = msg.is_take_reward();
// 	for(int i = 0;i < msg.base_info_size();++i)
// 	{
// 		const pb::WannaBeStrongerBaseInfo& base_info_ = msg.base_info(i);
// 		base_info[i].quest_id = base_info_.quest_id();
// 		base_info[i].cur_value1_ = base_info_.cur_value();
// 		base_info[i].quest_type = base_info_.quest_type();
// 		base_info[i].need_value1_ = base_info_.need_value1();
// 		base_info[i].need_value2_ = base_info_.need_value2();
// 	}

}
