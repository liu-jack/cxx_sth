#include "MapTableMgr.h"
#include "DbTool.h"
#include "datastruct/struct_city.h"
//#include "MapBox.h"
//#include "MapCity.h"
//#include "MapEvent.h"
//#include "MapFog.h"
#include "../city/city.h"
#include "Base.h"
#include "../WorldMap.h"
#include "ActivityControl/ActivityMgr.h"
#include "Activity.pb.h"
#include "ActivityCelebrity/Table/ActivityCelebrityTableMgr.h"
#define WUJIANGKAOSHANG 36
void MapTableMgr::Init()
{
	memset(m_Tuntian,0,sizeof(m_Tuntian));

    FOREACH_DB_ITEM(ptr1, DB_MapBox)		//世界宝箱 “world_boxes”表
    {
        m_BoxLst[ptr1->id] = new MapBox(*ptr1);
    }
    MMO_ASSERT(m_BoxLst.size() < 64); //用位标记记录箱子，超过64要扩展PlayerMapLogic::m_takeBoxBit

    int idxFortress[NEURRAL_COUNTRY_ID] = { 0 };
    FOREACH_DB_ITEM(ptr2, DB_MapCity)	//城市“country”表
    {
        MapCity* p = new MapCity(*ptr2);

        m_CityLst[ptr2->id] = p;

        m_OneMapHaveCitys[ptr2->map_id].push_back(p);

        if (p->IsCapital())
        {
            m_OneMapCapitalLst[ptr2->map_id].push_back(IntPair(ptr2->country, p->Id()));
        }
        else if (p->IsFortress())
        {
            int idxCountry = ptr2->country - 1;
            int i = idxFortress[idxCountry]++;
            m_FortressLst[idxCountry][i] = p->Id();
        }
		else if(p->IsTonden())
		{
			if(ptr2->map_id == 0)
			{
				m_Tuntian[ptr2->country - 1] = p->Id();
				LLOG("m_Tuntian[%d]=%d",ptr2->country - 1,p->Id());
			}
		}
    }
    ASSERT(
        idxFortress[0] == 6 &&
        idxFortress[1] == 6 &&
        idxFortress[2] == 6
        );
    FOREACH_DB_ITEM(ptr3, DB_MapEvent)			//世界事件 world_events表
    {
        switch (ptr3->type){
        case Public_Type:
            m_publicEventLst[ptr3->id] = new MapEvent(*ptr3);
            break;
        case Private_Type:
           
			if(ptr3->type_id == CELEBRITY_TYPE_ID)
			{
				m_celebrityEventLst[ptr3->id] = new MapEvent(*ptr3);
			}
			else
			{
				m_privateEventLst[ptr3->id] = new MapEvent(*ptr3);
			}
            break;
        default:
            break;
        }
    }
    FOREACH_DB_ITEM(ptr4, DB_MapFog)		//世界地图迷雾 “fog”表
    {
        int id = (ptr4->battle_city << 8) + ptr4->country;		//唯一的迷雾关卡id，由城市id与国家id组成
        MapFog* p = new MapFog(*ptr4);
        m_FogLst1[id] = p;
        m_FogLst2[ptr4->id] = p;
    }
    //MMO_ASSERT(m_FogLst1.size() < 64); //用位标记记录箱子，超过64要扩展PlayerMapLogic::m_takeBoxBit

    FOREACH_DB_ITEM(ptr5, DB_Military_Supplies)
    {
        m_ArmyResourceMap[ptr5->time] = ptr5;
    }

	FOREACH_DB_ITEM(ptr6, DB_TunTian_Up)
	{
		m_tuntianMap[ptr6->id] = new MapTunTian(*ptr6);
	}

	FOREACH_DB_ITEM(ptr7, DB_Expedition)
	{
		if(ptr7->troopType == 1)		//禁卫军
		{
			m_guardMap[ptr7->id] = new MapExpedition(*ptr7);
		}

		if(ptr7->troopType == 2)		//远征军
		{
			m_expeditionMap[ptr7->id] = new MapExpedition(*ptr7);
		}
	}
	FOREACH_DB_ITEM(ptr9, DB_OpenFog)
	{
		m_open_fog[ptr9->id] = new OpenFog(*ptr9);
	}
}

const MapBox* MapTableMgr::GetMapBox(uint16 boxId){
    return m_BoxLst.pfind(boxId);
}
const MapFog* MapTableMgr::GetMapFog(uint32 countryId, uint32 battleCityId)
{
    int id = (battleCityId<<8) + countryId;
    return m_FogLst1.pfind(id);
}
const MapFog* MapTableMgr::GetMapFog(uint32 fogId)
{
    return m_FogLst2.pfind(fogId);
}
const MapCity* MapTableMgr::GetMapCity(uint32 cityId){
    return m_CityLst.pfind(cityId);
}
const MapEvent* MapTableMgr::GetMapEvent(uint32 eventId)
{
    if (MapEvent* p = m_privateEventLst.pfind(eventId))
        return p;
    else
        return m_publicEventLst.pfind(eventId);
}

const MapEvent* MapTableMgr::getCelebrityMapEvent(uint32 eventId)
{
	return m_celebrityEventLst.pfind(eventId);
}

const DB_Military_Supplies* MapTableMgr::GetArmyResource(uint32 hourTime)
{
    return m_ArmyResourceMap.pfind(hourTime);
}
const MapTunTian* MapTableMgr::GetMapTuntian(uint32 tuntianlevel)
{
	return m_tuntianMap.pfind(tuntianlevel);
}

std::vector<MapExpedition*> MapTableMgr::GetExpedition(uint32 countryID)
{
	std::vector<MapExpedition*> retptr;
	for(ExpeditionMap::iterator it = m_expeditionMap.begin();
		it != m_expeditionMap.end(); it++)
	{
		if(it->second->GetTroopCountryID() == countryID)
			retptr.push_back(it->second);
	}
	return retptr;
}

std::vector<MapExpedition*> MapTableMgr::GetGuard(uint32 countryID)
{
	std::vector<MapExpedition*> retptr;
	for(ExpeditionMap::iterator it = m_guardMap.begin();
		it != m_guardMap.end(); it++)
	{
		if(it->second->GetTroopCountryID() == countryID)
			retptr.push_back(it->second);
	}
	return retptr;
}

uint32 MapTableMgr::GetCapitalCity(uint32 countryId, uint32 mapId /* = 0 */)
{
#ifdef _SERVER_CROSS_
	mapId = 1;
#endif
    IntPairVec& vec = m_OneMapCapitalLst[mapId];

    for (IntPairVec::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it->first == countryId)
        {
            return it->second;
        }
    }
    MMO_ASSERT(0);
    return 0;
}

uint32 MapTableMgr::GetTuntianCity(uint32 countryId, uint32 mapId /* = 0 */)
{
#ifdef _SERVER_CROSS_
	mapId = 1;
#endif

	return m_Tuntian[countryId-1];
}

uint32 MapTableMgr::GetFortressCity(uint32 countryId, uint32 idx)
{
    if (countryId > 3 || countryId <= 0 || idx >=3)
    {
        MMO_ASSERT(0);
        return 0;
    }
    return m_FortressLst[countryId-1][idx];
}
void MapTableMgr::ForEachMapCity(uint32 mapId, boost::function<void(const MapCity&)>& func)
{
	NLOG("MapTableMgr::ForEachMapCity mapId=%d",mapId);		//0是世界，1是跨服
    const std::vector<const MapCity*>& cityLst = m_OneMapHaveCitys[mapId];

    MMO_ASSERT(cityLst.size() > 0);
	//NLOG("cityLst %d",cityLst.size());
    for (uint i = 0; i < cityLst.size(); ++i)
    {
        func(*(cityLst[i]));
    }
}
void MapTableMgr::ForEachPrivateCityEvent(boost::function<void(const MapEvent&)>& func)
{
    for (EventMap::iterator it = m_privateEventLst.begin();
        it != m_privateEventLst.end(); ++it)
    {
#ifdef _MMO_SERVER_
		if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_HERO_REWARD))
		{
			if(it->second->GetEventTypeID() == WUJIANGKAOSHANG)
			{
				continue;
			}
		}
#endif
        func(*(it->second));
    }
}
void MapTableMgr::ForEachPublicCityEvent(boost::function<void(const MapEvent&)>& func)
{
    for (EventMap::iterator it = m_publicEventLst.begin();
        it != m_publicEventLst.end(); ++it)
    {
        func(*(it->second));
    }
}


bool MapTableMgr::GetFogReward( const uint32 id,IntPair* pair )
{
	/*const OpenFog* open_fog = m_open_fog.pfind(num);*/
	for(OpenFogMap::iterator iter = m_open_fog.begin();iter != m_open_fog.end();++iter)
	{
		if(iter->first == id)
		{
			pair->first = iter->second->RewardId();
			pair->second = iter->second->RewardNum();
			return true;
		}
	}
	return false;
}


// const uint32 MapTableMgr::getIdFromValue( const uint32 value )
// {
// 	OpenFogMap::iterator iter = m_open_fog.begin();
// 	for(;iter != m_open_fog.end();)
// 	{
// 		OpenFogMap::iterator old_iter = iter;
// 		OpenFogMap::iterator new_iter = ++old_iter;
// 		if(new_iter != m_open_fog.end())
// 		{
// 			if(value >= iter->second->OpenNum() && value < iter->second->OpenNum())
// 			{
// 				return iter->first;
// 			}
// 			++iter;
// 		}
// 		return iter->first;
// 	}
// 	return iter->first;
// }

const OpenFogMap& MapTableMgr::GetOpenFogMap() const
{
	return m_open_fog;
}
