#pragma once
#include "def/TypeDef.h"
#include "Base.h"

class Player;
class Character;
namespace Combat
{
    class CombatObj;
}
namespace pb
{
    class GS2C_Dungeon_Base;
    class GS2C_Dungeon_All_Info;
}

class PlayerDungeonLog {
public:
	struct CatchHero {
		uint64 playerId; //可能是Npc
		uint32 heroTid;
        bool   isCaned;

        //逃跑相关临时数据
		uint64 escapeEndTime;
		bool   isOffical;	//有官职
		bool   isShadow;
		uint32 level;
	};
public:
	std::vector<CatchHero> m_HeroLst;		//捕获到的武将列表
    int     m_DungeonLv;
    int     m_SilverCoinForBuild; //建造时累计的银币
    int     m_CaneLv;
    int     m_CaneExp;
    bool    m_freeTryCane; //免费试用笞杖
    int     m_TempCaneLv;
    uint64  m_TempCaneLvEndTime;
    uint64  m_CatchHeroTime;
    int     m_TodayCatchHeroCnt;  //今天抓了几个
    int     m_CatchHeroCntForExp; //连续抓了几个人

public:
    //统计数据，无需入库，仅界面显示用
    int     m_TodayCaneCnt;
    int     m_TodayCaneExp;
    uint64  m_CaneTime;

public:
    PlayerDungeonLog();
    void SaveTo(pb::GS2C_Dungeon_Base& msg);
    void SaveTo(pb::GS2C_Dungeon_All_Info& msg);
    void LoadFrom(const pb::GS2C_Dungeon_All_Info& msg);

    void Build_Token(Player& player);
    void Build_Silver(Player& player, bool isFast);
    void LevelUp(Player& player);

    int  GetCaneLv();
    bool Cane(Player& player,uint32 &player_exp); //笞杖
    void TryCane(Player& player, int dstLv);
    void CaneFastLevelUp(Player& player, int dstLv);
    void CatchObj(Player& player, Combat::CombatObj& obj);
    void Escape(Player* dungeonOwner, Character* escapeHero, uint64 endtime);
	void TryPrisonerEscape(Player* dungeonOwner, Character* obj);
    int  GetEscapeSecond();
    CatchHero* FindCatchHero(Combat::CombatObj& obj);

private:
    int CalcCaneExp(bool isOffical);
    int IncCaneBaseExp();
    int IncCaneOfficialExp();
    void AddCaneExpOnCatchHero(Player& player);
	void AddCaneExpOnCane(Player& player);
    bool  CanCatchObj(Combat::CombatObj& obj);
    float CalcCatchRate();
};