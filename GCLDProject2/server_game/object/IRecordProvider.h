#ifndef IRecordProvider_h__
#define IRecordProvider_h__

#include "def/TypeDef.h"
#include "boost/function.hpp"
union BehaviorType;

typedef boost::function<int()> RecordGetter;
typedef boost::function<int(int)> RecordGetterWithParam;
typedef boost::function<bool(uint64,uint32&)> DefaultGetter;



class IRecordProvider                      
{
    typedef std::map<uint64, RecordGetter> GetterMap;
    typedef std::map<uint64, RecordGetterWithParam> GetterWithParamMap;

public:
	~IRecordProvider();
public:
    virtual bool GetRecord( uint64 behavior, uint32& outRecord) const;
protected:
    void RegistRecordGetter( uint64 behavior, const RecordGetter& getter );
    void RegistDefault( const DefaultGetter& getter );
    void RegistGetterWithParam( uint32 counterType, const RecordGetterWithParam& getter);

private:
    GetterMap m_getters;
    GetterWithParamMap m_gettersWithParam;
    DefaultGetter m_defaultGetter;

};



#endif // IRecordProvider_h__
