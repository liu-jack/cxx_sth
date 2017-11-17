#include "ActTableMgr.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "System.h"


TournamentRank::TournamentRank(const DB_Country_PK_Rank& db)
    : m_db(db)
{
    Utility::SplitStr(m_db.rank, m_RankRange, ',');
}
Table_Suppress::Table_Suppress(const DB_Country_Suppress& db)
    : m_db(db)
{
    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        Utility::SplitStr(m_db.target_cities[i], m_TargetCity[i], ',');
    }
}
Table_Defence::Table_Defence(const DB_Country_DefenceBorderland& db)
    : m_db(db)
{
	for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
	{
		Utility::SplitStr(m_db.target_cities[i], m_TargetCity[i], ',');
	}
}

void ActTableMgr::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_CountryInvest)
    {
        m_InvestMap[ptr1->invest_stage] = ptr1;
    }
    FOREACH_DB_ITEM(ptr2, DB_CountryInvestTitle)
    {
        m_InvestTitleMap[ptr2->id] = ptr2;
    }
    FOREACH_DB_ITEM(ptr3, DB_Open_Frontier)
    {
        m_FrontierMap[ptr3->stage] = ptr3;
    }
	FOREACH_DB_ITEM(ptr4, DB_Country_DefenceBorderland)
	{
		Table_Defence * proto = new Table_Defence(*ptr4);
		m_DefenceBorderlandMap[ptr4->stage] = proto;
	}
}
const DB_CountryInvest* ActTableMgr::GetInvest(int stage)
{
    return m_InvestMap.pfind(stage);
}

int ActTableMgr::GetMaxInvestId()
{
	int ret = 0;
	for(InvestMap::iterator it = m_InvestMap.begin(); it != m_InvestMap.end(); it++)
	{
		if(ret < it->first)
			ret = it->first;
	}
	return ret;
}

int ActTableMgr::GetInvestTitleId(uint32 score)
{
	int ret = 0;
    for (InvestTitleMap::iterator it = m_InvestTitleMap.begin(); it != m_InvestTitleMap.end(); ++it)
    {
        if (score >= it->second->need_invest_amounts)
        {
			ret = MAX((int)it->second->id, ret);
        }
    }
    return ret;
}
const DB_Open_Frontier* ActTableMgr::GetFrontier(int stage)
{
    return m_FrontierMap.pfind(stage);
}
const Table_Suppress* ActTableMgr::GetSuppress(int stage)
{
    return m_SuppressMap.pfind(stage);
}
const Table_Defence* ActTableMgr::GetDefence(int stage)
{
    return m_DefenceBorderlandMap.pfind(stage);
}
int ActTableMgr::GetTournamentRankId(int rank)
{
    for (size_t i = 0; i < m_TournamentRankVec.size(); ++i)
    {
        std::vector<int>& vec = m_TournamentRankVec[i]->m_RankRange;
        if (rank >= vec[0] && rank <= vec[1])
        {
            return i;
        }
    }
    return -1;
}