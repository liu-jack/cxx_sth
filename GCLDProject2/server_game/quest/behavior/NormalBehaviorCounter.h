#ifndef NormalBehaviorCounter_h__
#define NormalBehaviorCounter_h__

#include "memory_buffer/MemoryPool.h"

#include "BehaviorCounterBase.h"


class NormalBehaviorCounter : public BehaviorCounterBase
{
    DECLARE_OBJ_POOL(NormalBehaviorCounter)
public:
    NormalBehaviorCounter( const BehaviorType& behavior);
    virtual ~NormalBehaviorCounter(){};
    virtual bool TrggerBehavior( const BehaviorType& behavior , int count = 1 );
    virtual int GetCount()  const;
    virtual void LoadFrom( const void * pMsg) {};
};



#endif // NormalBehaviorCounter_h__
