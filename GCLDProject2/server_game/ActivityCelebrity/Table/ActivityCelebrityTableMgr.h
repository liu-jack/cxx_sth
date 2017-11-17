
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#ifndef __ACTIVITY_CELEBRITY_TABLE_MGR__H
#define __ACTIVITY_CELEBRITY_TABLE_MGR__H


#include "SimpleSingleton.h"
#include "game_data/datastruct/struct_visit_famous.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"
#include "Enum.pb.h"
#include "def/ConstDef.h"
#include "Npc/Npc.h"
class ActivityVisitFamousReward
{
private:
	const DB_ActivityVisitFamousReward& db_activity_visit_famous_reward_;
public:
	ActivityVisitFamousReward(const DB_ActivityVisitFamousReward& db);
	uint32 Id() const;
	uint32 NeedRenown()const;
	string Reward() const;
};

struct infomation
{
	uint32 city_id;
	uint32 item_id;
	uint64 next_fresh_time;
};

class ActivityVisitFamousNpc
{
private:
	const DB_ActivityVisitFamousNpc& db_activity_visit_famous_npc_;
public:
	ActivityVisitFamousNpc(const DB_ActivityVisitFamousNpc& db);
	uint32 Id() const;
	string NpcList() const;
	uint32 CountryId() const;
	uint32 Weight() const;
	uint32 ArmyHZ() const;//多少时间出现一次
};

class ActivityVisitFamousEvent
{
private:
	const DB_ActivityVisitFamousEvent& db_activity_visit_famous_event_;
public:
	ActivityVisitFamousEvent(const DB_ActivityVisitFamousEvent& db);
	uint32 Id() const;
	uint32 EventId() const;
	uint32 Weight() const;
	string LeftImage() const;
	uint32 ImageNameId() const;
};

typedef PtrMap<uint32,ActivityVisitFamousReward> ActivityVisitFamousRewardMap;
typedef PtrMap<uint32,ActivityVisitFamousNpc> ActivityVisitFamousNpcMap;
typedef PtrMap<uint32,ActivityVisitFamousEvent> ActivityVisitFamousEventMap;

class ActivityCelebrityMgr :public SimpleSingleton<ActivityCelebrityMgr>
{
private:
	struct infomation  celebrity_info[NR_PLAYER_COUNTRY];
	ActivityVisitFamousRewardMap activity_visit_famous_reward_map_;
	ActivityVisitFamousNpcMap activity_visit_famous_npc_map_;
	ActivityVisitFamousEventMap activity_visit_famous_event_map_;
	IntPairVec npc_worm_,npc_human_,npc_god_;
	IntPairVec event_vec_;

private:
	int getTableIdForNpc(const uint32 id) const;
	int getTableIdForEvent() const;
public:
	ActivityCelebrityMgr();
	void Init();
	const ActivityVisitFamousReward* getActivityVisitFamousReward(const uint32 Table_Id);
	const ActivityVisitFamousNpc* getActivityVisitFamousNpc(const uint32 Table_Id);
	const ActivityVisitFamousEvent* getActivityVisitFamousEvent(const uint32 Table_Id);
	const ActivityVisitFamousRewardMap& getActivityVisitFamousRewardMap() const;

	uint32 GetNpcFreshTime(const uint32 id);//m second
	void GetNpc(const uint32 id,uint32 & npc_country_Id,IntPair& p_);
	void FillInfomation(uint32 country_id,uint32 city_id,uint64 next_fresh_time,Npc* npc);
	void setCityIdToZero(uint32 country_id);
	uint32 getCityId(uint32 country_id);
	infomation* getInfomation(const uint32 countryId);
	uint32 getRandEventId() const;
};

#define sActivityCelebrityMgr ActivityCelebrityMgr::Instance()


#endif