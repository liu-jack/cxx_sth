#pragma once
#include "def/TypeDef.h"
#include "utility/rank.h"
#include "def/ConstDef.h"

static const int Select_MaxCnt = 3;
static const int Select_Fail_Score = 4;
static const int Select_Win_Score[MAX_PLAYER_COUNTRY_ID] = { 10, 15, 25 };


class Player;
namespace Combat
{
	class CombatGroup;
}
namespace pb
{
    class Struct_Country_Task_Base;
    class GS2C_Country_Task_Player;
}

class ActTournament { // ���������
public:
    struct TaskPlayer { //�������
        TaskPlayer();
        uint64 playerId;
        int    rank;
        int    score;
        bool   isTakeReward; //����ȡ

        uint64 selectLst[Select_MaxCnt];

        int GetRankVal(){ return score; }
    };
    std::map<uint64, TaskPlayer>    m_playerLst;
    Rank<TaskPlayer>                m_RankLst;

    struct TaskCountry {
        int countryId;
        int amountScore;

  /*      bool operator < (const TaskCountry& other)
        {
            return amountScore > other.amountScore;
        }*/
    };
    TaskCountry m_countryRank[MAX_PLAYER_COUNTRY_ID];

    bool   m_isBegined;

public:
    ActTournament();
    static ActTournament& Instance(){ static ActTournament T; return T; }

    void SaveTo(pb::Struct_Country_Task_Base& msg);
    void SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();

    void RandOpponent(Player& player); //ѡ�������֣�ֻ���ǵй��佫
	bool Fighting(Player& player, std::vector<uint32>& heroIds, uint32 selectIdx);
    void OnFightingEnd(Player* player, TaskPlayer* pData, uint32 selectIdx, Combat::CombatGroup* pGroup, bool isAttackWin);

    void AddScore(Player& player, int score);
    TaskPlayer* GetPlayerData(Player& player);

    uint32 AddCountryScore(int countryId, int num);

    bool TakeReward(Player& player);
	void TakeAwardsBeforeStart();
private:
    bool RandFormPool(TaskPlayer& playerData, uint64 exceptPlayerId);
    bool RandFormRankLst(TaskPlayer& playerData);
    int  _RandFormRankLst(int left, int right);
};
#define sActTournament ActTournament::Instance()
