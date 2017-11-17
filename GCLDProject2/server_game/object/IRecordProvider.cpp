#include "IRecordProvider.h"
#include "quest/QuestDef.h"



IRecordProvider::~IRecordProvider()
{
	m_gettersWithParam.clear();
	m_getters.clear();
}

bool IRecordProvider::GetRecord( uint64 behavior, uint32& outRecord ) const
{
    if ( m_defaultGetter( behavior, outRecord))
    {
        return true;
    }

    GetterMap::const_iterator iter = m_getters.find(behavior);
    if ( iter != m_getters.end())
    {
        outRecord = (uint32)(iter->second)();
        return true;
    }
    
    BehaviorType bt( behavior);
    GetterWithParamMap::const_iterator iter2 = m_gettersWithParam.find( bt.m_counterType);
    if ( iter2 != m_gettersWithParam.end())
    {
        outRecord = (uint32)(iter2->second)( bt.m_condition);
        return true;
    }
// 	GetterWithParamMap2::const_iterator iter3 = m_getterWithParam2.find(bt.m_counterType);
// 	if(iter3!= m_getterWithParam2.end()){
// 		outRecord = (uint32)(iter3->second)(std::vector<char>(bt.m_condition));
// 		return true;
// 	}
    
    return false;
}

void IRecordProvider::RegistRecordGetter( uint64 behavior, const RecordGetter& getter )
{
    m_getters[behavior] = getter;
}

void IRecordProvider::RegistDefault( const DefaultGetter& getter )
{
    m_defaultGetter = getter;
}

void IRecordProvider::RegistGetterWithParam( uint32 counterType, const RecordGetterWithParam& getter )
{
    m_gettersWithParam[counterType] = getter;
}


