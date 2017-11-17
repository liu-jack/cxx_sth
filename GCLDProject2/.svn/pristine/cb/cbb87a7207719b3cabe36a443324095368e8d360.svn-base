#include "DailyBehaviorCounter.h"

#include "OS.h"
#include "utility/TimePeriod.h"



DailyBehaviorCounter::DailyBehaviorCounter( const BehaviorType& behavior )
: BehaviorCounterBase(behavior)
{
    
}

bool DailyBehaviorCounter::TrggerBehavior( const BehaviorType& behavior , int count /*= 1*/)
{
    if ( GetBehaviorType() == behavior )
    {
        if ( !IsCountAvailable() )
        {
            m_count = 0;
        }
        m_count += count;
        m_lastTriggerTime = sOS.GetRealTime();
        return true;
    }
    else
    {
       return false;
    }
}

int DailyBehaviorCounter::GetCount() const
{
    if ( IsCountAvailable())
    {
        return m_count;
    }
    else
    {
        return 0;
    }
}

bool DailyBehaviorCounter::IsCountAvailable() const
{
    return sPeriodSystem.IsInSameTimePeriod(m_lastTriggerTime, sOS.GetRealTime(), SECONDS_PER_DAY);
}
