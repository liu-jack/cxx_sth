#include "MapCountry.h"
#include "DbTool.h"
#include "datastruct/struct_country.h"
#include "utility/Utility.h"
#include "System.h"
#include <algorithm>


MapCountryUp::MapCountryUp(const DB_CountryUp& db) : m_db(db)
{
}
int MapCountryUp::Id() const { return m_db.id; }

MapCountryGuardUp::MapCountryGuardUp(const DB_CountryGuardUp& db) : m_db(db)
{
}
int MapCountryGuardUp::Id() const { return m_db.level; }
int MapCountryGuardUp::GetNeedExp() const { return m_db.need_exp; }
int MapCountryGuardUp::ExpeditionLV() const { return m_db.expedition_lv; }
int MapCountryGuardUp::LifeGuardLv() const { return m_db.lifeguard_lv; }

std::vector<int> MapCountryQuest::_StartTimeLst;
MapCountryQuest::MapCountryQuest(const DB_CountryQuest& db) 
    : m_db(db)
    , weight(db.weight)
{
    if (_StartTimeOneTask.empty())
    {
        Utility::SplitStr(m_db.start_time, _StartTimeOneTask, ',');
    }
	
	for(std::vector<int>::iterator it = _StartTimeOneTask.begin(); it != _StartTimeOneTask.end(); it++)
	{
		_StartTimeLst.push_back(*it);
	}

	Utility::SplitStr(m_db.player_quest_id1, _SelectTaskIdLst, ',');
	Utility::SplitStr(m_db.player_quest_id2, _SecondTaskIdLst, ',');
}
int MapCountryQuest::Id() const { return m_db.id; }
const char* MapCountryQuest::GetTaskComment() const { return m_db.comment.c_str(); }

void MapCountryQuest::GetTaskLst(std::vector<int>& lst)
{
    //std::random_shuffle(_SelectTaskIdLst.begin(), _SelectTaskIdLst.end());
	for (std::vector<int>::iterator it = _SelectTaskIdLst.begin(); it != _SelectTaskIdLst.end(); ++it)
	{
		lst.push_back(*it);
	}
//    lst.push_back(_SelectTaskIdLst[0]);
//    lst.push_back(_SelectTaskIdLst[1]);
//    lst.push_back(_SelectTaskIdLst[2]);
//    lst.push_back(m_db.player_quest_id2);
}
void MapCountryQuest::GetTaskSecond(std::vector<int>& lst)
{
	for (std::vector<int>::iterator it = _SecondTaskIdLst.begin(); it != _SecondTaskIdLst.end(); ++it)
	{
		lst.push_back(*it);
	}
}
int MapCountryQuest::GetNpcId(int countryId) const
{
    return m_db.npc_id[countryId-1];
}
int MapCountryQuest::GetNpcCnt() const
{
    return m_db.npc_num;
}
int MapCountryQuest::GetLastMinute() const
{
    return m_db.last_time;
}
bool MapCountryQuest::IsStartHour(int hour)
{
    for (std::vector<int>::iterator it = _StartTimeLst.begin(); it != _StartTimeLst.end(); ++it)
    {
        if (*it == hour) return true;
    }
    return false;
}

bool MapCountryQuest::IsStartHourOne(int hour)
{
	for (std::vector<int>::iterator it = _StartTimeOneTask.begin(); it != _StartTimeOneTask.end(); ++it)
	{
		if (*it == hour) return true;
	}
	return false;
}

CityTaskAttackDefenseQuest::CityTaskAttackDefenseQuest(const DB_Attack_Defense_Quest& db)
    : m_db(db)
{
    Utility::SplitStr(m_db.rank, m_RankRange, ',');
}
int CityTaskAttackDefenseQuest::Id() const { return m_db.id; }


void CountryTableMgr::Init()
{
	//ASSERT(0);
    FOREACH_DB_ITEM(ptr1, DB_CountryUp)
    {
        m_CountryLst[ptr1->id] = new MapCountryUp(*ptr1);
    }
    FOREACH_DB_ITEM(ptr2, DB_CountryGuardUp)
    {
		//ASSERT(0);
        m_GuardLst[ptr2->id] = new MapCountryGuardUp(*ptr2);
    }
    FOREACH_DB_ITEM(ptr3, DB_CountryQuest)
    {
        m_QuestLst.push_back(new MapCountryQuest(*ptr3));
    }
    FOREACH_DB_ITEM(ptr4, DB_Attack_Defense_Quest)
    {
        m_AttackDefenseQuestLst.push_back(new CityTaskAttackDefenseQuest(*ptr4));
    }
}
const MapCountryUp* CountryTableMgr::GetCountryUp(int lv) const
{
    return m_CountryLst.pfind(lv);
}
const MapCountryGuardUp* CountryTableMgr::GetGuardUp(int lvid) const
{
	//for(GuardUpMap::const_iterator ite = m_GuardLst.begin();
	//		ite != m_GuardLst.end(); ite++)
	//{
	//	if(ite->second->Id() == lv)
	//		return ite->second;
	//}
	GuardUpMap::const_iterator guardinfo = m_GuardLst.find(lvid);
	if(guardinfo != m_GuardLst.end())
	{
		return guardinfo->second;
	}

	return NULL;
    //return m_GuardLst.pfind(lv);
}
bool CountryTableMgr::GuardNpcUpLv(uint16& refLevel, uint32& levelid) const
{
	GuardUpMap::const_iterator guardinfo = m_GuardLst.find(levelid);
	if(guardinfo != m_GuardLst.end())
	{
		GuardUpMap::const_iterator guardinfonext = m_GuardLst.find(levelid+1);
		if(guardinfonext != m_GuardLst.end())
		{
			//ASSERT(0);
			levelid++;									//国家守卫系统等级
			refLevel = guardinfonext->second->Id();		//npc等级，对应npc表
			return true;
		}
	}

    return false;
}
bool CountryTableMgr::CountryUpLv(uint8& refLevel) const
{
    CountryUpMap::const_iterator it = m_CountryLst.find(refLevel);
    if (it != m_CountryLst.end())
    {
        if ((++it) != m_CountryLst.end())
        {
            refLevel = it->second->Id();
            return true;
        }
    }
    return false;
}
int CountryTableMgr::GetGuardIniLv() const
{
    return m_GuardLst.begin()->second->Id();
}
MapCountryQuest* CountryTableMgr::RandOneTask(uint32 checktime)
{
	QuestVec newQuestLst;
	for(QuestVec::iterator ite = m_QuestLst.begin(); ite != m_QuestLst.end(); ite++)
	{
		if((*ite)->IsStartHourOne((int)checktime))
		{
			newQuestLst.push_back(*ite);
		}
	}

    QuestVec::const_iterator it = System::RandWeight(newQuestLst);
    if (it != newQuestLst.end())
    {
        return *it;
    }
    //MMO_ASSERT(0);
    return NULL;
}

MapCountryQuest* CountryTableMgr::RandNextTask(int& nowHour)
{
	  int countlimit = 0;
	  MapCountryQuest* ret = NULL;
	  while (countlimit++ < 100)	//限制100次循环
	  {
		  nowHour++;
		  if(nowHour >= 24)
			  nowHour = 0;
		  
		  ret = RandOneTask(nowHour);
		  if(ret)
			  break;
	  }
	  return ret;
}

MapCountryQuest* CountryTableMgr::GetCountryTaskById(uint32 taskid)
{
	for(QuestVec::iterator ite = m_QuestLst.begin(); ite != m_QuestLst.end(); ite++)
	{
		if((*ite)->Id() == taskid)
		{
			return *ite;
		}
	}
	return NULL;
}

int CountryTableMgr::GetRankId(int rank)
{
    for (int i = 0; i < (int)m_AttackDefenseQuestLst.size(); ++i)
    {
        std::vector<int>& vec = m_AttackDefenseQuestLst[i]->m_RankRange;
        if (rank >= vec[0] && rank <= vec[1])
        {
            return m_AttackDefenseQuestLst[i]->Id();
        }
    }
    return -1;
}

int CountryTableMgr::GetNpcLvByLvID(uint32 levelID)
{
	return m_GuardLst[levelID]->Id();
}

int CountryTableMgr::GetExpeditionLvByLvID(uint32 levelID)
{
	return m_GuardLst[levelID]->ExpeditionLV();
}

int CountryTableMgr::GetLifeGuardLvByLvID(uint32 levelID)
{
	return m_GuardLst[levelID]->LifeGuardLv();
}