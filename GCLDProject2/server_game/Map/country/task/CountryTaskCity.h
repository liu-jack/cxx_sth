//��ս��������
#pragma once
#include "def/TypeDef.h"
#include "utility/rank.h"
class City;
class Player;
namespace pb
{
    class GS2C_Country_Task_Info;
    class GS2C_Country_Task_Player;
}

class CountryTaskCity {


    enum { Task_Cnt = 2 };		//�������񣬹������س�
    //����struct
    struct TaskCity {
        TaskCity();
        City*  destCity;   //ѡ���ı߽�ǳ�
        uint32 winCountry; //���������ʤ�Ĺ���
		uint32 Origin_countryID;
    };
    struct TaskPlayer {
        TaskPlayer();
        uint64 playerId;
        int    rank; //�Լ��ڰ��е���λ��1��ʼ
        uint32 statisticKill;
        bool   isTakeReward[Task_Cnt]; //����ȡ
        int    taskEndRank[Task_Cnt];  //�������ʱ������  ��������

        int GetRankVal(){ return statisticKill; }
    };

    bool                            m_isBegined;		//�����Ƿ���
    TaskCity                        m_task[Task_Cnt];	//��������ǳ�
    std::map<uint64, TaskPlayer>    m_playerLst;		//��������������Ϣ��������������
    Rank<TaskPlayer>                m_RankLst;			//����

	//typedef std::map<uint32, Rank<TaskPlayer>>	CountryRankMap;		//�������ҷֱ�����
	//typedef std::map<uint32, CountryRankMap>	CityTaskRankMap;	//����������е�������Ϣ
	//CityTaskRankMap					m_RankAllInfo;		//�����ǳأ��������ҵ����� 

	Rank<TaskPlayer>				m_RankAllInfo_0_1;
	Rank<TaskPlayer>				m_RankAllInfo_0_2;
	Rank<TaskPlayer>				m_RankAllInfo_0_3;
	Rank<TaskPlayer>				m_RankAllInfo_1_1;
	Rank<TaskPlayer>				m_RankAllInfo_1_2;
	Rank<TaskPlayer>				m_RankAllInfo_1_3;

	uint64 taskEndTime;

    CountryTaskCity();
public:
    static CountryTaskCity& Instance(){ static CountryTaskCity T; return T; }

    void SaveTo(pb::GS2C_Country_Task_Info& msg);
    void SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player);

    bool Begin(int lastSecond);
    void End();

    bool IsRunning();		//�����Ƿ��ڼ�����  ��������ǳض������˻�ʤ���У����������
    bool IsWinCountry(uint32 countryId, uint idx);
    bool CanTakeReward(uint64 playerId, uint idx); //����ȡ����
	bool TakeReward(Player& player, uint idx);

    void AddKillCnt(uint64 playerId, uint32 cnt);
    void OnCityBeOccupied(City* pCity); //�ǳ�����

	void TakeAwardsBeforeStart();
};
#define sCountryTaskCity CountryTaskCity::Instance()
