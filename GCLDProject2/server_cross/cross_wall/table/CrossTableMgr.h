#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "ptr_container/PtrMap.h"
#include "datastruct/struct_cross_kill_quest.h"

class Table_Cross_One
{
	const DB_CrossKillQuest& m_db;
public:
	std::vector<int> m_LevelLimit;
	std::vector<int> quest_last;	//第四阶段任务列表
	IntPairVec m_DefenceRewards;	//防守奖励
	IntPairVec m_StarRewards[3];	//1-3星奖励
public:
	explicit Table_Cross_One(const DB_CrossKillQuest& db);
public:
	uint32 GetQuest1() { return m_db.questid_1; }
	uint32 GetQuest2() { return m_db.questid_2; }
	uint32 GetQuest3() { return m_db.questid_3; }
	uint32 GetID()	{ return m_db.id; }
};

class CrossTableMgr {
private:
	typedef PtrMap<int, Table_Cross_One>         CrossMap;

private:
	CrossMap       m_CrossMap;
public:
	static CrossTableMgr& Instance(){ static CrossTableMgr T; return T; }

	void Init();

	Table_Cross_One* GetPQuestByLevel(int level);
};
#define sCrossTableMgr CrossTableMgr::Instance()