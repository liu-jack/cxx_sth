#pragma once
#include "def/TypeDef.h"

class Player;
namespace pb
{
    class GS2C_Smithy_All_Info;
}

class PlayerIronsmithLog {
public:
    struct SubSmelter
    {
        uint8  star;
        uint8  smeltTimes;

        SubSmelter() { 
            star = 1;
            smeltTimes = 0;
        }
    };
public:
    int     m_level;
    int     m_SilverCoinForBuild;
    std::vector<SubSmelter> m_subSmithyLst;

public:
    PlayerIronsmithLog();
    void SaveTo(pb::GS2C_Smithy_All_Info& msg);
    void LoadFrom(const pb::GS2C_Smithy_All_Info& msg);

    bool DoSmelter(Player& player, uint idx, int itemId = 0); // idx 1ÆðÊ¼
    void SubSmelterLevelUp(Player& player, uint idx);

    void BuildByToken(Player& player);
    void BuildBySilver(Player& player, bool isFast);
    void LevelUpToOpenSubSmelter(Player& player);
	void DoZeroResetSmeltTimes(Player& player);
	void UpdateSmeltTimesInfoToDb(Player& player);

	bool IsSubSmelterOpen(int idx);
};