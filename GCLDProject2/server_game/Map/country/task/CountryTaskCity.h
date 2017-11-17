//国战攻守任务
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


    enum { Task_Cnt = 2 };		//两个任务，攻城与守城
    //辅助struct
    struct TaskCity {
        TaskCity();
        City*  destCity;   //选定的边界城池
        uint32 winCountry; //攻守任务获胜的国家
		uint32 Origin_countryID;
    };
    struct TaskPlayer {
        TaskPlayer();
        uint64 playerId;
        int    rank; //自己在榜中的排位，1起始
        uint32 statisticKill;
        bool   isTakeReward[Task_Cnt]; //已领取
        int    taskEndRank[Task_Cnt];  //任务结束时的排名  两个排名

        int GetRankVal(){ return statisticKill; }
    };

    bool                            m_isBegined;		//任务是否开启
    TaskCity                        m_task[Task_Cnt];	//两个任务城池
    std::map<uint64, TaskPlayer>    m_playerLst;		//参与任务的玩家信息：排名，奖励等
    Rank<TaskPlayer>                m_RankLst;			//排名

	//typedef std::map<uint32, Rank<TaskPlayer>>	CountryRankMap;		//三个国家分别排名
	//typedef std::map<uint32, CountryRankMap>	CityTaskRankMap;	//两个任务城市的排名信息
	//CityTaskRankMap					m_RankAllInfo;		//两个城池，三个国家的排名 

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

    bool IsRunning();		//任务是否还在继续？  如果两个城池都决出了获胜城市，则任务结束
    bool IsWinCountry(uint32 countryId, uint idx);
    bool CanTakeReward(uint64 playerId, uint idx); //已领取奖励
	bool TakeReward(Player& player, uint idx);

    void AddKillCnt(uint64 playerId, uint32 cnt);
    void OnCityBeOccupied(City* pCity); //城池易主

	void TakeAwardsBeforeStart();
};
#define sCountryTaskCity CountryTaskCity::Instance()
