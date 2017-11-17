#include "CrossWarMgr.h"
#include "Logger.h"
#include "BaseDefine.pb.h"
#include "BaseDefineMgr.h"
#include "../CrossWorldMap.h"
#include "Map/city/city.h"
#include "CrossWar.pb.h"
#include "Map/country/country.h"
#include "session/GameServerSession.h"
#include "ServerOpcode.pb.h"
#include "../CrossActivity/CrossActMgr.h"
#include "session/PlayerPool.h"
#include "common/memory_buffer/NetPack.h"
#include "../CrossArsenal/CrossArsenalMgr.h"
#include "cross_wall/table/CrossTableMgr.h"
#include "InterServer.pb.h"

CrossWarMgr::CrossPlayer::CrossPlayer()
{
	ZeroMemoryThis;
}

CrossWarMgr::CrossWarMgr()
	: m_RankLst_1(40)
	, m_RankLst_2(40)
	, m_RankLst_3(40)
{
	m_cState = pb::State_Null;
	for(int i=0; i<3; i++)
	{
		m_totalDonateNum[i] = 0;
		m_totalShadowNum[i] = 0;
	}
	m_shadowMap.clear();
	m_playerLst.clear();
}

void CrossWarMgr::SaveTo(pb::GS2C_Cross_Donation_Datas& msg, uint32 countryid)
{
	if(m_cState == pb::State_Prepare || m_cState == pb::State_Preview)
	{
		pb::GS2C_Cross_Donation_Data* donateOne = msg.add_donation_datas();
		donateOne->set_item_type(pb::Office_Troops);
		donateOne->set_count(GetTotalDonateNum(countryid));

		pb::GS2C_Cross_Donation_Data* donateTwo = msg.add_donation_datas();
		donateTwo->set_item_type(pb::Shadow_Troops);
		donateTwo->set_count(GetTotalShadowNum(countryid));
	}
}

void CrossWarMgr::SaveTo(pb::Country_Cross_Star_info& msg, uint32 countryid)
{
	LLOG("CrossWarMgr::SaveTo");
	if(m_cState == pb::State_Start)
	{
		if(Country* country = sCrossWorldMap.GetCountry(countryid))
		{
			msg.set_countryid(countryid);
			msg.set_isdestroied(country->IsCrossCountryOccypied());
			msg.set_starnum(country->m_crossStarRec.size());
			LLOG("countryid=%d, isdestroy=%d, starnum=%d",countryid,country->IsCrossCountryOccypied(),country->m_crossStarRec.size());
		}
	}
}

void CrossWarMgr::UseTroops(uint32 countryid)
{
	if(m_totalDonateNum[countryid-1] > 0)
		m_totalDonateNum[countryid-1] -= 1;
	else
		m_totalDonateNum[countryid-1] = 0;
}

void CrossWarMgr::AddShadows(Character* charone)
{
	uint32 countryid = charone->GetCountryId();
	m_totalShadowNum[countryid-1] ++;
	LLOG("[1002] add shadows countryid = %d",countryid);

	std::map<uint32, std::vector<Character*>>::iterator mapone = m_shadowMap.find(countryid);
	if(mapone != m_shadowMap.end())
	{
		LLOG("[1002] add shadow 1");
		std::vector<Character*>& shadowVec = mapone->second;
		shadowVec.push_back(charone);
	}
	else
	{
		LLOG("[1002] add shadow 2");
		std::vector<Character*> shadowVec;
		shadowVec.push_back(charone);
		m_shadowMap.insert(std::make_pair(countryid,shadowVec));
	}
}

void CrossWarMgr::DevideShadowsToCity()
{
	LLOG("[1002] CrossWarMgr::DevideShadowsToCity");
	//三个国家的幻影分配到城池
	for(int i=1; i<=MAX_PLAYER_COUNTRY_ID; i++)
	{
		LLOG("[1002] countryid=%d",i);
		std::map<uint32, std::vector<Character*>>::iterator charVecIt = m_shadowMap.find(i);
		if(charVecIt != m_shadowMap.end())
		{
			LLOG("[1002] Get country %d shadows",i);
			std::vector<Character*>& charVec = charVecIt->second;
			int charNum = charVec.size();
			if(charNum>0)
			{
				LLOG("[1002] shadow num = %d",charNum);
				int char_num[3];
				int char_start[3];
				int city_id[3];

				char_num[0] = charNum/3;
				char_start[0] = 0;

				char_num[1] = charNum/3;
				char_start[1] = char_num[0];

				char_num[2] = charNum - char_num[0] - char_num[1];
				char_start[2] = char_num[0] + char_num[1];

				LLOG("[1002] %d %d %d %d %d %d",char_num[0],char_start[0],char_num[1],char_start[1],char_num[2],char_start[2]);

				if(i == 1)	//虫族
				{
					city_id[0] = GET_BASE_DEF_INT(pb::BD_PERISH_ZERG_SHADOW_CITY1);
					city_id[1] = GET_BASE_DEF_INT(pb::BD_PERISH_ZERG_SHADOW_CITY2);
					city_id[2] = GET_BASE_DEF_INT(pb::BD_PERISH_ZERG_SHADOW_CITY3);
				}
				else if(i == 2)	//人族
				{
					city_id[0] = GET_BASE_DEF_INT(pb::BD_PERISH_TERRAN_SHADOW_CITY1);
					city_id[1] = GET_BASE_DEF_INT(pb::BD_PERISH_TERRAN_SHADOW_CITY2);
					city_id[2] = GET_BASE_DEF_INT(pb::BD_PERISH_TERRAN_SHADOW_CITY3);
				}
				else if(i == 3)	//神族
				{
					city_id[0] = GET_BASE_DEF_INT(pb::BD_PERISH_PROTOSS_SHADOW_CITY1);
					city_id[1] = GET_BASE_DEF_INT(pb::BD_PERISH_PROTOSS_SHADOW_CITY2);
					city_id[2] = GET_BASE_DEF_INT(pb::BD_PERISH_PROTOSS_SHADOW_CITY3);
				}
				
				LLOG("[1002] city1=%d,city2=%d,city3=%d",city_id[0],city_id[1],city_id[2]);

				for(int j=0; j<3; j++)
				{
					City* city = sCrossWorldMap.GetCity(city_id[j]);
					{
						if(city && city->GetCountryId() == i)
						{
							LLOG("[1002] ready to add char");
							for(int k=char_start[j]; k<char_start[j]+char_num[j]; k++)
							{
								LLOG("[1002] add one char");
								city->_AddCombatObj(*charVec[k],i);
							}
						}
					}
				}
			}
		}
	}
}

void CrossWarMgr::CalcCrossWarResult()
{
	for(int i=1; i<=MAX_PLAYER_COUNTRY_ID; i++)	//三个国家
	{
		Country* countryOne = sCrossWorldMap.GetCountry(i);
		if(countryOne)
		{
			pb::CR2G_Country_Awards msgAward;
			msgAward.set_unique_index_id(countryOne->GetCrossUniqueID());
			if(!countryOne->IsCrossCountryOccypied())
			{
				msgAward.set_defanceawardtoexp(1);
			}

			msgAward.set_attackawardtoexp(countryOne->m_crossStarRec.size() * 2);

			if(Session* gamesession = countryOne->GetGameSessionFromCross())
				gamesession->Send(pb::SR2G_Cross_Result_Country,msgAward);
		}
	}
}

void CrossWarMgr::CrossEnd()
{
	TakeCrossWarRewardsOnEnd();
	sCrossActMgr.TakeAwardsOnEnd();
	NoticeCrossClost();

	for(int i=0; i<3; i++)
	{
		m_totalDonateNum[i] = 0;
		m_totalShadowNum[i] = 0;
	}
	m_shadowMap.clear();
	m_playerLst.clear();

	for(int i=1; i<=NEURRAL_COUNTRY_ID; i++)	//四个国家
	{
		Country* countryOne = sCrossWorldMap.GetCountry(i);
		if(countryOne)
		{
			if(Session* gamesession = countryOne->GetGameSessionFromCross())
			{
				gamesession->SetIsNeedClose(true);
			}
			countryOne->m_setCity.clear();
		}
	}

	//FOREACH_PLAYERPTR(playerId, playerPtr)
	//{
	//	//playerPtr->CloseSession();
	//	Player* player = sPlayerPool.GetByPlayerId(playerId);
	//	if(player)
	//		delete player;
	//}
	sPlayerPool.RemoveAllAccounts();

	sCrossWorldMap.ClearCountryEndCitys();
	sCrossArsenalMgr.Init();
}

void CrossWarMgr::TakeCrossWarRewardsOnEnd()
{
	for(int i=0; i < MAX_PLAYER_COUNTRY_ID; i++)
	{
		Country* country = sCrossWorldMap.GetCountry(i);
		if(country)
		{
			for(CrossPlayerMap::iterator itr = country->m_CrossPlayerLst.begin(); itr != country->m_CrossPlayerLst.end(); itr++)
			{
				::CrossPlayer& crossPlayer = itr->second;
				if(!crossPlayer.isTakeReward)	//没有领取
				{
					Player* playerone = sPlayerPool.GetByPlayerId(itr->first);
					if(playerone)
					{
						if(const Table_Cross_One* tableone = sCrossTableMgr.GetPQuestByLevel(sCrossActMgr.GetPlayerTaskLevel(*playerone)))
						{
							pb::SR2G_AwardTakePlayer msg; 
							msg.set_playerid(itr->first);
							if(!country->IsCrossCountryOccypied())	//防守成功
							{
								for(IntPairVec::const_iterator it = tableone->m_DefenceRewards.begin(); it != tableone->m_DefenceRewards.end(); it++)
								{
									pb::PairValue* pairvalue = msg.add_rewards();
									pairvalue->set_first(it->first);
									pairvalue->set_second(it->second);
								}
							}

							if(uint32 starNum = country->m_crossStarRec.size())	//星数奖励
							{
								for(IntPairVec::const_iterator ite = tableone->m_StarRewards[starNum-1].begin(); ite != tableone->m_StarRewards[starNum-1].end(); ite++)
								{
									pb::PairValue* pairvalue = msg.add_rewards();
									pairvalue->set_first(ite->first);
									pairvalue->set_second(ite->second);
								}
							}
							crossPlayer.isTakeReward = true;
							playerone->SendToServer(pb::SR2G_Award_Take_Player,msg,PACKET_TYPE_SERVER_CROSS);
						}
					}
				}
			}
		}
	}
}

void CrossWarMgr::NoticeCrossClost()
{
	for(int i=1; i<=MAX_PLAYER_COUNTRY_ID; i++)	//三个国家
	{
		Country* countryOne = sCrossWorldMap.GetCountry(i);
		if(countryOne)
		{
			if(Session* gamesession = countryOne->GetGameSessionFromCross())
			{
				NetPack pck(pb::SR2G_NOTICE_CLOSE_CROSS);
				gamesession->Send(pck);
			}
		}
	}
}

void CrossWarMgr::SetCrossState(pb::CROSS_STATE state, uint64 state_end_time)
{
	m_cState = state;
	m_state_end_time = state_end_time;
	switch (m_cState)
	{
	case pb::State_Null:
		//将未处理的数据处理掉并清空，通知gameserver准备断开连接，并断开连接
		sCrossWarMgr.CrossEnd();
		break;
	case pb::State_Collect_Info:
		break;
	case pb::State_Devide:
		break;
	case pb::State_Prepare:
		break;
	case pb::State_Preview:
		DevideShadowsToCity();		//将幻影分发到城池内

		break;
	case pb::State_Start:
		sCrossActMgr.StartActivity();	//活动开启
		break;
	case pb::State_Award:
		CalcCrossWarResult();	//计算结果奖励
		sCrossActMgr.EndActivity();
		break;
	default:
		break;
	}
}

pb::CROSS_STATE CrossWarMgr::GetCrossState()
{
	return m_cState;
}

void CrossWarMgr::AddKillCnt(uint64 playerId, uint32 countryid, uint32 cnt)
{
	TaskPlayerList& playerLst = m_playerLst[countryid];
	CrossPlayer& player = playerLst[playerId];
	player.statisticKill += cnt;
	switch(countryid)
	{
	case 1:
		m_RankLst_1.OnValueChange(player);
		break;
	case 2:
		m_RankLst_2.OnValueChange(player);
		break;
	case 3:
		m_RankLst_3.OnValueChange(player);
		break;
	}
}

int CrossWarMgr::GetCrossKillRank(uint64 playerId, uint32 country)
{
	TaskPlayerList& playerLst = m_playerLst[country];
	CrossPlayer& player = playerLst[playerId];
	return player.rank;
}