/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "PassStageActivity/Table/ActivtyStageRewardTableMgr.h"
#include "DbTool.h"

ActivityStageReward::ActivityStageReward( const DB_ActivityStageRewardDiamond& activity_stage_reward_diamond )
:db_activity_stage_reward_diamond_(activity_stage_reward_diamond)
{

}

uint32 ActivityStageReward::Id() const
{
	return db_activity_stage_reward_diamond_.id;
}

uint32 ActivityStageReward::StageLevelId() const
{
	return db_activity_stage_reward_diamond_.stage_level_id;
}

uint32 ActivityStageReward::NameId() const
{
	return db_activity_stage_reward_diamond_.name_id;
}

uint32 ActivityStageReward::RewardItemId() const
{
	return db_activity_stage_reward_diamond_.reward_item_id;
}

uint32 ActivityStageReward::RewardItemNum() const
{
	return db_activity_stage_reward_diamond_.reward_item_num;
}


void ActivityStageRewardTableMgr::Init()
{
	max_stage_id = 1;
	FOREACH_DB_ITEM(activity_stage_reward_diamond,DB_ActivityStageRewardDiamond)
	{
		uint32 Id = activity_stage_reward_diamond->id;
		ActivityStageReward *activity_stageReward = new ActivityStageReward(*activity_stage_reward_diamond);
		activity_stage_reward_diamond_map_[Id] = activity_stageReward;
		if(activity_stageReward->StageLevelId() > max_stage_id)
			max_stage_id = activity_stageReward->StageLevelId();
	}
}

const ActivityStageRewardMap& ActivityStageRewardTableMgr::GetActivityStageRewardMap() const
{
	return activity_stage_reward_diamond_map_;
}

const ActivityStageReward* ActivityStageRewardTableMgr::GetActivityStageReward( const uint32 id ) const
{
	return activity_stage_reward_diamond_map_.pfind(id);
}

uint32 ActivityStageRewardTableMgr::GetMaxStageId()
{
	return max_stage_id;
}
