#pragma once
#include "def/TypeDef.h"
#include "Enum.pb.h"
class Player;
namespace pb
{
	class GS2C_ActMgr_Begin;
}

#define NULL_ACTIVE_HOUR 25

class ActMgr {
public:
    enum ActivityEnum
    {
		Act_None			= 0,
        Attack_Defence_City = 1,//攻守城池
        Tournament,             //比武
        Occupy_City,            //开疆拓土
        Suppress_Monster,       //镇压蛮族
        Investment,             //投资任务
        Defence_Borderland,     //镇守边疆
        _MAX_TYPE
    };
public:
    static ActMgr& Instance(){ static ActMgr T; return T; }

	void Init();
    void _Update();
    void OnEnterNexHour(const int hour);

	bool isCrossWarCover;	//是否跨服覆盖，如果覆盖则不开启国战任务

	ActivityEnum currentAct;	//当前正在进行的活动
	ActivityEnum lastAct;		//上一个活动
	ActivityEnum NextAct;		//下一个开启的活动
	int NextActStartHour;//下一个活动开启时间

	void GetActivityList(Player* player,pb::GS2C_ActMgr_Begin& msg);
	void TakeAwardsBeforeStart(ActivityEnum tpy);

	void AddPlayerInActivity(uint64 playerid);
	void AddEventLogFMT(pb::EVENT_ID eventID, int paramCount, ...);
	size_t GetPlayerSetSize() { return player_set.size(); }

	bool IsNowActivity();
private:
    void UpdateActivity(int hour);

private:
	std::set<uint64> player_set;
};
#define sActMgr ActMgr::Instance()
