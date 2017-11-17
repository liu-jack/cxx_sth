#pragma once

#include "def/TypeDef.h"
struct DB_MapEvent;
const static uint32 CELEBRITY_TYPE_ID = 35;
class MapEvent {
    const DB_MapEvent& m_dbMapEvent;
public:
    IntPairVec  m_RewardLst;    //奖励
    IntPairVec  m_SelectLst;    //多选一奖励
    IntPairVec  m_CostLst;    //消耗
    std::vector<int>  m_happen_trigger;
    std::vector<int>  m_stop_trigger;
    std::vector<int>  m_appear_city;
	VecIntPairvec m_celebritylst;
   explicit MapEvent(const DB_MapEvent& db);
    uint32 Id() const;

    uint32 IntervalSec() const;
    uint32 DailyTimesLimit() const;

    int SelectRewardSize() const;
    int SelectRewardTyp(int idx) const;
    int SelectRewardVal(int idx) const;

	int SelectCelebrityRewardSize() const;
	const IntPairVec &SelectCelebrityReward(int idx) const;

   // int LastTimeSecond() const;
    uint8 RewardMaxCnt() const;
	uint64 EventLastTime() const;
	uint32 GetEventTypeID() const;
	const string& EventDisplay() const;
	const IntPair& GetCost(int idx) const;
};