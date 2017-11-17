#ifndef StageLevelLog_h__
#define StageLevelLog_h__

#include "def/TypeDef.h"

namespace pb{
	class GS2C_StageLevel;
}

struct DB_StageLevel;
class StageLog;

class StageLevelLog
{
    enum
    {
        RewardDiff = 5,
    };
public:
	StageLevelLog( const DB_StageLevel& Proto, StageLog* stage);
	~StageLevelLog();
	
public:
	uint32 StageId() const;
    uint32 LevelId() const;
    bool   IsClearance() const;
    void   SetClearance() { m_isClearance = true; }

    uint32 GetFightCountCurPeriod() const;
    void AddFightCountCurPeriod( uint32 count);

	bool LoadFrom(const pb::GS2C_StageLevel& levelInfo);
	void SaveTo(pb::GS2C_StageLevel& levelInfo) const;

	int  GetStageType();
    const DB_StageLevel& Proto() const { return m_LevelProto; }

private:
	const DB_StageLevel& m_LevelProto;
	StageLog*   m_pStageLog;

	bool        m_isClearance;
	uint32      m_fightCount;
};

#endif //StageLevelLog_h__

