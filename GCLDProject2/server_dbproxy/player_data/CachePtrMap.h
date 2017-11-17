#ifndef _CACHEPTRMAP_DEFINE_
#define _CACHEPTRMAP_DEFINE_

#include <map>
#include "../data/DbProxyDataMgr.h"
#include "def/TypeDef.h"
#include "def/MmoAssert.h"
#include "ModMarker.h"

// for test
#include "Logger.h"
// «ø“¿¿µ”⁄ sDataMgr.GetTable< Table>()

template< typename KType, class Table>
class CachePtrMap : public ModMarker< KType >, private std::map< KType, Table*>
{
public:
    ~CachePtrMap()
    {
    }
    typedef std::map< KType, Table*> Key2Table;
    typedef typename std::map< KType, Table*>::iterator Iter;
    typedef typename std::map< KType, Table*>::reverse_iterator RIter;

    Table* GetElement( KType id ) const
    {
        typename Key2Table::const_iterator it = Key2Table::find( id);
        if ( it != Key2Table::end())
        { 
            return it->second;
        }
        return NULL; 
    }

    bool Contain( KType id ) 
    {
        return GetElement( id) != NULL; 
    }

    void AddToPtrMap( Table* table)
    {
        MMO_ASSERT( !Contain(table->GetLogicalKey()));
        (*this)[ table->GetLogicalKey() ] = table;
    }

    Table* AddAndAddToCache( Table* table)
    {
        //MMO_ASSERT( !Contain(table->GetLogicalKey()));
		if(Contain(table->GetLogicalKey()))
		{
			NLOG("ERROR Table%s Get Logical Key Contained!! key=%llu",table->GetTableName(),table->GetLogicalKey());
			return NULL;
		}
		if(table == NULL) return NULL;
        Table* newPtr = sDataMgr.SaveAndAddTableEntry<Table>(table);
        if ( newPtr != NULL )
        {
            (*this)[ newPtr->GetLogicalKey() ] = newPtr;
        }
        else
        {
            ASSERT( false);
        }
        return newPtr;
    }

    bool DeleteEntry( KType id)
    {
        Iter it = GetItr( id);
        if (it != End())
        {
             sDataMgr.GetTable<Table>()->RemoveAndDeleteEntry( it->second );
             Key2Table::erase(it);
             return true;
        }
        return false;
    }

    bool DeleteEntry( Table* table)
    {
        Key2Table::erase(table->GetLogicalKey());
        return sDataMgr.GetTable<Table>()->RemoveAndDeleteEntry( table );
    }

	void JustRemoveFromContianer( Table* table )
	{
        Key2Table::erase(table->GetLogicalKey());
	}

	void JustRemoveAllFromContianer()
	{
        ModMarker<KType>::SetUnmodified();
        Key2Table::clear();
	}

    void RemoveAllEntry()
    {
		for(Iter it = Begin();it != End();++it)
		{
			sDataMgr.GetTable<Table>()->RemoveKeyEntry(it->second);
		}
        ModMarker<KType>::SetUnmodified();
        Key2Table::clear();
    }
	void RemoveSelfEntryFromCacheAndDb()
	{
		for(Iter it = Begin();it != End();++it)
		{
			sDataMgr.GetTable<Table>()->RemoveAndDeleteEntry(it->second);
		}
		ModMarker<KType>::SetUnmodified();
		Key2Table::clear();
	}


    void SaveMod()
    {
        for_each( ModMarker<KType>::ModIdBegin(),
            ModMarker<KType>::ModIdEnd(),
            boost::bind(&CachePtrMap::SaveElement, this, _1 ));

        ModMarker<KType>::SetUnmodified();
    }

	inline Iter Begin(){ return Key2Table::begin();}
    inline Iter End(){ return Key2Table::end();}

    inline RIter RBegin(){ return Key2Table::rbegin();}
    inline RIter REnd(){ return Key2Table::rend();}

	inline Iter GetItr( const KType &id ){   return Key2Table::find( id );   }

	inline Iter GetLowBoundItr( const KType &id ){   return Key2Table::lower_bound( id );   }
	inline Iter GetUpperBoundItr( const KType &id ){   return Key2Table::upper_bound( id );   }

	inline bool Empty( void ) {   return Key2Table::empty() ;   }

	inline size_t Size( void ) {   return Key2Table::size() ;   }
	inline size_t Size( void ) const {   return Key2Table::size() ;   }

	bool SwapValue( const KType &lId, const KType &rId )
	{
		ASSERT( lId != rId ) ;

		Iter lItr = GetItr( lId ) ;
		Iter rItr = GetItr( rId ) ;

		if( lItr == Key2Table::end() || rItr == Key2Table::end() )
		{
			return false ;
		}

		std::swap( lItr->second, rItr->second ) ;

		ASSERT( lId ==  lItr->second->GetLogicalKey() ) ;
		ASSERT( rId ==  rItr->second->GetLogicalKey() ) ;
		
		return true ;
	}


    void SaveOneMod(Table *pTable )
	{

#ifdef _DEBUG
		std::string str( pTable->GetTableName() ) ;
		DLOG( "SaveOneMod() table name is: %s", str.c_str() ) ;
#endif

        ASSERT( pTable !=NULL ) ; 
        if ( pTable->GetKey() == 0)
        {
            pTable->SetDbIncreaseId( sDataMgr.GetTable<Table>()->IncreaseMaxId());
        }
        sDataMgr.GetTable<Table>()->WriteEntry( pTable );
	}

private:
    void SaveElement( KType id) 
    {
        Table* table = GetElement( id);
        if ( table)
        {
            sDataMgr.GetTable<Table>()->WriteEntry( table);
        } 
    }

};

#endif //_CACHEPTRMAP_DEFINE_