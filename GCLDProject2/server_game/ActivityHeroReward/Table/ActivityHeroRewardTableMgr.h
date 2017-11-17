/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __ACTIVITY_HERO_REWARD_H
#define __ACTIVITY_HERO_REWARD_H

#include "SimpleSingleton.h"
#include "game_data/datastruct/struct_activity_general_reward.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"
#include "Enum.pb.h"

class ActivityGeneralRewardProto
{
private:
	const DB_ActivityGeneralRewardProto& db_activity_general_reward_proto_;
	IntPairVec critical_weight_;
	IntPairVec cost_vec_;
public:
	ActivityGeneralRewardProto(const DB_ActivityGeneralRewardProto& db);
	uint32 Id() const;
	uint32 TroopsId()const;
	uint32 RewardBoxFirst() const;
	uint32 RewardTroopsExp() const;
	const IntPairVec& RewardCriticalWeight() const;
	const IntPairVec& Cost() const;
};

typedef PtrMap<uint32,ActivityGeneralRewardProto> ActivityGeneralRewardProtoMap;

class ActivityGeneralRewardTableMgr:public SimpleSingleton<ActivityGeneralRewardTableMgr>
{
private:
	ActivityGeneralRewardProtoMap activity_general_reward_proto_map_;
	IntPairVec reward_box_item_;
public:
	ActivityGeneralRewardTableMgr();
	void Init();
	IntPairVec& getRewardBoxItem();
	const ActivityGeneralRewardProto* getActivityGeneralRewardProto(const uint32 troops_id);
};


#define sActivityGeneralRewardMgr (ActivityGeneralRewardTableMgr::Instance())
#endif