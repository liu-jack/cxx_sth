#ifndef DailyBehaviourCounter_h__
#define DailyBehaviourCounter_h__

#include "BehaviorCounterBase.h"
#include "memory_buffer/MemoryPool.h"


class DailyBehaviorCounter : public BehaviorCounterBase
{
    DECLARE_OBJ_POOL(DailyBehaviorCounter)
public:
    DailyBehaviorCounter(const BehaviorType& behavior); 
    virtual ~DailyBehaviorCounter(){};
    virtual bool TrggerBehavior( const BehaviorType& behavior , int count = 1 );
    virtual int GetCount()  const;
private:
    bool IsCountAvailable() const;
};

#endif // DailyBehaviourCounter_h__
