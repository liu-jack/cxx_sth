#pragma once
#include "def/TypeDef.h"
#include "Base.h"

class Player;
namespace pb
{
    class GS2C_Altar_All_Info;
}

class PlayerAltarLog {
public:
    struct SubAltar
    {
        bool  isBuildingUp;		//是否已经升级		每天可直接获取免费增兵次数，不需要研究
        bool  haveReward;		//是否有奖励，在研究之后置为true,获取之后置为false
        uint8 studyTimes;		//已经研究的次数，每日更新
        uint64 studyEndTime;	//研究结束时刻
        uint64 takeFreeTime;	//领取免费奖励的时刻，每天一次
        uint8  rewardCnt;		//可领取研究奖励的次数

        SubAltar(){ ZeroMemoryThis; }
    };
public:
    int     m_level;
    int     m_SilverCoinForBuild;
    std::vector<SubAltar> m_subAltarLst;

public:
    PlayerAltarLog();
    void SaveTo(pb::GS2C_Altar_All_Info& msg);
    void LoadFrom(const pb::GS2C_Altar_All_Info& msg);

    void Study(Player& player, uint altarLv);
    void TakeReward(Player& player, uint altarLv);
    void SubAltarLevelUp(Player& player, uint altarLv);

    void BuildAltar_Token(Player& player);
    void BuildAltar_Silver(Player& player, bool isFast);
    void LevelUpToOpenSubAltar(Player& player);
	bool isAllAwardsCanTake(Player& player);
private:
    void IncAddSoldierTimesMax(Player& player);
    int  DecLeaveUpCostWood(int costNum);
};