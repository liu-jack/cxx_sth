/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "ActivityHeroRewardTableMgr.h"
#include "DbTool.h"
#include "System.h"
#include "utility/Utility.h"


ActivityGeneralRewardProto::ActivityGeneralRewardProto(const DB_ActivityGeneralRewardProto& db)
:db_activity_general_reward_proto_(db)
{
	Utility::SplitStr2(db_activity_general_reward_proto_.cost,cost_vec_);
	std::vector<int> vec_int;
	Utility::SplitStr(db_activity_general_reward_proto_.reward_critical_weight,vec_int,',');
	critical_weight_.push_back(std::make_pair(1,vec_int[0]));
	critical_weight_.push_back(std::make_pair(2,vec_int[1]));
	critical_weight_.push_back(std::make_pair(5,vec_int[2]));
	critical_weight_.push_back(std::make_pair(10,vec_int[3]));
}

uint32 ActivityGeneralRewardProto::Id() const
{
	return db_activity_general_reward_proto_.id;
}

uint32 ActivityGeneralRewardProto::TroopsId() const
{
	return db_activity_general_reward_proto_.troops_id;
}

uint32 ActivityGeneralRewardProto::RewardBoxFirst() const
{
	return db_activity_general_reward_proto_.reward_box_first;
}

uint32 ActivityGeneralRewardProto::RewardTroopsExp() const
{
	return db_activity_general_reward_proto_.reward_troops_exp;
}

const IntPairVec& ActivityGeneralRewardProto::RewardCriticalWeight() const
{
	return critical_weight_;
}

const IntPairVec& ActivityGeneralRewardProto::Cost() const
{
	return cost_vec_;
}

ActivityGeneralRewardTableMgr::ActivityGeneralRewardTableMgr()
{
}

void ActivityGeneralRewardTableMgr::Init()
{
	FOREACH_DB_ITEM(ptr1,DB_ActivityGeneralRewardBox)
	{
		reward_box_item_.push_back(std::make_pair(ptr1->reward_item_id,ptr1->reward_item_num));
	}
	FOREACH_DB_ITEM(ptr2,DB_ActivityGeneralRewardProto)
	{
		ActivityGeneralRewardProto * reward = new ActivityGeneralRewardProto(*ptr2);
		activity_general_reward_proto_map_[ptr2->troops_id] = reward;
	}
}

IntPairVec& ActivityGeneralRewardTableMgr::getRewardBoxItem()
{
	return reward_box_item_;
}

const ActivityGeneralRewardProto* ActivityGeneralRewardTableMgr::getActivityGeneralRewardProto(const uint32 troops_id)
{
	return activity_general_reward_proto_map_.pfind(troops_id);
}
