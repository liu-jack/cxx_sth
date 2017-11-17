#include "city.h"
#include "../Table/MapTableMgr.h"
#include "../Table/MapCity.h"
#include "../../object/Player.h"
#include "../../Combat/CombatGroup.h"
#include "../../Combat/CombatObj.h"
#include "../../Combat/combat_define.h"
#include "../../Npc/Npc.h"
#include "../../character/Character.h"
#include "../PlayerMapLogic.h"
#include "City.pb.h"
#include "Opcode.pb.h"
#include "../country/country.h"
#include "../../Npc/NpcMgr.h"
#include "Combat.pb.h"
#include "../../session/PlayerPool.h"
#include "System.h"
#include "../Table/MapCountry.h"
#include "../base/BaseMap.h"
#include "../WorldMap.h"
#include "BaseDefine.pb.h"
#include "BaseDefineMgr.h"
#include "../../Technology/PlayerTechnology.h"
#include "../../Technology/Table/TechnologyTableMgr.h"
#include "Reinforcement/Reinforce.h"
#include "Social.pb.h"
#include "chat/ChatSpeaker.h"
#ifdef _MMO_SERVER_
#include "../../ActivityCelebrity/Table/ActivityCelebrityTableMgr.h"
#include "../../WorldFightAchievement/WorldFightAchievement.h"
#endif
#include "../../Combat/CombatObj.h"
#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"
//#include "../../Combat/CombatGroup.h"
#include "Activity/Personal/PersonalTask.h"
#include "Activity/OccupyCity/OccupyCity.h"
#include "Activity/Defence/DefenceBorderland.h"
#include "Activity/ActMgr.h"
#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossActivity/CrossDestroy/CrossDestroy.h"
#include "server_cross/cross_wall/CrossWorldMap.h"
#endif
#include "memory_buffer/NetPack.h"
using namespace pb;


City::City(uint32 id_, uint32 countryId, CMap& map)
    : id(id_)
	, m_map(map)
	, m_status(Normal)
    , m_countryId(countryId)
    , m_combatGroup(NULL)
{
    //STATIC_ASSERT(sizeof(m_status) * 8 >= _Status_Cnt, BitsTooShort);
}
void City::Init()
{
	//NLOG("City::Init ");
	if (sMapTableMgr.GetMapCity(id)->IsCapital())
	{
		m_status |= (1 << ForbidEnemy);
	}
	_AddDefenderNpc();

	m_country_call_up.clear();
	m_country_monster_call.clear();

#ifdef _SERVER_CROSS_
	//跨服服务器初始化有不同

	//首都初始化耐久度
	if(IsCapitalCity())
		m_durability = 100;
	else
		m_durability = 0;
#endif
}
void City::SaveTo(pb::Struct_City_Info& msg)
{
	//NLOG("City::SaveTo countryid= %d, cityid= %d",m_countryId,id);
    msg.set_city_id(id);
    msg.set_country(m_countryId);
    msg.set_status(m_status); //FIXME：重启后战斗就没了，状态无法保持，所以不能入库
    msg.set_combat_group_id(m_combatGroup ? m_combatGroup->m_unique_id : 0);
	//msg.set_callup_end_time(m_CallUpEndTime);
	//msg.set_country_call_up(m_country_call_up);
	//msg.set_m_unique_callid(m_unique_call_idx);

	for(std::vector<CallUpRec>::iterator it = m_country_call_up.begin(); it != m_country_call_up.end(); it++)
	{
		pb::callUPInfo* callinfoone = msg.add_m_call_info();
		callinfoone->set_callup_end_time(it->call_time);
		callinfoone->set_country_call_up(it->countryid);
		callinfoone->set_m_unique_callid(it->call_unique_id);
	}

	for(std::vector<CallUpRec>::iterator ite = m_country_monster_call.begin(); ite != m_country_monster_call.end(); ite++)
	{
		pb::callUPInfo* callinfoone = msg.add_m_monster_call_info();
		callinfoone->set_callup_end_time(ite->call_time);
		callinfoone->set_country_call_up(ite->countryid);
		callinfoone->set_m_unique_callid(ite->call_unique_id);
	}

	std::set<Combat::CombatObj*>::iterator it = m_defender.begin();
	for(; it != m_defender.end(); it++)
	{
		if (Npc* npc = dynamic_cast<Npc*>(*it))
		{
			if(npc->isExpedition)
			{
				ExpeditionNpc* expmsg = msg.add_exnpcs();
				expmsg->set_country_id(npc->GetCountryId());
				expmsg->set_npc_table_id(npc->GetTableId());
				expmsg->set_expeid(npc->expeid);
			}
		}
	}

	if(m_combatGroup)
	{
		if(m_combatGroup->m_group_attack.size()>0)
		{
			Combat::GroupLst::iterator ite = m_combatGroup->m_group_attack.begin();
			for(; ite != m_combatGroup->m_group_attack.end(); ite++)
			{
				if (Npc* npc = dynamic_cast<Npc*>(*ite))
				{
					if(npc->isExpedition)
					{
						ExpeditionNpc* expmsg = msg.add_exnpcs();
						expmsg->set_country_id(npc->GetCountryId());
						expmsg->set_npc_table_id(npc->GetTableId());
						expmsg->set_expeid(npc->expeid);
					}
				}
			}
		}
	}
}
void City::LoadFrom(const pb::Struct_City_Info& msg)
{
	//NLOG("City::LoadFrom before countryid= %d, after countryid= %d , cityid = %d",m_countryId,msg.country(),id);
	if(m_countryId != msg.country())
	{
		m_countryId = msg.country();
		_DelDefenderNpc();
		_AddDefenderNpc();
	}
    //m_status = msg.status();
}
void City::SetStatus(EStatus e, bool b, uint64 timeCallUp, uint32 countryid)
{
	//NLOG("City::SetStatus id=%d, m_status=%d,e=%d,b=%d",id,m_status,e,b);
	const bool oldStatus = HaveStatus(e);
    if (b)
        m_status |= (1 << e);   // 将n为置1
    else
        m_status &= ~(1 << e);  // 将n位置0

    if (e == Fighting) {
        //m_map.m_cityGraph.UpdateCityPath(m_map);
    }

	//if(e == CallUp && b)	//是征召
	//{
	//	m_lastCallUpTime = timeCallUp;
	//	m_CallUpEndTime = m_lastCallUpTime + GET_BASE_DEF_INT(pb::BD_DRAFT_MAX_LAST_TIME)*60;
	//}

	//if(e == CallUp && !b)
	//{
	//	m_country_call_up = 0;
	//}

	if(e == MonsterToken)
		LLOG("change state ,monster to %d",b);
	
	if (oldStatus != b) {
		pb::GS2C_City_Update_Info msg;
		SaveTo(*msg.mutable_city());
		if(e == MonsterToken || e == CallUp)
			sPlayerPool.BroadcastMsgToAll(pb::SMSG_CITY_UPDATE_INFO, msg);
		else
			m_map.Broadcast(pb::SMSG_CITY_UPDATE_INFO, msg);
		
	}
	//NLOG("City::SetStatus after m_status=%d",m_status);
}

void City::ClearCallUp()
{
	SetStatus(CallUp,false);
}

void City::CallUpEndCallBack(uint64 uniqueid)
{
	for(std::vector<CallUpRec>::iterator it = m_country_call_up.begin(); it != m_country_call_up.end(); it++)
	{
		if(it->call_unique_id == uniqueid)
		{
			m_country_call_up.erase(it);
			break;
		}
	}

	if(m_country_call_up.size() == 0)
		SetStatus(CallUp,false);

}

bool City::IsCallUpTokenInLst(uint64 unique_call_id)
{
	for(std::vector<CallUpRec>::iterator it = m_country_call_up.begin(); it != m_country_call_up.end(); it++)
	{
		if(it->call_unique_id == unique_call_id)
		{
			return true;
		}
	}
	return false;
}

uint64 City::GetCallUpUniqueid(uint32 countryid)
{
	for(std::vector<CallUpRec>::iterator it = m_country_call_up.begin(); it != m_country_call_up.end(); it++)
	{
		if(it->countryid == countryid)
		{
			return it->call_unique_id;
		}
	}
	return 0;
}

bool City::IsMonsterTokenInLst(uint64 unique_call_id)
{
	for(std::vector<CallUpRec>::iterator it = m_country_monster_call.begin(); it != m_country_monster_call.end(); it++)
	{
		if(it->call_unique_id == unique_call_id)
		{
			return true;
		}
	}
	return false;
}

uint64 City::GetMonsterCallUniqueid(uint32 countryid)
{
	for(std::vector<CallUpRec>::iterator it = m_country_monster_call.begin(); it != m_country_monster_call.end(); it++)
	{
		if(it->countryid == countryid)
		{
			return it->call_unique_id;
		}
	}
	return 0;
}

void City::MonsterCallEndCallBack(uint64 uniqueid)
{
	for(std::vector<CallUpRec>::iterator it = m_country_monster_call.begin(); it != m_country_monster_call.end(); it++)
	{
		if(it->call_unique_id == uniqueid)
		{
			m_country_monster_call.erase(it);
			break;
		}
	}

	if(m_country_monster_call.size() == 0)
		SetStatus(MonsterToken,false);
}

bool City::HaveStatus(EStatus e)
{
	//NLOG("City::HaveStatus id=%d m_status=%d",id,m_status);
    return (m_status & (1 << e)) != 0;
}

void City::AddCallUpCountry(uint32 countryid, uint64 timeCallUp, uint64 unique_call_id)
{
	CallUpRec callOne;
	callOne.countryid = countryid;
	callOne.call_time = timeCallUp;
	callOne.call_unique_id = unique_call_id;
	m_country_call_up.push_back(callOne);

	//通知所有自动国战的玩家
	FOREACH_PLAYERPTR(playerId, playerPtr)
	{
		if(playerPtr->GetCountryId() == countryid)	//是自己国家
		{
			if(playerPtr->m_AutoCountryWarLog->GetAutoCountryWar())	//有自动战斗
			{
				if(playerPtr->m_AutoCountryWarLog->GetAutoCallUp())	//有自动征召
				{
					playerPtr->m_AutoCountryWarLog->SetCallUpCity(id, unique_call_id);
					playerPtr->m_AutoCountryWarLog->SetDesCityId(id);
				}
			}
		}
	}
}

void City::AddMonsterCallCountry(uint32 countryid, uint64 timeMonsterCall, uint64 unique_call_id)
{
	CallUpRec callOne;
	callOne.countryid = countryid;
	callOne.call_time = timeMonsterCall;
	callOne.call_unique_id = unique_call_id;
	m_country_monster_call.push_back(callOne);
}

bool City::IsFighting()
{
    return (m_status & (1 << Fighting)) != 0;
}

bool City::isRushLegal(bool isAttacker,const int solider_times)
{
	if(m_combatGroup)
	{
		if(isAttacker)
			return ((m_combatGroup->m_group_attack.size() >= m_combatGroup->m_group_defence.size()*solider_times) ? true:false);
		else
			return ((m_combatGroup->m_group_defence.size() >= m_combatGroup->m_group_attack.size()*solider_times) ? true:false);
	}
	return false;
}

bool City::IsCapitalCity()
{
	const MapCity* table = sMapTableMgr.GetMapCity(id);
	return table->IsCapital();
}
bool City::IsTonden()
{
	const MapCity* table = sMapTableMgr.GetMapCity(id);
	return table->IsTonden();
}
bool City::IsFortressCity()
{
	const MapCity* table = sMapTableMgr.GetMapCity(id);
	return table->IsFortress();
}
bool City::IsEncircled()
{
    return (m_status & (1 << Encircled)) != 0;
}
bool City::IsCountryEdge(uint32 checkCID/* = 9*/)
{
    // 相连城池是否为敌国
    const MapCity* table = sMapTableMgr.GetMapCity(id);
	if(table->IsCapital() || table->IsTonden())		//首都或者屯田区
		return false;

    for (size_t i = 0; i < table->m_linkCity.size(); ++i)
    {
        const uint16 linkId = table->m_linkCity[i];
        City* pCity = m_map.GetCity(linkId);
		if(checkCID != 9)
		{
			if (pCity && pCity->GetCountryId() != checkCID)
			{
				return true;
			}
		}
		else
		{
			if (pCity && pCity->GetCountryId() != m_countryId)
			{
				return true;
			}
		}
    }
    return false;
}

bool City::HaveCountryEdge(uint32 countryid)
{
	const MapCity* table = sMapTableMgr.GetMapCity(id);
	if(GetCountryId() != countryid)
	{
		for (size_t i = 0; i < table->m_linkCity.size(); ++i)
		{
			const uint16 linkId = table->m_linkCity[i];
			City* pCity = m_map.GetCity(linkId);
			if(pCity->GetCountryId() != countryid && pCity->GetCountryId() != GetCountryId())
			{
				return true;
			}
		}
	}
	return false;
}

bool City::IsMonsterCity()
{
    if (&m_map == &sWorldMap)
    {
        return m_countryId == MONSTER_COUNTRY_ID;
    }
    return false;
}
bool City::CanPass()
{
	if (IsMonsterCity()) return false;

    return !IsFighting();
}

bool City::CanEnter(Combat::CombatObj& obj)
{
	//暂时屏蔽蛮族城池的概念
    //if (IsMonsterCity() && !HaveStatus(Open_Monster_City)) return false;

    if (HaveStatus(ForbidEnemy))
    {
        if (m_countryId != obj.GetCountryId()) return false;
    }

    return true;
}
bool City::CanExit(Combat::CombatObj& obj)
{
	if (obj.IsDead()) return true;

    if (IsFighting())
    {
        return false;
    }
    return true;
}
void City::NpcEnter(Npc& npc)
{
	//NLOG("City::NpcEnter");
    _AddCombatObj(npc, npc.GetCountryId());
}


void City::NpcExit(Npc& npc)
{
	//LLOG("[1003] NPC Exit City %d",id);
    _DelCombatObj(npc, npc.GetCountryId());

    _Trap(npc);
}
void City::HeroEnter(Player& player, Character& hero)
{
	//NLOG("City::HeroEnter countryid= %d, cityid= %d",m_countryId,id);
    //进入屯田区，粮食未满则自动屯田
    //const MapCity* table = sMapTableMgr.GetMapCity(id);
    //if (table->IsTonden())
    //{
    //    player.m_worldMapLogic->TondenBegin(player, hero);
    //}

    _AddCombatObj(hero, hero.GetCountryId());
// 	pb::GS2C_Combat_Call_Up msg;
// 	msg.set_result(HERO_IN_CITY);
// 	m_combatGroup->Broadcast(SMSG_CITY_UPDATE_INFO,msg);
// 	
    //TODO：广播给所该城池的玩家
}
void City::HeroExit(Player& player, Character& hero)
{
    //退出屯田区
    const MapCity* table = sMapTableMgr.GetMapCity(id);
    if (table->IsTonden())
    {
        player.m_worldMapLogic->TondenEnd(player, hero);
    }

	LLOG("[1003] Hero Exit City %d",id);
    _DelCombatObj(hero, hero.GetCountryId());

    _Trap(hero);

// 	pb::GS2C_Combat_Call_Up msg;
// 	msg.set_result(HERO_EXIT);
//  	m_combatGroup->Broadcast(SMSG_CITY_UPDATE_INFO,msg);


    //TODO：广播给所该城池的玩家
}

bool City::ReinforceEnter(Reinforce& reinforce)
{
	return _AddReinforce(reinforce);
}

void City::ReinforceExit(Reinforce& reinforce)
{
	_DelReinforce(reinforce);
}

bool City::_AddReinforce(Reinforce& reinforce)
{
	LLOG("City::_AddReinforce countryid= %d, cityid= %d, hero countryid= %d,reinid=%d",m_countryId,id,reinforce.GetCountryId(),reinforce.GetTableId());
	if (reinforce.GetCountryId() == m_countryId)	//加入防守队列中
	{
		if(reinforce.soldiers.empty())
		{
			LLOG("important bug！！！ soldier is empty！！！");
			return false;
		}
		bool ret = false;
		if (m_combatGroup) //有战斗
		{
			LLOG("!!!!!!!!! have combat !!!!");
			ret = m_combatGroup->AddReinforceToDefence(reinforce);
			if(ret)
			{
				m_defender.insert(&reinforce);
			}
		}
		return ret;
	}
	else		//敌对势力占有
	{
		if(reinforce.soldiers.empty())
		{
			LLOG("important bug！！！ soldier is empty！！！");
			return false;
		}
		bool ret = false;
		if (m_combatGroup) //有战斗
		{
			LLOG("!!!!!!!!! have combat !!!!");
			ret = m_combatGroup->AddReinforceToAttack(reinforce);
		}
		return ret;
	}
}

void City::AddCallBack_BeOccupied(const boost::function<void(City*)>& func)
{
    m_CallBack.push_back(func);
}
void City::AddCallBacK_DefenceFail(const boost::function<void(City*)>& func)
{
	m_DefenceFailCallBack.push_back(func);
}
void City::ClearCallBack_DefenceFail()
{
	m_DefenceFailCallBack.clear();
}
void City::OnBeOccupied(uint32 countryId)
{
    m_map.GetCountry(m_countryId)->OnCityBeOccupied(*this, countryId);
    m_map.GetCountry(countryId)->OnOccupyCity(*this, m_countryId);

	NLOG("be Occupied countryId=%d orgcountryId=%d, city=%d",countryId,m_countryId,GetCityId());
    m_countryId = countryId;

    // 相连敌国城池是否被包围
    const MapCity* table = sMapTableMgr.GetMapCity(id);
    for (size_t i = 0; i < table->m_linkCity.size(); ++i)
    {
        const uint16 linkId = table->m_linkCity[i];
        City* pCity = m_map.GetCity(linkId);
        if (pCity && pCity->GetCountryId() != m_countryId)
        {
            pCity->TryEncircled();
        }
    }
	TryEncircled();

    //刷3支NPC守城军，TODO:离首都越近，守城军的数量越多，属性越强
	if(m_countryId != MONSTER_COUNTRY_ID)
		_AddDefenderNpc();

    //城池是否易主
    for (CallBackVec::iterator it = m_CallBack.begin(); it != m_CallBack.end(); ++it)
    {
        (*it)(this);
    }
	m_CallBack.clear();

	if(m_DefenceFailCallBack.size()>0)
	{
		for(CallBackVec::iterator it = m_DefenceFailCallBack.begin(); it != m_DefenceFailCallBack.end(); ++it)
		{
			(*it)(this);
		}
		sDefenceBorderland._ChechCityCallBackClear();
	}
	//m_lastCallUpTime = sOS.TimeSeconds();
}
bool City::TryEncircled()
{
    int partnerCnt = 0;
    City* partnerCity = NULL;

    const MapCity* table = sMapTableMgr.GetMapCity(id);
    for (size_t i = 0; i < table->m_linkCity.size(); ++i)
    {
        const uint16 linkId = table->m_linkCity[i];
        City* pLinkCity = m_map.GetCity(linkId);
        if (pLinkCity && pLinkCity->GetCountryId() == m_countryId)
        {
            ++partnerCnt;

			pLinkCity->SetStatus(Encircled, false);

            // 本国相连城池有除自己之外的同伴
            if (m_map.HavePartnerCity(linkId, id)){
                return false;
            }else{
                partnerCity = pLinkCity;
            }
        }
    }
	if (partnerCnt > 1) { //有超过两个同伴
        return false;
    }

    //那个唯一的同伴、自己，都陷入包围
	SetStatus(Encircled, true);
	if (partnerCity) partnerCity->SetStatus(Encircled, true);
    return true;
}

bool City::CanBeAttack()
{
    if (sMapTableMgr.GetMapCity(id)->IsCapital())
    {
        return false;
    }
    return true;
}
static void _OnCombatEnd(City* city, Combat::CombatGroup* pGroup, bool isAttackWin)
{
	NLOG("city fight over");
	const TecTable* tec = NULL;
	//if(pGroup->m_group_attack.size()>0)
	//{
	//	for(Combat::GroupLst::iterator it = pGroup->m_group_attack.begin(); it != pGroup->m_group_attack.end(); ++it)
	//	{
	//		if (Character* hero = dynamic_cast<Character*>(*it))
	//		{
	//			//pGroup->_OtherKillStatisticData(*hero, Combat::Country);
	//			hero->CalculateAttr();
	//		}
	//	}
	//}

	if(pGroup->m_group_defence.size()>0)
	{
		for(Combat::GroupLst::iterator it = pGroup->m_group_defence.begin(); it != pGroup->m_group_defence.end(); ++it)
		{
			if (Character* hero = dynamic_cast<Character*>(*it))
			{
				//pGroup->_OtherKillStatisticData(*hero, Combat::Country);
				//hero->CalculateAttr();
				if(isAttackWin){
#ifdef _MMO_SERVER_
					if(city->id == sActivityCelebrityMgr.getCityId(hero->m_player->GetCountryId()))
					{
						sActivityCelebrityMgr.setCityIdToZero(hero->m_player->GetCountryId());
					}
#endif
				}
			}
		}
	}

	//通知client
	pGroup->BroadcastStatisticData(isAttackWin);
	//pGroup->SaveAllStaticData();
	if (isAttackWin)
	{
//////////////////////////////zhoulunhao

        city->m_defender.clear();
        city->m_combatGroup = NULL;

        // 检查战斗组攻击方
		int newCountryId = pGroup->m_group_attack.front()->GetCountryId();
		Combat::CombatObj *com_obj = pGroup->m_group_attack.front();
		NLOG("[city combat] Combat over , cityid=%d, attack win, org_country_id=%d, win_country_id=%d, isshadow=%d, ownerid=%lld",city->id,city->GetCountryId(),newCountryId,com_obj->IsShadow(),com_obj->GetPlayerId());
		Character* hero = dynamic_cast<Character*>(com_obj);//占领城池  攻占城池
		if(hero)
		{
			if(hero->m_player)
			{
				if(hero->m_player->HaveTechnology(EXP_UP_4))
				{
					tec = sTechnologyTableMgr.GetTable(EXP_UP_4);
					hero->m_player->AddXPByLimit(tec->Value1(),tec->Value2());
				}
				else if(hero->m_player->HaveTechnology(EXP_UP_2))
				{
					tec = sTechnologyTableMgr.GetTable(EXP_UP_2);
					hero->m_player->AddXPByLimit(tec->Value1(),tec->Value2());
				}
					
				if(hero->GetIsInAutoCountryWar())
					hero->m_player->m_AutoCountryWarLog->AddCityOccuty(1);

				hero->m_player->AddOccupyValue(1,city->GetCityId());///增加攻城值   zhoulunhao
				hero->m_player->AddValue(pb::PLAYER_FIELD_TODAY_OCCUPY_CITY,1);
				hero->m_player->AddContinueOccupyCityNum(1);
				hero->m_player->UpdateAllDailyQuestInfo(OCCUPY_OR_ZHUZHENG,1,0);//每日任务
				hero->m_player->UpdateAllDailyQuestInfo(OCCUPY_SOME_CITY,1,city->GetCityId());//每日任务
				sPersonTask.addProgress(OCCUPY_TIMES,1,*hero->m_player);
#ifdef _SERVER_CROSS_
					sCrossDestroy.addProgress(OCCUPY_TIMES,1,*hero->m_player);
#endif

				if(hero->m_player->HaveTechnology(GONGCHENGGONGXUN1))
				{
					tec = sTechnologyTableMgr.GetTable(GONGCHENGGONGXUN1);
					if(tec)
					{
						hero->m_player->AddExploitValue(tec->Value3());//攻占城池
					}
				}
#ifdef _MMO_SERVER_
				if(city->id == sActivityCelebrityMgr.getCityId(hero->m_player->GetCountryId()))
				{
					sActivityCelebrityMgr.setCityIdToZero(hero->m_player->GetCountryId());
				}
				hero->m_player->m_WorldFightAchievementLog->UpdateCityState(hero->m_player,city->id,CITY_OCCUPY_CITY,STATE_CAN_TAKE);
#endif
			}
		}
		else ////被任何其他打败
		{
#ifdef _MMO_SERVER_
			if(city->id == sActivityCelebrityMgr.getCityId(com_obj->GetCountryId()))
			{
				sActivityCelebrityMgr.setCityIdToZero(com_obj->GetCountryId());
			}
#endif
		}
///////////////zhoulunhao
		std::set<uint64> player_id_set;
		tec = sTechnologyTableMgr.GetTable(GONGCHENGGONGXUN1);
		Combat::GroupLst::iterator Iter =  pGroup->m_group_attack.begin();
		for(;Iter != pGroup->m_group_attack.end();++Iter)
		{
			Character* character = dynamic_cast<Character*>(*Iter);
			if(character && character->m_player )
			{
				if(hero && hero->m_player && character->m_player != hero->m_player)//如果攻击队列的第一个是玩家，则比较是否同一玩家
				{
					player_id_set.insert(character->m_player->GetGuid());
				}
				else if(hero == NULL)//如果是Npc,直接插入
				{
					player_id_set.insert(character->m_player->GetGuid());
				}
			}	
		}

		for(std::set<uint64>::iterator iter = player_id_set.begin();iter != player_id_set.end();++iter)
		{
			if( !pGroup->AttractCombat_attack_id.empty())
			{
				if(pGroup->IsIdInAttractCombatList(*iter))
				{
					Player* player = sPlayerPool.GetByPlayerId(*iter);
					if(player)
					{
						if(player->HaveTechnology(GONGCHENGGONGXUN1))
						{
							if(tec)
							{
								player->AddExploitValue(tec->Value2());//助攻
								player->AddValue(pb::PLAYER_FIELD_TODAY_ZHUGONG,1);
							}
						}
						//player->AddAttackCityValue(1,city->GetCityId());//攻打
#ifdef _MMO_SERVER_
						player->m_WorldFightAchievementLog->UpdateCityState(player,city->id,CITY_ATTACK_CITY,STATE_CAN_TAKE);
#endif
					}
				}
				else 
				{
					Player* player = sPlayerPool.GetByPlayerId(*iter);
					if(player)
					{
						if(player->HaveTechnology(GONGCHENGGONGXUN1))
						{
							if(tec)
							{
								player->AddExploitValue(tec->Value1());//助阵
								player->AddValue(pb::PLAYER_FIELD_TODAY_ZHUZHENG,1);
							}
						}
						player->UpdateAllDailyQuestInfo(OCCUPY_OR_ZHUZHENG,1,0);//每日任务
					}
				}
			}
			
			else //没有诱敌，
			{
				Player* player = sPlayerPool.GetByPlayerId(*iter);
				if(player)
				{
					if(player->HaveTechnology(GONGCHENGGONGXUN1))
					{
						if(tec)
						{
							player->AddExploitValue(tec->Value1());//助阵
							player->AddValue(pb::PLAYER_FIELD_TODAY_ZHUZHENG,1);
						}
					}
					player->UpdateAllDailyQuestInfo(OCCUPY_OR_ZHUZHENG,1,0);//每日任务
				}
			}
		}

		if(!pGroup->AttractCombat_attack_id.empty())
		{
			for(std::set<uint64>::iterator iter = pGroup->AttractCombat_attack_id.begin();iter != pGroup->AttractCombat_attack_id.end();++iter)
			{
				if(!pGroup->IsPlayerIdInSet(player_id_set,*iter))
				{
					Player* player = sPlayerPool.GetByPlayerId(*iter);
					if(player)
					{
						if(player->HaveTechnology(GONGCHENGGONGXUN1))
						{
							if(tec)
							{
								player->AddExploitValue(tec->Value2());//助攻
								player->AddValue(pb::PLAYER_FIELD_TODAY_ZHUGONG,1);
							}
						}
						//player->AddAttackCityValue(1,city->GetCityId());//攻打城市
#ifdef _MMO_SERVER_
						player->m_WorldFightAchievementLog->UpdateCityState(player,city->id,CITY_ATTACK_CITY,STATE_CAN_TAKE);
#endif
					}
				}
			}
		}
		


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//广播战报：国家
		pb::C2GS_Chat msg;
		char buf[256] = {0};
		std::string playername;
		uint32 vlevel = 0;
		uint32 npcid = 0;
		uint32 orgCountryid = city->GetCountryId();
		if(com_obj->IsPlayer() || com_obj->IsReinforce() || com_obj->IsShadow())	//玩家武将或召唤的增援部队或幻影，都隶属于玩家
		{
			playername = com_obj->GetPlayerName();
			if(Player* obj_player = sPlayerPool.GetByPlayerId(com_obj->GetPlayerId()))
				vlevel = obj_player->GetVipLevel();
		}
		else
		{
			npcid = com_obj->GetTableId();
		}
		SPRINTF(buf,"2,%s,%d,%d,%d,%d,%d",playername.c_str(),npcid,vlevel,orgCountryid,city->id,newCountryId);
		msg.set_type(pb::CHAT_COMBAT_BC);
		msg.set_chat_info(buf);
		LLOG("[1006] BroadCast Combat2:%s",buf);
		FOREACH_PLAYERPTR(PlayerId, playerPtr)
		{
			LLOG("[1006] send one player countryid = %d",playerPtr->GetCountryId());
			if(playerPtr->GetCountryId() == orgCountryid || playerPtr->GetCountryId() == newCountryId)
			{
				playerPtr->m_chatSpeaker->Say(playerPtr.get(),msg);
			}
		}


		city->OnBeOccupied(newCountryId);
		NLOG("[city combat] set country id=%d",newCountryId);

		sOccupyCity.NoticePlayerCityChange();

		city->SetStatus(City::Fighting, false);

        for (Combat::GroupLst::iterator it = pGroup->m_group_attack.begin(); it != pGroup->m_group_attack.end(); ++it)
        {
            city->_AddCombatObj(*(*it), (*it)->GetCountryId());
        }

		std::set<Combat::CombatObj*>::iterator it = city->m_defender.begin();
		for (; it != city->m_defender.end();)
		{
			if (Npc* npc = dynamic_cast<Npc*>(*it))
			{
				if(npc->isLifeGuard)	//是禁卫军，则从队伍删除
				{
					NLOG("life guard erase");
					npc->OnDead(*(it++));
				}
				else
					it++;
			}
			else
				it++;
		}

    }
    else
    {
		NLOG("[city combat] Combat over , cityid=%d, defence win, org_country_id=%d",city->id,city->GetCountryId());
        city->m_combatGroup = NULL;
        city->SetStatus(City::Fighting, false);

        std::set<Combat::CombatObj*>::iterator it = city->m_defender.begin();
        for (; it != city->m_defender.end();)
        {
			if (Npc* npc = dynamic_cast<Npc*>(*it))
			{
				if(npc->isLifeGuard)	//是禁卫军，则从队伍删除
				{
					NLOG("life guard erase");
					npc->OnDead(*(it++));
#ifdef _MMO_SERVER_
					if(city->id == sActivityCelebrityMgr.getCityId(npc->GetCountryId()))
					{
						sActivityCelebrityMgr.setCityIdToZero(npc->GetCountryId());
					}
#endif
				}
				else
					it++;
			}
			else
				it++;

        }
    }

	pGroup->BroadcastCombatGainsInfo(city->id,city->GetCountryId());

	//通知所有自动战斗的玩家
	FOREACH_PLAYERPTR(PlayerId,playerPtr)
	{
		if(playerPtr->m_AutoCountryWarLog->GetAutoCountryWar())		//是自动战斗
		{
			if(playerPtr->m_AutoCountryWarLog->GetDesCityId() == city->id)		//目标城池是该城池
			{
				playerPtr->m_AutoCountryWarLog->ChechAutoWarEnd(city->GetCountryId());
			}
		}
	}
}
void City::_AddCombatObj(Combat::CombatObj& obj, const uint32 countryId)
{
	//LLOG("City::_AddCombatObj countryid= %d, cityid= %d, hero countryid= %d, isShadow=%d, isNpc=%d",m_countryId,id,countryId,obj.IsShadow(),obj.IsNpc());
    if (countryId == m_countryId)	//武将所属势力占有城池
    {
		if(obj.soldiers.empty())
		{
			if(!IsCapitalCity())
				LLOG("important bug！！！ soldier is empty！！！");
			return;
		}
        m_defender.insert(&obj);

        if (m_combatGroup) //有战斗
        {
			LLOG("!!!!!!!!! have combat !!!!");
            m_combatGroup->AddObjToDefencek(obj);
        }
    }
    else		//敌对势力占有
    {
        if (m_combatGroup)
        {
			LLOG("!!!!!!!!! have combat !!!!");
            m_combatGroup->AddObjToAttack(obj);
        }
        else
        {
			LLOG("City:%d Have No Combat Group,m_defender.size=%d",id,m_defender.size());
			for(std::set<Combat::CombatObj*>::iterator it = m_defender.begin();
				it != m_defender.end(); it++)
			{
				if((*it)->IsDead())	//死亡？？
				{
					LLOG("!!!!!!! Important Bug, defender is dead!!");
					return;
				}
			}
			if (m_defender.empty()) _AddDefenderNpc();
			const MapCity* table = sMapTableMgr.GetMapCity(id);
			if(obj.soldiers.empty())
			{
				if(!IsCapitalCity())
					LLOG("important bug！！！ soldier is empty！！！");
				return;
			}
			LLOG("after m_defender.size=%d",m_defender.size());
			m_combatGroup = Combat::CombatGroup::CreateGroup(obj, m_defender, GetCombatTyp(),table->LandForm());
			if (m_combatGroup) {
				LLOG("city:%d create a combat group:%d",id,m_combatGroup->m_unique_id);
				m_combatGroup->AddCallBack_OnEnd(boost::bind(_OnCombatEnd, this, _1, _2));
				SetStatus(Fighting, true);
				if(IsFortressCity())
				{
					//发送战报通知
					pb::C2GS_Chat msg;
					char buf[256] = {0};
					SPRINTF(buf,"1,%d,%d",id,obj.GetCountryId());
					msg.set_type(pb::CHAT_COMBAT_BC);
					msg.set_chat_info(buf);
					LLOG("[1006] BroadCast Combat1:%s",buf);
					FOREACH_ONLINE_PLAYERPTR(PlayerId, playerPtr)
					{
						if(playerPtr->GetCountryId() == m_countryId)
						{
							playerPtr->m_chatSpeaker->Say(playerPtr.get(),msg);
						}
					}
				}
			}
        }
    }
}
void City::_DelCombatObj(Combat::CombatObj& obj, const uint32 countryId)
{
    if (countryId == m_countryId)		//武将的国家与城池所属国家一致，则为防守
    {
		std::set<Combat::CombatObj*>::iterator findObj = m_defender.find(&obj);
		if(findObj == m_defender.end())
		{
			PLOG("[1000]  IMPORTANT BUG!!!! CANT FIND OBJ TO DELETE!!! City ID = %d, defender size = %d",id,m_defender.size());
		}

        m_defender.erase(&obj);

		
        if (m_combatGroup) //有战斗
        {
            m_combatGroup->DelObjToDefencek(obj);
        }
    }
    else
    {
        if (m_combatGroup)
        {
            m_combatGroup->DelObjToAttack(obj);
        }
    }
}
void City::_DelReinforce(Reinforce& reinforce)
{
	if (reinforce.GetCountryId() == m_countryId)		//武将的国家与城池所属国家一致，则为防守
	{
		std::set<Combat::CombatObj*>::iterator findObj = m_defender.find(&reinforce);
		if(findObj == m_defender.end())
		{
			PLOG("[1000]  IMPORTANT BUG!!!! CANT FIND REINFORCE TO DELETE!!! City ID = %d, defender size = %d",id,m_defender.size());
		}

		m_defender.erase(&reinforce);


		if (m_combatGroup) //有战斗
		{
			m_combatGroup->DelObjToDefencek(reinforce);
		}
	}
	else
	{
		if (m_combatGroup)
		{
			m_combatGroup->DelObjToAttack(reinforce);
		}
	}
}
Combat::CombatType City::GetCombatTyp()
{
    if (m_map.type == CMap::Crusade) return Combat::CT_Crusade;

    //if (IsMonsterCity()) return Combat::Monster;

    return Combat::CT_Country;
}
void City::AddDefenderNpc(int npcId, int npcLv, int npcCnt)
{
	for (int i = 0; i < npcCnt; ++i)
	{
        sNpcMgr.CreateNpc(npcId, npcLv, m_countryId, this);
	}
}

void City::AddAttackNpc(int npcId, int npcLv, int npcCnt,int countryid)
{
	for (int i = 0; i < npcCnt; ++i)
	{
		sNpcMgr.CreateNpc(npcId, npcLv, countryid, this);
	}
}

void City::AddMonsterNpcsToDefence(uint32 num)
{
	if (Country* pCountry = m_map.GetCountry(m_countryId))
	{
		const MapCity* table = sMapTableMgr.GetMapCity(id);

		const std::vector<MapCity::NpcInfo>& npcLst = table->GetNpcLst(m_countryId);
		std::vector<MapCity::NpcInfo>::const_iterator it = npcLst.begin();

		int npcId = it->npc_id;
		int npcLv;
		if(m_countryId == MONSTER_COUNTRY_ID)
			npcLv = it->npc_lv>0? it->npc_lv:30;
		else
			npcLv =pCountry->m_countryNpcLv;

		AddDefenderNpc(npcId, npcLv, num);
	}
}

void City::AddCountryTaskNpcToDefence()
{
	if (MapCountryQuest* table = sCountryTableMgr.GetCountryTaskById(ActMgr::Attack_Defence_City))
	{
		if (Country* pCountry = m_map.GetCountry(m_countryId))
		{
			int npcLv;
			if(m_countryId == MONSTER_COUNTRY_ID)
				npcLv = 30;
			else
				npcLv =pCountry->m_countryNpcLv;
			AddDefenderNpc(table->GetNpcId(m_countryId), npcLv, table->GetNpcCnt());
		}
	}
}

void City::_AddDefenderNpc()
{
	//LLOG("City::_AddDefenderNpc countryid = %d, cityid = %d",m_countryId,id);
    if (Country* pCountry = m_map.GetCountry(m_countryId))
	{
		const MapCity* table = sMapTableMgr.GetMapCity(id);

        const std::vector<MapCity::NpcInfo>& npcLst = table->GetNpcLst(m_countryId);
        std::vector<MapCity::NpcInfo>::const_iterator it = npcLst.begin();
        for (; it != npcLst.end(); ++it)
		{
			int npcId = it->npc_id;
			int npcCnt = it->npc_cnt;
			int npcLv = it->npc_lv > 0 ? it->npc_lv : pCountry->m_countryNpcLv;

			AddDefenderNpc(npcId, npcLv, npcCnt);
		}
	}
}

void City::_DelDefenderNpc()
{
	//NLOG("City::_DelDefenderNpc countryid = %d, cityid = %d",m_countryId,id);
	for(std::set<Combat::CombatObj*>::iterator it = m_defender.begin();
		it != m_defender.end(); it++)
	{
		if(Npc* npcobj = dynamic_cast<::Npc*>(*it))
			npcobj->DeleteNpc(npcobj);
	}
	m_defender.clear();
	//ASSERT(0);
}

void City::SetMazeStatus(int countryId, bool b)
{
    switch (countryId) {
    case 1: SetStatus(Maze_Country_1, b); break;
    case 2: SetStatus(Maze_Country_2, b); break;
    case 3: SetStatus(Maze_Country_3, b); break;
    default: MMO_ASSERT(0); break;
    }
}
void City::SetTrapStatus(int countryId, bool b)
{
    switch (countryId) {
    case 1: SetStatus(Trap_Country_1, b); break;
    case 2: SetStatus(Trap_Country_2, b); break;
    case 3: SetStatus(Trap_Country_3, b); break;
    default: MMO_ASSERT(0); break;
    }
}

//Notice：由于迷宫会改变进出城池关系，不能像Trap似的直接放在City::On**Exit里
//常规逻辑，退出城池紧接着会进入下一个
bool City::_Maze(Combat::CombatObj& obj) //随机取相邻城池做目的地
{
    bool isDoMaze = false;
    const int countryId = obj.GetCountryId();

    if (HaveStatus(Maze_Country_1) && countryId != 1) isDoMaze = true;
    if (HaveStatus(Maze_Country_2) && countryId != 2) isDoMaze = true;
    if (HaveStatus(Maze_Country_3) && countryId != 3) isDoMaze = true;

    if (isDoMaze && System::Randf(0, 1) <= 0.6f)
    {
        const MapCity* table = sMapTableMgr.GetMapCity(id);

        int idx = System::Rand(0, table->m_linkCity.size());

        obj.MoveTo(table->m_linkCity[idx]);

        return true;
    }
    return false;
}
void City::_Trap(Combat::CombatObj& obj)
{
    bool isDoTrap = false;
    const int countryId = obj.GetCountryId();

    if (HaveStatus(Trap_Country_1) && countryId != 1) isDoTrap = true;
    if (HaveStatus(Trap_Country_2) && countryId != 2) isDoTrap = true;
    if (HaveStatus(Trap_Country_3) && countryId != 3) isDoTrap = true;

    if (isDoTrap) //扣当前血量的5%
    {
        obj.HurtPerHp(0.05f);
    }
}

const uint32 City::GetCityId() const
{
	return id;
}

void City::ReduceDurability(int num, uint32 attcountryid)
{
	int org_durability = m_durability;
	m_durability -= num;
	if(m_durability < 0)
		m_durability = 0;

	if(org_durability > 50 && m_durability <= 50)	//降到50与20时刷新守卫
		RefreshNPCNearBy(1);
	else if(org_durability > 20 && m_durability <= 20)
		RefreshNPCNearBy(2);

#ifdef _SERVER_CROSS_
	sCrossWorldMap.NoticeOfDurabilityChange(GetCountryId(),attcountryid);
#endif
}

void City::RefreshNPCNearBy(int stage)
{
	if(const MapCity* table = sMapTableMgr.GetMapCity(id))
	{
		if(table->IsCapital())		//是首都
		{
			for (size_t i = 0; i < table->m_linkCity.size(); ++i)
			{
				const uint16 linkId = table->m_linkCity[i];
				City* pCity = m_map.GetCity(linkId);
				if(pCity)
				{
					Country* pCountry = m_map.GetCountry(pCity->GetCountryId());
					if(pCountry)
					{
						if(pCity->GetCountryId() != GetCountryId())	//不是自己的国家
						{
							uint32 npcnum = pCity->m_defender.size();
							int npcId,npcLv;

							npcLv = pCountry->m_countryNpcLv;

							if(stage == 1)
							{
								switch (GetCountryId())
								{
								case 1:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_ZERG_TROOPS1);
									break;
								case 2:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_TERRAN_TROOPS1);
									break;
								case 3:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_PROTOSS_TROOPS1);
									break;
								case 5:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_AIMENG_TROOPS1);
									break;
								default:
									return;
								}
							}
							else
							{
								switch (GetCountryId())
								{
								case 1:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_ZERG_TROOPS2);
									break;
								case 2:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_TERRAN_TROOPS2);
									break;
								case 3:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_PROTOSS_TROOPS2);
									break;
								case 5:
									npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_AIMENG_TROOPS2);
									break;
								default:
									return;
								}
							}
							pCity->AddAttackNpc(npcId, npcLv, npcnum,GetCountryId());
						}
						else		//是自己的国家
						{
							if(pCity->IsFighting())		//有战斗
							{
								if(Combat::CombatGroup* Gcombat = pCity->GetCombatGroup())
								{
									uint32 npcnum = Gcombat->m_group_attack.size();
									int npcId,npcLv;
									
									npcLv = pCountry->m_countryNpcLv;

									if(stage == 1)
									{
										switch (GetCountryId())
										{
										case 1:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_ZERG_TROOPS1);
											break;
										case 2:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_TERRAN_TROOPS1);
											break;
										case 3:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_PROTOSS_TROOPS1);
											break;
										case 4:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_AIMENG_TROOPS1);
											break;
										default:
											return;
										}
									}
									else
									{
										switch (GetCountryId())
										{
										case 1:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_ZERG_TROOPS2);
											break;
										case 2:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_TERRAN_TROOPS2);
											break;
										case 3:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_PROTOSS_TROOPS2);
											break;
										case 4:
											npcId = GET_BASE_DEF_INT(pb::BD_CROSS_KILL_AIMENG_TROOPS2);
											break;
										default:
											return;
										}
									}
									pCity->AddDefenderNpc(npcId, npcLv, npcnum);
								}
							}
						}
					}
				}
			}
		}
	}
}