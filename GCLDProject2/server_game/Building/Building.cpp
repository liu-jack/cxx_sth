#include "Building.h"
#include "datastruct/struct_building.h"

#include "BuildingDefine.h"
#include "BuildingManager.h"
#include "BuildingLevel.h"

Building::Building(const DB_BuildingProto & dbBuildingProto)
:m_dbBuildingProto(dbBuildingProto)
,m_maxLv(0)
{
}

Building::~Building()
{
}

uint32 Building::id() const
{
	return m_dbBuildingProto.id;
}

void Building::AddBuildingLevel( BuildingLevel* buildLv )
{
	if (buildLv)
	{
		if (m_mapBuildingLv.pfind(buildLv->lv()))
		{
			m_mapBuildingLv.erase(buildLv->lv());
			m_mapBuildingLv[buildLv->lv()] = buildLv;
		   uint32 bLv =	buildLv->lv();
		   if (bLv > m_maxLv)
		   {
			   m_maxLv = bLv;
		   }
		} 
		else
		{
			m_mapBuildingLv[buildLv->lv()] = buildLv;
			uint32 bLv = buildLv->lv();
			if (bLv > m_maxLv)
			{
				m_maxLv = bLv;
			}
		}
	}
}

uint32 Building::moduleId() const
{
	return m_dbBuildingProto.module_id;
}

uint32 Building::LvUpTime( uint32 curLv ) const
{
	BuildingLevel* pBuildLv = m_mapBuildingLv.pfind(curLv);
	if (pBuildLv)
	{
		return pBuildLv->lvUpTime();
	}
	else
	{
		return 0;
	}
}

uint32 Building::LvReq() const
{
	return m_dbBuildingProto.level_req;
}

uint32 Building::Honor( uint32 lv ) const
{
	BuildingLevel* pBuildLv = m_mapBuildingLv.pfind(lv);
	if (pBuildLv)
	{
		return pBuildLv->honor();
	}
	else
	{
		return 0;
	}
}

uint32 Building::Exp(uint32 lv) const
{
	BuildingLevel* pBuildLv = m_mapBuildingLv.pfind(lv);
	if (pBuildLv)
	{
		return pBuildLv->exp();
	}
	else
	{
		return 0;
	}
}

uint32 Building::Icome( uint32 lv ) const
{
	BuildingLevel* pBuildLv = m_mapBuildingLv.pfind(lv);
	if (pBuildLv)
	{
		return pBuildLv->Icome();
	}
	return 0;
}

uint32 Building::IcomeLimit( uint32 lv ) const
{
	BuildingLevel* pBuildLv = m_mapBuildingLv.pfind(lv);
	if (pBuildLv)
	{
		return pBuildLv->IcomLimit();
	}
	return 0;
}

uint32 Building::NextRebuildId() const
{
	return m_dbBuildingProto.rebuild_order;
}

uint32 Building::RebuildLaborNeed() const
{
	return m_dbBuildingProto.need_people_num;
}

uint32 Building::RebuildNeedTime() const
{
	return m_dbBuildingProto.need_times;
}

uint32 Building::RebuildResAdd() const
{
	return m_dbBuildingProto.resource_add;
}

bool Building::LvUpReq( uint32 lv,uint32& coinReq, uint32& woodReq ) const
{
	const BuildingLevel* pBuildLv = m_mapBuildingLv.pfind(lv);
	if (pBuildLv)
	{
		coinReq = pBuildLv->coinReq();
		woodReq = pBuildLv->woodReq();
		return true;
	}
	return false;
}