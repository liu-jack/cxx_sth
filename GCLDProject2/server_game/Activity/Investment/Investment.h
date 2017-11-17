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

class Investment { //Ͷ��


public:
    struct TaskPlayer { //�������
        TaskPlayer();
		uint64 playerId;
		uint32 countryId;
        uint64 cdTime; //Ͷ��һ�μ�CDһ���ӣ��ۼƵ�ʮ���ӣ��޷�����Ͷ�ʣ�����11������CD
        uint32 score;	//Ͷ�ʵĽ��
//		uint32 curStage;	//��ǰͶ�ʽ׶�
        bool   isTakeReward; //����ȡ
		bool   canTakeExReward;//�Ƿ�����ȡ���⽱��
    };

	typedef std::map<uint64, TaskPlayer> TaskPlayerList;		//��Ӧ���ID�������Ϣ
    std::map<uint32, TaskPlayerList>    m_playerLst;		//��Ӧÿһ�׶ε�����б�

	std::map<uint64, uint32>	m_playerInvestTotal;

    struct TaskCountry {
        uint32 countryId;
        uint32 amountMoney;
    };
    TaskCountry m_countryRank[MAX_PLAYER_COUNTRY_ID];

    bool   m_isBegined;
    //bool   m_haveExReward;
    uint8  m_curStage; //Ͷ��������е��ڼ��׶�
    uint64 m_curStageBeginTime;

public:
    Investment();
    static Investment& Instance(){ static Investment T; return T; }

    void SaveTo(pb::GS2C_Country_Task_Insvest& msg);
    void SaveTo(pb::GS2C_Country_Invest_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();
	void TakeAwardsBeforeStart();
//���ܺ���
	bool GiveMoney(uint32 stage, Player& player, pb::GS2C_Country_Invest_Donate_Ret& msg);
	bool TakeReward(uint32 stage, Player& player, pb::GS2C_Country_Invest_Take_Reward& msg);
	bool ClearInvestCD(Player& player, pb::GS2C_Country_Invest_Clear_CD_Ret& msg);

//��������
    uint32 GetCountryMoney(int countryId);
    uint32 AddCountryMoney(int countryId, int money);

    TaskPlayer& GetPlayerData(uint32 stage, Player& player);

	bool isPlayerInList(uint32 stage, Player& player);
private:
    void _EnterNextStage();
};
#define sInvestment Investment::Instance()
