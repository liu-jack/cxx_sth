#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "utility/rank.h"

class Player;
namespace pb
{
    class Struct_Country_Task_Base;
    class GS2C_Country_Task_Player;
}

class SuppressMonster { //镇压蛮族
public:
    struct TaskPlayer { //参与玩家
        TaskPlayer();
        int    rank;
        uint32 statisticKill;
        bool   isTakeReward;

        int GetRankVal(){ return statisticKill; }
    };
    std::map<uint64, TaskPlayer>    m_playerLst;
    Rank<TaskPlayer>                m_RankLst;

    bool   m_isBegined;
    bool   m_isWaiting;
    uint8  m_curStage; //任务进行到第几阶段
    uint64 m_curStageBeginTime;

    bool   m_isContinue[MAX_PLAYER_COUNTRY_ID];

public:
    SuppressMonster();
    static SuppressMonster& Instance(){ static SuppressMonster T; return T; }

    void SaveTo(pb::Struct_Country_Task_Base& msg);
    void SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();

//功能函数
    void AddKillCnt(uint64 playerId, uint32 cnt);
    bool TakeReward(Player& player, int rank);
	void TakeAwardsBeforeStart();
//辅助函数
    TaskPlayer* GetPlayerData(Player& player);
private:
    void GetMonsterInfo(int countryId, int& npcId, int& npcLv);
    void ClearPlayerData();

private:
    void _EnterNextStage();

    void _AddMonsterInCity(int cityId, int npcCnt, int npcId, int npcLv);
    bool _IsCityMonsterClear(int cityId);
};
#define sSuppressMonster SuppressMonster::Instance()
