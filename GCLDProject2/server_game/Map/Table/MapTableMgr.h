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
    typedef PtrMap<int, MapFog> FogMap;		//�����
    typedef PtrMap<uint16, MapBox> BoxMap;
    typedef PtrMap<uint32, MapCity> CityMap;
    typedef PtrMap<uint32, MapEvent> EventMap;
    typedef PtrMap<uint32, const DB_Military_Supplies> ArmyResourceMap;
	typedef PtrMap<uint32, MapTunTian> TunTianMap;

	typedef PtrMap<uint32, MapExpedition> ExpeditionMap;		//Զ����
	typedef PtrMap<uint32, MapExpedition> GuardMap;				//������

    typedef std::map<uint32, std::vector<const MapCity*> >  OneMapHaveCitys;
    typedef std::map<uint32, IntPairVec >   OneMapCapitalLst;


    enum { //��world_events��event_type��
        Private_Type = 0,   //���Լ���
        Public_Type = 1,    //ȫ�������
    };

private:
    BoxMap      m_BoxLst;
    FogMap      m_FogLst1;		//����ؿ� ��uniqueIDʶ��
    FogMap      m_FogLst2;		//����ؿ� ��IDʶ��
    CityMap     m_CityLst;
	OpenFogMap m_open_fog; //����ܿ�����
    uint32      m_FortressLst[5][3]; //Ҫ��
    EventMap    m_privateEventLst;		//���˱���
    EventMap    m_publicEventLst;		//��������  ȡ�̶�������û����
	EventMap	m_celebrityEventLst; ///�ύ����
    ArmyResourceMap m_ArmyResourceMap;
    OneMapCapitalLst    m_OneMapCapitalLst; //[mapId] = <countryId, cityId>
	TunTianMap	m_tuntianMap;

	uint32		m_Tuntian[3];	//����

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

	std::vector<MapExpedition*> GetExpedition(uint32 countryID);	//���Զ������Ϣ������Ϊ����id��·��
	std::vector<MapExpedition*> GetGuard(uint32 countryID);		//��ý�������Ϣ������Ϊ����id����ǳص�

    void ForEachMapCity(uint32 mapId, boost::function<void(const MapCity&)>& func);
    void ForEachPrivateCityEvent(boost::function<void(const MapEvent&)>& func);
    void ForEachPublicCityEvent(boost::function<void(const MapEvent&)>& func);
	//void ForEachCelebrityCityEvent(boost::function<void(const MapEvent&)>func);
};
#define sMapTableMgr MapTableMgr::Instance()

#endif //__MapTableMgr_h__