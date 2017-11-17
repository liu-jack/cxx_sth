#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "ptr_container/PtrMap.h"
#include "datastruct/struct_country_invest.h"
#include "datastruct/struct_country_open_frontier.h"
#include "datastruct/struct_country_pk_rank.h"
#include "datastruct/struct_country.h"

class TournamentRank { // 比武积分赛 排行榜
    const DB_Country_PK_Rank& m_db;
public:
    std::vector<int>  m_RankRange;
public:
    TournamentRank(const DB_Country_PK_Rank& db);
};

class Table_Suppress { // 镇压蛮族
public:
    const DB_Country_Suppress& m_db;
    std::vector<int>  m_TargetCity[MAX_PLAYER_COUNTRY_ID];
public:
    Table_Suppress(const DB_Country_Suppress& db);
};

class Table_Defence { // 守卫边疆
public:
    const DB_Country_DefenceBorderland& m_db;
    std::vector<int>  m_TargetCity[MAX_PLAYER_COUNTRY_ID];
public:
    Table_Defence(const DB_Country_DefenceBorderland& db);
};


class ActTableMgr {
private:
    typedef PtrMap<int, const DB_CountryInvest>         InvestMap;
    typedef PtrMap<int, const DB_CountryInvestTitle>    InvestTitleMap;
    typedef PtrMap<int, const DB_Open_Frontier>         FrontierMap;
    typedef std::vector<TournamentRank*>                TournamentRankVec;
    typedef PtrMap<int, const Table_Suppress>           SuppressMap;
    typedef PtrMap<int, const Table_Defence>            DefenceBorderlandMap;

private:
    InvestMap       m_InvestMap;
    InvestTitleMap  m_InvestTitleMap;
    FrontierMap     m_FrontierMap;
    TournamentRankVec m_TournamentRankVec;
    SuppressMap             m_SuppressMap;
    DefenceBorderlandMap    m_DefenceBorderlandMap;
public:
    static ActTableMgr& Instance(){ static ActTableMgr T; return T; }

    void Init();

    const DB_CountryInvest* GetInvest(int stage);
    int GetInvestTitleId(uint32 score);
	int GetMaxInvestId();
    const DB_Open_Frontier* GetFrontier(int stage);
    const Table_Suppress* GetSuppress(int stage);
    const Table_Defence* GetDefence(int stage);

    int GetTournamentRankId(int rank);
};
#define sActTableMgr ActTableMgr::Instance()