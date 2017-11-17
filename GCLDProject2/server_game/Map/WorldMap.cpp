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
WorldMap::WorldMap(uint32 mapTid, CMap::MapEnum typ)	//mapTid��ʼ��Ϊ0��
    : CMap(mapTid, typ)
{
	//NLOG("mapTid=%d",mapTid);
}

//1 ��ū - ����
//2 ���� - ����
//3 �� - ����
//4 ����
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
		ServiceMgr::Register(SERVICE_COUNTRY_GUARD, GetCountry(i));		//��������ˢ�½�����
		// ��ҹ��ҵĹ���Npc
		//if(i == 1) continue;
		ServiceMgr::Register(SERVICE_COUNTRY_ATTACK_NPC, GetCountry(i));		//��������ˢ��Զ����
		ServiceMgr::Register(SERVICE_COUNTRY_CELEBRITY_NPC, GetCountry(i));		//��������ˢ�»����
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
			return true;			//Զ�����Ѿ��ƶ�����Ŀ�����
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