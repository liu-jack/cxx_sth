#pragma once

#include "DatabaseMgr.h"
#include "gamedata.h"

struct DbTool
{
	template<typename T>
	static const T* Get(int entry)
	{
		CTableCache<T>* pTable = sDBMgr.GetTable<T>();
		if (pTable == NULL)
		{
			return NULL;
		}
		return pTable->GetEntry(entry);
	}

	template<typename T>
	static T* GetRandom( void )
	{
		CTableCache<T>* pTable = sDBMgr.GetTable<T>();
		if (pTable == NULL)
		{
			return false;
		}

		int index = 1 + rand() % pTable->GetNumRows() ;
		 T *pRet =pTable->GetEntryBySN( index ) ;

		if( pRet == NULL && pTable->GetNumRows() > 0 )
		{
			pRet =pTable->GetEntryBySN( 0 ) ;
		}

		return pRet ;
	}

};

#define FOREACH_DB_ITEM(varName, dbStructName) \
	CTableCache<dbStructName>* pTable_##dbStructName = sDBMgr.GetTable<dbStructName>();\
	CTableCache<dbStructName>::iterator it_##dbStructName= pTable_##dbStructName->Begin();\
	const dbStructName* varName = NULL; \
	for (; \
		it_##dbStructName != pTable_##dbStructName->End() && (varName = &it_##dbStructName->second) != NULL; \
		it_##dbStructName ++ \
	)

