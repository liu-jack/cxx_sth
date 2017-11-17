/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "game_data/datastruct/struct_activity_xilian.h"
#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"
class Player;
class ActivityXilian
{
private:
	const struct DB_ActivityXilian& activity_xilian_;
	IntPairVec reward_list_;
public:
	ActivityXilian(const struct DB_ActivityXilian& activity_xilian);
	uint32 Id() const;
	const uint32 NeedTimes() const;
	const IntPairVec& GetRewardList() const;
};


typedef PtrMap<uint32,ActivityXilian> ActivityXilianMap;

class ActivityXilianMgr: public SimpleSingleton<ActivityXilianMgr>
{
private:
	ActivityXilianMap activity_xilian_map_;
public:
	static ActivityXilianMgr& Instance(){ static ActivityXilianMgr T; return T; }
	void Init();
	ActivityXilian* GetActivityXilian(const uint32 id);
	const ActivityXilianMap& GetActivityXilianMap() const;
};

#define sActivityXilianMgr  (ActivityXilianMgr::Instance())