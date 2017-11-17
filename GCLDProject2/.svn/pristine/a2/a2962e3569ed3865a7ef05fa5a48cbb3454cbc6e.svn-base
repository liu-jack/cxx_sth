#include "PlayerMapLogic.h"
#include "OS.h"
#include "MapLogic.pb.h"
#include "Combat.pb.h"
#include "Table/MapTableMgr.h"
#include "Table/MapBox.h"
#include "Table/MapEvent.h"
#include "Table/MapFog.h"
#include "../reward/reward.h"
#include "../Trigger/trigger.h"
#include "country/country.h"
#include "../object/Player.h"
#include "city/city.h"

#include "Enum.pb.h"
#ifdef _MMO_SERVER_
#include "../GameServer.h"
#include "../fuben/FubenDataMgr.h"
#include "../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "../character/Character.h"
#include "System.h"
#include "Opcode.pb.h"
#include "WorldMap.h"
#include "../CrossLogic/PlayerMapLogicDB.h"
#include "../Combat/CombatGroup.h"
#include "../Combat/CombatObj.h"
#include "../Npc/NpcMgr.h"
#include "../Npc/Npc.h"
#include "../character/CharacterStorage.h"
#include "../session/PlayerPool.h"
//#include "../AutomaticUpdate/AutomaticUpdate.h"
#include "World.pb.h"
#include "../CrossLogic/CountyDB.h"
#include "Building/BuildingLogMgr.h"
#include "../Technology/PlayerTechnology.h"
#include "../Technology/Table/TechnologyTableMgr.h"
#include "ActivityControl/ActivityMgr.h"

#include "ActivityCelebrity/Table/ActivityCelebrityTableMgr.h"
#include "InterServer.pb.h"

#define GERNEL_REWARD_EVENT 47

MapCityEvent PlayerMapLogic::m_publicCityEventLst;

uint32 PlayerMapLogic::m_CountryHaveCitys[24][3] = {0};

void EventCeil::Reset()
{
	eventid = 0;
	etype = Event_Null;
	cityid = 0;
	target = 0;
	curCompletion = 0;
	eawards.clear();
	isTakeAward = false;
}

void MysteryStore::Reset()
{
	leftFreshTimes = 0;
	myItems.clear();
}

PersonalEventChain::PersonalEventChain()
{
	lastFreshTime = 0;
	currentStep = 0;
	awardsEx.clear();
	isTakeExAward = false;
	currentEvent.Reset();
	mysteryStore.Reset();
}

void PersonalEventChain::SaveTo(pb::SG2D_PersonalEC& msg, uint64 playerid)
{
	msg.set_player_id(playerid);
	msg.set_last_fresh_time(lastFreshTime);
	msg.set_current_step(currentStep);
	msg.set_is_take_chain_award(isTakeExAward);
	msg.set_current_event_id(currentEvent.eventid);
	msg.set_city_id(currentEvent.cityid);
	msg.set_current_completion(currentEvent.curCompletion);
	msg.set_is_take_event_award(currentEvent.isTakeAward);
}

void PersonalEventChain::LoadFrom(pb::SG2D_PersonalEC& msg)
{
	 
}

CityEventChain::CityEventChain()
{
	lastFreshTime = 0;
	currentStep = 0;
	awardsEx.clear();
	isTakeExAward = false;
	currentEvent.Reset();
}

PersonalTriggerEvent::PersonalTriggerEvent()
{
	currentStep = 0;
	currentEvent.Reset();
	timeOfTriggerWork.clear();
}

FogInfo::FogInfo(uint32 fogid, uint64 uniqueid)
{
	m_fogID = fogid;
	m_uniqueID = uniqueid;
	m_fogGuardsInt.clear();
	m_fogGuardsObj.clear();
	guard_all_hp = 0;
	//m_fogObj.clear();
}

PlayerMapLogic::PlayerMapLogic()
    : m_countryId(1)
    , m_StoryCountryId(0)
	, m_trainType(0)
	, m_trainingStartTime(0)
	, m_lastUpdateMapTime(0)
{
	struct tm p;
	sOS.LocalTime(p);
    m_ArmyResourceUpdateDay = p.tm_yday;
    m_ArmyResourceUpdateHour = p.tm_hour;
	for(int i=0; i<ARRAY_SIZE(m_ulockFogCityBit); i++)
	{
		m_ulockFogCityBit[i] = 0;
	}
    m_countryDonateTime = 0;
    m_monsterDonateTime = 0;
	m_tondenCDEndTime = 0;
}
bool PlayerMapLogic::Init(Player& player)
{
    return false;
}
void PlayerMapLogic::SaveTo(pb::GS2C_MapLogic_Init& msg)
{
    msg.set_country_id(m_countryId);
    msg.set_take_box(m_takeBoxBit);

    for (MapCityEvent::iterator it = m_privateCityEventLst.begin();
        it != m_privateCityEventLst.end(); ++it)
    {
        pb::CityEvent* pInfo = msg.add_event_info();
        SaveTo(*pInfo, it->first, it->second);
    }
	if(m_CelebrityCityEventLst.first > 0)
	{
		pb::CityEvent* info = msg.add_event_info();
		SaveTo(*info,m_CelebrityCityEventLst.first,m_CelebrityCityEventLst.second);
	}

	for (MapCityEvent::iterator ite = m_publicCityEventLst.begin();
		ite != m_publicCityEventLst.end(); ++ite)
	{
		pb::CityEvent* pInfo = msg.add_public_lst();
		SaveTo(*pInfo, ite->first, ite->second);
	}

	for(int i=0; i<ARRAY_SIZE(m_ulockFogCityBit); i++)
	{
		msg.add_unlock_fog_city(m_ulockFogCityBit[i]);
	}

    for (map<uint8, uint64>::iterator it = m_tondenHeroBeginTime.begin();
        it != m_tondenHeroBeginTime.end(); ++it)
    {
        pb::PairTime* pInfo = msg.add_tonden_hero_begin_time();
        pInfo->set_first(it->first);
        pInfo->set_second(it->second);
    }

	for(int i=m_tondenHeroBeginTime.size(); i<5; i++)
	{
		pb::PairTime* pInfo = msg.add_tonden_hero_begin_time();
		pInfo->set_first(0);
		pInfo->set_second(0);
	}

	pb::GS2C_MapLogic_ArmyResource* pMsg = msg.mutable_army_resource();
	pMsg->set_update_day(m_ArmyResourceUpdateDay);
	pMsg->set_update_hour(m_ArmyResourceUpdateHour);

    msg.set_country_donate_time(m_countryDonateTime);
    msg.set_monster_donate_time(m_monsterDonateTime);
	msg.set_tonden_cdend_time(m_tondenCDEndTime);
}
void PlayerMapLogic::SaveTo(pb::GS2C_All_CITY_EVENT& msg)
{
	//NLOG("PlayerMapLogic  GS2C_All_CITY_EVENT");
    for (MapCityEvent::iterator it = m_privateCityEventLst.begin();
        it != m_privateCityEventLst.end(); ++it)
    {
        pb::CityEvent* pInfo = msg.add_event_lst();
        SaveTo(*pInfo, it->first, it->second);
		//NLOG("p pInfo.id=%d",it->first);
    }

	for (MapCityEvent::iterator ite = m_publicCityEventLst.begin();
		ite != m_publicCityEventLst.end(); ++ite)
	{
		pb::CityEvent* pInfo = msg.add_public_lst();
		SaveTo(*pInfo, ite->first, ite->second);
		//NLOG("c pInfo.id=%d",ite->first);
	}
	if(m_CelebrityCityEventLst.first > 0 && m_CelebrityCityEventLst.second.cityId != 0)
	{
		pb::CityEvent* pInfo = msg.add_event_lst();
		SaveTo(*pInfo, m_CelebrityCityEventLst.first, m_CelebrityCityEventLst.second);
	}
}
void PlayerMapLogic::SaveTo(pb::CityEvent& msg, uint32 eventId, const CityEventInfo& info)
{
    msg.set_event_id(eventId);
    msg.set_happen_time(info.happenTime);
    msg.set_daily_times(info.dailyTimes);
    msg.set_city_id(info.cityId);
    msg.set_is_begin(info.isBeginning);
	msg.set_iscelebrity(info.isCelebrity);
    msg.set_left_num(info.leftCnt);
    msg.set_event_end_time(info.endTime);
	for(std::set<uint64>::const_iterator it = info.takePlayers.begin();
		it != info.takePlayers.end(); it++)
	{
		msg.add_take_players(*it);
	}
}

void PlayerMapLogic::SaveTo(pb::GS2C_FOG_INFO_Lst& msg)
{
	for(std::vector<FogInfo*>::iterator ite = m_fogGuardians.begin();
		ite != m_fogGuardians.end(); ite++)
	{
		pb::Struct_FOG_INFO* fogOne = msg.add_fog_lst();
		fogOne->set_fog_id((*ite)->m_fogID);
		fogOne->set_uniqueid((*ite)->m_uniqueID);
		for(IntPairVec::iterator it = (*ite)->m_fogGuardsInt.begin();
			it != (*ite)->m_fogGuardsInt.end(); it++)
		{
			pb::PairValue* guardhp = fogOne->add_guard_hp();
			guardhp->set_first((*it).first);
			guardhp->set_second((*it).second);
		}

		for(IntPairVec::iterator itm = (*ite)->m_fogGuardsMaxhp.begin();
			itm != (*ite)->m_fogGuardsMaxhp.end(); itm++)
		{
			pb::PairValue* guardmaxhp = fogOne->add_guard_max();
			guardmaxhp->set_first((*itm).first);
			guardmaxhp->set_second((*itm).second);
		}
		fogOne->set_guard_up_hp((*ite)->guard_all_hp);
		//填充无效数据
		int sizefogguard = fogOne->guard_hp_size();
		for(int i=5-sizefogguard; i>0; i--)
		{
			pb::PairValue* guardhp = fogOne->add_guard_hp();
			guardhp->set_first(0);
			guardhp->set_second(0);
		}

		sizefogguard = fogOne->guard_max_size();
		for(int i=5-sizefogguard; i>0; i--)
		{
			pb::PairValue* guardmaxhp = fogOne->add_guard_max();
			guardmaxhp->set_first(0);
			guardmaxhp->set_second(0);
		}
	}
}


void PlayerMapLogic::SaveTo(pb::CelebrityEvent& msg,const MapEvent& event)
{
	msg.set_event_display(event.EventDisplay());
	pb::StReward *info = msg.mutable_perferto();
	info->set_type(event.GetCost(0).first);
	info->set_value(-event.GetCost(0).second);
}

//在玩家进入的时候从数据库中读取到本地
void PlayerMapLogic::LoadFrom(Player* player, const pb::GS2C_MapLogic_Init& msg)
{
#ifdef _MMO_SERVER_
    m_countryId = msg.country_id();
#endif
    m_takeBoxBit = msg.take_box();

    m_privateCityEventLst.clear();
    CityEventInfo data;
    for (int i = 0; i < msg.event_info_size(); ++i)
    {
        const pb::CityEvent& info = msg.event_info(i);
        data.happenTime = info.happen_time();
        data.dailyTimes = info.daily_times();
        data.cityId = info.city_id();
        data.isBeginning = info.is_begin();
		data.isCelebrity = info.iscelebrity();
		data.endTime = info.event_end_time();
		data.leftCnt = info.left_num();
		if(info.iscelebrity())
		{
			if(info.event_end_time() > 0){
				m_CelebrityCityEventLst.first = info.event_id();
				m_CelebrityCityEventLst.second = data;
			}
		}
		else
		{
			m_privateCityEventLst[info.event_id()] = data;
		}
    }	

	for(int i=0; i<msg.unlock_fog_city_size() && i<ARRAY_SIZE(m_ulockFogCityBit); i++)
	{
		m_ulockFogCityBit[i] = msg.unlock_fog_city(i);//解锁的迷雾关
	}

    m_tondenHeroBeginTime.clear();
    for (int i = 0; i < msg.tonden_hero_begin_time_size(); ++i)
    {
        const pb::PairTime& info = msg.tonden_hero_begin_time(i);
        m_tondenHeroBeginTime[info.first()] = info.second();

		if(info.first()>0)
		{
			Character* charone = player->m_characterStorage->GetCharacterByTableId(info.first());
			if(charone)
				charone->FlyTo(sMapTableMgr.GetTuntianCity(GetCountryId()));
		}
    }
    m_countryDonateTime = msg.country_donate_time();

	m_tondenCDEndTime = msg.tonden_cdend_time();

	const pb::GS2C_MapLogic_ArmyResource& pMsg = msg.army_resource();
	struct tm p;
	sOS.LocalTime(p);
	if(pMsg.update_day() == 0)
		m_ArmyResourceUpdateDay = p.tm_yday;
	else
		m_ArmyResourceUpdateDay = pMsg.update_day();

	if(pMsg.update_day() == 0 && pMsg.update_day() == 0)
		m_ArmyResourceUpdateHour = p.tm_hour;
	else
		m_ArmyResourceUpdateHour = pMsg.update_hour();

#ifdef _MMO_SERVER_
	///////////////保存所有迷雾关守卫信息//////////////////////
	m_fogGuardians.clear();		//开始的时候可能没有内容，只有在请求一次之后才会向数据库写入所有迷雾关的信息
	for(int i = 0; i<msg.fog_lst_size(); i++)		//如果没有则size为0
	{
		int realNpcCount = 0;		//有记录的npc守卫数量， 需要与stage表中的一致
		int tmpCount = 0;
		const pb::Struct_FOG_INFO& info = msg.fog_lst(i);
		if(info.fog_id() == 0) continue;	//记录有问题 迷雾id不能为0

		FogInfo* fogone = new FogInfo(info.fog_id(), info.uniqueid());

		//如果有记录，则info.guard_hp_size()必须为5   ，0填充
		ASSERT(info.guard_hp_size() == 5);
		for(int j = 0; j < info.guard_hp_size() && j < 5; j++)
		{
			//NLOG("guard_hp[%d].id = %d  hp = %d",j,info.guard_hp(j).first(),info.guard_hp(j).second());
			if(info.guard_hp(j).first() == 0) continue;//守卫id为0，说明是无用的字段

			IntPair tmp;
			tmp.first = info.guard_hp(j).first();
			tmp.second = info.guard_hp(j).second();
			fogone->m_fogGuardsInt.push_back(tmp);
			realNpcCount++;				
		}
		fogone->guard_all_hp = info.guard_up_hp();
		//创建守卫模型
		if(const MapFog* fogmapone = sMapTableMgr.GetMapFog(info.fog_id()))	//能获得该迷雾关信息
		{
			uint32 fubenstageid = fogmapone->StageId();
			if(const FubenStage* fubenstage = sFubenDataMgr.GetStage(fubenstageid))		//能获得该关卡信息
			{
				if(fubenstage->m_npcLst.size() != realNpcCount) continue;	//记录不一致
				//NLOG("m_npcLst size = %d",fubenstage->m_npcLst.size());
				//NLOG("realNpcCount = %d",realNpcCount);
				for(IntPairVec::const_iterator it = fubenstage->m_npcLst.begin();
					it != fubenstage->m_npcLst.end(); ++it, ++tmpCount)
				{
					Npc* pNpc = sNpcMgr.CreateNpc(it->first, it->second);
					//NLOG("NPC Address :%d",pNpc);
					IntPair maxhp;
					maxhp.first = fogone->m_fogGuardsInt[tmpCount].first;
					maxhp.second = pNpc->CurHp();
					fogone->m_fogGuardsMaxhp.push_back(maxhp);

					//NLOG("guard_maxhp[%d].id = %d  hp = %d",tmpCount,maxhp.first,maxhp.second);

					if(pNpc)
					{
						pNpc->setRestHP(fogone->m_fogGuardsInt[tmpCount].second);
						fogone->m_fogGuardsObj.insert(pNpc);
						//fogone->m_fogObj.insert(pNpc);
					}
					//NLOG("guard_curHP[%d].id = %d  hp = %d",tmpCount,maxhp.first,fogone->m_fogGuardsInt[tmpCount].second);
				}	

			}
			else
				continue;
		}
		else
			continue;

		m_fogGuardians.push_back(fogone);
	}
#endif

	//sAutoUpdate.Add(boost::bind(&PlayerMapLogic::updateArmyResourcePerHour,this),TIME_ZERO,PER_HOUR,&m_player);

	uint64 timenow = sOS.TimeSeconds();
	boost::function<void(const MapEvent&)> func = boost::bind(&PlayerMapLogic::_InitMapEvents, this, player, _1, timenow);
	sMapTableMgr.ForEachPrivateCityEvent(func);

}
void PlayerMapLogic::SaveTo(Player& player,pb::GS2C_MapLogic_ArmyResource& msg)
{
    _UpdateArmyResource(player);

    msg.set_update_day(m_ArmyResourceUpdateDay);
    msg.set_update_hour(m_ArmyResourceUpdateHour);
    
    for (IntPairVec::iterator it = m_ArmyResource.begin(); it != m_ArmyResource.end(); ++it)
    {
        pb::PairValue* pData = msg.add_resource_lst();
        pData->set_first(it->first);
        pData->set_second(it->second);
    }
    //下一个小时的军资，预显示用
    uint32 haveCityCnt = GetWorldMapCountry()->GetCityCount();
	uint8 realtime;
	if(m_ArmyResourceUpdateHour == 23)
		realtime = 0;
	else
		realtime = m_ArmyResourceUpdateHour + 1;
    if (const DB_Military_Supplies* table = sMapTableMgr.GetArmyResource(realtime))
    {
        uint32 silver = haveCityCnt * ArmyResource_Silver_Rate * table->silver_reward_times;
        uint32 iron = haveCityCnt * ArmyResource_Iron_Rate * table->iron_reward_times;
        pb::PairValue* pData = msg.add_resource_lst();
        pData->set_first(silver);
        pData->set_second(iron);
    }
}
void PlayerMapLogic::LoadFrom(const pb::GS2C_MapLogic_ArmyResource& msg)
{
    m_ArmyResourceUpdateDay = msg.update_day();
    m_ArmyResourceUpdateHour = msg.update_hour();

    IntPair tmp;
    m_ArmyResource.clear();
    for (int i = 0; i < msg.resource_lst_size(); ++i)
    {
        const pb::PairValue& data = msg.resource_lst(i);
        tmp.first = data.first();
        tmp.second = data.second();
        m_ArmyResource.push_back(tmp);
    }
}
bool PlayerMapLogic::_SetCountryId(Player& player, uint32 id)
{
	if (m_countryId == 0) {
		if(id == 0) m_countryId =1;
		else m_countryId = id;
		LLOG("Set Pplayer:%llu countryid=%d",player.GetGuid(),m_countryId);
		PlayerMapLogicDB::SendBaseToDb(player, *this);
		return true;
	}

#ifdef _SERVER_CROSS_
	if(id == 0) m_countryId =1;
	else m_countryId = id;
	LLOG("Set Pplayer:%llu countryid=%d",player.GetGuid(),m_countryId);
	PlayerMapLogicDB::SendBaseToDb(player, *this);
	return true;
#endif

	return false;
}
uint32 PlayerMapLogic::GetCountryId()
{
    if (m_StoryCountryId > 0)
    {
        return m_StoryCountryId;
    }

	//ASSERT(m_countryId != 0);
	if(m_countryId == 0)
		return 1;

    return m_countryId;
}
Country* PlayerMapLogic::GetWorldMapCountry()
{
    return sWorldMap.GetCountry(GetCountryId());
}
void PlayerMapLogic::_UpdateArmyResource(Player& player)
{
	if (player.GetLevel() < ArmyResource_Open_Level)	return;

    uint32 haveCityCnt = GetWorldMapCountry()->m_setCity.size();
	if(haveCityCnt == 0)
		return;

	struct tm p;
	sOS.LocalTime(p);
    if (m_ArmyResourceUpdateDay == p.tm_yday)
    {
        for (int i = m_ArmyResourceUpdateHour + 1; i <= p.tm_hour; ++i)
        {
            _AddArmyResource(i, haveCityCnt);
        }
    }
    else
    {
        for (int i = m_ArmyResourceUpdateHour + 1; i < 24; ++i)
        {
            _AddArmyResource(i, haveCityCnt);
        }
        for (int i = 0; i <= p.tm_hour; ++i)
        {
            _AddArmyResource(i, haveCityCnt);
        }
    }

    m_ArmyResourceUpdateDay = p.tm_yday;
    m_ArmyResourceUpdateHour = p.tm_hour;
	//PlayerMapLogicDB::SendBaseToDb(player, *this);
}

void PlayerMapLogic::updateArmyResourcePerHour(Player& player)
{
	pb::GS2C_MapLogic_ArmyResource msg;
	SaveTo(player,msg);
	player.Send(pb::SMSG_SUPPLY_REWARD, msg);
}

void PlayerMapLogic::_AddArmyResource(uint32 hourTime, uint32 cityCnt)
{
    if (m_ArmyResource.size() >= ArmyResource_Max_Size) return;

    if (const DB_Military_Supplies* table = sMapTableMgr.GetArmyResource(hourTime))
    {
        IntPair data;
        data.first = cityCnt * ArmyResource_Silver_Rate * table->silver_reward_times;
        data.second = cityCnt * ArmyResource_Iron_Rate * table->iron_reward_times;
        m_ArmyResource.push_back(data);
    }
}
void PlayerMapLogic::_UpdateHaveCityCntPerHour(uint32 hourTime)
{
    if (hourTime >= 24) return;

    for (int i = 1; i <= MAX_PLAYER_COUNTRY_ID; ++i)
    {
        m_CountryHaveCitys[hourTime][i - 1] = sWorldMap.GetCountry(i)->m_setCity.size();
    }
}
bool PlayerMapLogic::TakeArmyResource(Player& player, pb::GS2C_Take_Army_Resource_Result& msg)
{
    if (player.GetLevel() < ArmyResource_Open_Level) 
	{
		msg.set_backmsg(pb::LevelIllegal);
		return false;
	}

    _UpdateArmyResource(player);

    if (m_ArmyResource.size() > 0 && m_ArmyResource.size() <= ArmyResource_Max_Size)
    {
        IntPairVec reward;
        IntPair& data = m_ArmyResource.back();
		reward.push_back(std::make_pair(Reward::Coin_Silver, data.first));
		if (data.second > 0) reward.push_back(std::make_pair(Reward::Iron, data.second));
        if (sReward.Change(player, reward))
        {
            m_ArmyResource.pop_back();
		}
		msg.set_backmsg(pb::Success);
		player.UpdateAllDailyQuestInfo(TAKE_JUNZI,1,0);//每日任务

		if(m_ArmyResourceUpdateHour >= m_ArmyResource.size())
			m_ArmyResourceUpdateHour -= m_ArmyResource.size();
		else
		{
			m_ArmyResourceUpdateDay -= 1;
			m_ArmyResourceUpdateHour = m_ArmyResourceUpdateHour + 24 - m_ArmyResource.size();
		}
		PlayerMapLogicDB::SendBaseToDb(player, *this);
		if(m_ArmyResourceUpdateHour + m_ArmyResource.size() > 23)
		{
			m_ArmyResourceUpdateDay += 1;
			m_ArmyResourceUpdateHour = m_ArmyResourceUpdateHour + m_ArmyResource.size() - 24;
		}
		else
			m_ArmyResourceUpdateHour += m_ArmyResource.size();

		return true;
    }
	else
	{
		PlayerMapLogicDB::SendBaseToDb(player, *this);
		msg.set_backmsg(pb::NoAwards);
	}
    return false;
}
void PlayerMapLogic::TakeBox(Player& player, uint8 boxId)
{
    if (const MapBox* p = sMapTableMgr.GetMapBox(boxId))
    {
        if (m_takeBoxBit & (1LL << boxId))
        {
            return; //这个箱子已被拿过
        }
        m_takeBoxBit |= (1LL << boxId);

        sReward.Change(player, p->m_RewardLst);
        PlayerMapLogicDB::SendBaseToDb(player, *this);
    }
}
uint32 PlayerMapLogic::GetBoxCnt() const
{
    uint32 ret = 0;
    for (int i = 0; i < 8*sizeof(m_takeBoxBit); ++i)
    {
        if (m_takeBoxBit & (1LL << i)) ++ret;
    }
    return ret;
}

//每秒更新一次 个人世界事件
void PlayerMapLogic::_UpdateCityEvent(Player& player, uint64 timeNowMS)
{
	//uint64 begintime =sOS.TimeMS();
	bool freshNew = false;
	uint64 timenow = timeNowMS/1000;
	if (!sOS.IsSameDay(m_lastUpdateMapTime, timenow)) //跨天，次数归0
		freshNew = true;

    boost::function<void(const MapEvent&)> func = boost::bind(&PlayerMapLogic::_CheckCityEvent, this, &player, _1, timenow, freshNew);
    sMapTableMgr.ForEachPrivateCityEvent(func);

	m_lastUpdateMapTime = timenow;
	static bool send_flag[2] = {false,};
#ifdef _MMO_SERVER_
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_MAKE_FRIENDS))
	{
		if(!send_flag[0])
		{
			m_CelebrityCityEventLst.second.ClearInfo(true,0);
			if(m_CelebrityCityEventLst.first > 0){
				OnCelebrityCityEventChange(&player,m_CelebrityCityEventLst.second,m_CelebrityCityEventLst.first);
				PlayerMapLogicDB::UpdateCityEventToDb(player, m_CelebrityCityEventLst.first, m_CelebrityCityEventLst.second);
			}
			m_CelebrityCityEventLst.first = 0;
			send_flag[0] = true;
		}
	}
	else
	{
		send_flag[0] = false;
		if(m_CelebrityCityEventLst.first == 0)
		{
			UpdateCelebrityCityEvent(&player);
		}
	}
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_HERO_REWARD))
	{
		if(!send_flag[1])
		{
			MapCityEvent::iterator iter = m_privateCityEventLst.find(GERNEL_REWARD_EVENT);
			if(iter != m_privateCityEventLst.end())
			{
				iter->second.isBeginning = false;
				iter->second.endTime = 0;
				iter->second.cityId = 0;
				_OnCityEventEnd(player,iter->second,*sMapTableMgr.GetMapEvent(GERNEL_REWARD_EVENT));
				PlayerMapLogicDB::UpdateCityEventToDb(player, iter->first, iter->second);
				send_flag[1] = true;
			}
		}
	}
	else
	{
		send_flag[1] = false;
	}
#endif
}

//加载的时候初始化地图事件
void PlayerMapLogic::_InitMapEvents(Player* player, const MapEvent& eventTable, uint64 timeNow)
{
	MapCityEvent::iterator it = m_privateCityEventLst.find(eventTable.Id());
	if (it == m_privateCityEventLst.end()) // 首次发生该事件，插入
	{
		std::pair<MapCityEvent::iterator,bool> ret;
		ret = m_privateCityEventLst.insert(std::make_pair(
			eventTable.Id(),
			CityEventInfo(timeNow)
			));
		if (ret.second)
		{
			it = ret.first;
		}
		else
		{
			return;
		}
	}
}

//对每一个奖励任务
void PlayerMapLogic::_CheckCityEvent(Player* player, const MapEvent& eventTable, uint64 timeNow, bool freshTimes)
{
	uint64 TimesLimit = 0; 
	const TecTable* tec = NULL;
	
    MapCityEvent::iterator it = m_privateCityEventLst.find(eventTable.Id());
    if (it == m_privateCityEventLst.end()) // 首次发生该事件，插入
    {
		if(eventTable.Id() == GERNEL_REWARD_EVENT)
		{
			std::pair<MapCityEvent::iterator,bool> ret;
			ret = m_privateCityEventLst.insert(std::make_pair(
				eventTable.Id(),
				CityEventInfo(timeNow)
				));
			if (ret.second)
			{
				it = ret.first;
			}
			else
			{
				return;
			}
		}
		return;
    }
    CityEventInfo& info = it->second;		//事件记录
	if(freshTimes)
		info.dailyTimes = 0;
	if(!info.isBeginning)		//奖励事件没有开始
	{
		if (sTrigger.Check(player, eventTable.m_happen_trigger) && !(eventTable.m_stop_trigger.size()>0 && (eventTable.m_stop_trigger.front() != 0) && sTrigger.Check(player, eventTable.m_stop_trigger)))
		{
			if(player->HaveTechnology(BUWENDINGLICHANG) && eventTable.GetEventTypeID() == GET_BASE_DEF_INT( pb::BD_SHADOW_EVENTS_TYPE_ID))
			{
				tec = sTechnologyTableMgr.GetTable(BUWENDINGLICHANG);
				TimesLimit  = eventTable.DailyTimesLimit() + tec->Value1();
			}
			else
				TimesLimit  = eventTable.DailyTimesLimit();
			if (timeNow >= info.endTime + eventTable.IntervalSec() && 
				info.dailyTimes < TimesLimit)		//奖励时间间隔满足且奖励次数满足
			{
				info.cityId = _RandPrivateEventCity();		//随机一个城市
				if (info.cityId > 0)
				{
					info.happenTime = timeNow;
					info.isBeginning = true;
					info.leftCnt = eventTable.RewardMaxCnt();
					if(eventTable.EventLastTime()>0)		//有持续时间
						info.endTime = timeNow + eventTable.EventLastTime();
					else
						info.endTime = 0;
					++info.dailyTimes;
					info.takePlayers.clear();
					_OnCityEventBegin(*player, info, eventTable);
					//LLOG(" rand one city id=%d taskid=%d leftCnt=%d endTime=%lld",info.cityId,eventTable.Id(),info.leftCnt,info.endTime);
				}
				//uint64 up4 =sOS.DiffTimeMS(temp1);
				//uint64 total =sOS.DiffTimeMS(begintime);
				//NLOG( "--------_CheckCityEvent1 up1:%llu,up2:%llu,up3:%llu,up4:%llu,total:%llu",up1,up2,up3,up4,total) ;
			}
		}
	}
	else
	{
		//如果奖励事件已经开始
		City* city = sWorldMap.GetCity(info.cityId);
		
		//如果当前城池已经被占领，则取消这个城池的个人事件
		if(city->GetCountryId() != player->GetCountryId())
		{
			LLOG("city is occupied, taskid=%d,dailyTimes=%d",eventTable.Id(),info.dailyTimes);
			info.isBeginning = false;
			if(info.dailyTimes > 0)
				info.dailyTimes--;
			LLOG("after dailytimes=%d",info.dailyTimes);
			info.endTime = timeNow;
			info.cityId = 0;
			_OnCityEventEnd(*player, info, eventTable);
		}
		else		//城池没有被占领
		{
	
			if(info.endTime>0)		//有倒计时的活动
			{
				if(timeNow >= info.endTime)		//时间到
				{
					info.isBeginning = false;
					info.endTime = timeNow;
					info.cityId = 0;
					_OnCityEventEnd(*player, info, eventTable);
					//LLOG("finish one city id=%d taskid=%d",info.cityId,eventTable.Id());
				}
			}
			else	//没有倒计时的活动
			{
				if (eventTable.m_stop_trigger.size()>0 && (eventTable.m_stop_trigger.front() != 0) && sTrigger.Check(player, eventTable.m_stop_trigger))	//活动结束
				{
					info.isBeginning = false;
					info.endTime = 0;
					info.cityId = 0;
					_OnCityEventEnd(*player, info, eventTable);
				}
			}
		}
		//uint64  up2 = sOS.DiffTimeMS(temp1);
		//uint64 total =sOS.DiffTimeMS(begintime);
		//NLOG( "--------_CheckCityEvent2 up1:%llu,up2:%llu,total:%llu",up1,up2,total) ;
	}

    if (info.isBeginning)
    {
        if (eventTable.m_stop_trigger.size()>0 && (eventTable.m_stop_trigger.front() != 0) && sTrigger.Check(player, eventTable.m_stop_trigger))
        {
            info.isBeginning = false;
			info.endTime = timeNow;
			info.cityId = 0;
            _OnCityEventEnd(*player, info, eventTable);
        }
    }

	if(info.isBeginning)
	{
		if(info.endTime>0)
		{
			if(timeNow >= info.endTime)
			{
				info.isBeginning = false;
				info.endTime = timeNow;
				info.cityId = 0;
				_OnCityEventEnd(*player, info, eventTable);
				//LLOG("finish one city id=%d taskid=%d",info.cityId,eventTable.Id());
			}
		}
	}
	
}
void PlayerMapLogic::_OnCityEventBegin(Player& player, const CityEventInfo& info, const MapEvent& eventTable)
{
    //通知客户端
    pb::GS2C_CITY_EVENT msg;
    SaveTo(*msg.mutable_city_event(), eventTable.Id(), info);
	msg.set_ispublicevent(false);
    player.Send(pb::SMSG_CITY_EVENT, msg);
}
void PlayerMapLogic::_OnCityEventEnd(Player& player, const CityEventInfo& info, const MapEvent& eventTable)
{
    //通知客户端
    pb::GS2C_CITY_EVENT msg;
	SaveTo(*msg.mutable_city_event(), eventTable.Id(), info);
	msg.set_ispublicevent(false);
    player.Send(pb::SMSG_CITY_EVENT, msg);
}

//拾取奖励
void PlayerMapLogic::ClientHandleCityEvent(Player& player, uint32 cityId, uint32 eventId, uint8 selectIdx, pb::GS2C_Take_Award_Ret& msg,IntPairVec& rewrad)
{
	uint64 timeNowMS = sOS.TimeMS();
    _UpdateCityEvent(player,timeNowMS);

    bool isPrivateEvent = true;
    CityEventInfo* pInfo = NULL;
    //个人城池事件
    do {
        MapCityEvent::iterator it = m_privateCityEventLst.find(eventId);

        if (it == m_privateCityEventLst.end()) break;

        pInfo = &(it->second);
        isPrivateEvent = true;	//是个人事件

    } while (0);
    //全服共享城池事件
    do {
        MapCityEvent::iterator it = m_publicCityEventLst.find(eventId);

        if (it == m_publicCityEventLst.end()) break;

        pInfo = &(it->second);
        isPrivateEvent = false;

    } while (0);

    if ((pInfo && pInfo->isBeginning && pInfo->cityId == cityId && pInfo->leftCnt > 0) == false)
    {
		msg.set_ret(pb::CommonFail);
        return;
    }
    if (City* pCity = sWorldMap.GetCity(cityId))
    {
        if (pCity->IsFighting()) 
		{
			msg.set_ret(pb::CityInFighting);
			return;
		}
		
		if(!player.TestFlag(pb::PLAYER_CAN_FAR_TRIGE_PERSONAL_EVENT))
		{
			bool isherohere = false;
			const CharacterIds battleArray = player.m_characterStorage->GetCharIDLst();
			CharacterIds::const_iterator heroidx = battleArray.begin();
			for(; heroidx != battleArray.end(); heroidx++)
			{
				if (Character* character = player.m_characterStorage->MutableCharacter(*heroidx))
				{
					if(character->GetCurCity() == cityId)
					{
						isherohere = true;
					}
				}
			}
			if(!isherohere) 
			{
				msg.set_ret(pb::NoHeroHere);
				return;
			}
		}
    }
	else
	{
		msg.set_ret(pb::NoCity);
		return;
	}

	std::set<uint64>::iterator it = pInfo->takePlayers.find(player.GetGuid());
	if(it != pInfo->takePlayers.end())
	{
		msg.set_ret(pb::HasTaken);
		return;
	}

    if (const MapEvent* pEvent = sMapTableMgr.GetMapEvent(eventId))
    {

		if(pEvent->SelectRewardSize() > 0)
		{
			//给选择性资源
			if (selectIdx < pEvent->SelectRewardSize())
			{
				if (sReward.Change(player, pEvent->m_CostLst))
				{
					sReward.Append(rewrad, pEvent->SelectRewardTyp(selectIdx), pEvent->SelectRewardVal(selectIdx));

					pInfo->takePlayers.insert(player.GetGuid());
					//扣剩余次数
					if (--(pInfo->leftCnt) == 0) 
					{
						pInfo->isBeginning = false;
						pInfo->endTime = sOS.TimeSeconds();
						pInfo->cityId = 0;
						NLOG("take award over leftCnt = %d",pInfo->leftCnt);
					}
				}
				else
				{
					msg.set_ret(pb::CostFail);
					return;
				}
			}
			else
			{
				msg.set_ret(pb::SelectError);
				return;
			}
		}
		else
		{
			if (sReward.Change(player, pEvent->m_CostLst))
			{
				//给全奖励
				sReward.Append(rewrad, pEvent->m_RewardLst);

				pInfo->takePlayers.insert(player.GetGuid());

				//扣剩余次数
				if (--(pInfo->leftCnt) == 0) 
				{
					pInfo->isBeginning = false;
					pInfo->endTime = sOS.TimeSeconds();
					pInfo->cityId = 0;
					NLOG("take award over leftCnt = %d",pInfo->leftCnt);
				}
			}
			else
			{
				msg.set_ret(pb::CostFail);
				return;
			}
		}
    }
	else
	{
		msg.set_ret(pb::NoEvent);
		return;
	}

    //通知客户端
    if (isPrivateEvent)
    {
        PlayerMapLogicDB::UpdateCityEventToDb(player, eventId, *pInfo);

		pb::GS2C_CITY_EVENT sendmsg;
		SaveTo(*sendmsg.mutable_city_event(),eventId,*pInfo);
		sendmsg.set_ispublicevent(false);
		player.Send(pb::SMSG_CITY_EVENT, sendmsg);
    } 
    else
    {
		pb::GS2C_CITY_EVENT sendmsg;
		SaveTo(*sendmsg.mutable_city_event(), eventId, *pInfo);
		sendmsg.set_ispublicevent(true);
		sWorldMap.Broadcast(pb::SMSG_CITY_EVENT, sendmsg);
    }
	msg.set_ret(pb::AwardSuccess);
	player.UpdateAllDailyQuestInfo(CITY_EVENT,1,0);//每日任务
}
uint64 PlayerMapLogic::GetCityEventHappenTime(uint32 eventId) const
{
    //个人城池事件
    do {
        MapCityEvent::const_iterator it = m_privateCityEventLst.find(eventId);

        if (it == m_privateCityEventLst.end()) break;

        return it->second.happenTime;
    } while (0);
    //全服共享城池事件
    do {
        MapCityEvent::const_iterator it = m_publicCityEventLst.find(eventId);

        if (it == m_publicCityEventLst.end()) break;

        return it->second.happenTime;
    } while (0);

    return 0;
}


//每秒检验一次
void PlayerMapLogic::UpdateCityEvent()
{
    boost::function<void(const MapEvent&)> func = boost::bind(&PlayerMapLogic::_CheckCityEvent, _1, sOS.TimeSeconds());
    sMapTableMgr.ForEachPublicCityEvent(func);
}
void PlayerMapLogic::_CheckCityEvent(const MapEvent& eventTable, uint64 timeNow)
{
    MapCityEvent::iterator it = m_publicCityEventLst.find(eventTable.Id());
    if (it == m_publicCityEventLst.end()) // 首次发生该事件，插入
    {
		std::pair<MapCityEvent::iterator,bool> ret;
       ret = m_publicCityEventLst.insert(std::make_pair(
            eventTable.Id(),
            CityEventInfo(timeNow-timeNow%86400/*eventTable.IntervalSec()*/)
            ));
	   if (ret.second)
	   {
		   it = ret.first;
	   } 
	   else
	   {
		   return;
	   }
    }
	
    CityEventInfo& info = it->second;

	if(!sPeriodSettle.IsInSameTimePeriod(timeNow,info.happenTime,eventTable.IntervalSec()))		//如果不是同一时间段，则重新开始任务
	{
		//首先结束掉当前奖励
		info.isBeginning = false;
		_OnCityEventEnd(info, eventTable);
		if (sTrigger.Check(NULL, eventTable.m_happen_trigger) && !(eventTable.m_stop_trigger.size()>0 && sTrigger.Check(NULL, eventTable.m_stop_trigger) && eventTable.m_stop_trigger.front() != 0))
		{
			info.cityId = _RandPublicEventCity(eventTable);
            if (info.cityId > 0)
			{
                info.happenTime = timeNow;
                info.isBeginning = true;
				info.leftCnt = eventTable.RewardMaxCnt();
				info.takePlayers.clear();
                _OnCityEventBegin(info, eventTable);
            }
        }
	}

}
void PlayerMapLogic::_OnCityEventBegin(const CityEventInfo& info, const MapEvent& eventTable)
{
    //广播给世界地图上的玩家
    pb::GS2C_CITY_EVENT msg;
	SaveTo(*msg.mutable_city_event(), eventTable.Id(), info);
	msg.set_ispublicevent(true);
    sWorldMap.Broadcast(pb::SMSG_CITY_EVENT, msg);
}
void PlayerMapLogic::_OnCityEventEnd(const CityEventInfo& info, const MapEvent& eventTable)
{
    //广播给世界地图上的玩家
    pb::GS2C_CITY_EVENT msg;
	SaveTo(*msg.mutable_city_event(), eventTable.Id(), info);
	msg.set_ispublicevent(true);
    sWorldMap.Broadcast(pb::SMSG_CITY_EVENT, msg);
}
uint16 PlayerMapLogic::_RandPrivateEventCity()
{
    const std::set<uint16>& setCityId = GetWorldMapCountry()->GetCityLst();		//本国的城市

    // 剔除m_privateCityEventLst里的城池，再随机
    std::vector<uint16> fitCityLst;
    std::set<uint16>::const_iterator it = setCityId.begin();
    for (; it != setCityId.end(); ++it)
    {
		//LLOG("have city id = %d",*it);
		if(const MapCity* table = sMapTableMgr.GetMapCity(*it))
		{
			//LLOG("aa");
			if(table->IsTonden()) continue;	//屯田区不出现奖励

			//LLOG("bb");
			bool isFitCity = true;
			MapCityEvent::const_iterator itr = m_privateCityEventLst.begin();		//个人奖励时间
			for (; itr != m_privateCityEventLst.end(); ++itr)
			{
				//LLOG("check one city %d",itr->second.cityId);
				if (itr->second.cityId == *it)
				{
					isFitCity = false;
					break;
				}
			}
			
			if (isFitCity) fitCityLst.push_back(*it);
		}
    }

    if (fitCityLst.empty())
    {
        return 0;
    }
    else
    {
		//LLOG("city size=%d",fitCityLst.size());
        int rand = sOS.Rand(0, fitCityLst.size());
        return fitCityLst[rand];
    }
}
uint16 PlayerMapLogic::_RandPublicEventCity(const MapEvent& eventTable)
{
    // 剔除m_publicCityEventLst里的城池，再随机
    std::vector<uint16> fitCityLst;
    std::vector<int>::const_iterator it = eventTable.m_appear_city.begin();
    for (; it != eventTable.m_appear_city.end(); ++it)
    {
        bool isFitCity = true;
        MapCityEvent::const_iterator itr = m_publicCityEventLst.begin();
        for (; itr != m_publicCityEventLst.end(); ++itr)
        {
            if (itr->second.cityId == *it)
            {
                isFitCity = false;
                break;
            }
        }

        if (isFitCity) fitCityLst.push_back(*it);
    }

    if (fitCityLst.empty())
    {
        return 0;
    }
    else
    {
        int rand = sOS.Rand(0, fitCityLst.size());
        return fitCityLst[rand];
    }
}

void PlayerMapLogic::UpdateCelebrityCityEvent(Player* player)
{
#ifdef _MMO_SERVER_
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_MAKE_FRIENDS))
	{
		m_CelebrityCityEventLst.second.ClearInfo(true,0);
		if(m_CelebrityCityEventLst.first > 0){
			OnCelebrityCityEventChange(player,m_CelebrityCityEventLst.second,m_CelebrityCityEventLst.first);
			PlayerMapLogicDB::UpdateCityEventToDb(*player, m_CelebrityCityEventLst.first, m_CelebrityCityEventLst.second);
		}
		return;
	}
	if(m_CelebrityCityEventLst.second.dailyTimes >= GET_BASE_DEF_UINT(pb::BD_ACTIVITY_VISIT_FAMOUS_MAX_TIME))
	{
		return;
	}
	if(m_CelebrityCityEventLst.second.endTime != 0)
	{
		m_CelebrityCityEventLst.second.endTime = 0;
	}
	const MapEvent* map_event = sMapTableMgr.getCelebrityMapEvent(sActivityCelebrityMgr.getRandEventId());
	if(map_event)
	{
		CheckCelebrityCityEvent(player,map_event->Id(),sOS.TimeSeconds());
	}
#endif
}

void PlayerMapLogic::CheckCelebrityCityEvent(Player* player,const uint32 event_id, uint64 activity_start)
{
#ifdef _MMO_SERVER_
	m_CelebrityCityEventLst.first = event_id;
	CityEventInfo& info = m_CelebrityCityEventLst.second;		//事件记录
	info.takePlayers.clear();
	info.isBeginning = true;
	if(info.happenTime == 0)
		info.happenTime = activity_start;
	info.isCelebrity = true;
	if (!sOS.IsSameDay(info.happenTime, activity_start)) //跨天，次数归0
	{
		info.dailyTimes = 0;
		info.happenTime = activity_start;
	}
	if (info.dailyTimes < GET_BASE_DEF_UINT(pb::BD_ACTIVITY_VISIT_FAMOUS_MAX_TIME))		//
	{
		info.cityId = RandCelebrityPrivateEventCity();		//随机一个城市
		if (info.cityId > 0)
		{
			++info.dailyTimes;
		}
	}
		//如果奖励事件已经开始
	City* city = sWorldMap.GetCity(info.cityId);
	//如果当前城池已经被占领，则取消这个城池的个人事件
	if(city && city->GetCountryId() != player->GetCountryId())
	{
		if(info.dailyTimes > 0)
			info.dailyTimes--;
		info.cityId = 0;
	}
	info.leftCnt = GET_BASE_DEF_UINT(pb::BD_ACTIVITY_VISIT_FAMOUS_MAX_TIME) - info.dailyTimes;
	OnCelebrityCityEventChange(player,info,event_id);
	PlayerMapLogicDB::UpdateCityEventToDb(*player, m_CelebrityCityEventLst.first, m_CelebrityCityEventLst.second);
#endif
}


void PlayerMapLogic::OnCelebrityCityEventChange(Player* player,const CityEventInfo& info,const uint32 event_id)
{
	pb::GS2C_CITY_EVENT sendmsg;
	SaveTo(*sendmsg.mutable_city_event(),event_id,info);
	sendmsg.set_ispublicevent(false);
	player->Send(pb::SMSG_CITY_EVENT, sendmsg);
}

void PlayerMapLogic::PackEventInfo(const MapEvent& eventTable,pb::CelebrityEvent& msg)
{
	SaveTo(msg, eventTable);
}

uint16 PlayerMapLogic::RandCelebrityPrivateEventCity()
{
	uint16 city_Id =  _RandPrivateEventCity();
	if(city_Id == 0)
	{
		city_Id = (uint16)sMapTableMgr.GetCapitalCity(m_countryId);
	}
	return city_Id;
}

void PlayerMapLogic::ClientHandleCelebrityCityEvent(Player* player, uint32 cityId, uint32 eventId, uint8 selectIdx, pb::GS2C_UseCurrencyToTakeRewardRsp& msg,IntPairVec& reward)
{
#ifdef _MMO_SERVER_
	CityEventInfo& Info = m_CelebrityCityEventLst.second;

	if (City* pCity = sWorldMap.GetCity(cityId))
	{
		if (pCity->IsFighting()) 
		{
			msg.set_result(pb::CityInFighting);
			return;
		}
	}
	else
	{
		msg.set_result(pb::NoCity);
		return;
	}
	std::set<uint64>::iterator it = Info.takePlayers.find(player->GetGuid());
	if(it != Info.takePlayers.end())
	{
		msg.set_result(pb::HasTaken);
		return;
	}
	const MapEvent* pEvent = sMapTableMgr.getCelebrityMapEvent(eventId);
	if (pEvent)
	{
		if(!sReward.Change(*player,pEvent->GetCost(selectIdx)))
		{
			msg.set_result(pb::CostFail);
			return;
		}
		else
		{
			Info.isBeginning = false;
			OnCelebrityCityEventChange(player,Info,m_CelebrityCityEventLst.first);
			Info.takePlayers.insert(player->GetGuid());
			if(pEvent->GetCost(selectIdx).first == Reward::Coin_Gold || pEvent->GetCost(selectIdx).first == Reward::RMB_Gold)
			{
				UpdateCelebrityCityEvent(player);
			}
			else
			{
				sAutoUpdateGame.AddEvent(boost::bind(&PlayerMapLogic::UpdateCelebrityCityEvent, this, player),GET_BASE_DEF_UINT(pb::BD_ACTIVITY_VISIT_FAMOUS_WAIT_TIME) * 60);
				m_CelebrityCityEventLst.second.endTime = sOS.TimeSeconds() + GET_BASE_DEF_UINT(pb::BD_ACTIVITY_VISIT_FAMOUS_WAIT_TIME) * 60;
			}
		}
		if(pEvent->SelectCelebrityRewardSize() > 0)
		{
			//给选择性资源
			if (selectIdx < pEvent->SelectCelebrityRewardSize())
			{
				sReward.Append(reward, pEvent->SelectCelebrityReward(selectIdx));
			}
			else
			{
				msg.set_result(pb::SelectError);
				return;
			}
		}
	}
	else
	{
		msg.set_result(pb::NoEvent);
		return;
	}
	msg.set_result(pb::AwardSuccess);
	player->UpdateAllDailyQuestInfo(CITY_EVENT,1,0);//每日任务
	PlayerMapLogicDB::UpdateCityEventToDb(*player, m_CelebrityCityEventLst.first, m_CelebrityCityEventLst.second);
#endif
}


//解锁迷雾关
void PlayerMapLogic::UnlockFog(Player& player, uint32 fogId)
{
	LLOG("Unlock Fog, fogid = %d",fogId);
    if (sMapTableMgr.GetMapFog(fogId))
    {
		LLOG("org fog m_ulockFogCityBit[%d]=%llu, or %llu",(fogId-1)/32,m_ulockFogCityBit[(fogId-1)/32],(1ULL << (((uint64)fogId-1)%32)));
        m_ulockFogCityBit[(fogId-1)/32] |= (1ULL << (((uint64)fogId-1)%32));
		LLOG("end fog m_ulockFogCityBit[%d]=%llu",(fogId-1)/32,m_ulockFogCityBit[((uint64)fogId-1)/32]);
        PlayerMapLogicDB::SendBaseToDb(player, *this);

		pb::GS2C_UNLOCK_FOGS msg;
		for(int i=0; i<ARRAY_SIZE(m_ulockFogCityBit); i++)
		{
			msg.add_unlock_fog_city(m_ulockFogCityBit[i]);
		}
		player.Send(pb::SMSG_UNLOCK_FOGS, msg);
		//PlayerMapLogicDB::SendBaseToDb(player, *this);

    }
}

//获取迷雾关信息，如果没有，则添加新的
void PlayerMapLogic::dealRequireFogInfo(Player& player, pb::C2GS_REQUEST_FOG_INFO& requires)
{
#ifdef _MMO_SERVER_

	bool isgetrec = false;		//是否已经记录了迷雾守卫

	pb::GS2C_FOG_INFO_Lst sendmsg;		//向玩家发送

	//将二进制转换为id
	std::vector<uint32> fog_ids;
	fog_ids.clear();
	for(int i=0; i<requires.fog_id_size(); i++)
	{
		for(uint32 j=0; j<32; j++)
		{
			if(((requires.fog_id(i)) & (1ULL<<j)) != 0)
			{
				fog_ids.push_back(i*32 + j + 1);
			}
		}
	}

	for(std::vector<uint32>::iterator ite = fog_ids.begin();
		ite != fog_ids.end(); ite++)
	{
		for(std::vector<FogInfo*>::iterator git = m_fogGuardians.begin();
			git != m_fogGuardians.end(); git++)
		{
			if((*git)->m_fogID == (*ite))
			{
				//NLOG("get one rec");
				//有迷雾守卫的记录
				isgetrec = true;
				pb::Struct_FOG_INFO* fogOne = sendmsg.add_fog_lst();
				fogOne->set_fog_id(*ite);
				fogOne->set_uniqueid((*git)->m_uniqueID);
				for(IntPairVec::iterator gInt = (*git)->m_fogGuardsInt.begin();
					gInt != (*git)->m_fogGuardsInt.end(); gInt++)
				{
					pb::PairValue* intp = fogOne->add_guard_hp();
					(*intp).set_first(gInt->first);
					(*intp).set_second(gInt->second);
				}
				for(IntPairVec::iterator mInt = (*git)->m_fogGuardsMaxhp.begin();
					mInt != (*git)->m_fogGuardsMaxhp.end(); mInt++)
				{
					pb::PairValue* intp = fogOne->add_guard_max();
					(*intp).set_first(mInt->first);
					(*intp).set_second(mInt->second);
				}
				fogOne->set_guard_up_hp((*git)->guard_all_hp);
				break;
			}
		}

		//如果没有记录，则创建记录
		if(!isgetrec)
		{
			//NLOG("no rec");
			if(const MapFog* fogmapone = sMapTableMgr.GetMapFog(*ite))
			{
				uint32 FubenStageOne = fogmapone->StageId();
				if(const FubenStage* fubenstage = sFubenDataMgr.GetStage(FubenStageOne))
				{
					uint64 uniqueid = (player.GetGuid() << 6) + (*ite);
					FogInfo* tmpFogGuardianOne = new FogInfo(*ite, uniqueid);

					pb::Struct_FOG_INFO* fogOne = sendmsg.add_fog_lst();
					fogOne->set_fog_id(*ite);
					fogOne->set_uniqueid(uniqueid);
					for(IntPairVec::const_iterator it = fubenstage->m_npcLst.begin();
						it != fubenstage->m_npcLst.end(); ++it)
					{
						Npc* pNpc = sNpcMgr.CreateNpc(it->first, it->second);
						MMO_ASSERT(pNpc);
						//NLOG("NPC Address :%d",pNpc);
						IntPair	gfogInt;
						gfogInt.first = it->first;
						gfogInt.second = pNpc->CurHp();
						tmpFogGuardianOne->m_fogGuardsInt.push_back(gfogInt);
						tmpFogGuardianOne->m_fogGuardsObj.insert(pNpc);
						//tmpFogGuardianOne->m_fogObj.insert(pNpc);
						tmpFogGuardianOne->m_fogGuardsMaxhp.push_back(gfogInt);

						pb::PairValue* intp = fogOne->add_guard_hp();
						intp->set_first(gfogInt.first);
						intp->set_second(gfogInt.second);

						pb::PairValue* intm = fogOne->add_guard_max();
						intm->set_first(gfogInt.first);
						intm->set_second(gfogInt.second);
						tmpFogGuardianOne->guard_all_hp += gfogInt.second;
					}
					fogOne->set_guard_up_hp(tmpFogGuardianOne->guard_all_hp);
					m_fogGuardians.push_back(tmpFogGuardianOne);
					//NLOG("write data one");
					//向数据库写入数据
				}
			}
		}

		isgetrec = false;
	}

	PlayerMapLogicDB::UpdateFogRec(player, *this);
	player.Send(pb::SMSG_FOG_INFO, sendmsg);

#endif
}

void PlayerMapLogic::EnterFog(Player& player, uint32 fog_id, vector<uint32>& heroIds)
{
#ifdef _MMO_SERVER_

	if(const MapFog* fogmapone = sMapTableMgr.GetMapFog(fog_id))
	{
		uint32 FubenStageOne = fogmapone->StageId();
		//if(player.GetLevel() < (int32)fogmapone->ShowLevel()) return;

		if(const FubenStage* fubenstage = sFubenDataMgr.GetStage(FubenStageOne))
		{
			for(std::vector<FogInfo*>::iterator git = m_fogGuardians.begin();
				git != m_fogGuardians.end(); git++)
			{
				if((*git)->m_fogID == fog_id)
				{
					if((*git)->m_fogGuardsObj.empty()) return;

					//有迷雾守卫的记录

					std::vector<Combat::CombatObj*> attack;	//玩家进攻
					for (vector<uint32>::iterator it = heroIds.begin(); it != heroIds.end(); ++it)
					{
						Character* pHero = player.m_characterStorage->MutableCharacter(*it);
						MMO_ASSERT(pHero);
						if (pHero) {
							attack.push_back(pHero);
						}
					}

					if (Combat::CombatGroup* pGroup = Combat::CombatGroup::CreateGroup(attack, (*git)->m_fogGuardsObj, Combat::CT_Fog,fubenstage->LandForm()))
					{
						//战斗结束回调OnStageEnd
						pGroup->Watch(player.GetGuid());	//玩家标识符
						pb::GS2C_Combat_Pre_Begin msg;
						pGroup->SaveTo(msg);
						msg.set_isautocombat(player.IsAutoCombat());
						player.Send(pb::SMSG_COMBAT_PRE_BATTLE, msg);
						pGroup->AddCallBack_OnEnd(boost::bind(&PlayerMapLogic::OnFogEnd, this, &player, fog_id, _1, _2));
						pGroup->AddCallBack_OnNpcDead(boost::bind(&PlayerMapLogic::OnNpcDead, this, &player, fog_id, _1, _2));
					}

				}
			}

		}
	}

#endif
	return;
}

void PlayerMapLogic::OnNpcDead(Player* player, uint32 fogId, Combat::CombatGroup* pGroup, Combat::CombatObj* obj)
{
#ifdef _MMO_SERVER_

	for(std::vector<FogInfo*>::iterator fogit = m_fogGuardians.begin();
		fogit != m_fogGuardians.end(); fogit++)
	{
		if((*fogit)->m_fogID == fogId)
		{
			if(!(*fogit)->m_fogGuardsObj.empty())
			{
				IntPairVec::iterator IntObj = (*fogit)->m_fogGuardsInt.begin();
				for(std::set<Combat::CombatObj*>::iterator objIte = (*fogit)->m_fogGuardsObj.begin();
					objIte != (*fogit)->m_fogGuardsObj.end(); objIte++, IntObj++)
				{
					if(obj == (*objIte))
					{
						(*fogit)->m_fogGuardsInt.erase(IntObj);
						(*fogit)->m_fogGuardsObj.erase(objIte);
						break;
					}
				}
			}
			break;
		}
	}
	//向数据库写入数据
	PlayerMapLogicDB::UpdateFogRec(*player, *this);

#endif
}

//迷雾站结束，没有过关奖励，只有士兵的经验金币等奖励
void PlayerMapLogic::OnFogEnd(Player* player, uint32 fogId, Combat::CombatGroup* pGroup, bool isAttackWin)
{
#ifdef _MMO_SERVER_
	const MapFog* fogmapone = sMapTableMgr.GetMapFog(fogId);
	uint32 FubenStageOne = fogmapone->StageId();
	const FubenStage* pStageTable = sFubenDataMgr.GetStage(FubenStageOne);
	if (pStageTable == NULL) return;


	IntPairVec reward;
	if(isAttackWin)
	{
		UnlockFog(*player,fogId);
		//给过关奖励
		pStageTable->GetReward(reward);
		sReward.Change(*player, reward);
		if(player->IsOpenFogIsActive())
		{
			player->AddValue(pb::PLAYER_FIELD_OPEN_FOG_COUNT,1);
		}
	}
	else
	{
		pb::GS2C_FOG_INFO_Lst sendmsg;
		for(std::vector<FogInfo*>::iterator git = m_fogGuardians.begin();
			git != m_fogGuardians.end(); git++)
		{
			if((*git)->m_fogID == fogId)
			{
				pb::Struct_FOG_INFO* fogOne = sendmsg.add_fog_lst();
				fogOne->set_fog_id(fogId);
				fogOne->set_uniqueid((*git)->m_uniqueID);
				for(IntPairVec::iterator gInt = (*git)->m_fogGuardsInt.begin();
					gInt != (*git)->m_fogGuardsInt.end(); gInt++)
				{
					pb::PairValue* intp = fogOne->add_guard_hp();
					(*intp).set_first(gInt->first);
					(*intp).set_second(gInt->second);
				}
				for(IntPairVec::iterator mInt = (*git)->m_fogGuardsMaxhp.begin();
					mInt != (*git)->m_fogGuardsMaxhp.end(); mInt++)
				{
					pb::PairValue* intp = fogOne->add_guard_max();
					(*intp).set_first(mInt->first);
					(*intp).set_second(mInt->second);
				}
				fogOne->set_guard_up_hp((*git)->guard_all_hp);
				break;
			}
		}
		player->Send(pb::SMSG_FOG_INFO, sendmsg);
	}


	//通知client
	struct stFunc {
		bool                 _isAttackWin;
		Combat::CombatGroup* _pGroup;
		IntPairVec           _reward;
		uint64               _fubenOwnerId;

		stFunc(bool isAttackWin, Combat::CombatGroup* pGroup,IntPairVec reward, uint64 fubenOwnerId)
			: _isAttackWin(isAttackWin)
			, _pGroup(pGroup)
			, _reward(reward)
			, _fubenOwnerId(fubenOwnerId)
		{}
		void operator()(uint64 playerId)
		{
			if (Player* player = sPlayerPool.GetByPlayerId(playerId))
			{
				NLOG("################# send awards #############");
				pb::GS2C_Combat_End msg;
				msg.set_is_attack_win(_isAttackWin);
				_pGroup->SaveCombatInfosOfGains(msg, playerId);
				msg.set_group_id(_pGroup->m_unique_id);
				if (playerId == _fubenOwnerId) PackRewardMsg(_reward, msg);
				player->Send(pb::SMSG_COMBAT_COMBAT_END, msg);
			}
		}
	};
	stFunc func(isAttackWin, pGroup, reward, player->GetGuid());
	pGroup->ForEachWatchPlayer(func);
	//pGroup->SaveAllStaticData();

	//struct _Temp_Hero_Func //辅助函数对象
	//{
	//	bool operator()(Character* p)
	//	{
	//		p->CalculateAttr();
	//		return false;
	//	}
	//};
	//_Temp_Hero_Func objFunc;
	//player->m_characterStorage->ForEachBattleCharacterInCombat(objFunc,pGroup->m_unique_id);

	//同步迷雾副本守卫信息
	for(std::vector<FogInfo*>::iterator fogit = m_fogGuardians.begin();
		fogit != m_fogGuardians.end(); fogit++)
	{
		if((*fogit)->m_fogID == fogId)
		{
			(*fogit)->m_fogGuardsInt.clear();
			(*fogit)->m_fogGuardsMaxhp.clear();
			if(!(*fogit)->m_fogGuardsObj.empty())
			{
				for(std::set<Combat::CombatObj*>::iterator objIte = (*fogit)->m_fogGuardsObj.begin();
					objIte != (*fogit)->m_fogGuardsObj.end(); objIte++)
				{
					IntPair pint;
					NLOG("obj Address :%d",(*objIte));
					//Npc* npcobj = dynamic_cast<::Npc*>(*objIte);
					pint.first = (int)((*objIte)->GetTableId());
					pint.second = (*objIte)->CurHp();
					(*fogit)->m_fogGuardsInt.push_back(pint);

					IntPair mInt;
					mInt.first = (int)(*objIte)->GetTableId();
					mInt.second = ((*objIte)->stPro.rowSoldierMaxCnt) * (int)((*objIte)->GetMaxRow());
					(*fogit)->m_fogGuardsMaxhp.push_back(mInt);
				}
			}
		}
	}
	//向数据库写入数据
	PlayerMapLogicDB::UpdateFogRec(*player, *this);
#endif
}

bool PlayerMapLogic::TondenBegin(Player& player, Character& hero) //屯粮
{
	uint32 icomLimitSum = player.GetBuildingIncomeLimit(MODULE_LONGCHANG);
	int tmpnum = player.GetCurrency(eFood);
	if((int)icomLimitSum <= tmpnum)
		return false;

    //uint8 heroIdx = hero.GetID();
	uint8 heroIdx = hero.GetID();
    m_tondenHeroBeginTime.insert(std::make_pair(heroIdx, sOS.TimeSeconds()));
    PlayerMapLogicDB::SendBaseToDb(player, *this);
	hero.SetIsTunTian(true);
	return true;
}
void PlayerMapLogic::TondenEnd(Player& player, Character& hero) //屯粮
{
    //uint8 heroIdx = hero.GetID();
	uint8 heroIdx = hero.GetID();
    std::map<uint8, uint64>::iterator it = m_tondenHeroBeginTime.find(heroIdx);
    if (it == m_tondenHeroBeginTime.end()) return;

    int diffSec = (int)MIN(sOS.TimeSeconds() - it->second, Tonden_Last_Time_Max);

	if(Country* country = sWorldMap.GetCountry(m_countryId))
	{
		if(const MapTunTian* tuntianinfo = sMapTableMgr.GetMapTuntian(country->m_tuntianLevel))
		{
			int rewardNum = (diffSec / Tonden_CD) * player.GetLevel() * tuntianinfo->GetAwardFactor();
			sReward.Change(player, Reward::Food, rewardNum);

			//SMSG_TUNTIAN_REWARD
			if(rewardNum > 0)
			{
				pb::GS2C_TunTianReward sendmsg;
				sendmsg.set_food_count(rewardNum);
				player.Send(pb::SMSG_TUNTIAN_REWARD, sendmsg);
			}
		}
		else
			NLOG("Cant Find tuntianinfo");
	}
	else
		NLOG("Cant Find Country");
	


    m_tondenHeroBeginTime.erase(heroIdx);
    PlayerMapLogicDB::SendBaseToDb(player, *this);
	hero.SetIsTunTian(false);
}

//请求练兵信息
void PlayerMapLogic::FillTrainInfo(Player& player, pb::GS2C_Train_Info& msg, uint32 trainIdx /* = 0 */)
{
	uint64 nowtime = sOS.TimeSeconds();
	bool isSuccess = false;
	const CharacterIds battleArray = player.m_characterStorage->GetCharIDLst();
	
	//NLOG("PlayerMapLogic::FillTrainInfo,idx=%d",trainIdx);

	if(trainIdx != 0)
	{
		int costTroops = 0;
		CharacterIds::const_iterator heroidx = battleArray.begin();
		for(; heroidx != battleArray.end(); heroidx++)
		{
			if (Character* character = player.m_characterStorage->MutableCharacter(*heroidx))
			{
				if(const MapCity* table = sMapTableMgr.GetMapCity(character->GetCurCity()))
				{
					if(table->IsTonden())
					{
						TondenEnd(player,*character);
						costTroops++;
					}
				}
			}
		}

		switch(trainIdx)
		{
		case 1:
			if(sReward.Change(player, Reward::TrainToken_Normal, -costTroops))
			{
				m_trainingStartTime = nowtime;
				m_trainType = trainIdx;
				isSuccess = true;
			}
			break;
		case 2:
			if(sReward.Change(player, Reward::TrainToken_Fight, -costTroops))
			{
				m_trainingStartTime = nowtime;
				m_trainType = trainIdx;
				isSuccess = true;
			}
			break;
		case 3:
			if(sReward.Change(player, Reward::TrainToken_Double, -costTroops))
			{
				m_trainingStartTime = nowtime;
				m_trainType = trainIdx;
				isSuccess = true;
			}
			break;
		case 4:
			if(sReward.Change(player, Reward::TrainToken_Reduce, -costTroops))
			{
				m_trainingStartTime = nowtime;
				m_trainType = trainIdx;
				isSuccess = true;
			}
			break;
		default:
			isSuccess = false;
			break;
		}
	}

	if(isSuccess)
	{
		m_trainHeros.clear();
		CharacterIds::const_iterator heroidx = battleArray.begin();
		for(; heroidx != battleArray.end(); heroidx++)
		{
			if (Character* character = player.m_characterStorage->MutableCharacter(*heroidx))
			{
				if(const MapCity* table = sMapTableMgr.GetMapCity(character->GetCurCity()))
				{
					if(table->IsTonden())
					{
						character->isInTraining = true;
						m_trainHeros.push_back(character->GetID());
					}
					else
						character->isInTraining = false;
				}
			}
		}
	}

	if(m_trainingStartTime + Train_Last_Time > nowtime)		//练兵时间没有结束
	{
		msg.set_istraining(true);
		if(m_trainHeros.size()>0)
		{
			for(std::vector<uint32>::iterator it = m_trainHeros.begin();
				it!= m_trainHeros.end(); it++)
			{
				msg.add_herosid(*it);
			}
		}
		msg.set_trainendtime(m_trainingStartTime + Train_Last_Time);
		msg.set_traintype(m_trainType);
	}
	else
	{
		msg.set_istraining(false);
		msg.set_trainendtime(0);
		msg.set_traintype(0);
	}
}

void PlayerMapLogic::TrainFinish(Player* player, pb::GS2C_Train_Info& msg)
{
	uint64 nowtime = sOS.TimeSeconds();
	if(m_trainType == 0 || m_trainType > 4)
	{
		NLOG("no heros in training");
		m_trainType = 0;
		return;
	}

	if(m_trainingStartTime + Train_Last_Time > nowtime )	//训练没有结束
	{
		NLOG("train is not over");
		return;
	}

	int expUpFactor = GET_BASE_DEF_INT(pb::BD_TRAINING_GET_EXP_FACTOR);		//经验提升系数
	switch(m_trainType)
	{
	case 1:		//普通练兵令	 国战经验提高50%
		for(std::vector<uint32>::iterator it = m_trainHeros.begin();
			it!= m_trainHeros.end(); it++)
		{
			if (Character* character = player->m_characterStorage->MutableCharacter(*it))
			{
				character->resetTrainAdditionUp();
				character->expPercent = 50;
				character->isInTraining = false;
				//增加武将经验
				int expadd = (int)character->GetLevel() * (int)(expUpFactor/100);
				character->TryAddXP(expadd);
				pb::hero_exp_get* expmsg = msg.add_expget();
				expmsg->set_chatid(*it);
				expmsg->set_exp(expadd);
			}
		}
		break;
	case 2:		//战力练兵令
		for(std::vector<uint32>::iterator it = m_trainHeros.begin();
			it!= m_trainHeros.end(); it++)
		{
			if (Character* character = player->m_characterStorage->MutableCharacter(*it))
			{
				character->resetTrainAdditionUp();
				character->expPercent = 50;
				character->fightPercent = 50;
				character->isInTraining = false;
				//增加武将经验
				int expadd = (int)character->GetLevel() * (int)(expUpFactor/100);
				character->TryAddXP(expadd);
				pb::hero_exp_get* expmsg = msg.add_expget();
				expmsg->set_chatid(*it);
				expmsg->set_exp(expadd);
			}
		}
		break;
	case 3:		//双倍击杀练兵令
		for(std::vector<uint32>::iterator it = m_trainHeros.begin();
			it!= m_trainHeros.end(); it++)
		{
			if (Character* character = player->m_characterStorage->MutableCharacter(*it))
			{
				character->resetTrainAdditionUp();
				character->expPercent = 50;
				character->doubleKill = 100;
				character->isInTraining = false;
				//增加武将经验
				int expadd = (int)character->GetLevel() * (int)(expUpFactor/100);
				character->TryAddXP(expadd);
				pb::hero_exp_get* expmsg = msg.add_expget();
				expmsg->set_chatid(*it);
				expmsg->set_exp(expadd);
			}
		}
		break;
	case 4:		//减耗练兵令
		for(std::vector<uint32>::iterator it = m_trainHeros.begin();
			it!= m_trainHeros.end(); it++)
		{
			if (Character* character = player->m_characterStorage->MutableCharacter(*it))
			{
				character->resetTrainAdditionUp();
				character->expPercent = 50;
				character->reduceCostHP = 50;
				character->isInTraining = false;
				//增加武将经验
				int expadd = (int)character->GetLevel() * (int)(expUpFactor/100);
				character->TryAddXP(expadd);
				pb::hero_exp_get* expmsg = msg.add_expget();
				expmsg->set_chatid(*it);
				expmsg->set_exp(expadd);
			}
		}
		break;
	default:
		return;
		break;
	}

	m_trainType = 0;
	m_trainingStartTime = 0;
	m_trainHeros.clear();
	m_trainingEndTime = sOS.TimeMS();
	player->UpdateAllDailyQuestInfo(TUNTIAN_TRAINING,1,0);//每日任务

#ifdef _MMO_SERVER_
	int lasttime = GET_BASE_DEF_INT(pb::BD_TRAINING_BUFF_LAST_TIME);
	sAutoUpdateGame.AddEvent(boost::bind(&PlayerMapLogic::AdditionUpEnd, this, player, m_trainingEndTime),lasttime * 60);
#endif
}

void PlayerMapLogic::AdditionUpEnd(Player* player,uint64 time) 
{
	if(m_trainingEndTime == time)
	{
		const CharacterIds battleArray = player->m_characterStorage->GetCharIDLst();

		CharacterIds::const_iterator heroidx = battleArray.begin();
		for(; heroidx != battleArray.end(); heroidx++)
		{
			if (Character* character = player->m_characterStorage->MutableCharacter(*heroidx))
			{
				character->TrainAddtionUpEnd();
			}
		}
	}
}

void PlayerMapLogic::TrainInstant(Player* player, pb::GS2C_Train_Info& msg)
{
	int costgold = GET_BASE_DEF_INT( pb::BD_ACCELERATE_TRAINING_COST) * m_trainHeros.size();
	if(player->EnoughCurrency(eSysGold,costgold))
	{
		player->TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,costgold);
		m_trainingStartTime = 0;
		TrainFinish(player,msg);
	}
}

void PlayerMapLogic::TondenTrainBegin(Player& player, Character& hero, uint32 tokenItemId) //练兵
{
    TondenEnd(player, hero);

    if (sReward.Change(player, tokenItemId, -1)) {
#ifdef _MMO_SERVER_

#endif
    }
}

void PlayerMapLogic::TondenDonateMoney(Player& player, pb::GS2C_TunTian_Donate_Ret& msg)
{
	uint32 donatecost = GET_BASE_DEF_INT( pb::BD_TUNTIAN_COST_SILVER);	//屯田消耗
	uint32 donateexp = GET_BASE_DEF_INT(pb::BD_TUNTIAN_GET_EXP);	//捐赠一次获得经验
	uint64 nowtime = sOS.TimeSeconds() ;

	if(m_tondenCDEndTime <= nowtime)	//没有cd
		m_tondenCDEndTime = nowtime;

	uint64 timediff = m_tondenCDEndTime - nowtime;
	if(timediff >= 60*7)		//cd中
	{
		msg.set_donateret(2);
	}
	else
	{
		if (Country* country = sWorldMap.GetCountry(m_countryId))
		{
			if(const MapTunTian* tuntianinfo = sMapTableMgr.GetMapTuntian(country->m_tuntianLevel))
			{
				if(sReward.Change(player, Reward::Coin_Silver, -((int)donatecost)))	//银币足够
				{

					uint32 needExp = tuntianinfo->GetExp();
					if(needExp>0)
					{
						country->m_tuntianExp += 1;
						if(country->m_tuntianExp >= needExp)
						{
							country->m_tuntianLevel += 1;
							country->m_tuntianExp = 0;
						}
						sReward.Change(player, Reward::PlayerExp, donateexp);
						m_tondenCDEndTime += 60;
						msg.set_donateret(0);
						msg.set_tuntianlevel(country->m_tuntianLevel);
						msg.set_tuntianexp(country->m_tuntianExp);
						PlayerMapLogicDB::SendBaseToDb(player, *this);
						CountryDB::SendBaseToDb(*country);
						player.UpdateAllDailyQuestInfo(DONATE_TUNTIAN,1,0);//每日任务
						NLOG("tonden level = %d",country->m_tuntianLevel);
					}
					else
						msg.set_donateret(3);

				}
				else
					msg.set_donateret(1);
			}
			else
				msg.set_donateret(3);
		}
		else
			msg.set_donateret(3);
	}
	msg.set_cdendtime(m_tondenCDEndTime);
}

void PlayerMapLogic::ClearCDorFresh(Player& player, bool isClear, pb::GS2C_Clear_Tuntian_CD_Ret& msg)
{
	bool retSucc = false;

	if (Country* country = sWorldMap.GetCountry(m_countryId))
	{
		if(isClear)		//清除CD
		{
			int costgold = GET_BASE_DEF_INT( pb::BD_TUNTIAN_CLEAR_CD_COST_GOLD);
			if(player.EnoughCurrency(eSysGold,costgold))
			{
				player.TryDeductCurrency(pb::IR_CLEAR_TONDENCD_COST,eSysGold,costgold);
				retSucc = true;
				m_tondenCDEndTime = sOS.TimeSeconds();
			}

		}
		else		//刷新信息
		{
			retSucc = true;
		}
		msg.set_cdendtime(m_tondenCDEndTime);
		msg.set_tuntianexp(country->m_tuntianExp);
		msg.set_tuntianlevel(country->m_tuntianLevel);
		NLOG("tonden level = %d",country->m_tuntianLevel);
	}
	msg.set_isclearsucc(retSucc);

}

void PlayerMapLogic::SendMonsterAllInfo(Player& player,pb::MonsterCityLst& msg)
{
	if(Country* myCountry = sWorldMap.GetCountry(player.GetCountryId()))
	{
		myCountry->m_MonsterCity.SaveTo(msg);
	}
}

bool PlayerMapLogic::MonsterDonate(Player& player, uint32 dest_countryId,pb::GS2C_Monster_City_Donate& msg)
{
    if (Country* dest_country = sWorldMap.GetCountry(dest_countryId))
    {
		Country* myCountry = sWorldMap.GetCountry(player.GetCountryId());
		if(myCountry && myCountry->m_MonsterCity.Donate(player,*myCountry,*dest_country,msg))
		{
			CountryDB::SendBaseToDb(*myCountry);
			return true;
		}
		else
		{
			return false;
		}
    }
    return false;
}
bool PlayerMapLogic::ClearMonsterDonateCD(Player& player)
{
    return MonsterCity::ClearDonateCD(player);
}

void PlayerMapLogic::ClearMonsterCityInvadeCount(const uint32 country_id)
{
	if (Country* country = sWorldMap.GetCountry(country_id))
	{
		return country->m_MonsterCity.RefreshTodayInvadeCnt(country_id);
	}
}

bool PlayerMapLogic::TakeFogReward( const uint32 id,IntPair* pair_reward )
{
	return sMapTableMgr.GetFogReward(id,pair_reward);
}