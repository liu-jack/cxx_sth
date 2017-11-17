#include "StageLogger.h"

#include <boost/smart_ptr.hpp>
#include "utility/STLUtility.h"
#include "System.h"

#include "StageLog.h"
#include "StageLevelLog.h"
#include "StageManager.h"
#include "datastruct/struct_stage.h"
#include "object/PlayerDataManager.h"
#include "object/PlayerValueOwner.h"    


// 无尽模式添加
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h" 
#include "Enum.pb.h"
#include "Stage.pb.h"

using namespace pb;

StageLogger::StageLogger()
{
}

StageLogger::~StageLogger()
{
}

//保存参数中的信息，章节信息与关卡信息
void StageLogger::LoadFrom(const pb::GS2C_AllStageInfo& logInfo)
{
	m_stageLogMap.clear();
	for (int j = 0; j < logInfo.stage_level_infos_size(); j++ )
	{
		const GS2C_StageLevel& curLevelInfo = logInfo.stage_level_infos(j);
        const DB_StageProto* curStage = sStageManager.GetStageByLevelId( curLevelInfo.level_id());
        if ( curStage == NULL)
            continue;

        uint32  curstageId = curStage->id;
        StageLog* stage_log = m_stageLogMap.pfind( curstageId);
        if (stage_log == NULL)
        {
            stage_log = new StageLog(*curStage);
            if ( stage_log != NULL)
            {
                m_stageLogMap[curstageId] = stage_log;
            }
        }

        if (stage_log != NULL)
        {
            stage_log->CreateLevel(curLevelInfo);
        }
	}
}


//检查章节是否可以扫清
uint32 StageLogger::CheckGoBattleState( uint32 level_id, const PlayerValueOwner& valueOwner) const
{
    const DB_StageLevel* theLevel = sStageManager.GetStageLevelById( level_id);
    if ( theLevel == NULL)		//未找到章节
        return pb::ErrInvalidProto;
    if ( !IsStageUnlock( theLevel->stage_id, valueOwner))		//该关卡未解锁
        return pb::ErrStageLock;

    if ( !IsStageLevelUnlock( level_id, valueOwner))			//该章节未解锁
        return pb::ErrStageLevelLock;

	/*if (!valueOwner.EnoughCurrency(theLevel->level_cost_type, theLevel->level_cost_value))
	{
		return pb::ErrNotEnoughCurr;
	}*/
        
    
    return pb::ErrCommonSuccess;
}

//向参数传递数据，所有关卡章节信息
void StageLogger::SaveStageLoggerTo(pb::GS2C_AllStageInfo& info)
{
	StageLogMap::iterator iterStage = m_stageLogMap.begin();
	for (; iterStage != m_stageLogMap.end(); iterStage++)
	{
		StageLog* pStage = iterStage->second;
		if (pStage)
		{
			pStage->SaveTo(info);
		}
	}
}

//关卡是否扫清
bool StageLogger::IsStageClearance( uint32 stage_id ) const
{
    StageLog* stage_log = m_stageLogMap.pfind( stage_id);
    if ( stage_log == NULL)
    {
        return false;
    }
    else
    {
        return stage_log->IsStageClearance();
    }
}

//关卡是否解锁
bool StageLogger::IsStageUnlock( uint32 stage_id , const PlayerValueOwner& valueOwner) const
{
    const DB_StageProto* stage = sStageManager.GetStageById( stage_id);
    if ( stage == NULL)
        return false;

    bool isUnlock = false;

    if (valueOwner.GetLevel() >= (int)stage->stage_unlock_level)
    {
		isUnlock = true;
    }
    return isUnlock;
}

//章节是否解锁
bool StageLogger::IsStageLevelUnlock( uint32 level_id , const PlayerValueOwner& valueOwner) const
{
    const DB_StageLevel* stagelevel = sStageManager.GetStageLevelById( level_id);
    if ( stagelevel == NULL)
        return false;

    bool isUnlock = false;
	if (stagelevel->level_unlock_guanqia == 0)
	{
		isUnlock = true;
	}
	else if (IsLevelClearance(stagelevel->level_unlock_guanqia) && (valueOwner.GetLevel() >= (int)stagelevel->level_unlock_level))
	{
		isUnlock = true;
	}

    return isUnlock;    
}

//章节是否扫清
bool StageLogger::IsLevelClearance( uint32 level_id ) const
{
    StageLevelLog* stageLevelLog = GetStageLevelLog( level_id);
    if ( stageLevelLog)
    {
        return stageLevelLog->IsClearance();
    }
    return false;
}

//获得章节
StageLevelLog* StageLogger::GetStageLevelLog( uint32 level_id ) const
{
    const DB_StageLevel* theStageLevel = sStageManager.GetStageLevelById( level_id);
    if ( theStageLevel != NULL)
    {
        StageLog* stage_log = m_stageLogMap.pfind( theStageLevel->stage_id);
        if ( stage_log )
        {
            return	stage_log->GetStageLevelLog( level_id);
        }
    }
    return NULL;
}

//（添加）获得章节
StageLevelLog* StageLogger::AddOrGetStageLevelLog( uint32 level_id, bool& newStage, bool& newLevel)
{
    newStage = false;
    newLevel = false;
    const DB_StageProto* theStage = sStageManager.GetStageByLevelId( level_id);
    if( theStage != NULL )
    {
        StageLog* stage_log = m_stageLogMap.pfind( theStage->id );
        if ( stage_log == NULL)
        {
            stage_log = new StageLog( *theStage);
            if ( stage_log)
            {
                m_stageLogMap[ theStage->id] = stage_log;
                newStage = true;
            }
        }

        if ( stage_log != NULL)
        {
            return stage_log->AddOrGetStageLevelLog( level_id, newLevel);
        }
    }

    return NULL;
}

//通过章节id获得关卡
StageLog* StageLogger::GetStageLogForLevelId( uint32 level_id ) const
{
    const DB_StageProto* theStage = sStageManager.GetStageByLevelId( level_id);
    if ( theStage)
    {
        return m_stageLogMap.pfind( theStage->id);
    }
    return NULL;

}

//通过关卡id获得关卡
StageLog* StageLogger::GetStageLogForStageId( uint32 stageId ) const
{
    return m_stageLogMap.pfind( stageId);
}

