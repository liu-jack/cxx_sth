#pragma once

#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
struct DB_Trigger;
struct DB_FunctionOpen;
class Player;
class Trigger {
    typedef PtrMap<uint16, const DB_Trigger> TriggerMap;
    TriggerMap m_TriggerLst;
public:
    typedef bool(Trigger::*TriggerFunc)(Player*, uint32, uint32);
    enum {
        BottomLevel	 = 1,	//玩家达到等级
		VipLevel,			//VIP等级
		ActivityId,			//某个活动id
        DuringTheTime,      //在时间段内
		TaskId,				//某个任务id
		CityRewardLastTime,	//城池奖励持续时长
		BottomToTopLevel,	//玩家等级下限-上限
		FuBenStageId,		//副本关卡id
		TechnologyID,		//学院id
		CONFIGUREACTIVITY,  //可配置的活动

        MAX_ENUM
    };
    static Trigger& Instance(){ static Trigger T; return T; }

    void Init();
    bool Check(Player* player, const int triggerId);
    bool Check(Player* player, const std::vector<int>& triggerIds);

    //各类判断函数
    bool IsBottomLevel(Player* player, uint32 bottomLv, uint32 val2);					//玩家等级是否达到
	bool IsVipEnough(Player* player, uint32 vipLv, uint32 val2);						//玩家vip等级是否达到
	bool IsActivityFinished(Player* player, uint32 ActivityID, uint32 val2);			//是否完成了某一个活动
	bool IsDuringTime(Player* player, uint32 timeBegin, uint32 timeEnd);				//是否在时间段内
	bool IsTaskFinished(Player* player, uint32 TaskID, uint32 val2);					//是否完成了指定任务
    bool IsCityEventInTime(Player* player, uint32 eventId, uint32 longSecond);			//是否城池奖励持续时长内
	bool IsLevelLegal(Player* player, uint32 levelBottom, uint32 LevelTop);				//等级是否合法范围内
	bool IsStageLevelFinished(Player* player, uint32 stageId, uint32 val2);					//某一小章节是否完成
	bool IsTechnologyGet(Player* player, uint32 technologyId, uint32 val2);				//某一科技是否已经完成研究
	bool IsConfigureActivityIsActive(Player* player,uint32 activity_id,uint32 val2);	//后台配置活动
};

class FunctionOpen
{
	typedef PtrMap<uint32,const DB_FunctionOpen> FunctionOpenMap;
	FunctionOpenMap function_open_list_;
public:
	static FunctionOpen& Instance(){ static FunctionOpen T; return T; }
	void Init();
	uint32 GetTriggerId(const uint32 functionId) const;
};
#define sTrigger Trigger::Instance()
#define sFunctionOpen FunctionOpen::Instance()
