#include "BuildingLevel.h"
#include "datastruct/struct_building.h"

#include "BuildingDefine.h"
//#include "BuildingManager.h"

//#include "def/MmoAssert.h"
//#include "Loot/LootManager.h"
//
//#include "object/PlayerDataManager.h"
//#include "object/PlayerValueOwner.h"
//
//#include "BaseDefineMgr.h"
//#include "BaseDefine.pb.h"


BuildingLevel::BuildingLevel(const DB_BuildingLevelUp & dbBuildingLv)
:m_dbBuildingLvProto(dbBuildingLv)
{
}

BuildingLevel::~BuildingLevel()
{
}

uint32 BuildingLevel::id() const
{
	return m_dbBuildingLvProto.id;
}

uint32 BuildingLevel::building_id() const
{
	return m_dbBuildingLvProto.build_id;
}

uint32 BuildingLevel::lvUpTime() const
{
	return m_dbBuildingLvProto.time_req;
}

uint32 BuildingLevel::Icome() const
{
	return m_dbBuildingLvProto.growspeed;
}

uint32 BuildingLevel::IcomLimit() const
{
	return m_dbBuildingLvProto.limit_raise;
}

uint32 BuildingLevel::coinReq() const
{
 return m_dbBuildingLvProto.coin_req;
}

uint32 BuildingLevel::woodReq() const
{
 return m_dbBuildingLvProto.wood_req;
}

uint32 BuildingLevel::lv() const
{
	return m_dbBuildingLvProto.lv;
}

uint32 BuildingLevel::honor() const
{
	return m_dbBuildingLvProto.honor_reward;
}

uint32 BuildingLevel::exp() const
{
	return m_dbBuildingLvProto.exp_reward;
}

