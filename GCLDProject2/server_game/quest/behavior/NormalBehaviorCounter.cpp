#include "NormalBehaviorCounter.h"
#include "time.h"
#include "OS.h"


NormalBehaviorCounter::NormalBehaviorCounter( const BehaviorType& behavior)
: BehaviorCounterBase(behavior)
{
    
}

bool NormalBehaviorCounter::TrggerBehavior( const BehaviorType& behavior , int count/* = 1*/ )
{
    if (GetBehaviorType() == behavior)
    {
        m_count += count;
        m_lastTriggerTime = sOS.GetRealTime();
        return true;
    }
    else
    {
        return false;
    }

}

int NormalBehaviorCounter::GetCount() const
{
    return m_count;
}


