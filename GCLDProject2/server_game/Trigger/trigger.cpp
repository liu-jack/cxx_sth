#include "trigger.h"
#include "DbTool.h"
#include "datastruct/struct_trigger.h"
#include "datastruct/struct_building.h"
#include "OS.h"
#include "../object/Player.h"
#include "../Map/PlayerMapLogic.h"
#include "../Technology/PlayerTechnology.h"
#include "quest/QuestLogger.h"
#include "stage/StageLogger.h"
#include "Activity/ActMgr.h"
#include "Activity/Defence/DefenceBorderland.h"
#include "Activity/Tournament/Tournament.h"
#include "Activity/OccupyCity/OccupyCity.h"
#include "Activity/Investment/Investment.h"
#include "Activity/Suppress/SuppressMonster.h"
#include "fuben/FubenLog.h"

static const Trigger::TriggerFunc g_handler[] = {
    &Trigger::IsBottomLevel,
    &Trigger::IsVipEnough,
	&Trigger::IsActivityFinished,
	&Trigger::IsDuringTime,
	&Trigger::IsTaskFinished,
	&Trigger::IsCityEventInTime,
	&Trigger::IsLevelLegal,
	&Trigger::IsStageLevelFinished,
	&Trigger::IsTechnologyGet,
	&Trigger::IsConfigureActivityIsActive,
};
//STATIC_ASSERT_ARRAY_LENGTH(g_handler, Trigger::MAX_ENUM);

void Trigger::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_Trigger)
    {
        m_TriggerLst[ptr1->id] = ptr1;
    }
}
bool Trigger::Check(Player* player, const int triggerId)
{
	//return true;

    if (const DB_Trigger* pData = m_TriggerLst.pfind(triggerId))
    {
        return (this->*g_handler[pData->type-1])(player, pData->value1, pData->value2);
    }
    return true;
}
bool Trigger::Check(Player* player, const std::vector<int>& triggerIds)
{
	//return true;

    std::vector<int>::const_iterator it = triggerIds.begin();
    for (; it != triggerIds.end(); ++it)
    {
        if (!Check(player, *it)) return false;
    }
    return true;
}


//----------------------------------------------------------
//各类判断函数

//玩家等级是否达到
bool Trigger::IsBottomLevel(Player* player, uint32 val1, uint32 val2)
{
	if(!player) return false;
    return player->GetLevel() >= (int32)val1;
}

//玩家vip等级是否达到
bool Trigger::IsVipEnough(Player* player, uint32 vipLv, uint32 val2)
{
	if(!player) return false;
	return player->GetVipLevel() >= (int32)vipLv;
}

//是否完成了某一个活动   unfinish liup  后续系统
bool Trigger::IsActivityFinished(Player* player, uint32 ActivityID, uint32 val2)
{
	switch(ActivityID)
	{
	case ActMgr::Attack_Defence_City:// 攻守城池
		return !sDefenceBorderland.m_isBegined;

	case ActMgr::Tournament:
		return !sActTournament.m_isBegined;
	case ActMgr::Occupy_City:
		return !sOccupyCity.m_isBegined;
	case ActMgr::Suppress_Monster:
		return !sSuppressMonster.m_isBegined;
	case ActMgr::Investment:
		return !sInvestment.m_isBegined;
	case ActMgr::Defence_Borderland:
		return !sDefenceBorderland.m_isBegined;
	default:
		break;

	}
	return false;
}

//是否在时间段内
bool Trigger::IsDuringTime(Player* player, uint32 val1, uint32 val2)
{
    int64 timeNow = sOS.TimeSeconds();

    return timeNow >= sOS.ParseTime(val1) && timeNow < sOS.ParseTime(val2);
}

//是否完成了指定任务
bool Trigger::IsTaskFinished(Player* player, uint32 TaskID, uint32 val2)
{
	if(!player) return false;
	return player->m_questLogger->GetQuestFinished(TaskID);
}

//是否城池奖励持续时长内
bool Trigger::IsCityEventInTime(Player* player, uint32 eventId, uint32 longSecond)
{
	if(!player) return false;
	uint64 happenTime = player->m_worldMapLogic->GetCityEventHappenTime(eventId);

	return sOS.TimeSeconds() - happenTime < longSecond;
}

//等级是否在限定范围内
bool Trigger::IsLevelLegal(Player* player, uint32 levelBottom, uint32 LevelTop)
{
	if(!player) return false;
	return (player->GetLevel() >= (int32)levelBottom) && (player->GetLevel() <= (int32)LevelTop);
}

//某一关卡是否完成  unfinished liup fuben系统
bool Trigger::IsStageLevelFinished(Player* player, uint32 stageId, uint32 val2)
{
#ifdef _MMO_SERVER_
	if(!player) return false;
	return player->m_fubenLog->IsPassStage(stageId) == 1;
#endif
	return false;
}

//某一科技是否已经完成研究
bool Trigger::IsTechnologyGet(Player* player, uint32 technologyId, uint32 val2)
{
	if(!player) return false;
	return player->m_Technology->Have(technologyId);
}

bool Trigger::IsConfigureActivityIsActive(Player* player,uint32 activity_id,uint32 val2)
{
	if(!player) return false;
	return player->IsActivityStillActive(activity_id);
}

void FunctionOpen::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_FunctionOpen)
	{
		function_open_list_[ptr1->function_id] = ptr1;
	}
}

uint32 FunctionOpen::GetTriggerId(const uint32 functionId) const
{
	const DB_FunctionOpen* f_open = function_open_list_.pfind(functionId);
	if(f_open)
	{
		return f_open->trigger_id;
	}
	return 0;
}
