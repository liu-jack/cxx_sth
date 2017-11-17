#include "BaseMap.h"
#include "../../object/Player.h"
#include "../Table/MapTableMgr.h"
#include "../city/city.h"
#include "../country/country.h"
#include "InterServer.pb.h"
#include "Country.pb.h"
#ifdef _MMO_SERVER_
#include "../../GameServer.h"
#endif

CMap::CMap(uint32 mapTid, MapEnum typ)
    : Tid(mapTid)
    , type(typ)
{

}
CMap::~CMap()
{
    for (MapCityConter::iterator it = m_cityLst.begin(); it != m_cityLst.end(); ++it)
    {
        delete it->second;
    }
    for (MapCountry::iterator it = m_countryLst.begin(); it != m_countryLst.end(); ++it)
    {
        delete it->second;
    }
}
void CMap::AddToMap(Player& player)
{
    m_PlayerLst.insert(&player);
}
void CMap::DelFromMap(Player& player)
{
    m_PlayerLst.erase(&player);
}

#define FOREACH_WORLD_MAP_PLAYER(varName) \
    Player* varName = NULL; \
    std::set<Player*>::iterator it = m_PlayerLst.begin(); \
    for (; it != m_PlayerLst.end() && (varName = *it); ++it)

void CMap::Broadcast(int opCode, google::protobuf::Message& msg)
{
    FOREACH_WORLD_MAP_PLAYER(player)
    {
        player->Send(opCode, msg);
    }
}

void CMap::BroadcastInCountry(int opCode, google::protobuf::Message& msg, uint32 countryid)
{
	FOREACH_WORLD_MAP_PLAYER(player)
	{
		if(player->GetCountryId() == countryid)
			player->Send(opCode, msg);
	}
}

void CMap::ForEachPlayer(boost::function<void(Player*)>& func)
{
    FOREACH_WORLD_MAP_PLAYER(player)
    {
        func(player);
    }
}

City* CMap::GetCity(uint32 id)
{
    MapCityConter::iterator it = m_cityLst.find(id);
    if (it != m_cityLst.end())
    {
        return it->second;
    }
    return NULL;
}
void CMap::ForEachCity(boost::function<void(City*)> func)
{
    for (MapCityConter::iterator it = m_cityLst.begin(); it != m_cityLst.end(); ++it)
    {
        func(it->second);
    }
}
void CMap::BuildEdgeCityLst(std::vector<City*>& lst)
{
    for (MapCityConter::iterator it = m_cityLst.begin(); it != m_cityLst.end(); ++it)
    {
        if (it->second->IsCountryEdge())
        {
            lst.push_back(it->second);
        }
    }
}
void CMap::ResetEncircledCity()
{
	LLOG("CMap::ResetEncircledCity");
    for (MapCityConter::iterator it = m_cityLst.begin(); it != m_cityLst.end(); ++it)
    {
        it->second->TryEncircled();
    }
}

bool CMap::AnyCountryCanCrossWar()
{
	for (MapCountry::iterator it = m_countryLst.begin(); it != m_countryLst.end(); ++it)
	{
		Country* ctry = it->second;
		if (ctry)
		{
			if (ctry->id == MONSTER_COUNTRY_ID) continue;		//蛮族不在其中

			LLOG("AnyCountryCanCrossWar country[%d].level = %d",ctry->id,ctry->GetLevel());
			if(ctry->GetLevel() >= 4)
				return true;
		}
	}
	return false;
}

bool CMap::HavePartnerCity(uint32 id, uint32 exceptId /* = 0 */)
{
    const uint32 countryId = GetCity(id)->GetCountryId();
	const MapCity* table = sMapTableMgr.GetMapCity(id);
    for (size_t i = 0; i < table->m_linkCity.size(); ++i)
    {
        const uint32 linkId = table->m_linkCity[i];

        if (linkId == exceptId) continue;

        if (GetCity(linkId)->GetCountryId() == countryId)
        {
            return true;
        }
    }
    return false;
}
Country* CMap::GetCountry(uint32 id)
{
    MapCountry::iterator it = m_countryLst.find(id);
    if (it != m_countryLst.end())
    {
        return it->second;
    }
    return NULL;
}

void CMap::SaveCountryData(Country* country, pb::SD2G_Country_AllInfo& msg)
{
	//if (country->id == MONSTER_COUNTRY_ID) return;

	const std::set<uint16>& lst = country->GetCityLst();
	for (std::set<uint16>::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		GetCity(*it)->SaveTo(*msg.add_city_lst());
	}
	country->SaveTo(*msg.add_base_info());
}

void CMap::SaveAllCountryData(pb::SD2G_Country_AllInfo& msg)
{
	for (MapCountry::iterator it = m_countryLst.begin(); it != m_countryLst.end(); ++it)
	{
		SaveCountryData(it->second,msg);
	}
} 

void CMap::SaveAllCountryInfo(pb::GS2C_Country_Info& msgInfo)
{
	for (MapCountry::iterator it = m_countryLst.begin(); it != m_countryLst.end(); ++it)
	{
		Country* ctry = it->second;
		if (ctry)
		{
			ctry->SaveTo(*msgInfo.add_country());
		}
	}
}

void CMap::SaveCountryDevideinfo(pb::SG2B_Country_Info& msgInfo)
{
#ifdef _MMO_SERVER_
	for (MapCountry::iterator it = m_countryLst.begin(); it != m_countryLst.end(); ++it)
	{
		Country* ctry = it->second;
		if (ctry)
		{
			if (ctry->id > MAX_PLAYER_COUNTRY_ID) continue;

			pb::Country_Info_Devide* msgone = msgInfo.add_country_info();
			msgone->set_countryid(ctry->id);
			msgone->set_pros_degree(ctry->m_Prosperity_Degree);
		}
	}
	msgInfo.set_server_id(sGameSvr.GetServerId());
#endif
}


void CMap::UpdateCountryWarInfo(const pb::CountryCrossWarInfo& msgInfo)
{
	Country* pCountry = GetCountry(msgInfo.id());
	if (pCountry)
	{
		pCountry->UpdateCrossWarInfo(msgInfo.cross_war_info());
	}
}

void CMap::_InitCountryCity(const MapCity& city)
{
	//NLOG("CMap::_InitCountryCity cityid=%d",city.Id());
    const uint32 cityId = city.Id();
    const uint32 countryId = city.CountryId();

    Country* pCountry = GetCountry(countryId);
    if (pCountry == NULL) {
		LLOG("[1002] no country then create one");
        pCountry = new Country(countryId);
        m_countryLst.insert(std::make_pair(countryId, pCountry));
    }
    pCountry->m_setCity.insert(cityId);
    pCountry->Init();

    City* pCity = new City(cityId, countryId, *this);
    m_cityLst.insert(std::make_pair(cityId, pCity));
    pCity->Init();
}
void CMap::Init()
{
	NLOG("CMap::Init Tid=%d",Tid);
    boost::function<void(const MapCity&)> func = boost::bind(&CMap::_InitCountryCity, this, _1);
    sMapTableMgr.ForEachMapCity(Tid, func);

	//添加蛮族国家
	Country* pCountry = GetCountry(MONSTER_COUNTRY_ID);
	if(pCountry == NULL)
	{
		LLOG("add monster country");
		pCountry = new Country(MONSTER_COUNTRY_ID);
		m_countryLst.insert(std::make_pair(MONSTER_COUNTRY_ID, pCountry));
	}

    m_cityGraph.Init(sMapTableMgr.m_OneMapHaveCitys[Tid]);
}

void CMap::ClearCountryEndCitys()
{
	for(MapCityConter::iterator cityit = m_cityLst.begin(); cityit != m_cityLst.end(); cityit++)
	{
		delete cityit->second;
	}

	for(MapCountry::iterator countryit = m_countryLst.begin(); countryit != m_countryLst.end(); countryit++)
	{
		delete countryit->second;
	}

	m_PlayerLst.clear();
	m_cityLst.clear();
	m_countryLst.clear();
}