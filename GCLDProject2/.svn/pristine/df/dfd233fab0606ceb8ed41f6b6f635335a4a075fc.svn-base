#include "BuildingManager.h"
#include "DbTool.h"
#include "datastruct/struct_building.h"
#include "Building.h"
#include "Module.h"
#include "BuildingLevel.h"
#include "BuildingDefine.h"
BuildingManager::BuildingManager()
{
}

BuildingManager::~BuildingManager()
{
}

void BuildingManager::Init()
{
	FOREACH_DB_ITEM( DBModule, DB_Module)
	{
		uint32 moduleid = DBModule->id;
		Module* pModule = new Module(*DBModule);
		m_moduleMap[moduleid] = pModule;
		if (pModule)
		{
			m_moduleIdSet.insert(pModule->id());
		}
	}

	FOREACH_DB_ITEM(DBproto, DB_BuildingProto)
	{
		uint32 protoID = DBproto->id;
		Building* pBuilding = new Building(*DBproto);
		m_buildingMap[protoID] = pBuilding;
		Module* pModule = m_moduleMap.pfind(pBuilding->moduleId());
		if (pModule)
		{
			pModule->AddBuildId(protoID);
			m_buildIdSet.insert(protoID);
		}
	}

	FOREACH_DB_ITEM(DBuildingLv, DB_BuildingLevelUp)
	{
		BuildingLevel* pBuildinglv = new BuildingLevel(*DBuildingLv);
		if (pBuildinglv)
		{
			Building* pBuilding = m_buildingMap.pfind(pBuildinglv->building_id());
			if (pBuilding)
			{
				pBuilding->AddBuildingLevel(pBuildinglv);
			}
		}
	}

	FOREACH_DB_ITEM(DBbuildInvest, DB_ImperialRebuild)
	{
		m_RebuildInvestMap[DBbuildInvest->id] = DBbuildInvest;
	}
}

const DB_ImperialRebuild* BuildingManager::GetRebuildInvestInfo(uint32 roundId)
{
	return m_RebuildInvestMap.pfind(roundId);
}

const Module* BuildingManager::GetModuleById( uint32 Id ) const
{
	return m_moduleMap.pfind(Id);
}

const Building* BuildingManager::GetBuildingById( uint32 Id ) const
{
	return m_buildingMap.pfind(Id);
}

uint32 BuildingManager::GetBuildingModuleId( uint32 Id) const
{
	const Building* curBuild = m_buildingMap.pfind(Id);
	if (curBuild)
	{
		return curBuild->moduleId();
	}
	return 0;
}

const void BuildingManager::GetBuildingIds( std::vector<uint32>& vecBuildIds ) const
{
	std::copy(m_buildIdSet.begin(),m_buildIdSet.end(),std::back_inserter(vecBuildIds));
}

const void BuildingManager::GetMouduleIds( std::vector<uint32>& vecModuldIds ) const
{
	std::copy(m_moduleIdSet.begin(),m_moduleIdSet.end(),std::back_inserter(vecModuldIds));
}

const void BuildingManager::GetModuleBuildingIds( uint32 moduleId,std::vector<uint32>& vecBuildIds ) const
{
	Module* pModule = m_moduleMap.pfind(moduleId);
	if (pModule)
	{
		pModule->GetBuildIds(vecBuildIds);
	}
}

uint32 BuildingManager::GetBuildHonorForLvUp( uint32 buildId, uint32 lv )
{
	Building* pBuilding = m_buildingMap.pfind(buildId);
	if (pBuilding)
	{
		return pBuilding->Honor(lv);
	}
	return 0;
}

uint32 BuildingManager::GetBuildExpForLvUp( uint32 buildId, uint32 lv )
{
	Building* pBuilding = m_buildingMap.pfind(buildId);
	if (pBuilding)
	{
		return pBuilding->Exp(lv);
	}
	return 0;
}

uint32 BuildingManager::GetMoudleTimeType( uint32 modId ) const
{
  const Module* curModule = GetModuleById(modId);
  if (curModule)
  {
	  return curModule->TimeType();
  }
  return 0;
}

uint32 BuildingManager::GetMoudleIncreaseTimeInterval( uint32 modId ) const
{
	uint32 timeInterval = 0;
	const Module* curModule = GetModuleById(modId);
	if (curModule)
	{
		uint32 timerType = curModule->TimeType();
		switch(timerType)
		{
		case TIMERBYMINUTE:
			{
				timeInterval = TIMERBYMINUTESECONDS;
			}
			break;
		case TIMERBYHOUR:
			{
				timeInterval = TIMERBYHOURSECONDS;
			}
			break;
		case TIMERBYFOURHOUR:
			{
				timeInterval = TIMERBYFOURHOURSECONDS;
			}
			break;
		}
	}
	return timeInterval;
}