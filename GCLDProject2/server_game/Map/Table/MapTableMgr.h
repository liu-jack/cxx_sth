#ifndef __MapTableMgr_h__
#define __MapTableMgr_h__
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
#include "boost/function.hpp"
#include "MapBox.h"
#include "MapFog.h"
#include "MapEvent.h"
#include "MapCity.h"
#include "datastruct/struct_city.h"
#include "MapTunTian.h"
#include "MapExpedition.h"
#include "OpenFog.h"
//class MapBox;
//class MapFog;
//class MapCity;
//class MapEvent;
typedef PtrMap<uint32,OpenFog> OpenFogMap;
class MapTableMgr {
    typedef PtrMap<int, MapFog> FogMap;		//迷雾表
    typedef PtrMap<uint16, MapBox> BoxMap;
    typedef PtrMap<uint32, MapCity> CityMap;
    typedef PtrMap<uint32, MapEvent> EventMap;
    typedef PtrMap<uint32, const DB_Military_Supplies> ArmyResourceMap;
	typedef PtrMap<uint32, MapTunTian> TunTianMap;

	typedef PtrMap<uint32, MapExpedition> ExpeditionMap;		//远征军
	typedef PtrMap<uint32, MapExpedition> GuardMap;				//禁卫军

    typedef std::map<uint32, std::vector<const MapCity*> >  OneMapHaveCitys;
    typedef std::map<uint32, IntPairVec >   OneMapCapitalLst;


    enum { //见world_events表，event_type项
        Private_Type = 0,   //仅自己的
        Public_Type = 1,    //全服共享的
    };

private:
    BoxMap      m_BoxLst;
    FogMap      m_FogLst1;		//迷雾关卡 由uniqueID识别
    FogMap      m_FogLst2;		//迷雾关卡 由ID识别
    CityMap     m_CityLst;
	OpenFogMap m_open_fog; //迷雾管卡奖励
    uint32      m_FortressLst[5][3]; //要塞
    EventMap    m_privateEventLst;		//个人宝箱
    EventMap    m_publicEventLst;		//公共宝箱  取固定次数就没有了
	EventMap	m_celebrityEventLst; ///结交名仕
    ArmyResourceMap m_ArmyResourceMap;
    OneMapCapitalLst    m_OneMapCapitalLst; //[mapId] = <countryId, cityId>
	TunTianMap	m_tuntianMap;

	uint32		m_Tuntian[3];	//屯田

	ExpeditionMap m_expeditionMap;
	GuardMap	  m_guardMap;
public:
    OneMapHaveCitys     m_OneMapHaveCitys;

public:
    static MapTableMgr& Instance(){ static MapTableMgr T; return T; }

    void Init();
    const MapBox* GetMapBox(uint16 boxId);
    const MapFog* GetMapFog(uint32 countryId, uint32 battleCityId);
    const MapFog* GetMapFog(uint32 fogId);
    const MapCity* GetMapCity(uint32 cityId);
    const MapEvent* GetMapEvent(uint32 eventId);
	const MapEvent* getCelebrityMapEvent(uint32 eventId);
    const DB_Military_Supplies* GetArmyResource(uint32 hourTime);
	const MapTunTian* GetMapTuntian(uint32 tuntianlevel);

    uint32 GetCapitalCity(uint32 countryId, uint32 mapId = 0);
    uint32 GetFortressCity(uint32 countryId, uint32 idx);
	bool GetFogReward(const uint32 id,IntPair* pair);
	const OpenFogMap& GetOpenFogMap() const;
	uint32 GetTuntianCity(uint32 countryId, uint32 mapId = 0);

	std::vector<MapExpedition*> GetExpedition(uint32 countryID);	//获得远征军信息，参数为国家id与路线
	std::vector<MapExpedition*> GetGuard(uint32 countryID);		//获得禁卫军信息，参数为国家id与检测城池点

    void ForEachMapCity(uint32 mapId, boost::function<void(const MapCity&)>& func);
    void ForEachPrivateCityEvent(boost::function<void(const MapEvent&)>& func);
    void ForEachPublicCityEvent(boost::function<void(const MapEvent&)>& func);
	//void ForEachCelebrityCityEvent(boost::function<void(const MapEvent&)>func);
};
#define sMapTableMgr MapTableMgr::Instance()

#endif //__MapTableMgr_h__