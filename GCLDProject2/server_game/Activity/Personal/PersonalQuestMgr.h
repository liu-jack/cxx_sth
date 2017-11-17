#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "ptr_container/PtrMap.h"
#include "datastruct/struct_personal_quest.h"

class Table_PQuest{
public:
	const DB_PersonalQuest& m_db;
	std::vector<int> m_TypeValue;
	IntPairVec m_Rewards;
public:
	Table_PQuest(const DB_PersonalQuest& db);
};

class PersonalQuestMgr{
private:
	typedef PtrMap<int, const Table_PQuest>		PersonalQuestMap;

private:
	PersonalQuestMap	m_PQuestMap;

public:
	static PersonalQuestMgr& Instance() { static PersonalQuestMgr T; return T; }

	void Init();

	const Table_PQuest* GetPQuest(int id);
};
#define sPersonalQuestMgr PersonalQuestMgr::Instance()