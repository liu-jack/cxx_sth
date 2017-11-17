#include "StageManager.h"
#include "DbTool.h"
#include "datastruct/struct_stage.h"
StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
	FOREACH_DB_ITEM(DBproto, DB_StageProto)
	{
		uint32 protoID = DBproto->id;
		m_stageMap[protoID] = DBproto;
	}

	
	FOREACH_DB_ITEM( DBStageLevel, DB_StageLevel)
	{
		uint32 levelID = DBStageLevel->id;
		uint32 stageID = DBStageLevel->stage_id;
		m_stageLevelMap[levelID] = DBStageLevel;
		m_allLevelIds.insert(levelID);
		m_stageLevelIdsMap[stageID].insert(levelID);
	}
}

const DB_StageLevel* StageManager::GetStageLevelById( uint32 stateLevelId) const
{
	return m_stageLevelMap.pfind(stateLevelId);
}

const DB_StageProto* StageManager::GetStageById( uint32 stageId) const
{
	return m_stageMap.pfind( stageId);
}



const DB_StageProto* StageManager::GetStageByLevelId( uint32 levelId) const
{
    const DB_StageLevel* stage_level = GetStageLevelById( levelId);
    if ( stage_level == NULL)
    {
        return NULL;
    }
    else
    {
        return GetStageById( stage_level->stage_id);
    }
}

uint32 StageManager::GetStageLevelIdCount( uint32 stageId ) const
{
    StageLevelsMap::const_iterator iter = m_stageLevelIdsMap.find(stageId);
    if (iter != m_stageLevelIdsMap.end())
    {
        const LevelIdSet& idSet = iter->second;
		return idSet.size();
    }
    return 0;
}
