#pragma once
#include "def/TypeDef.h"
#include "Country.pb.h"
#include "server_game/character/Character.h"
#include "utility/rank.h"
//
//enum CrossWarState	//跨服战状态
//{
//	State_Null,		//空闲状态
//	State_Collect_Info,	//收集游戏服务器信息
//	State_Devide,	//分配服务器
//	State_Prepare,	//筹备阶段
//	State_Preview,	//预览阶段
//	State_Start,	//开启跨服灭国
//	State_Award,	//领奖阶段
//};
namespace pb
{
	class GS2C_Cross_Donation_Datas;
	class Country_Cross_Star_info;
}


class CrossWarMgr
{
public:
	struct CrossPlayer { //参与玩家
		CrossPlayer();
		uint32 countryId;
		int    rank;				//排名		rank=0没上榜
		uint32 statisticKill;		//杀人数

		int GetRankVal(){ return statisticKill; }
	};

	typedef std::map<uint64, CrossPlayer> TaskPlayerList;		//对应玩家ID的玩家信息
	std::map<uint32, TaskPlayerList>    m_playerLst;		//对应每个国家的玩家列表  countryid
	Rank<CrossPlayer>                m_RankLst_1;				//国家1的排行榜
	Rank<CrossPlayer>                m_RankLst_2;
	Rank<CrossPlayer>                m_RankLst_3;

public:
	CrossWarMgr();
	static CrossWarMgr& Instance(){ static CrossWarMgr T; return T; }
	
	void SaveTo(pb::GS2C_Cross_Donation_Datas& msg, uint32 countryid);

	void SaveTo(pb::Country_Cross_Star_info& msg, uint32 countryid);

	uint32 GetTotalDonateNum(uint32 countryid) { return m_totalDonateNum[countryid-1]; }
	void AddTotalDonateNum(uint32 countryid) { m_totalDonateNum[countryid-1]+=2; }	//捐赠一次增加两次
	void UseTroops(uint32 countryid);

	uint32 GetTotalShadowNum(uint32 countryid) { return m_totalShadowNum[countryid-1]; }
	void AddShadows(Character* charone);

	void DevideShadowsToCity();

	void CalcCrossWarResult();

	void CrossEnd();	//跨服战结束

	void SetCrossState(pb::CROSS_STATE state, uint64 state_end_time);
	pb::CROSS_STATE GetCrossState();

	void AddKillCnt(uint64 playerId, uint32 countryid, uint32 cnt);		//增加击杀数

	int GetCrossKillRank(uint64 playerId, uint32 country);

	void NoticeCrossClost();
	void TakeCrossWarRewardsOnEnd();

public:
	pb::CROSS_STATE m_cState;
	uint64 m_state_end_time;
	uint32 m_totalDonateNum[3];	//三个国家分别的总投资征召令个数
	uint32 m_totalShadowNum[3];	//三个国家分别的总投资幻影个数
	std::map<uint32, std::vector<Character*>>	m_shadowMap;	//三个国家投资幻影的列表
};

#define sCrossWarMgr CrossWarMgr::Instance()