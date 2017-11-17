#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "City.pb.h"
#include "../PlayerMapLogic.h"
#include "city.h"
#include "World.pb.h"
#include "../../Combat/CombatGroup.h"
#include "../WorldMap.h"
#include "../country/country.h"
#include "CrossWar.pb.h"
#include "character/CharacterStorage.h"
#include "Country.pb.h"

#ifdef _SERVER_CROSS_
#include "cross_wall/CrossWarManager/CrossWarMgr.h"
#include "cross_wall/CrossWorldMap.h"
#include "cross_wall/CrossActivity/CrossKill/CrossKill.h"
#endif

using namespace pb;

static void SaveCityInfo(City* city, GS2C_City_All_Info* msg)
{
    city->SaveTo(*msg->add_city_lst());
}
OpHandler(CMSG_CITY_ALL_INFO)
{
    GS2C_City_All_Info msg;

	Country* country;
#ifdef _MMO_SERVER_
	country = sWorldMap.GetCountry(GetCountryId());
	sWorldMap.ForEachCity(boost::bind(&SaveCityInfo, _1, &msg));
#endif

#ifdef _SERVER_CROSS_
	country = sCrossWorldMap.GetCountry(GetCountryId());
	sCrossWorldMap.ForEachCity(boost::bind(&SaveCityInfo, _1, &msg));
#endif

	if(country)
	{
		for(std::vector<PlayerCallRec>::iterator it = country->m_player_call_rec.begin();
				it != country->m_player_call_rec.end(); it++)
		{
			if(it->playerid  == GetGuid())
			{
				msg.add_call_up_down_list(it->uniqueid);
			}
		}

		for(std::vector<PlayerCallRec>::iterator it = country->m_player_monster_call_rec.begin();
			it != country->m_player_monster_call_rec.end(); it++)
		{
			if(it->playerid  == GetGuid())
			{
				msg.add_monster_call_up_down_lst(it->uniqueid);
			}
		}

		GS2C_Notice_Country_NPC_NextTime* nexttime = msg.add_npc_time();
		nexttime->set_npctype(1);
		nexttime->set_nexttime(country->m_AttackNpc_1_RefreshTime);

		GS2C_Notice_Country_NPC_NextTime* nexttime2 = msg.add_npc_time();
		nexttime2->set_npctype(2);
		nexttime2->set_nexttime(country->m_AttackNpc_2_RefreshTime);

		LLOG("m_AttackNpc_1_RefreshTime=%lld,m_AttackNpc_2_RefreshTime=%lld",country->m_AttackNpc_1_RefreshTime,country->m_AttackNpc_2_RefreshTime);
	}
    Send(SMSG_CITY_ALL_INFO, msg);
}

OpHandler(CMSG_REQUEST_FIGHT_LST)
{
    C2GS_Request_Fight_Lst req; pack >> req;

	City* city;
#ifdef _MMO_SERVER_
	city = sWorldMap.GetCity(req.city_id());
#endif

#ifdef _SERVER_CROSS_
	city = sCrossWorldMap.GetCity(req.city_id());
#endif

    if (city)
    {
		NLOG("get city");
		GS2C_City_Fight_Info msg;
		msg.set_city_id(req.city_id());
        if (city->m_combatGroup)
		{
			NLOG("is in combat");
            city->m_combatGroup->SaveTo(msg);
		}
		Send(SMSG_CITY_FIGHT_INFO, msg);
    }
}

#ifdef _SERVER_CROSS_
OpHandler(CMSG_ENTER_CROSS_WAR)		//已经注册了则直接发送城池信息
{
	if(sCrossWarMgr.GetCrossState() == State_Null) return;

	LLOG("[1002] Get CMSG_ENTER_CROSS_WAR");
	GS2C_Enter_Cross_War msg;

	msg.set_error_code(pb::ErrCommonSuccess);
	msg.set_country_id(GetCountryId());

	if(sCrossWarMgr.GetCrossState() == State_Preview || sCrossWarMgr.GetCrossState() == State_Start)	//预览阶段或者开战阶段发送城池信息
		sCrossWorldMap.ForEachCity(boost::bind(&SaveCityInfo, _1, msg.mutable_cityinfo()));
	
	if(sCrossWarMgr.GetCrossState() == State_Start || sCrossWarMgr.GetCrossState() == State_Preview)		//预览阶段或者战斗阶段才会发送武将信息
		m_characterStorage->SaveTo(*msg.mutable_character_info());
	
	if(sCrossWarMgr.GetCrossState() == State_Preview || sCrossWarMgr.GetCrossState() == State_Start || sCrossWarMgr.GetCrossState() == State_Award)	//战斗与领奖阶段才会发星数
	{
		for(int i=1; i<4; i++)	//三个国家星数
		{
			sCrossWarMgr.SaveTo(*msg.add_cross_country_stars(),i);
		}
	}

	if(sCrossWarMgr.GetCrossState() == State_Start || sCrossWarMgr.GetCrossState() == State_Award)//战斗与领奖阶段才会发杀敌任务id
	{
		msg.set_constantid(sCrossKill.GetTaskId(*this));
	}

	msg.set_doublekillendtime(GetCrossKillEndTime());
	msg.set_hammernum(GetCrossHammerNum());
	if(Country* country = sCrossWorldMap.GetCountry(GetCountryId()))
	{
		msg.set_shieldendtime(country->GetShieldEndTime());

		CrossPlayerMap::iterator it = country->m_CrossPlayerLst.find(GetGuid());
		if(it != country->m_CrossPlayerLst.end())		//有击杀记录
		{
			CrossPlayer& crossPlayer = it->second;
			msg.set_hastakestaraward(crossPlayer.isTakeReward);
		}
		else
			msg.set_hastakestaraward(false);

	}

	GS2C_Country_Info* countryinfo = msg.mutable_countrys();
	sWorldMap.SaveAllCountryInfo(*countryinfo);

	Send(SMSG_ENTER_CROSS_WAR, msg);

	sCrossWorldMap.AddToMap(*this);

	LLOG("[1002] Send SMSG_ENTER_CROSS_WAR to Player");
}
#endif