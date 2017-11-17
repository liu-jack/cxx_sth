#include "WorldMap.h"
#include "def/ConstDef.h"
#include "../object/Player.h"
#include "city/city.h"
#include "country/country.h"
#include "Table/MapTableMgr.h"
#include "../Service/ServiceMgr.h"
//#include "AutomaticUpdate/AutomaticUpdate.h"
#include "../session/PlayerPool.h"
#include "PlayerMapLogic.h"
static uint32 expedition_id_fore = 0;
WorldMap::WorldMap(uint32 mapTid, CMap::MapEnum typ)	//mapTid初始化为0？
    : CMap(mapTid, typ)
{
	//NLOG("mapTid=%d",mapTid);
}

//1 匈奴 - 虫族
//2 罗马 - 人族
//3 大汉 - 神族
//4 蛮族
void WorldMap::Init()
{
    CMap::Init();
	expe_city_rec.clear();
}	

void WorldMap::InitArmysAfterDB()
{
#ifdef _MMO_SERVER_
	LLOG("WorldMap::InitArmysAfterDB");
	for (int i = 1; i <= MAX_PLAYER_COUNTRY_ID; ++i)
	{
		ServiceMgr::Register(SERVICE_COUNTRY_GUARD, GetCountry(i));		//三个国家刷新禁卫军
		// 玩家国家的攻城Npc
		//if(i == 1) continue;
		ServiceMgr::Register(SERVICE_COUNTRY_ATTACK_NPC, GetCountry(i));		//三个国家刷新远征军
		ServiceMgr::Register(SERVICE_COUNTRY_CELEBRITY_NPC, GetCountry(i));		//三个国家刷新活动军队
	}
#endif
}

uint32 WorldMap::CreateExpeditionID(uint32 heroid)
{
	return ( heroid << 16) + (++expedition_id_fore);
}

bool WorldMap::isExpeditionMoveTo(uint32 expeid, uint32 cityid)
{
	std::map<uint32, uint32>::iterator it = expe_city_rec.find(expeid);
	if(it != expe_city_rec.end())
	{
		if(it->second == cityid)
			return true;			//远征军已经移动到了目标城市
		else
			return false;
	}
	else
		return false;
}

void WorldMap::setExpeditionMoveTo(uint32 expeid, uint32 cityid)
{
	expe_city_rec[expeid] = cityid;
}

void WorldMap::addCountryExp(uint32 countryid, uint32 expnum)
{
	if(Country* countryone = GetCountry(countryid))
	{
		countryone->AddCountryExp(expnum);
	}
}

void WorldMap::addCountryNpcExp(uint32 countryid, uint32 expnum)
{
	if(Country* countryone = GetCountry(countryid))
	{
		countryone->AddNpcExp(expnum);
	}
}

Country* WorldMap::GetCountryByUniqueIdInCross(uint32 uniqueid)
{
	Country* ret = NULL;
	for(int i=1; i<=NR_PLAYER_COUNTRY; i++)
	{
		if(Country* countryone = GetCountry(i))
		{
			if(countryone->GetCrossUniqueID() == uniqueid)
			{
				ret = countryone;
				break;
			}
		}
	}
	return ret;
}