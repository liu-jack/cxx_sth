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

static const int Win_Reward_Id = 9999; //��loot������

CountryTaskCity::TaskCity::TaskCity()		//����ǳس�ʼ��
{
    ZeroMemoryThis;
}
CountryTaskCity::TaskPlayer::TaskPlayer()		//���������Ϣ��ʼ��
{
    ZeroMemoryThis;
}
CountryTaskCity::CountryTaskCity()
    : m_isBegined(false)	
	, m_RankLst(40)			//��40����
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

//��ս���������Ϣ
void CountryTaskCity::SaveTo(pb::GS2C_Country_Task_Info& msg)
{
    msg.set_is_running(IsRunning());

    for (int i = 0; i < Task_Cnt; ++i)
    {
        pb::Struct_Country_Task_City* taskCity = msg.add_city();
		taskCity->set_city_id(m_task[i].destCity ? m_task[i].destCity->id : 0);	//����id
        taskCity->set_win_country(m_task[i].winCountry);		//��ʤ�Ĺ���
        taskCity->set_end_time(taskEndTime);				//�������ʱ�� ��һСʱ
		taskCity->set_origin_country(m_task[i].Origin_countryID);
    }
}

//���һλ��ҵ���λ����Ϣ
void CountryTaskCity::SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player)
{
	int tmprank;
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
    if (it != m_playerLst.end())
    {
		AddKillCnt(player.GetGuid(),0);	//����ˢ������

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

//���ʼ
bool CountryTaskCity::Begin(int lastSecond)
{
    if (m_isBegined) return false;

	LLOG("task begin");

    //��վ�����
    m_playerLst.clear();
	m_RankLst.Clear();
	m_RankAllInfo_0_1.Clear();
	m_RankAllInfo_0_2.Clear();
	m_RankAllInfo_0_3.Clear();
	m_RankAllInfo_1_1.Clear();
	m_RankAllInfo_1_2.Clear();
	m_RankAllInfo_1_3.Clear();
    memset(m_task, 0, sizeof(m_task));	//����ǳ�

    std::vector<City*> lst;		//�������ӵжԹ��ҵĳ���
    sWorldMap.BuildEdgeCityLst(lst);	//������������ж�״̬�ĳǳ�
    std::random_shuffle(lst.begin(), lst.end());
	bool getEdgeCity = false;
    if (lst.size() < Task_Cnt) {
        MMO_ASSERT(0);	//����жԳ��в���
        return false;
    }
    for (int i = 0; i < Task_Cnt; ++i)		//ȡǰ��������
    {
		if(i == 1)	//�ڶ�������Ҫ����ɸѡ
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
        m_task[i].destCity->AddCallBack_BeOccupied(boost::bind(&CountryTaskCity::OnCityBeOccupied, this, _1));	//�ǳر�ռ��ʱ�ص�	

		LLOG("task city id = %d",m_task[i].destCity->id);
    }
	
	taskEndTime = sOS.TimeSeconds() + lastSecond;

    m_isBegined = true;
#ifdef _MMO_SERVER_
    sAutoUpdateGame.AddEvent(boost::bind(&CountryTaskCity::End, this), lastSecond);	//����ʱ�� �����
#endif

	//���Ϳ�ʼ��Ϣ
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

//����ʱ
void CountryTaskCity::End()
{
	if(!IsRunning()) return;	//�Ѿ�������������Ļ�ʤ����

    //���ط�Ҫ�ص������������ʤ��
    for (int i = 0; i < Task_Cnt; ++i)
    {
        if (m_task[i].winCountry == 0)		//û�о�����ʤ����
        {
            m_task[i].winCountry = m_task[i].destCity->GetCountryId();	//�����������һ�ʤ
			
			if (Country* country = sWorldMap.GetCountry(m_task[i].winCountry))
			{
				country->AddCountryExp(GET_BASE_DEF_INT( pb::BD_COUNTRY_ATTACK_DEFENSE_COUNTRYEXP_REWARD));
			}

			//��¼��ǰ�����˵���������
			{
				for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
				{
					TaskPlayer& data = it->second;
					data.taskEndRank[i] = data.rank;
				}
			}
        }
    }

	//����
	pb::GS2C_Country_Task_Info sendmsg;
	sendmsg.set_is_running(IsRunning());
	for (int i = 0; i < Task_Cnt; ++i)
	{
		pb::Struct_Country_Task_City* taskCity = sendmsg.add_city();
		taskCity->set_city_id(m_task[i].destCity ? m_task[i].destCity->id : 0);	//����id
		taskCity->set_win_country(m_task[i].winCountry);		//��ʤ�Ĺ���
		taskCity->set_end_time(taskEndTime);				//�������ʱ�� ��һСʱ

		if(m_task[i].destCity)
			m_task[i].destCity->m_CallBack.clear();
	}

	

#ifdef  _MMO_SERVER_
	sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TASK_END, sendmsg);
#endif

}

//�����Ƿ����ڽ���
bool CountryTaskCity::IsRunning() 
{
    if (m_isBegined)
    {
        for (int i = 0; i < Task_Cnt; ++i)
        {
            if (m_task[i].winCountry == 0) return true;		//û�о�����ʤ����
		}
		m_isBegined = false;
		sPersonTask.End();
    }
	return false;
}

//�Ƿ��ǻ�ʤ����
bool CountryTaskCity::IsWinCountry(uint32 countryId, uint idx)
{
	if(idx < Task_Cnt)
        if (m_task[idx].winCountry == countryId) return true;
    return false;
}

//����ܷ�ʰȡ����
bool CountryTaskCity::CanTakeReward(uint64 playerId, uint idx)
{
    if (idx >= Task_Cnt) return false;

    if (m_task[idx].winCountry == 0) return false; //��û����

    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(playerId);
    if (it == m_playerLst.end())
    {
        return false;
    }
    return !it->second.isTakeReward[idx];
}

//ʰȡ����
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

	AddKillCnt(playerId,0);	//����ˢ������

    TaskPlayer& refInfo = m_playerLst[playerId];

    refInfo.isTakeReward[idx] = true;

	//��ȡ��������
	Country::TakeRankReward(player, LOOTTYPE_COUNTRYQUEST, refInfo.taskEndRank[idx]);

    if (IsWinCountry(player.GetCountryId(),idx))
    {
		//��ȡ��ʤ����
        Country::TakeWinReward(player, LOOTTYPE_COUNTRYQUEST);
    }
	return true;
}

void CountryTaskCity::TakeAwardsBeforeStart()
{
	//û��Ľ����Զ��������
	for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
	{
		TaskPlayer& data = it->second;
		for (int i = 0; i < Task_Cnt; ++i){
			if (!data.isTakeReward[i])			//����δ��ȡ
			{
				if (Player* palyer = sPlayerPool.GetByPlayerId(data.playerId))
				{
					TakeReward(*palyer, i);
				}
			}
		}
	}

	//��վ�����
	m_playerLst.clear();
	m_RankLst.Clear();
	m_RankAllInfo_0_1.Clear();
	m_RankAllInfo_0_2.Clear();
	m_RankAllInfo_0_3.Clear();
	m_RankAllInfo_1_1.Clear();
	m_RankAllInfo_1_2.Clear();
	m_RankAllInfo_1_3.Clear();
	memset(m_task, 0, sizeof(m_task));	//����ǳ�

}

//��¼��һ�ɱ
void CountryTaskCity::AddKillCnt(uint64 playerId, uint32 cnt)
{
	NLOG("CountryTaskCity::AddKillCnt playerId=%lld, cnt=%d",playerId,cnt);
	if(IsRunning())
	{
		if (Player* player = sPlayerPool.GetByPlayerId(playerId))
		{
			TaskPlayer& obj = m_playerLst[playerId];		//�������¼��ң����û������m_playerLst�����
			obj.statisticKill += cnt;
			obj.playerId = playerId;
			//for (int i = 0; i < Task_Cnt; ++i)
			//{
			//	if(!m_task[i].winCountry)		//�ó���û�о�����ʤ����
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
					//	if(!m_task[i].winCountry)		//�ó���û�о�����ʤ����
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
			if (m_task[i].destCity->GetCityId() == pCity->GetCityId() && m_task[i].winCountry == 0)		//��ռ�����Ϊ������У�������δ���
			{
				m_task[i].winCountry = pCity->GetCountryId();
				LLOG("set wincounry = %d",m_task[i].winCountry);
				if (Country* country = sWorldMap.GetCountry(pCity->GetCountryId()))
				{
					country->AddCountryExp(GET_BASE_DEF_INT( pb::BD_COUNTRY_ATTACK_DEFENSE_COUNTRYEXP_REWARD));
				}

				//��¼��ǰ�����˵���������
				{
					for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
					{
						TaskPlayer& data = it->second;
						data.taskEndRank[i] = data.rank;
					}
				}

				//����
				pb::GS2C_Country_Task_Info sendmsg;
				sendmsg.set_is_running(IsRunning());
				for (int i = 0; i < Task_Cnt; ++i)
				{
					pb::Struct_Country_Task_City* taskCity = sendmsg.add_city();
					taskCity->set_city_id(m_task[i].destCity ? m_task[i].destCity->id : 0);	//����id
					taskCity->set_win_country(m_task[i].winCountry);		//��ʤ�Ĺ���
					taskCity->set_end_time(taskEndTime);				//�������ʱ�� ��һСʱ
				}
	#ifdef  _MMO_SERVER_
				sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TASK_END, sendmsg);
	#endif

			}
		}
	}



	return;
}