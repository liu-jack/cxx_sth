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

class DefenceBorderland { //�����߽�
public:
    struct TaskPlayer { //�������
        TaskPlayer();
        int    rank;			//����
        uint32 statisticKill;	//��ɱ��
        bool   isTakeReward;	//�Ƿ���ȡ����
        int GetRankVal(){ return statisticKill; }
    };
	typedef std::map<uint64, TaskPlayer> TaskPlayerList;	//�����id����������б�
    std::map<uint32, TaskPlayerList>    m_playerLst;	//���ս׶���������б�
	Rank<TaskPlayer>                m_RankLst_Country1;
	Rank<TaskPlayer>                m_RankLst_Country2;
	Rank<TaskPlayer>                m_RankLst_Country3;

    bool   m_isBegined;
    uint8  m_curStage; //������е��ڼ��׶�
    uint64 m_nextStageBeginTime;	//��һ�׶ο�ʼʱ��
    bool   m_isCountryFailed[MAX_PLAYER_COUNTRY_ID];	//���ҷ���ʧ��

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

//���ܺ���
    void AddKillCnt(uint64 playerId, uint32 cnt);
	bool TakeReward(uint32 stage, Player& player, pb::GS2C_Country_DefenceBorderland_Take_Reward& msg);
	void TakeAwardsBeforeStart();
	
	void MonsterCallRequest(Player& player, pb::C2GS_Combat_Call_Request& callmsg, uint16 destCityId,pb::GS2C_Combat_Call_Request& msg);

    void CreatAttackMonster(int dstCityId, int npcId, int npcLv, int npcCnt, uint32 taskCountryid, uint32 defenceNum);		//������������
    void _OnFailed(int countryId, City* pCity);
	void _ChechCityCallBackClear();
//��������
	bool isPlayerInList(uint32 stagenum, Player& player);
    TaskPlayer& GetPlayerData(uint32 stage, Player& player);

	void DeliveringMonsters();	//Ͷ������

	void _EnterNextSubStage();	//����С�׶�  1,2,3,4,5,6,6,7,8,9

private:
    void _EnterNextStage(uint32 stagenum);		//������һ��׶�

	void _TakePlayerAwardsOutOfTime(uint32 stagenum);	//��ʱû����ȡ�����������Զ���ȡ

    void GetMonsterInfo(Country& country, int& npcId, int& npcLv);

    void ClearPlayerData();

	City* GetMonsterTokenCity(uint32 countryid);	//���������ǳ�
};
#define sDefenceBorderland DefenceBorderland::Instance()
