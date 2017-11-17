#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "utility/rank.h"
#include "BaseDefine.pb.h"
#include "BaseDefineMgr.h"

//static const int CD_Second_OnceAdd = GET_BASE_DEF_INT( pb::BD_INVEST_ADD_COOLDOWN_TIME);
//static const int CD_Second_MAX = GET_BASE_DEF_INT( pb::BD_INVEST_MAX_COOLDOWN_TIME);


class Player;
namespace pb
{
    class GS2C_Country_Task_Insvest;
    class GS2C_Country_Invest_Player;
	class GS2C_Country_Invest_Donate_Ret;
	class GS2C_Country_Invest_Take_Reward;
	class GS2C_Country_Invest_Clear_CD_Ret;
}

class Investment { //投资


public:
    struct TaskPlayer { //参与玩家
        TaskPlayer();
		uint64 playerId;
		uint32 countryId;
        uint64 cdTime; //投资一次加CD一分钟；累计到十分钟，无法继续投资；可用11金币清空CD
        uint32 score;	//投资的金额
//		uint32 curStage;	//当前投资阶段
        bool   isTakeReward; //已领取
		bool   canTakeExReward;//是否能领取额外奖励
    };

	typedef std::map<uint64, TaskPlayer> TaskPlayerList;		//对应玩家ID的玩家信息
    std::map<uint32, TaskPlayerList>    m_playerLst;		//对应每一阶段的玩家列表

	std::map<uint64, uint32>	m_playerInvestTotal;

    struct TaskCountry {
        uint32 countryId;
        uint32 amountMoney;
    };
    TaskCountry m_countryRank[MAX_PLAYER_COUNTRY_ID];

    bool   m_isBegined;
    //bool   m_haveExReward;
    uint8  m_curStage; //投资任务进行到第几阶段
    uint64 m_curStageBeginTime;

public:
    Investment();
    static Investment& Instance(){ static Investment T; return T; }

    void SaveTo(pb::GS2C_Country_Task_Insvest& msg);
    void SaveTo(pb::GS2C_Country_Invest_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();
	void TakeAwardsBeforeStart();
//功能函数
	bool GiveMoney(uint32 stage, Player& player, pb::GS2C_Country_Invest_Donate_Ret& msg);
	bool TakeReward(uint32 stage, Player& player, pb::GS2C_Country_Invest_Take_Reward& msg);
	bool ClearInvestCD(Player& player, pb::GS2C_Country_Invest_Clear_CD_Ret& msg);

//辅助函数
    uint32 GetCountryMoney(int countryId);
    uint32 AddCountryMoney(int countryId, int money);

    TaskPlayer& GetPlayerData(uint32 stage, Player& player);

	bool isPlayerInList(uint32 stage, Player& player);
private:
    void _EnterNextStage();
};
#define sInvestment Investment::Instance()
