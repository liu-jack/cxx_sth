#include "CrossTableMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"
#include "System.h"

Table_Cross_One::Table_Cross_One(const DB_CrossKillQuest& db)
:m_db(db)
{

	Utility::SplitStr(m_db.level_area, m_LevelLimit, ',');
	Utility::SplitStr(m_db.questid_4_list, quest_last, ',');
	Utility::SplitStr2(m_db.defense_reward, m_DefenceRewards);

	Utility::SplitStr2(m_db.one_star_reward, m_StarRewards[0]);
	Utility::SplitStr2(m_db.two_star_reward, m_StarRewards[1]);
	Utility::SplitStr2(m_db.three_star_reward, m_StarRewards[2]);
}

void CrossTableMgr::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_CrossKillQuest)
	{
		m_CrossMap[ptr1->id] = new Table_Cross_One(*ptr1);
	}
}

Table_Cross_One* CrossTableMgr::GetPQuestByLevel(int level)
{
	for(CrossMap::iterator it = m_CrossMap.begin(); it != m_CrossMap.end();	it++)
	{
		Table_Cross_One* tableOne = it->second;
		if(level >= tableOne->m_LevelLimit[0] && level <= tableOne->m_LevelLimit[1])
			return tableOne;
	}
	return NULL;
}