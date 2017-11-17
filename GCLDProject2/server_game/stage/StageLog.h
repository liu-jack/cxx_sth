#ifndef StageLog_h__
#define StageLog_h__
#include "def/TypeDef.h"

#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP


class StageLevelLog;
struct DB_StageProto;
namespace pb
{
	class GS2C_StageReward;
	class GS2C_AllStageInfo;
	class GS2C_StageLevel;
}

class StageLog
{
private:
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, StageLevelLog> LevelLogsMap;
#else
	typedef PtrHashMap<uint32, StageLevelLog> LevelLogsMap;
#endif // USE_PTR_HASH_MAP
public:
	StageLog( const DB_StageProto& Proto);
	~StageLog();
	void SaveTo(pb::GS2C_AllStageInfo& info) const;   

    uint32 StageId() const;
    bool IsStageClearance() const;

    bool CreateLevel(const pb::GS2C_StageLevel& levelInfo);

	StageLevelLog* GetStageLevelLog( uint32 levelId );
    StageLevelLog* AddOrGetStageLevelLog( uint32 levelId, bool& newLevel);

    const DB_StageProto& Proto() const { return m_StageProto;}

private:
	const DB_StageProto& m_StageProto;
	LevelLogsMap m_stageLevelsMap;
};
#endif //StageLog_h__