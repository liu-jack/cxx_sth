#include "country.h"
#include "OS.h"
#include "../../Npc/NpcMgr.h"
#include "../../Npc/Npc.h"
#include "../Table/MapTableMgr.h"
#include "../../Service/ServiceMgr.h"
#include "../Table/MapCountry.h"
#include "datastruct/struct_country.h"
#include "../../object/Player.h"
#include "../PlayerMapLogic.h"
#include "../../reward/reward.h"
#include "../../CrossLogic/CountyDB.h"
//#include "Country.pb.h"
#include "../city/city.h"
#include "Base.h"
#include "../../Palace/PalaceMgr.h"
#ifdef _MMO_SERVER_
#include "../../AutomaticUpdate/AutoUpdateGame.h"
#include "../../GameServer.h"
#include "ActivityControl/ActivityMgr.h"
#endif
#include "../../session/PlayerPool.h"
#include "task/CountryTaskCity.h"
#include "../../Loot/LootManager.h"
#include "../../Loot/LootList.h"
#include "Opcode.pb.h"
#include "../../Activity/Table/ActTableMgr.h"
#include "../WorldMap.h"
#include "../../CrossLogic/PlayerMapLogicDB.h"
#include "../../../net_framework/ServerClient.h"
//#include "../../AutomaticUpdate/AutomaticUpdate.h"
#include "../../Technology/PlayerTechnology.h"
#include "../../Technology/Table/TechnologyTableMgr.h"
#include "World.pb.h"
#include "Country.pb.h"
#include "../../ActivityCelebrity/Table/ActivityCelebrityTableMgr.h"
#include "System.h"
#include "ActivityControl/ActivityMgr.h"
#include "Activity.pb.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossWorldMap.h"
#include "CrossWar.pb.h"
#include "object/Player.h"
#include "cross_wall/CrossWarManager/CrossWarMgr.h"
#endif

static const int Win_Reward_Id = 9999; //见loot表配置

uint32 _Service_Country_AttackNpc(void* p)
{
    Country* pCountry = (Country*)p;

	pCountry->_RefreshAttackNpc(sOS.TimeSeconds()); //		半小时刷一次

    return GET_BASE_DEF_INT( pb::BD_EXPEDITION_TROOPS_REFRESH_TIME) * 60 * 1000;
}

uint32 _Service_Country_JinWeiNpc(void* p)
{
	Country* pCountry = (Country*)p;

	pCountry->_RefreshJinWeiNpc(sOS.TimeSeconds()); //		一小时刷一次

	return GET_BASE_DEF_INT( pb::BD_LIFEGUARD_TROOPS_REFRESH_TIME) * 60 * 1000;
}
#ifdef _MMO_SERVER_
uint32 _Service_Country_Celebrity_Npc(void *p)
{
	Country* pCountry = (Country*)p;
	if(sActivityMgr.IsActivityActive(pb::ACTIVITY_MAKE_FRIENDS))
	{
		NLOG("activity_make_friends start,and npc freshed!");
		pCountry->_RefreshCelebrityNpc(sOS.TimeSeconds()); //		半小时
		return sActivityCelebrityMgr.GetNpcFreshTime(pCountry->id);
	}
	else
	{
		return 1000;///one second
	}
}
#endif
Country::TaskPlayer::TaskPlayer()
{
	ZeroMemoryThis;
}

CrossPlayer::CrossPlayer()
{
    ZeroMemoryThis;
}
Country::Country(uint8 id_)
    : id(id_)
    , m_RankLst(40)
	, m_Prosperity_Degree(2000)
	, m_CrossRank(40)
	, m_pSession(NULL)
{
    //memset(&m_exp, 0, sizeof(*this) - MMO_OFFSET(Country, m_exp));
	m_exp = 0;
	m_level = 1;
	m_donateTimes = 0;  //捐赠次数

	m_npclevelid = 1;
	m_countryNpcLv = sCountryTableMgr.GetGuardIniLv();;
	m_countryNpcExp = 0;
	m_lastCheckNpcExp = 0;

    m_OpenLevelUpTaskTime = 0;
    m_CurLevelDoTaskCnt = 0;
    m_KillMonsterNum = 0;
    m_isMonsterTaskOpen = false;
	m_isMonsterTaskSucceed = false;
	isOpenedCrossWar = false;//是否已开启跨服战
	cross_war_start_time = 0;//跨服战开时间
	cross_anabasis_start_time = 0;//远征开始时间
	isCrossAnabasisSide = false;//远征方,false为被远征方
	isOpenedAnabasis = false;

	crossServerIp = "192.168.2.169";
	crossServerPort = "55400";
	cross_state = pb::State_Null;

	anabasiServerIp = "192.168.2.169";
	anabasiServerPort = "55400";
	anabasis_state = pb::State_Null;

	cross_uniqueID = 0;
	m_isHasShield = false;
	m_shieldEndTime = 0;

	if(id != MONSTER_COUNTRY_ID)
	{
		m_capitalCityId = sMapTableMgr.GetCapitalCity(id);//首都
		switch (id) {
		case pb::COUNTRY_WORM: m_monsterCityId = GET_BASE_DEF_UINT(pb::BD_WILD_NEAR_ZERG);  break;
		case pb::COUNTRY_MAN: m_monsterCityId = GET_BASE_DEF_UINT(pb::BD_WILD_NEAR_TERRAN);  break;
		case pb::COUNTRY_GOD: m_monsterCityId = GET_BASE_DEF_UINT(pb::BD_WILD_NEAR_PROTOSS); break;
		default: break;
			}
	}

    m_AttackNpc_1_RefreshTime = 0;
    m_AttackNpc_2_RefreshTime = 0;
	m_npc_refresh_time = 0;
	m_tuntianLevel = 1;
	m_tuntianExp = 0;
	m_player_call_rec.clear();
	m_npc_expedirion.clear();
	m_crossStarRec.clear();
	m_CrossPlayerLst.clear();

	OriGameCountryId = 0;
	OriGameServerId = 0;
}
Country::~Country()
{
#ifdef _MMO_SERVER_
    ServiceMgr::UnRegister(SERVICE_COUNTRY_ATTACK_NPC, this);
	ServiceMgr::UnRegister(SERVICE_COUNTRY_GUARD, this);
	ServiceMgr::UnRegister(SERVICE_COUNTRY_CELEBRITY_NPC, this);
#endif
}
void Country::Init()
{

}
void Country::SaveTo(pb::CountyInfo& msgData)
{
    msgData.set_id(id);
    msgData.set_exp(m_exp);
    msgData.set_level(m_level);
    msgData.set_donate_times(m_donateTimes);
	m_countryNpcLv = sCountryTableMgr.GetNpcLvByLvID(m_npclevelid);
    msgData.set_country_npc_level(m_countryNpcLv);
    msgData.set_country_npc_exp(m_countryNpcExp);
    msgData.set_have_city_cnt(m_setCity.size());
    // 升级任务 蛮族试练
    msgData.set_open_level_up_task_time(m_OpenLevelUpTaskTime);
    msgData.set_cur_level_do_task_cnt(m_CurLevelDoTaskCnt);
    msgData.set_kill_monster_num(m_KillMonsterNum);
    msgData.set_is_monster_task_open(m_isMonsterTaskOpen);
    msgData.set_is_monster_task_succeed(m_isMonsterTaskSucceed);

	SaveCrossWarInfo(*msgData.mutable_cross_war_info());
    // 蛮族：捐赠、发动入侵
    m_MonsterCity.SaveTo(*msgData.mutable_monster_city_lst());///
	msgData.set_last_check_npc_exp(m_lastCheckNpcExp);
	msgData.set_npc_level_id(m_npclevelid);
	msgData.set_tuntian_level(m_tuntianLevel);
	msgData.set_tuntian_exp(m_tuntianExp);
	msgData.set_prosperity_degree(m_Prosperity_Degree);
}

void Country::SaveCrossWarInfo(pb::CrossWarInfo &info )
{
	//info.set_is_open_cross_war(isOpenedCrossWar);
	//info.set_cross_war_start_time(cross_war_start_time);
	//info.set_cross_server_ip(crossServerIp);
	//TODO
	//info.set_cross_server_ip("192.168.2.169");
	//info.set_cross_server_port(crossServerPort);
	//info.set_is_open_anabasis(isOpenedAnabasis);
	//info.set_is_anabasis(isCrossAnabasisSide);
	//info.set_anabasis_start_time(cross_anabasis_start_time);
	//info.set_anabasis_server_port(anabasiServerPort);
	//info.set_anabasis_server_ip(anabasiServerIp);
	//info.set_cross_state(cross_state);
	//info.set_anabasis_state(anabasis_state);
}

void Country::UpdateCrossWarInfo(const pb::CrossWarInfo &info )
{
	//isCrossAnabasisSide = info.is_anabasis();
	//isOpenedCrossWar = info.is_open_cross_war();
	//isOpenedAnabasis = info.is_open_anabasis();
	//crossServerIp = info.cross_server_ip();
	//TODO
	//crossServerIp = "192.168.2.169";
	//crossServerPort = info.cross_server_port();
	//cross_war_start_time = info.cross_war_start_time();
	//cross_anabasis_start_time = info.anabasis_start_time();
	//anabasiServerIp = info.anabasis_server_ip();
	//anabasiServerPort = info.anabasis_server_port();
	//cross_state = info.cross_state();
	//anabasis_state = info.anabasis_state();
	
}

uint32 Country::RandOneCityForCelebrity()
{
	uint32 capacityId = sMapTableMgr.GetCapitalCity(id);
	const MapCity* table = sMapTableMgr.GetMapCity(capacityId);
	if(m_setCity.size() == 2)///只有屯田区和首都
	{
		if(table)
		{
			return table->m_linkCity[System::Rand(0,table->m_linkCity.size())];
		}
		else
		{
			return 0;
		}
	}
	else
	{
		std::vector<int> tmp_vec;
		std::vector<int> tmp_vec2;
		for(std::set<uint16>::iterator iter = m_setCity.begin();iter != m_setCity.end();++iter)
		{
			if(City* city = sWorldMap.GetCity(*iter))
			{
				if(city->IsCapitalCity() || city->IsTonden())
					continue;
				if(city->IsFighting())
				{
					tmp_vec.push_back(*iter);
				}
				tmp_vec2.push_back(*iter);
			}
		}
		if(tmp_vec.empty() && tmp_vec2.empty())
			return 0;
		if(tmp_vec.empty())
		{
			return tmp_vec2[System::Rand(0,tmp_vec2.size())];
		}
		else
		{
			return tmp_vec[System::Rand(0,tmp_vec.size())];
		}
	}
}

pb::CROSS_STATE Country::GetCrossState()
{
  return cross_state;
}

void Country::SetCrossState( pb::CROSS_STATE state )
{
	if(cross_state != state)
	{
		cross_state = state;
	}
}

void Country::SetCrossUniqueID(uint32 uniqueid)
{
	cross_uniqueID = uniqueid;
}

bool Country::IsCanCrossWarState()
{
	//TODO
	//cross_state = pb::CROSS_STATE_COMBAT;
	//return true;
	LLOG("[1002] check is can cross war, cross_state=%d",cross_state);
	bool curIsCross = false;
	bool curIsanabasis = false;
	if (cross_state == pb::State_Prepare || cross_state == pb::State_Start || cross_state == pb::State_Preview || cross_state == pb::State_Award)
	{
		curIsCross = true;
	}
	if (anabasis_state == pb::State_Prepare || anabasis_state == pb::State_Start || anabasis_state == pb::State_Preview || cross_state == pb::State_Award)
	{
		curIsanabasis = true;
	}
	//ASSERT((curIsCross && curIsanabasis) == false);
	return curIsanabasis || curIsCross;
}

void Country::UpdateCurCrossServer()
{
#ifdef  _MMO_SERVER_
	if (IsCanCrossWarState())
	{
		if (cross_state == pb::State_Prepare || cross_state == pb::State_Start || cross_state == pb::State_Preview)
		{
			if (m_curCrossServerPtr.get())
			{
				if(!m_curCrossServerPtr->IsCurClient(crossServerIp,crossServerPort))
				{
					m_curCrossServerPtr = sGameSvr.GetCrossServerClient(crossServerIp,crossServerPort);
				}
			}
			else
			{
				m_curCrossServerPtr = sGameSvr.GetCrossServerClient(crossServerIp,crossServerPort);
			}
		} 
		else if (anabasis_state == pb::State_Prepare || anabasis_state == pb::State_Start || anabasis_state == pb::State_Preview)
		{
			if (m_curCrossServerPtr.get())
			{
				if(!m_curCrossServerPtr->IsCurClient(anabasiServerIp,anabasiServerPort))
				{
					m_curCrossServerPtr = sGameSvr.GetCrossServerClient(anabasiServerIp,anabasiServerPort);
				}
			}
			else
			{
				m_curCrossServerPtr = sGameSvr.GetCrossServerClient(crossServerIp,crossServerPort);
			}
		}
	}
	else
	{
		m_curCrossServerPtr.reset();
	}
#endif
}

pb::CROSS_STATE Country::GetAnabasisState()
{
	return anabasis_state;
}

void Country::SetAnabasisState( pb::CROSS_STATE state )
{
	if (anabasis_state != state)
	{
		anabasis_state = state;
	}
}

bool Country::SendCrossServerNetPack( NetPack &pack )
{
#ifdef  _MMO_SERVER_
	UpdateCurCrossServer();
	if (m_curCrossServerPtr.get())
	{
		return m_curCrossServerPtr->Send(pack,PACKET_TYPE_CLIENT_CROSS);
	}
#endif
	return false;
}

bool Country::SendCrossServerMsg(const int opCode, const ::google::protobuf::Message &msg,char clientType,uint64 sessionId)
{
#ifdef  _MMO_SERVER_
	LLOG("[1002] SendCrossServerMsg");
	UpdateCurCrossServer();
	if (m_curCrossServerPtr.get())
	{
		LLOG("[1002] get ptr");
		return m_curCrossServerPtr->Send(opCode,sessionId,clientType,msg);
	}
#endif
	return false;
}

void Country::LoadFrom(const pb::CountyInfo& msgData)
{
    m_exp = msgData.exp();
    m_level = msgData.level();
    m_donateTimes = msgData.donate_times();

	m_npclevelid = msgData.npc_level_id();
	m_countryNpcLv = sCountryTableMgr.GetNpcLvByLvID(m_npclevelid);
    //m_countryNpcLv = msgData.country_npc_level();
    m_countryNpcExp = msgData.country_npc_exp();
	m_lastCheckNpcExp = msgData.last_check_npc_exp();

	m_tuntianLevel = msgData.tuntian_level();
	m_tuntianExp = msgData.tuntian_exp();

    // 升级任务 蛮族试练
    m_OpenLevelUpTaskTime = msgData.open_level_up_task_time();
    m_CurLevelDoTaskCnt = msgData.cur_level_do_task_cnt();
    m_KillMonsterNum = msgData.kill_monster_num();
    m_isMonsterTaskOpen = msgData.is_monster_task_open();
    m_isMonsterTaskSucceed = msgData.is_monster_task_succeed();
	UpdateCrossWarInfo(msgData.cross_war_info());
    // 蛮族：捐赠、发动入侵
    m_MonsterCity.LoadForm(msgData.monster_city_lst());

    if (m_isMonsterTaskOpen) {
		// 开放对应蛮族城池
		if (City* city = sWorldMap.GetCity(m_monsterCityId))
		{
			city->SetStatus(City::Open_Monster_City, true);

			const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;
			city->AddDefenderNpc(table.savage_id, table.savage_lv, table.savage_num);
		}
        // 重设结束定时器
        const uint64 timeNow = sOS.TimeSeconds();
#ifdef _MMO_SERVER_
        if (timeNow < m_OpenLevelUpTaskTime + 4 * 3600) {
            int interval = (int)(m_OpenLevelUpTaskTime + 4 * 3600 - timeNow);
            sAutoUpdateGame.AddEvent(
                boost::bind(&Country::_CheckMonsterTaskIsSucceed, this),
                interval
                );
        }else{
            _CheckMonsterTaskIsSucceed();
        }
#endif
    }

	if(m_lastCheckNpcExp == 0)	//上次检测npc的时间为0 （第一次检测，置当前时间，存入数据库）
	{
		m_lastCheckNpcExp = sOS.TimeSeconds();
		CountryDB::SendBaseToDb(*this);
	}
//ASSERT(0);
	while(!sPeriodEvent.IsInTodayTimePeriod(m_lastCheckNpcExp,86400))
	{

		uint64 DaySecond = 86400;
		//NLOG("before check time = %lld,exp=%d",m_lastCheckNpcExp,m_countryNpcExp);
		m_lastCheckNpcExp += (std::min)((sOS.TimeSeconds()-m_lastCheckNpcExp),DaySecond);

		m_countryNpcExp += GET_BASE_DEF_INT( pb::BD_COUNTRY_GUARD_GET_EXP_EVERYDAY);
		//NLOG("after check time = %lld,exp=%d",m_lastCheckNpcExp,m_countryNpcExp);

		//NLOG("level up need exp = %d",sCountryTableMgr.GetGuardUp((int)m_npclevelid)->GetNeedExp());

		if ((int)m_countryNpcExp >= sCountryTableMgr.GetGuardUp((int)m_npclevelid)->GetNeedExp())
		{
			//ASSERT(0);
			if (sCountryTableMgr.GuardNpcUpLv(m_countryNpcLv, m_npclevelid))
			{
				m_countryNpcExp = 0;
			}
		}

		//CountryDB::SendBaseToDb(*this);
	}

	m_Prosperity_Degree = msgData.prosperity_degree();
	if(m_Prosperity_Degree == 0)
		m_Prosperity_Degree = 2000;

	//sAutoUpdate.Add(boost::bind(&Country::updateNpcExpPerDay,this),detail_enum::TIME_ZERO,detail_enum::PER_DAY);
}

void Country::updateNpcExpPerDay()
{
	//Second DaySecond = 86400;
	//while(!sPeriodEvent.IsInTodayTimePeriod(m_lastCheckNpcExp,DaySecond))
	//{
		//m_lastCheckNpcExp += min((sOS.TimeSeconds()-m_lastCheckNpcExp),(uint64)DaySecond);
	m_lastCheckNpcExp = sOS.TimeSeconds();
	m_countryNpcExp += GET_BASE_DEF_INT( pb::BD_COUNTRY_GUARD_GET_EXP_EVERYDAY);
	if ((int)m_countryNpcExp >= sCountryTableMgr.GetGuardUp((int)m_npclevelid)->GetNeedExp())
	{
		if (sCountryTableMgr.GuardNpcUpLv(m_countryNpcLv,m_npclevelid))
		{
			m_countryNpcExp = 0;
		}
	}
	CountryDB::SendBaseToDb(*this);
	//}
}


void Country::SaveTo(pb::Country_Levle_Up_Task_Rank& msgData)
{
    for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it){
        TaskPlayer& obj = it->second;
        pb::CountyLevelUpTaskPlayer* pData = msgData.add_player_lst();
        pData->set_playerid(it->first);
        pData->set_country(id);
        pData->set_rank(obj.rank);
        pData->set_statistic_kill(obj.statisticKill);
        pData->set_is_take_reward(obj.isTakeReward);
    }
    msgData.set_country(id);
}
void Country::LoadFrom(const pb::Country_Levle_Up_Task_Rank& msgData)
{
    for (int i = 0; i < msgData.player_lst_size(); ++i) {
        const pb::CountyLevelUpTaskPlayer& data = msgData.player_lst(i);
        TaskPlayer& obj = m_playerLst[data.playerid()];
        obj.rank = data.rank();
        obj.statisticKill = data.statistic_kill();
        obj.isTakeReward = data.is_take_reward();
    }

    // 重建排行榜
    for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
    {
        m_RankLst.OnValueChange(it->second);
    }
}
void Country::SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player)
{
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
    if (it == m_playerLst.end()) return;

    TaskPlayer& data = it->second;

    msg.set_rank(data.rank);
    msg.set_kill_count(data.statisticKill);
    msg.add_is_take_reward(data.isTakeReward);
    Country::GetRankReward(player, LOOTTYPE_COUNTRYQUEST, data.rank).SaveTo(*msg.add_rank_reward());
}
void Country::OnCityBeOccupied(City& city, uint8 enemyCountryId)
{
    m_setCity.erase(city.id);
    CountryDB::SendDelCityToDb(id, city);
}
void Country::OnOccupyCity(City& city, uint8 destCountryId)
{
    m_setCity.insert(city.id);
    CountryDB::SendNewCityToDb(id, city);
}
bool Country::LeveUp()
{
    const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;		//country_up表

    if (m_exp >= table.exp && m_donateTimes >= table.need_donate_times && m_isMonsterTaskSucceed)
    {
        if (sCountryTableMgr.CountryUpLv(m_level))
        {
            m_exp = 0;
            m_donateTimes = 0;
            m_isMonsterTaskSucceed = false;
            CountryDB::SendBaseToDb(*this);
            return true;
        }
    }
    return false;
}
void Country::AddCountryExp(uint exp)
{
    m_exp += exp;

	//const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;		//country_up表
	uint32 needexp = sCountryTableMgr.GetCountryUp(m_level)->m_db.exp;
	while(m_exp >= needexp)
	{
		if (sCountryTableMgr.CountryUpLv(m_level))
		{
			m_exp = m_exp - needexp;
			m_donateTimes = 0;
			m_isMonsterTaskSucceed = false;
			CountryDB::SendBaseToDb(*this);
			needexp = sCountryTableMgr.GetCountryUp(m_level)->m_db.exp;
		}
		else
			break;
	}
	//if (m_exp >= table.exp)
	//{
	//	if (sCountryTableMgr.CountryUpLv(m_level))
	//	{
	//		m_exp = m_exp - table.exp;
	//		m_donateTimes = 0;
	//		m_isMonsterTaskSucceed = false;
	//		CountryDB::SendBaseToDb(*this);
	//	}
	//}

    CountryDB::SendBaseToDb(*this);
}
void Country::AddNpcExp(uint exp)
{
    m_countryNpcExp += exp;

	while((int)m_countryNpcExp >= sCountryTableMgr.GetGuardUp((int)m_npclevelid)->GetNeedExp())
	{
		int needexp = sCountryTableMgr.GetGuardUp((int)m_npclevelid)->GetNeedExp();
		if(sCountryTableMgr.GuardNpcUpLv(m_countryNpcLv,m_npclevelid))
			m_countryNpcExp -= needexp;
	}
    //if ((int)m_countryNpcExp >= sCountryTableMgr.GetGuardUp((int)m_npclevelid)->GetNeedExp())
    //{
    //    if (sCountryTableMgr.GuardNpcUpLv(m_countryNpcLv,m_npclevelid))
    //    {
    //        m_countryNpcExp = 0;
    //    }
    //}
    CountryDB::SendBaseToDb(*this);
}
void Country::Donate(Player& player)
{
    const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;

    const uint64 timeNow = sOS.TimeSeconds();
    if (timeNow - player.m_worldMapLogic->m_countryDonateTime >= table.donate_cd)
    {
        if (sReward.Change(player, Reward::Coin_Silver, -(int)table.donate_cost))
        {
            player.m_worldMapLogic->m_countryDonateTime = timeNow;
            ++m_donateTimes;
            CountryDB::SendBaseToDb(*this);
            PlayerMapLogicDB::SendBaseToDb(player, *player.m_worldMapLogic.get());
        }
    }
}
void Country::ClearDonateCD(Player& player)
{
    const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;

    if (sReward.Change(player, Reward::Coin_Gold, -(int)table.clear_donate_cd_cost))
    {
        player.m_worldMapLogic->m_countryDonateTime = 0;
        CountryDB::SendBaseToDb(*this);
        PlayerMapLogicDB::SendBaseToDb(player, *player.m_worldMapLogic.get());
    }
}
void Country::_RefreshAttackNpc(uint64 timeNowSec)
{
#ifdef _MMO_SERVER_
	//LLOG("Country::_RefreshAttackNpc");
	//LLOG("timeNowSec=%d,m_AttackNpc_1_RefreshTime=%llu",timeNowSec,m_AttackNpc_1_RefreshTime);

	uint64 time_opentime = (uint64)sActivityMgr.GetOpenTimeById(sActivityMgr.GetCurServerId());
	if(time_opentime == 0) return;

	if(((uint64)sActivityMgr.GetOpenTimeById(sActivityMgr.GetCurServerId()) + GET_BASE_DEF_INT(pb::BD_EXPEDITION_TROOPS_START_TIME) * 3600) > sOS.TimeSeconds())	return;

    if (timeNowSec >= m_AttackNpc_1_RefreshTime)
    {
		//LLOG("timeNowSec");
        m_AttackNpc_1_RefreshTime = timeNowSec + GET_BASE_DEF_INT( pb::BD_EXPEDITION_TROOPS_REFRESH_TIME) * 60;

		pb::GS2C_Notice_Country_NPC_NextTime sendmsg;
		sendmsg.set_npctype(1);
		sendmsg.set_nexttime(m_AttackNpc_1_RefreshTime);
		sWorldMap.BroadcastInCountry(pb::SMSG_NOTICE_NPC_NEXT_TIME,sendmsg,id);
		//LLOG("_RefreshAttackNpc time = %llu",m_AttackNpc_1_RefreshTime);

		std::vector<MapExpedition*> expmap = sMapTableMgr.GetExpedition(id);
		
		//LLOG("expmap.size=%d",expmap.size());
		for(std::vector<MapExpedition*>::iterator it = expmap.begin();
			 it != expmap.end(); it++)
		{
			MapExpedition* oneExpe = (*it);
			int npcnum = oneExpe->GetNpcNum();
			uint32 expeid = sWorldMap.CreateExpeditionID(oneExpe->GetNpcID());
			//LLOG("countryid = %d, npcnum = %d",id,npcnum);
			for(int i=0; i<npcnum; i++)
			{
				if (Npc* pNpc = sNpcMgr.CreateNpc(oneExpe->GetNpcID(), sCountryTableMgr.GetExpeditionLvByLvID(m_npclevelid), id, sWorldMap.GetCity(m_capitalCityId)))	//远征军在主城创建
				{
					if(oneExpe->GetCitys().size()>0)
					{
						pNpc->isExpedition = true;
						pNpc->expeid = expeid;
						pNpc->SetMovePath(oneExpe->GetCitys());
						m_npc_expedirion[expeid].push_back(pNpc->GetUniqueId());
						//LLOG("create one npc! npcid=%d,level=%d",oneExpe->GetNpcID(),sCountryTableMgr.GetExpeditionLvByLvID(m_npclevelid));
					}
				}
			}
			pb::GS2C_Expedition_Army_Info sendmsg;
			sendmsg.set_expedition_id(expeid);
			sendmsg.set_city_id(m_capitalCityId);
			sendmsg.set_hero_id(oneExpe->GetNpcID());
			sWorldMap.BroadcastInCountry(pb::SMSG_NPC_POS_LIST,sendmsg,id);
			sWorldMap.setExpeditionMoveTo(expeid,m_capitalCityId);
		}
    }
#endif
}

void Country::_RefreshJinWeiNpc(uint64 timeNowSec)
{
	//LLOG("Country::_RefreshJinWeiNpc");
	//LLOG("timeNowSec=%lld,m_AttackNpc_2_RefreshTime=%llu",timeNowSec,m_AttackNpc_2_RefreshTime);
	if (timeNowSec >= m_AttackNpc_2_RefreshTime)
	{
		m_AttackNpc_2_RefreshTime = timeNowSec + GET_BASE_DEF_INT( pb::BD_LIFEGUARD_TROOPS_REFRESH_TIME) * 60;
		
		pb::GS2C_Notice_Country_NPC_NextTime sendmsg;
		sendmsg.set_npctype(2);
		sendmsg.set_nexttime(m_AttackNpc_2_RefreshTime);
		sWorldMap.BroadcastInCountry(pb::SMSG_NOTICE_NPC_NEXT_TIME,sendmsg,id);
		//LLOG("_RefreshJinWeiNpc time = %llu",m_AttackNpc_2_RefreshTime);

		std::vector<MapExpedition*> expmap = sMapTableMgr.GetGuard(id);

		for(std::vector<MapExpedition*>::iterator it = expmap.begin();
			it != expmap.end(); it++)
		{
			MapExpedition* oneExpe = (*it);
			int npcnum = oneExpe->GetNpcNum();
			//LLOG("countryid = %d, npcnum = %d",id,npcnum);
			const std::vector<int>& citys = oneExpe->GetCitys(); 
			for(std::vector<int>::const_iterator ite = citys.begin();
					ite != citys.end(); ite++)
			{
				if (City* city = sWorldMap.GetCity(*ite))
				{
					//LLOG("cityid = %d, countryid = %d, mycountryid = %d",city->id,city->GetCountryId(),id);
					if(city->GetCountryId() != id || city->IsFighting())
					{
						for(int i=0; i<npcnum; i++)
						{
							if (Npc* pNpc = sNpcMgr.CreateNpc(oneExpe->GetNpcID(), sCountryTableMgr.GetLifeGuardLvByLvID(m_npclevelid), id, city))	//远征军在主城创建
							{
								pNpc->isLifeGuard = true;
								//LLOG("create one npc! npcid=%d,level=%d",oneExpe->GetNpcID(),sCountryTableMgr.GetExpeditionLvByLvID(m_npclevelid));
							}
						}
					}
				}
			}
		}
	}
}

void Country::_RefreshCelebrityNpc(uint64 timeNow)
{
#ifdef _MMO_SERVER_
	if(timeNow >= m_npc_refresh_time)
	{
		m_npc_refresh_time = timeNow + sActivityCelebrityMgr.GetNpcFreshTime(id) / 1000;
		uint32 npc_country_id = 0;
		IntPair p;
		sActivityCelebrityMgr.GetNpc(id,npc_country_id,p);
		uint32 cityId = RandOneCityForCelebrity();
		if(cityId == 0)
		{
			NLOG("Oops!!!!! city id is ZERO,the celebrity fresh time is out date!!!!!!!!!!!!!!");
			return;
		}
		Npc* pNpc = NULL;
		City* city = sWorldMap.GetCity(cityId);
		if(city)
		{
			for(int i = 0;i < p.second;++i)
			{
				pNpc = sNpcMgr.CreateNpc(p.first,1,npc_country_id,city);///创建1级的npc
			}
		}
		sActivityCelebrityMgr.FillInfomation(id,cityId,m_npc_refresh_time,pNpc);
		NLOG("city Id = %u,m_npc_refresh_time = %llu",cityId,m_npc_refresh_time);
	}
#endif
}

bool Country::HaveSpecialCity(MapCity::SpecialCity e)
{
    uint32 cityId = MapCity::GetSpacialCity(e);

    return m_setCity.find(cityId) != m_setCity.end();
}

bool Country::IsOpenAnabasis()
{
	return isOpenedAnabasis;
}

bool Country::IsOpenCrossWar()
{
	return isOpenedCrossWar;
}

uint32 Country::GetCityCount()
{
   return (uint32)m_setCity.size();
}

bool Country::OpenMonsterTask(Player& player)
{
    //TODO：国家任务期间无法开启
    
    if (OS::TimeHour() < 8) return false; // 0-8点无法开启

    const int curOfficeId = sPalaceMgr(id)->GetOfficeId(player.GetGuid());

    if (curOfficeId != 1 && curOfficeId != 2) return false;

    const uint64 timeNow = sOS.TimeSeconds();
    if (timeNow < m_OpenLevelUpTaskTime + 24 * 3600) return false;

    m_OpenLevelUpTaskTime = timeNow;
    m_isMonsterTaskOpen = true;
    ++m_CurLevelDoTaskCnt;

    //没领的奖励自动发给玩家
    for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
    {
        if (!it->second.isTakeReward)
        {
            if (Player* palyer = sPlayerPool.GetByPlayerId(it->first))
            {
                TakeMonsterTaskReward(*palyer);
            }
        }
    }
    m_playerLst.clear();
    m_RankLst.Clear();

    // 开放对应蛮族城池
    if (City* city = sWorldMap.GetCity(m_monsterCityId))
    {
        city->SetStatus(City::Open_Monster_City, true);

		const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;
		city->AddDefenderNpc(table.savage_id, table.savage_lv, table.savage_num);
    }
#ifdef _MMO_SERVER_
    // 4小时后检查是否完成，服务器宕机重启，须重设此定时器
    sAutoUpdateGame.AddEvent(
        boost::bind(&Country::_CheckMonsterTaskIsSucceed, this),
        4 * 3600
        );
#endif
    CountryDB::SendBaseToDb(*this);
    return true;
}
void Country::_CloseMonsterTask()
{
    m_isMonsterTaskOpen = false;
    m_KillMonsterNum = 0;

    // 关闭对应蛮族城池
    if (City* city = sWorldMap.GetCity(m_monsterCityId))
    {
        city->SetStatus(City::Open_Monster_City, false);
    }
    CountryDB::SendBaseToDb(*this);
}
void Country::_CheckMonsterTaskIsSucceed()
{
    const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;

    if (m_isMonsterTaskOpen && m_KillMonsterNum < table.savage_num)
    {
        _CloseMonsterTask();
    }
}
void Country::AddKillMonsterNum(uint64 playerId, uint num)
{
    if (!m_isMonsterTaskOpen) return;

    TaskPlayer& obj = m_playerLst[playerId];
    obj.statisticKill += num;
    if (m_RankLst.OnValueChange(obj))
	{
		CountryDB::SendLevelUpTaskRankToDb(*this);
	}

    m_KillMonsterNum += num;

    const DB_CountryUp& table = sCountryTableMgr.GetCountryUp(m_level)->m_db;
    if (m_KillMonsterNum >= table.need_kill_num)
    {
        m_isMonsterTaskSucceed = true;
        m_CurLevelDoTaskCnt = 0;
        _CloseMonsterTask();
    }
}
void Country::TakeMonsterTaskReward(Player& player)
{
    const uint64 playerId = player.GetGuid();
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(playerId);
    if (it == m_playerLst.end() || !it->second.isTakeReward)
    {
        return;
    }
    it->second.isTakeReward = true;
    Country::TakeRankReward(player, LOOTTYPE_COUNTRYQUEST, it->second.rank);

    CountryDB::SendLevelUpTaskRankToDb(*this);
}
void Country::TakeWinReward(Player& player, int victimType) //国战奖励
{
    LootList& lootList = GetWinReward(player, victimType);

    //TODO:zhoumf:重新约定消息号、pb::ITEM_REASON
    pb::GS2C_LootList msg;
    if (lootList.PickAllMoneyAndItems(&player, msg, pb::IR_CYCLE_GET))
    {
        player.Send(pb::SMSG_ITEM_GET_LIKE_LOOT, msg);
    }
	//国战胜利获得军功
	// player.AddExploitValue();
	// player.AddOccupyValue()
}
void Country::TakeRankReward(Player& player, int victimType, int rank)
{
    LootList& lootList = GetRankReward(player, victimType, rank);

	//TODO:zhoumf:重新约定消息号、pb::ITEM_REASON
	pb::GS2C_LootList msg;
	if (lootList.PickAllMoneyAndItems(&player, msg, pb::IR_CYCLE_GET))
	{
		player.Send(pb::SMSG_ITEM_GET_LIKE_LOOT, msg);
	}
}
LootList& Country::GetWinReward(Player& player, int victimType)
{
    static LootList s_lootList;
    s_lootList.Clear();

    LootParam param;
    param.victim_type = victimType;
    param.victim_id = Win_Reward_Id;
    param.victim_lv = player.GetLevel();
    param.player_vip_lv = player.GetVipLevel();
    sLootMgr.FillLootList(&player,param, s_lootList);
	return s_lootList;
}
LootList& Country::GetRankReward(Player& player, int victimType, int rank)
{
    static LootList s_lootList;
    s_lootList.Clear();

    int rankId = -1;
    switch (victimType) {
    case LOOTTYPE_COUNTRYQUEST:
    case LOOTTYPE_ACT_OCCUPY_CITY:
    case LOOTTYPE_ACT_DEFENCE_EDGE:
        rankId = sCountryTableMgr.GetRankId(rank);		//攻守城市等任务公用一个奖励排名段
        break;
    case LOOTTYPE_ACT_INVESTMENT:
        rankId = rank;
        break;
    case LOOTTYPE_ACT_TOURNAMENT:
        rankId = sActTableMgr.GetTournamentRankId(rank);
        break;
    default:
        MMO_ASSERT(0);
        break;
    }

    LootParam param;
    param.victim_type = victimType;
    param.victim_id = rankId;
    param.victim_lv = player.GetLevel();
    param.player_vip_lv = player.GetVipLevel();
    sLootMgr.FillLootList(&player,param, s_lootList);
	return s_lootList;
}

void Country::SetCrossConnectSession(const SessionAddress& sessionAdd)
{
	crossServerIp = sessionAdd.second;
	crossServerPort = sessionAdd.first;
}

#ifdef _SERVER_CROSS_
bool Country::IsCrossCountryOccypied()
{
	uint32 capCityId = sMapTableMgr.GetCapitalCity(id,1);
	City* capCity = sCrossWorldMap.GetCity(capCityId);
	if(capCity)
		return capCity->m_durability <= 0;
	else
		return true;
}
#endif

uint32 Country::GetCrossCountryDurability()
{
#ifdef _SERVER_CROSS_
	uint32 capCityId = sMapTableMgr.GetCapitalCity(id,1);
	City* capCity = sCrossWorldMap.GetCity(capCityId);
	if(capCity)
		return capCity->m_durability;
	else
		return 0;
#endif
	return 0;
}

void Country::ModifyProsDegree(int mods)
{
#ifdef _MMO_SERVER_
	m_Prosperity_Degree += mods;

	if(m_Prosperity_Degree < 500)
		m_Prosperity_Degree = 500;

	CountryDB::SendBaseToDb(*this);
#endif
}

void Country::AddCrossKill(uint64 playerId, uint32 cnt)
{
#ifdef _SERVER_CROSS_
	if(!IsCrossCountryOccypied())	//未被占领
	{
		if (Player* player = sPlayerPool.GetByPlayerId(playerId))
		{
			if(player->GetCountryId() == id)
			{
				std::map<int, uint64> m_lastRank;
				for(int i=1; i<=40; i++)
				{
					CrossPlayer* rankone = m_CrossRank.GetRanker(i);
					if(rankone)
					{
						m_lastRank[i] = rankone->playerId;
					}
				}

				CrossPlayer& obj = m_CrossPlayerLst[playerId];		//在这里记录玩家，如果没有则在m_playerLst中添加
				obj.crossKill += cnt;
				obj.playerId = playerId;
				m_CrossRank.OnValueChange(obj);
				NLOG("obj.crossKill=%d, obj.rank=%d",obj.crossKill,obj.rank);

				std::map<int, uint64> m_nowRank;
				for(int i=1; i<=40; i++)
				{
					CrossPlayer* rankone = m_CrossRank.GetRanker(i);
					if(rankone)
					{
						m_nowRank[i] = rankone->playerId;
					}
				}

				std::vector<uint64> m_needNotice;
				for(std::map<int, uint64>::iterator it = m_lastRank.begin(); it != m_lastRank.end(); it++)
				{
					std::map<int, uint64>::iterator findit = m_nowRank.find(it->first);
					if(findit != m_nowRank.end())
					{
						if(findit->second != it->second)
							m_needNotice.push_back(it->second);
					}
					else
					{
						m_needNotice.push_back(it->second);
					}
				}

				for(std::map<int, uint64>::iterator ite = m_nowRank.begin(); ite != m_nowRank.end(); ite++)
				{
					std::map<int, uint64>::iterator findite = m_lastRank.find(ite->first);
					if(findite != m_lastRank.end())
					{
						if(findite->second != ite->second)
						{
							std::vector<uint64>::iterator its = find(m_needNotice.begin(),m_needNotice.end(),ite->second);
							if(its == m_needNotice.end())
								m_needNotice.push_back(ite->second);
						}
					}
					else
					{
						m_needNotice.push_back(ite->second);
					}
				}

				//对所有m_needNotice内的用户发送更新消息
				for(std::vector<uint64>::iterator playerid = m_needNotice.begin(); playerid != m_needNotice.end(); playerid++)
				{
					if(Player* playerone = sPlayerPool.GetByPlayerId(*playerid))
					{
						pb::GS2C_Cross_Rank_Info msg;
						msg.set_rank_num(sCrossWarMgr.GetCrossKillRank(*playerid,id));
						playerone->Send(pb::SMSG_CROSS_KILL_ENEMY_RANK_RET,msg);
					}
				}
			}
		}
	}
#endif
}

void Country::AddShieldTime()
{
	if(m_shieldEndTime <= sOS.TimeSeconds())
		m_shieldEndTime = sOS.TimeSeconds() + GET_BASE_DEF_INT( pb::BD_CROSS_DEFENSE_SHIELD_LAST_TIME)*60;
	else
		m_shieldEndTime += GET_BASE_DEF_INT( pb::BD_CROSS_DEFENSE_SHIELD_LAST_TIME)*60;

	m_isHasShield = true;
}

bool Country::GetHasShield()
{
	if(m_isHasShield)	//有击杀令效果
	{
		if(sOS.TimeSeconds() >= m_shieldEndTime)	//超时，则取消击杀令效果
		{
			m_isHasShield = false;
			return false;
		}
		else
		{
			return true;
		}
	}
	else
		return false;
}

uint64 Country::GetShieldEndTime()
{
	return m_shieldEndTime;
}