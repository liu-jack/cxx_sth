#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "utility/rank.h"

class Player;
namespace pb
{
    class GS2C_Country_Task_Occupy;
    class GS2C_Country_Occupy_Player;
	class GS2C_Country_Occupy_Take_Reward;
}

#define MAX_OCCUPY_STAGE 3

class OccupyCity {
public:
    struct TaskPlayer { //参与玩家
        TaskPlayer();
		uint32 countryId;
        int    rank;				//排名		rank=0没上榜，最后一档5
        uint32 statisticKill;		//杀人数
		bool isTakeReward;			//是否领取了奖励
		bool canTakeExReward;//是否能领取额外奖励

        int GetRankVal(){ return statisticKill; }
    };
	typedef std::map<uint64, TaskPlayer> TaskPlayerList;	//按玩家id来区别玩家列表
    std::map<uint32, TaskPlayerList>    m_playerLst;		//按阶段来区分玩家列表

	Rank<TaskPlayer>                m_RankLst_1;		//国家1的排行榜
	Rank<TaskPlayer>                m_RankLst_2;
	Rank<TaskPlayer>                m_RankLst_3;

    bool   m_isBegined;
    uint8  m_curStage; //任务进行到第几阶段
    uint64 m_curStageBeginTime;	//当前阶段开始时间
    uint8  m_curStageBegin_CountryHaveCity[MAX_PLAYER_COUNTRY_ID];	//0,1,2 国家占领城池数	阶段结束之后增加国家经验，然后重置

public:
    OccupyCity();
    static OccupyCity& Instance(){ static OccupyCity T; return T; }

    void SaveTo(pb::GS2C_Country_Task_Occupy& msg);
    void SaveTo(pb::GS2C_Country_Occupy_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();

//功能函数
    void AddKillCnt(uint64 playerId, uint32 cnt);		//增加击杀数
    void _GivePlayerReward(Player& player, int rank, bool isCountryWin);
	void _TakeRewards(Player&player,uint32 stage,pb::GS2C_Country_Occupy_Take_Reward& msg);//发送获取奖励
	bool isPlayerInList(uint32 stage, Player& player);
//辅助函数

    TaskPlayer& GetPlayerData(uint32 stage, Player& player);		//获得玩家
	void TakeAwardsBeforeStart();					//下一个任务开始之前将奖励都领取了

	void NoticePlayerCityChange();
private:
    void _EnterNextStage();		//进入下一个阶段
};
#define sOccupyCity OccupyCity::Instance()
