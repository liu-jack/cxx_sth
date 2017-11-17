#include "QuestRecord.h"

#include "OS.h"
#include "QuestProto.h"
#include "utility/TimePeriod.h"
#include "../object/IRecordProvider.h"
#include "Quest.pb.h"



QuestRecord::QuestRecord( const QuestProto& proto, const IRecordProvider& provider)
: m_proto( proto)
, m_activeTime( 0)
, m_provider( provider)
{
}

bool QuestRecord::IsFinished() const
{
    return GetObjectiveValue() >= TargetValue();
}


uint32 QuestRecord::TargetValue() const
{
    return m_proto.TargetValue();
}

bool QuestRecord::IsRepeatable() const
{
    return m_proto.IsRepeatable();
}

bool QuestRecord::IsSequentQuest() const
{
    return m_proto.IsSequentQuest();
}

bool QuestRecord::IsActived() const
{
    if ( Proto().IsRepeatable())
    {
        return m_activeTime <= sOS.GetRealTime();
    }
    else
    {
        return true;
    }
}

uint32 QuestRecord::GetObjectiveValue() const
{
    uint32 record = 0;
    m_provider.GetRecord( Proto().GetBehaviorType().GetUint64(), record);
    return record;
}

void QuestRecord::SetActiveTime( time_t t )
{
    m_activeTime = t;
}

void QuestRecord::SetInactiveUntilNextPeriod()
{
    SetActiveTime( sPeriodSystem.GetNowPeriodEndTime(SECONDS_PER_DAY) );
}

void QuestRecord::ResetActiveTime()
{
    SetActiveTime( sPeriodSystem.GetNowPeriodBeginTime(SECONDS_PER_DAY) );
}

void QuestRecord::SaveTo( pb::GxDB_Quest& questMsg ) const
{
    questMsg.set_is_deleted( false);
    questMsg.set_quest_id( Proto().ID());
    questMsg.set_active_time( m_activeTime);
    questMsg.set_value( GetObjectiveValue());
}


void QuestRecord::LoadFrom( const pb::GxDB_Quest& questMsg )
{
    m_activeTime = questMsg.active_time();
}

bool QuestRecord::IsMatchVipLv( int vipLv ) const
{
    return Proto().MatchVipRequire( vipLv);
}

bool QuestRecord::IsMatchLevel( int level ) const
{
    return Proto().MatchLevelRequire( level);
}



uint64 QuestRecord::GetActiveTime() const{
	return m_activeTime;
}


bool QuestRecord::CheckCategory(uint32 category1,uint32 category2,uint32 category3) const
{
	return m_proto.CheckCategory(category1,category2,category3);
}
