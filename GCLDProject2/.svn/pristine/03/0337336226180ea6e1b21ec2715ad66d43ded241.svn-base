#include "OccupyCity.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "../../session/PlayerPool.h"
#include "../Table/ActTableMgr.h"
#include "../../reward/reward.h"
#ifdef _MMO_SERVER_
#include "../../AutomaticUpdate/AutoUpdateGame.h"
#include "GameServer.h"
#endif
#include "../../Map/country/country.h"
#include "../../Map/country/task/CountryTaskCity.h"
#include "../../Map/Table/MapCountry.h"
#include "Country.pb.h"
#include "../../Loot/LootList.h"
#include "Map/WorldMap.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "../ActMgr.h"
#include "../Personal/PersonalTask.h"

using namespace pb;
OccupyCity::TaskPlayer::TaskPlayer()
{
    ZeroMemoryThis;
}
OccupyCity::OccupyCity()
    : m_RankLst_1(40)
	, m_RankLst_2(40)
	, m_RankLst_3(40)
{
    m_isBegined = false;
    m_curStage = 0;
    m_curStageBeginTime = 0;
    ZeroMemoryArray(m_curStageBegin_CountryHaveCity);
}
void OccupyCity::SaveTo(pb::GS2C_Country_Task_Occupy& msg)
{
	msg.set_is_begin(m_isBegined);
	msg.set_cur_stage_begin_time(m_curStageBeginTime);
	int stagenum = 0;
	if(m_curStage > MAX_OCCUPY_STAGE)
		stagenum = MAX_OCCUPY_STAGE;
	else
		stagenum = m_curStage;	
	msg.set_cur_stage(stagenum);

	if (const DB_Open_Frontier* table = sActTableMgr.GetFrontier(stagenum))
	{
		uint8 maxOccupyCityNum = sWorldMap.GetCountry(1)->m_setCity.size() + sWorldMap.GetCountry(2)->m_setCity.size() + sWorldMap.GetCountry(3)->m_setCity.size()-4;	//三个国家总城池数减主城与屯田
		for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
		{
			msg.add_cur_stage_now_country_have_citys(sWorldMap.GetCountry(i + 1)->m_setCity.size());
			msg.add_cur_stage_begin_have_citys(m_curStageBegin_CountryHaveCity[i]);
			uint8 needTotalCities = MIN(maxOccupyCityNum,m_curStageBegin_CountryHaveCity[i] + table->need_cities);
			msg.add_need_total_citys(needTotalCities);
		}
	}
}
void OccupyCity::SaveTo(pb::GS2C_Country_Occupy_Player& msg, Player& player)
{
	/*int laststage = 0;*/
	for(int stagenum = 1; stagenum <= m_curStage; stagenum++)
	{
		if(isPlayerInList(stagenum,player))
		{
	/*		laststage = stagenum;*/
			TaskPlayer& pData = GetPlayerData(stagenum,player);
			if(!pData.isTakeReward)
			{
				int ranktmp;
				if(pData.rank == 0)
					ranktmp = 999;
				else
					ranktmp = pData.rank;
				int rank = sCountryTableMgr.GetRankId(ranktmp);
				msg.set_stageid(stagenum);
				msg.set_rankid(rank);
				msg.set_rank(pData.rank);
				msg.set_is_take_reward(pData.isTakeReward);
				msg.set_cantakeexreward(pData.canTakeExReward);
				Country::GetRankReward(player, LOOTTYPE_ACT_INVESTMENT, ranktmp).SaveTo(*msg.mutable_rank_reward());
				Country::GetWinReward(player, LOOTTYPE_ACT_INVESTMENT).SaveTo(*msg.mutable_win_reward());
				return;
			}
		}
	}
	//if(laststage != 0)
	//{
	//	TaskPlayer& pData = GetPlayerData(laststage,player);
	//}
}

bool OccupyCity::Begin(int lastSecond)
{
    if (m_isBegined) return false;

    m_isBegined = true;
    m_curStage = 0;
    m_curStageBeginTime = 0;
    m_playerLst.clear();
	m_RankLst_1.Clear();
	m_RankLst_2.Clear();
	m_RankLst_3.Clear();

    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        m_curStageBegin_CountryHaveCity[i] = sWorldMap.GetCountry(i+1)->m_setCity.size();		//当前国家所拥有的城池数量
    }

    _EnterNextStage();		//进入下一阶段
#ifdef _MMO_SERVER_
    sAutoUpdateGame.AddEvent(boost::bind(&OccupyCity::End, this), lastSecond);		//持续时间到了之后结束
#endif
	return true;
}
void OccupyCity::End()
{
    m_isBegined = false;
	sPersonTask.End();
}

void OccupyCity::_EnterNextStage()
{
    // 结算上一轮国家完成度奖励
    if (const DB_Open_Frontier* table = sActTableMgr.GetFrontier(m_curStage))
    {
        uint8 haveCityCnt[MAX_PLAYER_COUNTRY_ID];	//当前占领城市数
        bool isCountryWin[MAX_PLAYER_COUNTRY_ID];
		uint8 maxOccupyCityNum = sWorldMap.GetCountry(1)->m_setCity.size() + sWorldMap.GetCountry(2)->m_setCity.size() + sWorldMap.GetCountry(3)->m_setCity.size()-4;	//三个国家总城池数减主城与屯田
        for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
        {
            Country* country = sWorldMap.GetCountry(i + 1);
            haveCityCnt[i] = country->m_setCity.size();
			uint8 needTotalCities = MIN(maxOccupyCityNum,m_curStageBegin_CountryHaveCity[i] + table->need_cities);
            isCountryWin[i] = (haveCityCnt[i] >= needTotalCities);

            if (isCountryWin[i]) 
			{
				country->AddCountryExp(table->reward_country_exp);

				TaskPlayerList& m_curPlayerLst = m_playerLst[m_curStage];
				for (TaskPlayerList::iterator it = m_curPlayerLst.begin(); it != m_curPlayerLst.end(); ++it)
				{
					TaskPlayer& data = it->second;
					if(data.countryId == i+1)
						data.canTakeExReward = true;		//该国家的玩家能够领取额外奖励
				}
			}
		}

		m_RankLst_1.Clear();
		m_RankLst_2.Clear();
		m_RankLst_3.Clear();
        memcpy(m_curStageBegin_CountryHaveCity, haveCityCnt, sizeof(haveCityCnt));
    }

    m_curStage += 1;
    m_curStageBeginTime = OS::TimeSeconds();

    if (const DB_Open_Frontier* table = sActTableMgr.GetFrontier(m_curStage))		//如果下一个阶段存在，则进入下一个阶段倒计时
    {
#ifdef _MMO_SERVER_
        sAutoUpdateGame.AddEvent(boost::bind(&OccupyCity::_EnterNextStage, this), table->stage_last_time);
#endif
        
    }
}
void OccupyCity::_GivePlayerReward(Player& player, int rank, bool isCountryWin)
{
    Country::TakeRankReward(player, LOOTTYPE_ACT_OCCUPY_CITY, rank);

    if (isCountryWin) Country::TakeWinReward(player, LOOTTYPE_ACT_OCCUPY_CITY);
	
}

void OccupyCity::_TakeRewards(Player& player, uint32 stage, GS2C_Country_Occupy_Take_Reward & msg)
{
	msg.set_issuccess(false);
	if(m_curStage <= stage) 
	{
		msg.set_errret(pb::OccupyStageNotMatch);
		return;		//申请的阶段必须小于当前阶段
	}

	if(isPlayerInList(stage,player))	//在目标阶段有上阵
	{
		TaskPlayer& pData = GetPlayerData(stage,player);
		if (pData.isTakeReward) 
		{
			msg.set_errret(pb::rewardHasTaken);
			return;	//已经领取
		}
		pData.isTakeReward = true;
		int ranktmp;
		if(pData.rank == 0)
			ranktmp = 999;
		else
			ranktmp = pData.rank;
		Country::TakeRankReward(player, LOOTTYPE_ACT_OCCUPY_CITY, sCountryTableMgr.GetRankId(ranktmp));
		if (pData.canTakeExReward)
		{
			pData.canTakeExReward = false;
			Country::TakeWinReward(player, LOOTTYPE_ACT_OCCUPY_CITY);
		}
		msg.set_issuccess(true);
		return;
	}
	else
	{
		msg.set_errret(pb::stageNotInvested);
	}
	return;
}

void OccupyCity::TakeAwardsBeforeStart()
{
	//没领的奖励自动发给玩家
	for(std::map<uint32, TaskPlayerList>::iterator ite = m_playerLst.begin(); ite != m_playerLst.end(); ite++)
	{
		TaskPlayerList& m_curPlayerLst = ite->second;
		for (TaskPlayerList::iterator it = m_curPlayerLst.begin(); it != m_curPlayerLst.end(); ++it)
		{
			if (!it->second.isTakeReward)
			{
				if (Player* palyer = sPlayerPool.GetByPlayerId(it->first))
				{
					pb::GS2C_Country_Occupy_Take_Reward msg;
					_TakeRewards(*palyer,ite->first,msg);
				}
			}
		}
	}

	m_curStage = 0;
	m_curStageBeginTime = 0;
	m_playerLst.clear();
	m_RankLst_1.Clear();
	m_RankLst_2.Clear();
	m_RankLst_3.Clear();

}

void OccupyCity::AddKillCnt(uint64 playerId, uint32 cnt)
{
    if (m_isBegined)
    {
		if (Player* player = sPlayerPool.GetByPlayerId(playerId))
		{
			//TaskPlayerList& playerlststage = m_playerLst[m_curStage];
			TaskPlayer& obj = GetPlayerData(m_curStage,*player);
			obj.statisticKill += cnt;
			switch(player->GetCountryId())
			{
			case 1:
				m_RankLst_1.OnValueChange(obj);
				break;
			case 2:
				m_RankLst_2.OnValueChange(obj);
				break;
			case 3:
				m_RankLst_3.OnValueChange(obj);
				break;
			}
		}
    }
}

OccupyCity::TaskPlayer& OccupyCity::GetPlayerData(uint32 stage, Player& player)
{
	TaskPlayerList& playerlststage = m_playerLst[stage];
	std::map<uint64, TaskPlayer>::iterator it = playerlststage.find(player.GetGuid());
	if (it == playerlststage.end())
	{
		TaskPlayer& playerone = playerlststage[player.GetGuid()];
		playerone.countryId = player.GetCountryId();
		playerone.isTakeReward = false;
		playerone.canTakeExReward = false;
		playerone.statisticKill = 0;
		playerone.rank = 0;

		sActMgr.AddPlayerInActivity(player.GetGuid());

		return playerone;
	}

	return it->second;
}

//玩家是否在当前阶段的队列中 check
bool OccupyCity::isPlayerInList(uint32 stage, Player& player)
{
	TaskPlayerList& playerlststage = m_playerLst[stage];
	std::map<uint64, TaskPlayer>::iterator it = playerlststage.find(player.GetGuid());
	if (it != playerlststage.end())
	{
		return true;
	}
	return false;
}

void OccupyCity::NoticePlayerCityChange()
{
	if (m_isBegined)
	{
		pb::GS2C_Country_Task_Occupy msg;
		SaveTo(msg);
//#ifdef _MMO_SERVER_
//		sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TAST_OCCUPY, msg);
//#endif
		FOREACH_ONLINE_PLAYERPTR(PlayerId, playerPtr)
		{
			playerPtr->Send(SMSG_COUNTRY_TAST_OCCUPY,msg);
			pb::GS2C_Country_Occupy_Player sendmsg;
			SaveTo(sendmsg,*playerPtr);
			playerPtr->Send(SMSG_COUNTRY_OCCUPY_PLAYER,sendmsg);
		}
	}
}