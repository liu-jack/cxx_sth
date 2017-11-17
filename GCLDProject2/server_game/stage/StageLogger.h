#ifndef StageLogger_h__
#define StageLogger_h__
#include "def/TypeDef.h"
#include "StageDefine.h"

#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP

#include "boost/scoped_ptr.hpp"


namespace pb
{
	class GS2C_AllStageInfo;
}

class PlayerValueOwner;
class StageLog;
struct DB_StageLevel;
class StageLevelLog;


class StageLogger
{
public:
	StageLogger();
	~StageLogger();
private:
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, StageLog> StageLogMap;
#else
	typedef PtrHashMap<uint32, StageLog> StageLogMap;
#endif // USE_PTR_HASH_MAP
	
public: 
	void LoadFrom(const pb::GS2C_AllStageInfo& logInfo);
    
    uint32 CheckGoBattleState( uint32 stage_level_id, const PlayerValueOwner& valueOwner) const;

    bool IsLevelClearance( uint32 level_id ) const;
    bool IsStageClearance( uint32 stage_id) const;

    bool IsStageUnlock( uint32 stage_id, const PlayerValueOwner& valueOwner) const;
    bool IsStageLevelUnlock( uint32 level_id , const PlayerValueOwner& valueOwner) const;

	void SaveStageLoggerTo(pb::GS2C_AllStageInfo& info);
    StageLevelLog* AddOrGetStageLevelLog( uint32 level_id, bool& newStage, bool& newLevel);
    StageLevelLog* GetStageLevelLog( uint32 level_id) const;

    StageLog* GetStageLogForLevelId( uint32 levelId) const;
    StageLog* GetStageLogForStageId( uint32 stageId) const;


protected:
	StageLogMap     m_stageLogMap;

};

#endif // StageLogger_h__
