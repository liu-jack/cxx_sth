#include "CountryTaskCity.h"
#include "../../../Loot/LootManager.h"
#include "../../../object/Player.h"
#include "../../../Loot/LootList.h"
#include "Opcode.pb.h"
#include "Country.pb.h"
#include "Base.h"
#include "../../city/city.h"
#include "../../Table/MapCountry.h"
#include "../../Table/MapCity.h"
#include "../../../session/PlayerPool.h"
#ifdef _MMO_SERVER_
#include "../../../AutomaticUpdate/AutoUpdateGame.h"
#include "../../../GameServer.h"
#endif
#include "../country.h"
#include "Map/WorldMap.h"
#include "Activity/ActMgr.h"
#include "Activity/Personal/PersonalTask.h"

static const int Win_Reward_Id = 9999; //见loot表配置

CountryTaskCity::TaskCity::TaskCity()		//任务城池初始化
{
    ZeroMemoryThis;
}
CountryTaskCity::TaskPlayer::TaskPlayer()		//任务玩家信息初始化
{
    ZeroMemoryThis;
}
CountryTaskCity::CountryTaskCity()
    : m_isBegined(false)	
	, m_RankLst(40)			//排40个人
	, m_RankAllInfo_0_1(40)
	, m_RankAllInfo_0_2(40)
	, m_RankAllInfo_0_3(40)
	, m_RankAllInfo_1_1(40)
	, m_RankAllInfo_1_2(40)
	, m_RankAllInfo_1_3(40)
{
	//for(int j=0; j<Task_Cnt; j++)
	//{
	//	for(int i=1; i<=3; i++)
	//	{
	//		CountryRankMap& countryrank = m_RankAllInfo[j];
	//		countryrank[i] = new Rank<TaskPlayer>(40);
	//	}
	//}
}

//国战任务城市信息
void CountryTaskCity::SaveTo(pb::GS2C_Country_Task_Info& msg)
{
    msg.set_is_running(IsRunning());

    for (int i = 0; i < Task_Cnt; ++i)
    {
        pb::Struct_Country_Task_City* taskCity = msg.add_city();
		taskCity->set_city_id(m_task[i].destCity ? m_task[i].destCity->id : 0);	//城市id
        taskCity->set_win_country(m_task[i].winCountry);		//获胜的国家
        taskCity->set_end_time(taskEndTime);				//任务结束时间 ？一小时
		taskCity->set_origin_country(m_task[i].Origin_countryID);
    }
}

//获得一位玩家的排位等信息
void CountryTaskCity::SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player)
{
	int tmprank;
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
    if (it != m_playerLst.end())
    {
		AddKillCnt(player.GetGuid(),0);	//重新刷新排名

        TaskPlayer& data = it->second;
        msg.set_rank(data.rank);
        msg.set_kill_count(data.statisticKill);
        for (int i = 0; i < Task_Cnt; ++i)
        {
            msg.add_is_take_reward(data.isTakeReward[i]);
			if(m_task[i].winCountry)
				tmprank = data.taskEndRank[i];
			else
				tmprank = data.rank;
			msg.add_task_end_rank(tmprank);
			Country::GetRankReward(player, LOOTTYPE_COUNTRYQUEST, tmprank).SaveTo(*msg.add_rank_reward());
		}
		Country::GetWinReward(player, LOOTTYPE_COUNTRYQUEST).SaveTo(*msg.mutable_win_reward());
    }
}

//活动开始
bool CountryTaskCity::Begin(int lastSecond)
{
    if (m_isBegined) return false;

	LLOG("task begin");

    //清空旧数据
    m_playerLst.clear();
	m_RankLst.Clear();
	m_RankAllInfo_0_1.Clear();
	m_RankAllInfo_0_2.Clear();
	m_RankAllInfo_0_3.Clear();
	m_RankAllInfo_1_1.Clear();
	m_RankAllInfo_1_2.Clear();
	m_RankAllInfo_1_3.Clear();
    memset(m_task, 0, sizeof(m_task));	//任务城池

    std::vector<City*> lst;		//所有连接敌对国家的城市
    sWorldMap.BuildEdgeCityLst(lst);	//查找所有满足敌对状态的城池
    std::random_shuffle(lst.begin(), lst.end());
	bool getEdgeCity = false;
    if (lst.size() < Task_Cnt) {
        MMO_ASSERT(0);	//满足敌对城市不足
        return false;
    }
    for (int i = 0; i < Task_Cnt; ++i)		//取前两个城市
    {
		if(i == 1)	//第二个国家要进行筛选
		{
			int countryid = m_task[0].destCity->GetCountryId();
			for(size_t j=1; j<lst.size(); j++)
			{
				if(lst[j]->HaveCountryEdge(countryid))
				{
					m_task[i].destCity = lst[j];
					getEdgeCity = true;
					break;
				}
			}
			if(!getEdgeCity)
			{
				m_task[i].destCity = lst[i];
			}
		}
		else
			m_task[i].destCity = lst[i];

		m_task[i].Origin_countryID = m_task[i].destCity->GetCountryId();
		m_task[i].destCity->AddCountryTaskNpcToDefence();	
        m_task[i].destCity->AddCallBack_BeOccupied(boost::bind(&CountryTaskCity::OnCityBeOccupied, this, _1));	//城池被占领时回调	

		LLOG("task city id = %d",m_task[i].destCity->id);
    }
	
	taskEndTime = sOS.TimeSeconds() + lastSecond;

    m_isBegined = true;
#ifdef _MMO_SERVER_
    sAutoUpdateGame.AddEvent(boost::bind(&CountryTaskCity::End, this), lastSecond);	//持续时间 活动结束
#endif

	//发送开始消息
	pb::GS2C_Country_Task_Info sendmsg;
	sendmsg.set_is_running(IsRunning());

	for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
	{
		TaskPlayer& data = it->second;
		if (Player* palyer = sPlayerPool.GetByPlayerId(data.playerId))
		{
			palyer->Send(pb::SMSG_COUNTRY_TASK_CITY,sendmsg);
		}
	}
	
    return true;
}

//任务到时
void CountryTaskCity::End()
{
	if(!IsRunning()) return;	//已经决出两个任务的获胜国家

    //防守方要守到任务结束才算胜利
    for (int i = 0; i < Task_Cnt; ++i)
    {
        if (m_task[i].winCountry == 0)		//没有决出获胜国家
        {
            m_task[i].winCountry = m_task[i].destCity->GetCountryId();	//城市所属国家获胜
			
			if (Country* country = sWorldMap.GetCountry(m_task[i].winCountry))
			{
				country->AddCountryExp(GET_BASE_DEF_INT( pb::BD_COUNTRY_ATTACK_DEFENSE_COUNTRYEXP_REWARD));
			}

			//记录当前所有人的最终排名
			{
				for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
				{
					TaskPlayer& data = it->second;
					data.taskEndRank[i] = data.rank;
				}
			}
        }
    }

	//结束
	pb::GS2C_Country_Task_Info sendmsg;
	sendmsg.set_is_running(IsRunning());
	for (int i = 0; i < Task_Cnt; ++i)
	{
		pb::Struct_Country_Task_City* taskCity = sendmsg.add_city();
		taskCity->set_city_id(m_task[i].destCity ? m_task[i].destCity->id : 0);	//城市id
		taskCity->set_win_country(m_task[i].winCountry);		//获胜的国家
		taskCity->set_end_time(taskEndTime);				//任务结束时间 ？一小时

		if(m_task[i].destCity)
			m_task[i].destCity->m_CallBack.clear();
	}

	

#ifdef  _MMO_SERVER_
	sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TASK_END, sendmsg);
#endif

}

//任务是否正在进行
bool CountryTaskCity::IsRunning() 
{
    if (m_isBegined)
    {
        for (int i = 0; i < Task_Cnt; ++i)
        {
            if (m_task[i].winCountry == 0) return true;		//没有决出获胜国家
		}
		m_isBegined = false;
		sPersonTask.End();
    }
	return false;
}

//是否是获胜国家
bool CountryTaskCity::IsWinCountry(uint32 countryId, uint idx)
{
	if(idx < Task_Cnt)
        if (m_task[idx].winCountry == countryId) return true;
    return false;
}

//玩家能否拾取奖励
bool CountryTaskCity::CanTakeReward(uint64 playerId, uint idx)
{
    if (idx >= Task_Cnt) return false;

    if (m_task[idx].winCountry == 0) return false; //还没打完

    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(playerId);
    if (it == m_playerLst.end())
    {
        return false;
    }
    return !it->second.isTakeReward[idx];
}

//拾取奖励
bool CountryTaskCity::TakeReward(Player& player, uint idx)
{
	if(idx >= Task_Cnt) 
	{
		return false;
	}

    const uint64 playerId = player.GetGuid();

    if (!CanTakeReward(playerId, idx))
    {
        return false;
    }

	AddKillCnt(playerId,0);	//重新刷新排名

    TaskPlayer& refInfo = m_playerLst[playerId];

    refInfo.isTakeReward[idx] = true;

	//获取排名奖励
	Country::TakeRankReward(player, LOOTTYPE_COUNTRYQUEST, refInfo.taskEndRank[idx]);

    if (IsWinCountry(player.GetCountryId(),idx))
    {
		//获取获胜奖励
        Country::TakeWinReward(player, LOOTTYPE_COUNTRYQUEST);
    }
	return true;
}

void CountryTaskCity::TakeAwardsBeforeStart()
{
	//没领的奖励自动发给玩家
	for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
	{
		TaskPlayer& data = it->second;
		for (int i = 0; i < Task_Cnt; ++i){
			if (!data.isTakeReward[i])			//奖励未领取
			{
				if (Player* palyer = sPlayerPool.GetByPlayerId(data.playerId))
				{
					TakeReward(*palyer, i);
				}
			}
		}
	}

	//清空旧数据
	m_playerLst.clear();
	m_RankLst.Clear();
	m_RankAllInfo_0_1.Clear();
	m_RankAllInfo_0_2.Clear();
	m_RankAllInfo_0_3.Clear();
	m_RankAllInfo_1_1.Clear();
	m_RankAllInfo_1_2.Clear();
	m_RankAllInfo_1_3.Clear();
	memset(m_task, 0, sizeof(m_task));	//任务城池

}

//记录玩家击杀
void CountryTaskCity::AddKillCnt(uint64 playerId, uint32 cnt)
{
	NLOG("CountryTaskCity::AddKillCnt playerId=%lld, cnt=%d",playerId,cnt);
	if(IsRunning())
	{
		if (Player* player = sPlayerPool.GetByPlayerId(playerId))
		{
			TaskPlayer& obj = m_playerLst[playerId];		//在这里记录玩家，如果没有则在m_playerLst中添加
			obj.statisticKill += cnt;
			obj.playerId = playerId;
			//for (int i = 0; i < Task_Cnt; ++i)
			//{
			//	if(!m_task[i].winCountry)		//该城市没有决出获胜国家
			//	{
					switch(player->GetCountryId())
					{
					case 1:
						//if(i==0)
							m_RankAllInfo_0_1.OnValueChange(obj);
						//else
						//	m_RankAllInfo_1_1.OnValueChange(obj);
						break;
					case 2:
						//if(i==0)
							m_RankAllInfo_0_2.OnValueChange(obj);
						//else
						//	m_RankAllInfo_1_2.OnValueChange(obj);
						break;
					case 3:
						//if(i==0)
							m_RankAllInfo_0_3.OnValueChange(obj);
						//else
						//	m_RankAllInfo_1_3.OnValueChange(obj);
						break;
					}
					//for (int i = 0; i < Task_Cnt; ++i)
					//{
					//	if(!m_task[i].winCountry)		//该城市没有决出获胜国家
					//	{
					//		obj.taskEndRank[i] = obj.rank;
					//	}
					//}
					NLOG("obj.statisticKill=%d, obj.rank=%d",obj.statisticKill,obj.rank);
			//	}
			//}

			sActMgr.AddPlayerInActivity(playerId);

		}
	}

}
void CountryTaskCity::OnCityBeOccupied(City* pCity)
{
	LLOG("CountryTaskCity::OnCityBeOccupied, cityid=%d",pCity->GetCityId());
	if(IsRunning())
	{
		for (int i = 0; i < Task_Cnt; ++i)
		{
			if (m_task[i].destCity->GetCityId() == pCity->GetCityId() && m_task[i].winCountry == 0)		//被占领城市为任务城市，且任务还未完成
			{
				m_task[i].winCountry = pCity->GetCountryId();
				LLOG("set wincounry = %d",m_task[i].winCountry);
				if (Country* country = sWorldMap.GetCountry(pCity->GetCountryId()))
				{
					country->AddCountryExp(GET_BASE_DEF_INT( pb::BD_COUNTRY_ATTACK_DEFENSE_COUNTRYEXP_REWARD));
				}

				//记录当前所有人的最终排名
				{
					for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
					{
						TaskPlayer& data = it->second;
						data.taskEndRank[i] = data.rank;
					}
				}

				//结束
				pb::GS2C_Country_Task_Info sendmsg;
				sendmsg.set_is_running(IsRunning());
				for (int i = 0; i < Task_Cnt; ++i)
				{
					pb::Struct_Country_Task_City* taskCity = sendmsg.add_city();
					taskCity->set_city_id(m_task[i].destCity ? m_task[i].destCity->id : 0);	//城市id
					taskCity->set_win_country(m_task[i].winCountry);		//获胜的国家
					taskCity->set_end_time(taskEndTime);				//任务结束时间 ？一小时
				}
	#ifdef  _MMO_SERVER_
				sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TASK_END, sendmsg);
	#endif

			}
		}
	}



	return;
}