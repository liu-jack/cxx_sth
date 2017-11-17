#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
struct DB_CountryUp;
struct DB_CountryGuardUp;
struct DB_CountryQuest;
struct DB_Attack_Defense_Quest;

class MapCountryUp {
public:
    const DB_CountryUp& m_db;

    MapCountryUp(const DB_CountryUp& db);
    int Id() const;
};

class MapCountryGuardUp {
    const DB_CountryGuardUp& m_db;
public:
    MapCountryGuardUp(const DB_CountryGuardUp& db);
    int Id() const;
    int GetNeedExp() const;
	int ExpeditionLV() const;
	int LifeGuardLv() const;
};

class MapCountryQuest {
    const DB_CountryQuest& m_db;
    std::vector<int>  _SelectTaskIdLst; //个人任务，六选三
	std::vector<int>  _SecondTaskIdLst; //第二阶段个人任务，六选三
    static std::vector<int>  _StartTimeLst;
	std::vector<int> _StartTimeOneTask;	//开启时间列表
public:
    const int         weight;
public:
    MapCountryQuest(const DB_CountryQuest& countryDB);
    int Id() const;

    void GetTaskLst(std::vector<int>& lst); //六选三
    int  GetNpcId(int countryId) const;
    int  GetNpcCnt() const;
    int  GetLastMinute() const;
	void GetTaskSecond(std::vector<int>& lst);
	bool IsStartHourOne(int hour);
    static bool IsStartHour(int hour);
	const char* GetTaskComment() const;
};

class CityTaskAttackDefenseQuest { // 攻守任务 排行榜
    const DB_Attack_Defense_Quest& m_db;
public:
    std::vector<int>  m_RankRange;
public:
    CityTaskAttackDefenseQuest(const DB_Attack_Defense_Quest& db);
    int Id() const;
};


class CountryTableMgr {
    typedef PtrMap<int, MapCountryUp>       CountryUpMap;
    typedef PtrMap<int, MapCountryGuardUp>  GuardUpMap;
    typedef std::vector<MapCountryQuest*>   QuestVec;
    typedef std::vector<CityTaskAttackDefenseQuest*>   AttackDefenseQuestVec;

private:
    CountryUpMap    m_CountryLst;
    GuardUpMap      m_GuardLst;
    QuestVec        m_QuestLst;
    AttackDefenseQuestVec m_AttackDefenseQuestLst;

public:
    static CountryTableMgr& Instance(){ static CountryTableMgr T; return T; }

    void Init();
    const MapCountryUp* GetCountryUp(int lv) const;
    const MapCountryGuardUp* GetGuardUp(int lv) const;
    bool GuardNpcUpLv(uint16& refLevel, uint32& levelid) const;
    bool CountryUpLv(uint8& refLevel) const;
    int GetGuardIniLv() const;
    int GetRankId(int rank);
	int GetNpcLvByLvID(uint32 levelID);
	int GetExpeditionLvByLvID(uint32 levelID);
	int GetLifeGuardLvByLvID(uint32 levelID);

    MapCountryQuest* RandOneTask(uint32 checktime);
	MapCountryQuest* GetCountryTaskById(uint32 taskid);
	MapCountryQuest* RandNextTask(int& nowHour);
};
#define sCountryTableMgr CountryTableMgr::Instance()
