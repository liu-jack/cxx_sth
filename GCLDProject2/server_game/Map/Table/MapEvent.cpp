#include "MapEvent.h"
#include "datastruct/struct_city.h"
#include "utility/Utility.h"
#include "OS.h"

MapEvent::MapEvent(const DB_MapEvent& db) : m_dbMapEvent(db)
{
    Utility::SplitStr(m_dbMapEvent.happen_trigger, m_happen_trigger, ',');
    Utility::SplitStr(m_dbMapEvent.stop_trigger, m_stop_trigger, ',');
    Utility::SplitStr(m_dbMapEvent.city_id, m_appear_city, ',');

    Utility::SplitStr2(m_dbMapEvent.reward_lst, m_RewardLst);
    Utility::SplitStr2(m_dbMapEvent.cost_lst, m_CostLst);
	if(m_dbMapEvent.type_id == CELEBRITY_TYPE_ID)
	{
		Utility::SplitStr(m_dbMapEvent.select_lst,m_celebritylst);
	}
	else
	{
		Utility::SplitStr2(m_dbMapEvent.select_lst, m_SelectLst);
	}
	for (IntPairVec::iterator it = m_CostLst.begin(); it != m_CostLst.end(); ++it)
	{
        it->second *= -1;
	}
}
uint32 MapEvent::Id() const { return m_dbMapEvent.id; }

uint32 MapEvent::IntervalSec() const { return m_dbMapEvent.interval_sec; }
uint32 MapEvent::DailyTimesLimit() const 
{
	return m_dbMapEvent.times_limit; 
}

int MapEvent::SelectRewardSize() const{
    return m_SelectLst.size();
}
int MapEvent::SelectRewardTyp(int idx) const{
    return m_SelectLst[idx].first;
}
int MapEvent::SelectRewardVal(int idx) const{
    return m_SelectLst[idx].second;
}

int MapEvent::SelectCelebrityRewardSize() const
{
	return m_celebritylst.size();
}

const IntPairVec& MapEvent::SelectCelebrityReward(int idx) const
{
	return m_celebritylst[idx];
}


//int MapEvent::LastTimeSecond() const
//{
//    return m_dbMapEvent.last_sec;
//}
uint8 MapEvent::RewardMaxCnt() const
{
	if(m_dbMapEvent.reward_max_cnt == 0)
		return 1;
	else
		return m_dbMapEvent.reward_max_cnt;
}

uint64 MapEvent::EventLastTime() const
{
	return m_dbMapEvent.event_last_time;
}

uint32 MapEvent::GetEventTypeID() const
{
	return m_dbMapEvent.type_id;
}

const string& MapEvent::EventDisplay() const
{
	return m_dbMapEvent.event_display;
}

const IntPair& MapEvent::GetCost(int idx) const
{
	return m_CostLst[idx];
}
