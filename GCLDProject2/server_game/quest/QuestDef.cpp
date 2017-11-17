#include "QuestDef.h"




bool operator<( const BehaviorType &lhs, const BehaviorType &rhs )
{
    return lhs.m_behaviorType < rhs.m_behaviorType;
}

bool operator==( const BehaviorType &lhs, const BehaviorType &rhs )
{
    return lhs.m_behaviorType == rhs.m_behaviorType;
}

BehaviorType::BehaviorType( const uint64 &type ) : m_behaviorType( type )
{

}

BehaviorType::BehaviorType( void ) : m_behaviorType( 0 )
{

}

BehaviorType::BehaviorType( uint32 category1, uint32 category2, uint32 category3, uint32 condition , uint32 eventType) 
: m_condition( condition ), m_cat3( category3 ), m_cat2( category2 ), m_cat1( category1 ), m_eventType(eventType)
{

}
