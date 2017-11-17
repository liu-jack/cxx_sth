#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "utility/rank.h"

class Player;
class Country;
class City;
namespace pb
{
    class GS2C_Country_Task_DefenceBorderland;
    class GS2C_Country_DefenceBorderland_Player;
	class GS2C_Country_DefenceBorderland_Take_Reward;
	class C2GS_Combat_Call_Request;
	class GS2C_Combat_Call_Request;
}

#define MAX_STAGE_NUM 3

class DefenceBorderland { //守卫边疆
public:
    struct TaskPlayer { //参与玩家
        TaskPlayer();
        int    rank;			//排名
        uint32 statisticKill;	//击杀数
        bool   isTakeReward;	//是否领取奖励
        int GetRankVal(){ return statisticKill; }
    };
	typedef std::map<uint64, TaskPlayer> TaskPlayerList;	//按玩家id来区别玩家列表
    std::map<uint32, TaskPlayerList>    m_playerLst;	//按照阶段区分玩家列表
	Rank<TaskPlayer>                m_RankLst_Country1;
	Rank<TaskPlayer>                m_RankLst_Country2;
	Rank<TaskPlayer>                m_RankLst_Country3;

    bool   m_isBegined;
    uint8  m_curStage; //任务进行到第几阶段
    uint64 m_nextStageBeginTime;	//下一阶段开始时间
    bool   m_isCountryFailed[MAX_PLAYER_COUNTRY_ID];	//国家防守失败

	uint32 m_substage;
	uint64 m_nextSubStageTime;
	bool m_canCountryTakeAward[MAX_PLAYER_COUNTRY_ID][MAX_STAGE_NUM];
public:
    DefenceBorderland();
    static DefenceBorderland& Instance(){ static DefenceBorderland T; return T; }

    void SaveTo(pb::GS2C_Country_Task_DefenceBorderland& msg);
    void SaveTo(pb::GS2C_Country_DefenceBorderland_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();

//功能函数
    void AddKillCnt(uint64 playerId, uint32 cnt);
	bool TakeReward(uint32 stage, Player& player, pb::GS2C_Country_DefenceBorderland_Take_Reward& msg);
	void TakeAwardsBeforeStart();
	
	void MonsterCallRequest(Player& player, pb::C2GS_Combat_Call_Request& callmsg, uint16 destCityId,pb::GS2C_Combat_Call_Request& msg);

    void CreatAttackMonster(int dstCityId, int npcId, int npcLv, int npcCnt, uint32 taskCountryid, uint32 defenceNum);		//创建蛮族入侵
    void _OnFailed(int countryId, City* pCity);
	void _ChechCityCallBackClear();
//辅助函数
	bool isPlayerInList(uint32 stagenum, Player& player);
    TaskPlayer& GetPlayerData(uint32 stage, Player& player);

	void DeliveringMonsters();	//投放蛮族

	void _EnterNextSubStage();	//进入小阶段  1,2,3,4,5,6,6,7,8,9

private:
    void _EnterNextStage(uint32 stagenum);		//进入下一大阶段

	void _TakePlayerAwardsOutOfTime(uint32 stagenum);	//超时没有领取任务奖励，则自动领取

    void GetMonsterInfo(Country& country, int& npcId, int& npcLv);

    void ClearPlayerData();

	City* GetMonsterTokenCity(uint32 countryid);	//获得蛮王令城池
};
#define sDefenceBorderland DefenceBorderland::Instance()
