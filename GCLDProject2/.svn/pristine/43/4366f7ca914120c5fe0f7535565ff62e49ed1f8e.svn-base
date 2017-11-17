#include "DefenceBorderland.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "../../session/PlayerPool.h"
#include "../Table/ActTableMgr.h"
#include "../../reward/reward.h"
#ifdef _MMO_SERVER_
#include "../../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "../../Map/country/country.h"
#include "../../Map/country/task/CountryTaskCity.h"
#include "../../Map/Table/MapCountry.h"
#include "datastruct/struct_country.h"
#include "../../Npc/NpcMgr.h"
#include "../../Npc/Npc.h"
#include "../../Map/city/city.h"
#include "Country.pb.h"
#include "../../Loot/LootList.h"
#include "Map/WorldMap.h"

#include "Service/ServiceMgr.h"
#include "Combat/CombatGroup.h"
#include "Combat.pb.h"
#include "character/CharacterStorage.h"
#include "character/Character.h"
#include "../ActMgr.h"
#include "../Personal/PersonalTask.h"

#define DEFENCE_BORDERLAND_SUB_TIME 10 * 60
#define DEFENCE_BORDERLAND_TIME	30 * 60

uint32 _Service_Delivering_Monsters(void* p)		//每10分钟刷新一次
{
	LLOG("_Service_Delivering_Monsters");
	DefenceBorderland* def = (DefenceBorderland*)p;
	def->_EnterNextSubStage();
	return DEFENCE_BORDERLAND_SUB_TIME * 1000;
}

DefenceBorderland::TaskPlayer::TaskPlayer()
:rank(0)
,isTakeReward(false)
{
    ZeroMemoryThis;
}
DefenceBorderland::DefenceBorderland()
    : m_RankLst_Country1(40)
	, m_RankLst_Country2(40)
	, m_RankLst_Country3(40)
	, m_substage(0)
{
    m_isBegined = false;
    m_curStage = 0;
    m_nextStageBeginTime = 0;
	m_nextSubStageTime = 0;
    ZeroMemoryArray(m_isCountryFailed);
	ZeroMemoryArray(m_canCountryTakeAward);
}
void DefenceBorderland::SaveTo(pb::GS2C_Country_Task_DefenceBorderland& msg)
{
    msg.set_is_begin(m_isBegined);
    msg.set_cur_stage(m_curStage);
    msg.set_cur_sub_stage(m_substage);
	msg.set_cur_stage_end_time(m_nextStageBeginTime);
	msg.set_cur_sub_stage_end_time(m_nextSubStageTime);

    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        msg.add_is_country_failed(m_isCountryFailed[i]);
    }

	if (const Table_Defence* table = sActTableMgr.GetDefence(m_curStage))
	{
		for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
		{
			const std::vector<int>& cityLst = table->m_TargetCity[i];
			for (std::vector<int>::const_iterator it = cityLst.begin(); it != cityLst.end(); ++it)
			{
				if(City* cityone = sWorldMap.GetCity(*it))
				{
					if(cityone->IsFighting())	//如果该城市在战斗(不在战斗中说明没有蛮族)
					{
						if(Combat::CombatGroup* Gcombat = cityone->GetCombatGroup())
						{
							int monsternum = Gcombat->GetMonsterInAttackLst();
							if(monsternum > 0)
							{
								msg.add_cityid_monster_attack(*it);
							}
						}
					}
				}
			}
		}
	}

}
void DefenceBorderland::SaveTo(pb::GS2C_Country_DefenceBorderland_Player& msg, Player& player)
{
	//int laststage = 0;
	uint32 killNum = 0;
	if(isPlayerInList(m_curStage,player))
	{
		TaskPlayer& pCData = GetPlayerData(m_curStage,player);
		killNum = pCData.statisticKill;
	}
	else
	{
		killNum = 0;
	}

	for(int stagenum = 1; stagenum <= m_curStage; stagenum++)	//从第一阶段到当前阶段
	{
		if(isPlayerInList(stagenum,player))		//如果这一阶段有玩家信息
		{
		/*	laststage = stagenum;*/
			TaskPlayer& pData = GetPlayerData(stagenum,player);
			if(!pData.isTakeReward)		//该玩家没有领奖
			{
				int ranktmp;
				if(pData.rank == 0)
					ranktmp = 999;
				else
					ranktmp = pData.rank;
				int rank = sCountryTableMgr.GetRankId(ranktmp);
				if(rank<0)
					rank = 0;
				msg.set_stageid(stagenum);
				msg.set_rankid(rank);
				msg.set_rank(pData.rank);
				msg.set_killnum(killNum);
				msg.set_is_take_reward(pData.isTakeReward);
				msg.set_cantakeexreward(m_canCountryTakeAward[player.GetCountryId()-1][stagenum]);
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
bool DefenceBorderland::Begin(int lastSecond)
{
	LLOG("DefenceBorderland::Begin");
    if (m_isBegined) return false;


    m_isBegined = true;
    m_curStage = 0;
    m_nextStageBeginTime = 0;
	m_substage = 0;
	m_nextSubStageTime = 0;
	m_playerLst.clear();
    ZeroMemoryArray(m_isCountryFailed);
	ZeroMemoryArray(m_canCountryTakeAward);

	m_RankLst_Country1.Clear();
	m_RankLst_Country2.Clear();
	m_RankLst_Country3.Clear();

	ServiceMgr::Register(SERVICE_DELIVE_MONSTER,this);

	return true;
}
void DefenceBorderland::End()
{
    m_isBegined = false;
	sPersonTask.End();
}

void DefenceBorderland::_EnterNextSubStage()
{
	LLOG("DefenceBorderland::_EnterNextSubStage");
	m_substage += 1;
	m_nextSubStageTime = sOS.TimeSeconds() + DEFENCE_BORDERLAND_SUB_TIME;	//当前时间+10分钟

	if(m_substage == 1 || m_substage == 4 || m_substage == 7 || m_substage == 10)		//当前阶段（0分，30分，60分，90分）进入下一个大阶段
	{
		_EnterNextStage(m_substage);
	}
	
	if(m_substage == 10)		//当前阶段（90分）结束
	{
		End();
		ServiceMgr::UnRegister(SERVICE_DELIVE_MONSTER,this);
	}
	
	LLOG("m_curStage=%d, m_substage=%d",m_curStage,m_substage);

	if (const Table_Defence* table = sActTableMgr.GetDefence(m_curStage))
	{
		LLOG("1");
		for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
		{
			LLOG("2");
			if (m_isCountryFailed[i]) continue; //任务失败不再刷蛮族了

			LLOG("3");
			Country* country = sWorldMap.GetCountry(i+1);

			int npcId = 0, npcLv = 0;
			GetMonsterInfo(*country, npcId, npcLv);
			LLOG("npcId=%d,npcLv=%d",npcId,npcLv);
			const std::vector<int>& cityLst = table->m_TargetCity[i];
			for (std::vector<int>::const_iterator it = cityLst.begin(); it != cityLst.end(); ++it)
			{
				LLOG("add monster cityid=%d, npcId=%d, npcLv=%d, num=%d",*it, npcId, npcLv, table->m_db.npc_num);
				CreatAttackMonster(*it, npcId, npcLv, table->m_db.npc_num,i+1,table->m_db.defence_num);	//添加蛮族
			}
		}
	}
}

void DefenceBorderland::_EnterNextStage(uint32 stagenum)
{
	LLOG("DefenceBorderland::_EnterNextStage");
    // 检查上一轮结果
    if (const Table_Defence* table = sActTableMgr.GetDefence(m_curStage)) {	//0,1,2,3
        for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i) 
		{
			bool isfail = false;
			const std::vector<int>& cityLst = table->m_TargetCity[i];
			for (std::vector<int>::const_iterator it = cityLst.begin(); it != cityLst.end(); ++it)
			{
				if(City* city = sWorldMap.GetCity(*it))
				{
					if(city->GetCountryId() != i+1)
						isfail = true;
				}
			}
			if(isfail && stagenum != 1 && !m_isCountryFailed[i])
			{
				LLOG("country%d is failed",i+1);
				m_isCountryFailed[i] = true;

				//TODO：发布蛮王令牌，失败方接“助蛮伐敌”任务
				City* desCity = GetMonsterTokenCity(i+1);
				if(desCity)
				{
					LLOG("destcityid=%d",desCity->id);
					uint64 lasttime = sOS.TimeSeconds();

					uint64 callendtime = lasttime + GET_BASE_DEF_INT(pb::BD_COUNTRY_DEFENSE_FRONTIER_DRAFT_LAST_TIME) * 60;
					uint64 uniquecallid = lasttime << 16 | desCity->id;

					desCity->AddMonsterCallCountry(i+1,callendtime,uniquecallid);
					desCity->SetStatus(City::MonsterToken,true);
					// 5min
#ifdef _MMO_SERVER_
					sAutoUpdateGame.AddEvent(
						boost::bind(&City::MonsterCallEndCallBack, desCity, uniquecallid),
						GET_BASE_DEF_INT(pb::BD_COUNTRY_DEFENSE_FRONTIER_DRAFT_LAST_TIME) * 60/* 5*60*/
						);
#endif
				}

				if(m_isCountryFailed[0] && m_isCountryFailed[1] && m_isCountryFailed[2])
					End();

				pb::GS2C_Country_Task_DefenceBorderland updatemsg;
				SaveTo(updatemsg);
				sWorldMap.Broadcast(pb::SMSG_COUNTRY_TASK_DEFENCEBORDERLAND,updatemsg);

			}

            if (m_isCountryFailed[i] == false)
            {
                sWorldMap.GetCountry(i+1)->AddCountryExp(table->m_db.reward_country_exp);
				m_canCountryTakeAward[i][m_curStage] = true;
				LLOG("country%d can take",i+1);
            }
			else
			{
				m_canCountryTakeAward[i][m_curStage] = false;
			}
        }

//#ifdef _MMO_SERVER_
//		sAutoUpdateGame.AddEvent(boost::bind(&DefenceBorderland::_TakePlayerAwardsOutOfTime, this, m_curStage), TAKE_AWARD_RESIST_TIME);
//#endif

    }
	
    m_curStage += 1;
    m_nextStageBeginTime = OS::TimeSeconds() + DEFENCE_BORDERLAND_TIME;

}

bool DefenceBorderland::TakeReward(uint32 stage, Player& player, pb::GS2C_Country_DefenceBorderland_Take_Reward& msg)
{
	msg.set_issuccess(false);
	if(m_curStage <= stage) 
	{
		msg.set_errret(pb::StageNotMatch);
		return false;		//申请的阶段必须小于当前阶段
	}

	if(isPlayerInList(stage,player))	//在目标阶段投资过
	{
		TaskPlayer& pData = GetPlayerData(stage,player);
		if (pData.isTakeReward) 
		{
			msg.set_errret(pb::rewardHasTaken);
			return false;	//已经领取
		}

		pData.isTakeReward = true;

		int ranktmp;
		if(pData.rank == 0)
			ranktmp = 999;
		else
			ranktmp = pData.rank;
		Country::TakeRankReward(player, LOOTTYPE_ACT_DEFENCE_EDGE, ranktmp);

		if (m_canCountryTakeAward[player.GetCountryId()][stage])
		{
			Country::TakeWinReward(player, LOOTTYPE_ACT_DEFENCE_EDGE);
		}
		msg.set_issuccess(true);
		return true;
    }
	else
		msg.set_errret(pb::NoRecord);
	return false;
}

void DefenceBorderland::_TakePlayerAwardsOutOfTime(uint32 stagenum)
{
	TaskPlayerList& playerlststage = m_playerLst[stagenum];
	for (TaskPlayerList::iterator it = playerlststage.begin(); it != playerlststage.end(); ++it)
	{
		if (!it->second.isTakeReward)
		{
			if (Player* palyer = sPlayerPool.GetByPlayerId(it->first))
			{
				pb::GS2C_Country_DefenceBorderland_Take_Reward msg;
				TakeReward(stagenum,*palyer,msg);
			}
		}
	}

}

void DefenceBorderland::TakeAwardsBeforeStart()
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
					pb::GS2C_Country_DefenceBorderland_Take_Reward msg;
					TakeReward(ite->first,*palyer,msg);
				}
			}
		}
	}

	m_playerLst.clear();
	//m_haveExReward = false;	//是否有额外奖励
	m_curStage = 0;			//当前任务阶段
	m_nextStageBeginTime = 0;//当前阶段开始时间
	m_substage = 0;
	m_nextSubStageTime = 0;
	ZeroMemoryArray(m_isCountryFailed);
	ZeroMemoryArray(m_canCountryTakeAward);

	m_RankLst_Country1.Clear();
	m_RankLst_Country2.Clear();
	m_RankLst_Country3.Clear();

}


void DefenceBorderland::AddKillCnt(uint64 playerId, uint32 cnt)
{
	if(m_isBegined)
	{
		if (Player* player = sPlayerPool.GetByPlayerId(playerId))
		{
			TaskPlayer& obj = GetPlayerData(m_curStage,*player);		//在这里记录玩家，如果没有则在m_playerLst中添加
			obj.statisticKill += cnt;
			switch(player->GetCountryId())
			{
			case 1:
				m_RankLst_Country1.OnValueChange(obj);
				break;
			case 2:
				m_RankLst_Country2.OnValueChange(obj);
				break;
			case 3:
				m_RankLst_Country3.OnValueChange(obj);
				break;
			}
			NLOG("obj.statisticKill=%d, obj.rank=%d",obj.statisticKill,obj.rank);
		}
	}
}

DefenceBorderland::TaskPlayer& DefenceBorderland::GetPlayerData(uint32 stage, Player& player)
{
	TaskPlayerList& playerlststage = m_playerLst[stage];
	std::map<uint64, TaskPlayer>::iterator it = playerlststage.find(player.GetGuid());
	if (it == playerlststage.end())
	{
		TaskPlayer& playerone = playerlststage[player.GetGuid()];
		playerone.rank = 0;
		playerone.statisticKill = 0;
		playerone.isTakeReward = false;

		sActMgr.AddPlayerInActivity(player.GetGuid());

		return playerone;
	}
	return it->second;
}
void DefenceBorderland::GetMonsterInfo(Country& country, int& npcId, int& npcLv)
{
    if (const MapCountryUp* table = sCountryTableMgr.GetCountryUp(country.GetLevel()))
    {
        npcId = table->m_db.defence_borderland_npc_id;
        npcLv = table->m_db.defence_borderland_npc_lv;
    }
}
void DefenceBorderland::CreatAttackMonster(int dstCityId, int npcId, int npcLv, int npcCnt, uint32 taskCountryid, uint32 defenceNum)
{
    if (City* pCity = sWorldMap.GetCity(dstCityId))
    {
		if(pCity->GetCountryId() != MONSTER_COUNTRY_ID)	//如果是蛮族的城池，则不添加蛮族攻击
		{
			for(int i=0; i<npcCnt; i++)
			{
				sNpcMgr.CreateNpc(npcId, npcLv, MONSTER_COUNTRY_ID, pCity);
			}
		}

		pCity->AddMonsterNpcsToDefence(defenceNum);//均添加防守npc
        pCity->AddCallBacK_DefenceFail(boost::bind(&DefenceBorderland::_OnFailed, this, taskCountryid, _1));
    }
}

void DefenceBorderland::_OnFailed(int countryId, City* pCity)
{
	LLOG("DefenceBorderland::_OnFailed");
	if(!m_isCountryFailed[countryId - 1] && m_isBegined)	//没有失败
	{
		if(pCity->GetCountryId() != countryId)
		{
			LLOG("_OnFailed country=%d",countryId);
			m_isCountryFailed[countryId - 1] = true;

			//TODO：发布蛮王令牌，失败方接“助蛮伐敌”任务
			City* desCity = GetMonsterTokenCity(countryId);
			if(desCity)
			{
				LLOG("destcityid=%d",desCity->id);
				uint64 lasttime = sOS.TimeSeconds();

				uint64 callendtime = lasttime + GET_BASE_DEF_INT(pb::BD_COUNTRY_DEFENSE_FRONTIER_DRAFT_LAST_TIME) * 60;
				uint64 uniquecallid = lasttime << 16 | desCity->id;

				desCity->AddMonsterCallCountry(countryId,callendtime,uniquecallid);
				desCity->SetStatus(City::MonsterToken,true);
				// 5*60
#ifdef _MMO_SERVER_
				sAutoUpdateGame.AddEvent(
					boost::bind(&City::MonsterCallEndCallBack, desCity, uniquecallid),
					GET_BASE_DEF_INT(pb::BD_COUNTRY_DEFENSE_FRONTIER_DRAFT_LAST_TIME) * 60
					);
#endif
			}

			if(m_isCountryFailed[0] && m_isCountryFailed[1] && m_isCountryFailed[2])
				End();

			pb::GS2C_Country_Task_DefenceBorderland updatemsg;
			SaveTo(updatemsg);
			sWorldMap.Broadcast(pb::SMSG_COUNTRY_TASK_DEFENCEBORDERLAND,updatemsg);

		}

	}
}

void DefenceBorderland::_ChechCityCallBackClear()
{
	if(!m_isBegined)
	{
		if (const Table_Defence* table = sActTableMgr.GetDefence(3))
		{
			for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
			{
				const std::vector<int>& cityLst = table->m_TargetCity[i];
				for (std::vector<int>::const_iterator it = cityLst.begin(); it != cityLst.end(); ++it)
				{
					if (City* pCity = sWorldMap.GetCity(*it))
					{
						pCity->ClearCallBack_DefenceFail();
					}
				}
			}
		}
	}
}

void DefenceBorderland::ClearPlayerData()
{
    //std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin();
    //for (; it != m_playerLst.end(); ++it)
    //{
    //    if (Player* palyer = sPlayerPool.GetByPlayerId(it->first)){
    //        TakeReward(*palyer, it->second.rank);
    //    }
    //}
    m_playerLst.clear();
	m_RankLst_Country1.Clear();
	m_RankLst_Country2.Clear();
	m_RankLst_Country3.Clear();
}

//玩家是否在当前阶段的队列中 check
bool DefenceBorderland::isPlayerInList(uint32 stage, Player& player)
{
	TaskPlayerList& playerlststage = m_playerLst[stage];
	std::map<uint64, TaskPlayer>::iterator it = playerlststage.find(player.GetGuid());
	if (it != playerlststage.end())
	{
		return true;
	}
	return false;
}

City* DefenceBorderland::GetMonsterTokenCity(uint32 countryid)
{
	std::vector<uint32> countryidlst;
	for(int i=1; i<4; i++)		//三个国家
	{
		if(i != countryid)
		{
			if(!m_isCountryFailed[i - 1])	//如果该国家防守未失败
				countryidlst.push_back(i);
		}
	}

	if(countryidlst.size() == 0)
		return NULL;

	City* desCity = NULL;
	int desMonsNum = 999;
	if(countryidlst.size() == 1)
	{
		if (const Table_Defence* table = sActTableMgr.GetDefence(m_curStage))
		{
			const std::vector<int>& cityLst = table->m_TargetCity[countryidlst.front()-1];
			for (std::vector<int>::const_iterator it = cityLst.begin(); it != cityLst.end(); ++it)
			{
				if (City* pCity = sWorldMap.GetCity(*it))
				{
					if(pCity->IsFighting())	//如果该城市在战斗(不在战斗中说明没有蛮族)
					{
						if(Combat::CombatGroup* Gcombat = pCity->GetCombatGroup())
						{
							int monsternum = Gcombat->GetMonsterInAttackLst();
							if(monsternum > 0)
							{
								if(monsternum<desMonsNum)
								{
									desMonsNum = monsternum;
									desCity = pCity;
								}
							}
						}
					}
				}
			}
			return desCity;
		}
	}

	if(countryidlst.size() == 2)
	{
		if (const Table_Defence* table = sActTableMgr.GetDefence(m_curStage))
		{
			uint32 countrydes;
			uint32 countrybackup;
			Country* counA = sWorldMap.GetCountry(countryidlst.front());
			Country* counB = sWorldMap.GetCountry(countryidlst.back());
			if(counA->GetLevel() > counB->GetLevel())	//A国家等级高
				countrydes = countryidlst.front();
			else if(counA->GetLevel() < counB->GetLevel())	//B国家等级高
				countrydes = countryidlst.back();
			else if(counA->m_exp > counB->m_exp)	//等级相同，A国家经验高
				countrydes = countryidlst.front();
			else if(counA->m_exp < counB->m_exp)	//等级相同，B国家经验高
				countrydes = countryidlst.back();
			else		//等级相同，经验也相同
				countrydes = sOS.Rand(1, 3)>1? countryidlst.back():countryidlst.front();	//随机一个目标国家

			countrybackup = countrydes==countryidlst.back()? countryidlst.front():countryidlst.back();	//备用国家

			const std::vector<int>& cityLst = table->m_TargetCity[countrydes-1];
			for (std::vector<int>::const_iterator it = cityLst.begin(); it != cityLst.end(); ++it)
			{
				if (City* pCity = sWorldMap.GetCity(*it))
				{
					if(pCity->IsFighting())	//如果该城市在战斗(不在战斗中说明没有蛮族)
					{
						if(Combat::CombatGroup* Gcombat = pCity->GetCombatGroup())
						{
							int monsternum = Gcombat->GetMonsterInAttackLst();
							if(monsternum > 0)
							{
								if(monsternum<desMonsNum)
								{
									desMonsNum = monsternum;
									desCity = pCity;
								}
							}
						}
					}
				}
			}

			if(!desCity)
			{
				desMonsNum = 999;
				const std::vector<int>& cityLstBackup = table->m_TargetCity[countrybackup-1];
				for (std::vector<int>::const_iterator it = cityLstBackup.begin(); it != cityLstBackup.end(); ++it)
				{
					if (City* pCity = sWorldMap.GetCity(*it))
					{
						if(pCity->IsFighting())	//如果该城市在战斗(不在战斗中说明没有蛮族)
						{
							if(Combat::CombatGroup* Gcombat = pCity->GetCombatGroup())
							{
								int monsternum = Gcombat->GetMonsterInAttackLst();
								if(monsternum > 0)
								{
									if(monsternum<desMonsNum)
									{
										desMonsNum = monsternum;
										desCity = pCity;
									}
								}
							}
						}
					}
				}
			}
			return desCity;
		}
	}

	return NULL;
}

void DefenceBorderland::MonsterCallRequest(Player& player, pb::C2GS_Combat_Call_Request& callmsg, uint16 destCityId,pb::GS2C_Combat_Call_Request& msg)
{
	LLOG("DefenceBorderland::MonsterCallRequest destCityId=%d",destCityId);

	City* pCity = sWorldMap.GetCity(destCityId);
	Country* country = sWorldMap.GetCountry(player.GetCountryId());
	if(pCity && country)
	{}
	else
	{
		msg.set_result(pb::OTHER_ERR);
		return;
	}

	for(std::vector<PlayerCallRec>::iterator it = country->m_player_monster_call_rec.begin();
		it != country->m_player_monster_call_rec.end(); it++)
	{
		if(it->playerid == player.GetGuid() && pCity->IsMonsterTokenInLst(it->uniqueid))	//已经使用过的ID在列表
		{
			msg.set_result(pb::CALL_DOWN);
			return;
		}
	}

	int herosize = callmsg.hero_id_size();
	uint32 heroid;
	bool hasHeroSuc = false;

	uint64 callupuniqueid = pCity->GetMonsterCallUniqueid(player.GetCountryId());

	for(int i=0; i<herosize; i++)
	{
		heroid = callmsg.hero_id(i);
		if(Character* pHero = player.m_characterStorage->MutableCharacter(heroid))
		{
			if(pHero->CurHp() != 0)	//空血不许征召
			{
				if (City* pCity = sWorldMap.GetCity(destCityId))
				{
					if(pCity->HaveStatus(City::MonsterToken) && callupuniqueid)	//当前城市有征召状态并且有该国家的征召
					{
						if(pHero->GetCurCity() != (uint32)destCityId)	//武将不在目标城池
						{
							if(!pHero->isInCombat)	//武将不在战斗队列中
							{
								pHero->_ExitCity(*sWorldMap.GetCity(pHero->GetCurCity()));
								pHero->_EnterCity(*pCity);  
								//if(pHero->FlyTo(destCityId))
								//{
								msg.set_result(pb::SUCCESS);
								msg.set_call_up_down_id(callupuniqueid);
								pHero->initPath();

								//}
								//else
								//	msg.set_result(pb::REQUEST_LEGAL);

								//TODO:给武将奖励
								hasHeroSuc = true;
								sReward.Change(player, Reward::Food, GET_BASE_DEF_INT(pb::BD_ACCEPT_DRAFT_REWARD_FOOD));
								player.AddExploitValue(GET_BASE_DEF_INT(pb::BD_ACCEPT_DRAFT_REWARD_EXPLOIT));
							}
							else
								msg.set_result(pb::HERO_BUSY);
						}
						else
							msg.set_result(pb::HERO_IN_CITY);
					}
					else
						msg.set_result(pb::CITY_STATE_ERR);
				}
				else
					msg.set_result(pb::CITY_NOT_FOUND1);
			}
			else
				msg.set_result(pb::HERO_ERR);
		}
		else
			msg.set_result(pb::HERO_ERR);
	}

	if(hasHeroSuc)
	{
		msg.set_result(pb::SUCCESS);
		PlayerCallRec recone;
		recone.playerid = player.GetGuid();
		recone.uniqueid = callupuniqueid;
		country->m_player_monster_call_rec.push_back(recone);
	}

	return;
}