#pragma once

#include "SimpleSingleton.h"
#include "boost/numeric/conversion/cast.hpp"
#include "DbTool.h"
#include "def/MmoAssert.h"

class BaseDefineMgr : public SimpleSingleton<BaseDefineMgr>
{
    friend class SimpleSingleton<BaseDefineMgr>;
    typedef std::vector< const BaseDefine* > BaseDefVec;
public:
    template<typename T>
    T GetValue( int id, T defaultValue);
    void Init();
private:
    BaseDefineMgr();
    ~BaseDefineMgr(){};

    BaseDefVec  m_baseDefVec;

};

template<typename T>
T BaseDefineMgr::GetValue( int id, T defaultValue)
{
    const BaseDefine* def = m_baseDefVec[ id];
    MMO_ASSERT( def != NULL) ;
    if (def)
    {
        return boost::numeric_cast<T>(def->value);
    }
    return defaultValue;    
}

#define sBaseDefineMgr BaseDefineMgr::Instance()

#define GET_BASE_DEF_UINT( id) sBaseDefineMgr.GetValue<unsigned>(id,0)
#define GET_BASE_DEF_INT( id)   sBaseDefineMgr.GetValue<int>(id,0)
#define GET_BASE_DEF_FLOAT( id) sBaseDefineMgr.GetValue<float>(id,0.0)
