/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "game_data/datastruct/struct_activity_star_box.h"
#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"
class Player;
class ActivityStarBox
{
private:
	const struct DB_ActivityStarBox& activity_star_box_;
	IntPairVec reward_list_;
	std::vector<int> level_range_;
public:
	ActivityStarBox(const struct DB_ActivityStarBox& activity_star_box);
	uint32 Id() const;
	uint32 StarLevel() const;
	uint32 NeedScore() const;
	uint32 RewardTimes() const;
	uint32 QuickLvUpCost() const;
	uint32 NextLvNeedWaitTime() const;
	const std::vector<int>& GetLevelRange() const;
	const IntPairVec& GetRewardList() const;
};


typedef PtrMap<uint32,ActivityStarBox> ActivityStarBoxMap;

class ActivityStarBoxMgr: public SimpleSingleton<ActivityStarBoxMgr>
{
private:
	ActivityStarBoxMap activity_star_box_map_;
	uint32 max_star_level_;
public:
	static ActivityStarBoxMgr& Instance(){ static ActivityStarBoxMgr T; return T; }
	void Init();
	const uint32 GetId(Player* player) const;
	const ActivityStarBox* GetActivityStarBox(const uint32 id) const;
	const ActivityStarBoxMap& GetStarBoxMap() const;
	const uint32 GetMaxStarLevel() const;
	//const uint32 GetNextNeedScoreFromNextLevel(Player* player,const uint32 cur_level) const;
};

#define sActivityStarBoxMgr  (ActivityStarBoxMgr::Instance())