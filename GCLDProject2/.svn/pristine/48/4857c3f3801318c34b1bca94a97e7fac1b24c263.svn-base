#include "BehaviorCounterBase.h"
#include "Quest.pb.h"


void BehaviorCounterBase::LoadFrom( const pb::GxDB_Behavior& Msg )
{
    m_count = Msg.count();
    m_lastTriggerTime = Msg.last_trigger_time();
}

void BehaviorCounterBase::SaveTo( pb::GxDB_Behavior& Msg ) const
{
    Msg.set_behavior( m_behaviorType.GetUint64());
    Msg.set_count( m_count);
    Msg.set_last_trigger_time( m_lastTriggerTime);
}
