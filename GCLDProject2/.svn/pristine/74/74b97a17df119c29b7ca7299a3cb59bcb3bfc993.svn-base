#pragma once

#include "def/TypeDef.h"

#include "../QuestDef.h"

namespace pb
{
    class GxDB_Behavior;
}


class BehaviorCounterBase
{
public:
    explicit BehaviorCounterBase(const BehaviorType& behaviorType) 
        : m_behaviorType(behaviorType), m_count(0), m_lastTriggerTime(0){};
    virtual ~BehaviorCounterBase(){};
    virtual bool TrggerBehavior( const BehaviorType& behavior , int count = 1 ) = 0;
    virtual int GetCount() const = 0;

    const BehaviorType& GetBehaviorType() const { return m_behaviorType;}
    
    void LoadFrom( const pb::GxDB_Behavior& Msg);
    void SaveTo( pb::GxDB_Behavior& Msg) const;

protected:
    BehaviorType m_behaviorType;
    uint32      m_count;
    time_t      m_lastTriggerTime;
};


