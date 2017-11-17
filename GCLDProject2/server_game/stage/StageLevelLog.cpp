#include "StageLevelLog.h"

#include "OS.h"

#include "Stage.pb.h"
#include "StageLog.h"
#include "StageDefine.h"
#include "StageManager.h"
//#include "object/PlayerDataManager.h"

#include "def/ObjectDefines.h"
//#include "Enum.pb.h"
#include "datastruct/struct_stage.h"
using namespace pb;

//构造函数初始化,传入一个章节的信息
StageLevelLog::StageLevelLog( const DB_StageLevel& Proto, StageLog* stage)
: m_LevelProto(Proto)
, m_pStageLog(stage)
, m_isClearance(false)
, m_fightCount( 0)
{

}

StageLevelLog::~StageLevelLog()
{
}

//获得关卡id
uint32 StageLevelLog::StageId() const
{
	return m_LevelProto.stage_id;
}

//获得章节id
uint32 StageLevelLog::LevelId() const
{
	return m_LevelProto.id;
}

//增加当前时间片的攻击次数
void StageLevelLog::AddFightCountCurPeriod( uint32 count )
{
	m_fightCount += count;
}

//保存参数中的信息，章节是否扫清，攻击次数
bool StageLevelLog::LoadFrom(const pb::GS2C_StageLevel& levelInfo)
{
	m_isClearance          =levelInfo.is_clearance();
	m_fightCount = levelInfo.fight_count();
	return true;
}

//向参数传递数据，章节id，章节是否扫清，攻击次数
void StageLevelLog::SaveTo(pb::GS2C_StageLevel& levelInfo) const
{
	levelInfo.set_level_id( LevelId());
	levelInfo.set_fight_count( m_fightCount);
	levelInfo.set_is_clearance( IsClearance());
}

//获得章节类型，0,1,2,3,4
int StageLevelLog::GetStageType()
{
	return m_LevelProto.stage_type;
}

//章节是否扫清
bool StageLevelLog::IsClearance() const
{
    return m_isClearance;
}




