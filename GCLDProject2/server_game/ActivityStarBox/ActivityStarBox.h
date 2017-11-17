/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#pragma once
#include "def/TypeDef.h"
#include "Table/ActivityStarBoxTableMgr.h"
#include "Base.h"
#include "boost/noncopyable.hpp"
class Player;
namespace pb
{
	class GxDB_ActivityStarBox;
	class GxDB_ActivityStarBoxInfo;
	class GS2C_ActivityStarBoxInfo;
}

struct ActivityStarBoxStruct
{
	uint32 star_level;
	uint32 is_take_reward;
	int32 npc_left_reward_times;//剩余领取次数
	uint64 last_take_rewrd_time;
	uint32 npc_is_opened;
	ActivityStarBoxStruct() {ZeroMemoryThis;}
};
typedef std::map<uint32,ActivityStarBoxStruct> ActivityStarBoxStructMap;
class ActivityStarBoxLog :public boost::noncopyable
{
private:
	ActivityStarBoxStructMap activity_star_box_struct_map_;
public:
	ActivityStarBoxLog();
	~ActivityStarBoxLog();
	void Init();
	void SaveTo(Player* player,pb::GS2C_ActivityStarBoxInfo& msg);
	void SaveTo(pb::GxDB_ActivityStarBox& msg);
	void LoadFrom(const pb::GxDB_ActivityStarBox& msg);
	void UpdateTakeRewardToDb(const uint32 id);
	//void UpdateLevUpToDb(const uint32 id,const uint32 level);
	void UpdateNpcTakeRewardTimes(const uint32 id);
	void UpdateLastTakeRewardTime(const uint32 id);
	void UpdateNpcIsOpened(const uint32 id);
	uint32 GetLeftTimesOfNpcTakeReward(const uint32 id);
	ActivityStarBoxStructMap& GetActivityStarBoxStructMap();
	bool CanUpdateBoxLevel(const uint32 id);
	bool UpdateBoxLevel(Player* player,const uint32 id);
	bool HasTakeReward(const uint32 id);
	bool IsLvHasTakeReard(const uint32 lv);
	void Clear();
};