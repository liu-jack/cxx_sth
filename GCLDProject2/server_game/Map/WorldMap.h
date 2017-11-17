#pragma once
#include "base/BaseMap.h"
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossWorldMap.h"
#endif
class Country;

class WorldMap : public CMap 
{
	WorldMap(uint32 mapTid, CMap::MapEnum typ);
public:
    // 常规的世界地图
    static WorldMap& Instance(){ static WorldMap T(0, CMap::World); return T; }

public:
    //virtual func
    void Init();

	//void updateArmyResourcePerHour();
	
	//void updateWorldMapAwardsPrivate();
	//void updateWorldMapAwardsPublic();

	uint32 CreateExpeditionID(uint32 heroid);
	
	bool isExpeditionMoveTo(uint32 expeid, uint32 cityid);
	void setExpeditionMoveTo(uint32 expeid, uint32 cityid);

	void addCountryExp(uint32 countryid, uint32 expnum);
	void addCountryNpcExp(uint32 countryid, uint32 expnum);

	Country* GetCountryByUniqueIdInCross(uint32 uniqueid);
	
	void InitArmysAfterDB();

private:
	std::map<uint32, uint32> expe_city_rec;		//<expeid, cityid>

};

#ifdef _MMO_SERVER_
#define sWorldMap WorldMap::Instance()
#endif

#ifdef _SERVER_CROSS_
#define sWorldMap CrossWorldMap::Instance()
#endif