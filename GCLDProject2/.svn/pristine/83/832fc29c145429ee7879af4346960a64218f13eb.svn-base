/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "game_data/datastruct/struct_pass_stage_activity.h"
#include "Quest.pb.h"

class Player;

class ActivityStageReward
{
private:
	const struct DB_ActivityStageRewardDiamond& db_activity_stage_reward_diamond_;
public:
	ActivityStageReward(const DB_ActivityStageRewardDiamond& activity_stage_reward_diamond);
	uint32 Id() const;
	uint32 StageLevelId() const;
	uint32 NameId() const;
	uint32 RewardItemId() const;
	uint32 RewardItemNum() const;
};


typedef PtrMap<uint32,ActivityStageReward> ActivityStageRewardMap;


class ActivityStageRewardTableMgr : public SimpleSingleton<ActivityStageRewardTableMgr>
{
private:
	ActivityStageRewardMap activity_stage_reward_diamond_map_;
	uint32 max_stage_id;
public:
	static ActivityStageRewardTableMgr& Instance(){ static ActivityStageRewardTableMgr T; return T; }
	void Init();
	const ActivityStageRewardMap& GetActivityStageRewardMap() const;
	const ActivityStageReward* GetActivityStageReward(const uint32 id) const;
	uint32 GetMaxStageId();
};

#define sActivityStageRewardTableMgr ActivityStageRewardTableMgr::Instance()
