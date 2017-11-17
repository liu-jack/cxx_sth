#pragma once

#include "data/DbProxyDataMgr.h"
#include "def/TypeDef.h"
#include "def/MmoAssert.h"

// «ø“¿¿µ”⁄ sDataMgr.GetTable< Table>()

template< class Table>
class CachePtr
{
public:
    CachePtr() : m_pCachePtr( NULL){} 

    inline Table* GetContent( ) const
    {
        return m_pCachePtr; 
    }
    
    bool Exist() const 
    {
        return m_pCachePtr != NULL; 
    }
    
    void SetContent( Table* table)
    {
        MMO_ASSERT( m_pCachePtr == NULL);
        m_pCachePtr = table;
    }

    Table* SetAndAddToCache( Table* table)
    {
        MMO_ASSERT( m_pCachePtr == NULL);
		m_pCachePtr = sDataMgr.SaveAndAddTableEntry<Table>(table);
        return m_pCachePtr;
    }
    
    void RemoveContentFromCache()
    {
        if ( m_pCachePtr)
            sDataMgr.GetTable<Table>()->RemoveKeyEntry( m_pCachePtr);
    }
    
    void SaveMod()
    {
        if ( m_pCachePtr)
        {
            sDataMgr.GetTable<Table>()->WriteEntry( m_pCachePtr);
        } 
    }

    Table * operator->() const // never throws
    {
        MMO_ASSERT( m_pCachePtr != 0 );
        return m_pCachePtr;
    }

    operator bool() const
    {
        return m_pCachePtr != 0;
    }
private:
    Table*      m_pCachePtr ;
};
