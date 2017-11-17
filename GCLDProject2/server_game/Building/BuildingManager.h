#ifndef BuildingManager_h__
#define BuildingManager_h__
#include "SimpleSingleton.h"
#include "def/TypeDef.h"
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP



class Module;
class Building;
struct DB_ImperialRebuild;
class BuildingManager : public SimpleSingleton< BuildingManager>
{
private:
    friend class SimpleSingleton< BuildingManager>;

#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, Module> ModulesMap;
	typedef PtrMap<uint32, Building> BuildingMap;
	typedef PtrMap<uint32, const DB_ImperialRebuild> ImperialRebuildMap;
#else
	typedef PtrHashMap<uint32, Module> ModulesMap;
	typedef PtrHashMap<uint32, Building> BuildingMap;
	typedef PtrHashMap<uint32,const DB_ImperialRebuild> ImperialRebuildMap;
#endif

private:
	BuildingManager();

public:
	~BuildingManager();

public:
	void Init();
	const DB_ImperialRebuild* GetRebuildInvestInfo(uint32 roundId);
	const Module* GetModuleById(uint32 Id) const;
	const Building*      GetBuildingById(uint32 Id) const;
	uint32 GetBuildingModuleId( uint32 Id) const;
	const void GetBuildingIds(std::vector<uint32>& vecBuildIds) const;
	const void GetMouduleIds( std::vector<uint32>& vecModuldIds )const;
	const void      GetModuleBuildingIds(uint32 moduleId,std::vector<uint32>& vecBuildIds) const;
	uint32 GetBuildHonorForLvUp(uint32 buildId, uint32 lv);
	uint32 GetBuildExpForLvUp( uint32 buildId, uint32 lv );
	uint32 GetMoudleTimeType(uint32 modId) const;
	uint32 GetMoudleIncreaseTimeInterval( uint32 modId ) const;
private:
	BuildingMap			m_buildingMap;
	ModulesMap			m_moduleMap;
	ImperialRebuildMap  m_RebuildInvestMap;
	std::set<uint32>    m_moduleIdSet;
	std::set<uint32>    m_buildIdSet;
};
#define sBuildingManager BuildingManager::Instance()

#endif // BuildingManager_h__
