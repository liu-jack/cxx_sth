#ifndef StageManager_h__
#define StageManager_h__
#include "SimpleSingleton.h"
#include "def/TypeDef.h"
#include <boost/unordered_map.hpp>

#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP
struct DB_StageLevel;
struct DB_StageProto;
class StageManager : public SimpleSingleton< StageManager>
{
private:
    friend class SimpleSingleton< StageManager>;

#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32,const DB_StageLevel> LevelsMap;
	typedef PtrMap<uint32,const DB_StageProto> StageMap;
#else
	typedef PtrHashMap<uint32,const DB_StageLevel> LevelsMap;
	typedef PtrHashMap<uint32, const DB_StageProto> StageMap;
#endif

	typedef std::set<uint32> LevelIdSet;
	typedef boost::unordered_map<uint32, LevelIdSet> StageLevelsMap;

private:
	StageManager();

public:
	~StageManager();

public:
	void Init();
	const DB_StageLevel* GetStageLevelById(uint32 stateLevelId) const;
	const DB_StageProto*      GetStageById(uint32 stageId) const;
	const DB_StageProto*      GetStageByLevelId(uint32 levelId) const;

    uint32 GetStageLevelIdCount(uint32 stageId) const;
	const std::set<uint32>& GetAllLevelIds() const { return m_allLevelIds;}
private:
	StageMap        m_stageMap;			//关卡表
	LevelsMap		m_stageLevelMap;	//章节表

    StageLevelsMap  m_stageLevelIdsMap;	//每个关卡id所有的章节id

	LevelIdSet      m_allLevelIds;		//所有章节id
};

#define sStageManager StageManager::Instance()

#endif // StageManager_h__
