#include "PersonalQuestMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"
#include "System.h"

Table_PQuest::Table_PQuest(const DB_PersonalQuest& db)
:m_db(db)
{
	Utility::SplitStr(m_db.type_value, m_TypeValue, ',');
	Utility::SplitStr2(m_db.rewards, m_Rewards);
}

void PersonalQuestMgr::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_PersonalQuest)
	{
		m_PQuestMap[ptr1->id] = new Table_PQuest(*ptr1);
	}
}

const Table_PQuest* PersonalQuestMgr::GetPQuest(int id)
{
	return m_PQuestMap.pfind(id);
}