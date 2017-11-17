#ifdef MYSQL_SUPPORT 
#include "mysqldata.h"
#else
#include "gamedata.h"
#endif

#include "DbStringMgr.h"
#include "Cast.h"

template<class T>
MULTILANG CDbStringMgr<T>::m_curLang = LANG_EN;



//Get Part String by Specific the Language
template<class T>
bool CDbStringMgr<T>::GetString(uint32 strId, wchar_t * strArray, int buffLen, MULTILANG speLang)
{
	ASSERT(buffLen > 0);
	CTableCache<T> * String_Table = (CTableCache<T> *)(sDBMgr.GetTable<T>());//->LoadData();
	const T* templeteType = String_Table->GetEntry(strId);
	if (templeteType)
	{
#ifndef LOAD_ONE_LANGUAGE 
		if (templeteType->str[speLang].empty())
		{
			speLang = LANG_CN;
		}

		utf82unicode(templeteType->str[speLang].c_str(), strArray, buffLen);
#else
		utf82unicode(templeteType->str.c_str(), strArray, buffLen);

#endif

		return true;
	}
	strArray[0] = 0;
	return false;
}


template<class T>
bool CDbStringMgr<T>::GetUtf8String(uint32 strId, char * buffer, MULTILANG speLang)
{
	CTableCache<T> * String_Table = (CTableCache<T> *)(sDBMgr.GetTable<T>());//->LoadData();
	T templeteType;
	if(String_Table->GetEntry(strId, &templeteType))
	{
#ifndef LOAD_ONE_LANGUAGE 
		if (templeteType.str[speLang].empty())
		{
			speLang = LANG_EN;
		}

		strcpy(buffer, templeteType.str[speLang].c_str());
#else
		strcpy(buffer, templeteType.str.c_str());

#endif
		return true;

	}
	buffer[0]= 0;
	return false;
}

#ifdef MYSQL_SUPPORT
template<class T>
uint32	CDbStringMgr<T>::m_nextStrID = 0; //Not Define


CStringRow::CStringRow(void)
{
	ResetStrings();
}

CStringRow::~CStringRow(void)
{
}

void CStringRow::ResetStrings()
{
	for(int i=0; i< LANG_MAX; i++)
	{
		m_strArray[i][0] = 0;
	}
}
 

//Part Update Save 
template<class T>
bool CDbStringMgr<T>::SaveString(uint32 strId, const wchar_t * strArray, MULTILANG speLang)
{
	T strItem;
	
	CMysqlTableCache<T>::ReadEntryFromDB(strId, &strItem);
	strItem.Id = strId;

	char charArray[CStringRow::MAX_STRING_LENGTH * 3];
	unicode2utf8(strArray, charArray, sizeof(charArray));
	
	strItem.str[speLang] = charArray;

	CTableCache<T> * String_Table = (CTableCache<T> *)(sDBMgr.GetTable<T>());
	return String_Table->AddEntrySync(&strItem);
}

//Part New Save
template<class T>
uint32 CDbStringMgr<T>::NewString(const wchar_t * strArray, MULTILANG speLang)
{
	sDBMgr.GetTable< T >()->BeginWrite();
	m_nextStrID = CMysqlTableCache<T>::GetMaxID() + 1;
	
	T strItem;
	
	strItem.Id = m_nextStrID;

	char charArray[CStringRow::MAX_STRING_LENGTH * 3];
	unicode2utf8(strArray, charArray, sizeof(charArray));
	
	strItem.str[speLang] = charArray;

	CTableCache<T> * String_Table = (CTableCache<T> *)(sDBMgr.GetTable<T>());
	if(String_Table->AddEntrySync(&strItem))
	{
		sDBMgr.GetTable< T >()->EndWrite();
		return strItem.Id;
	}
	sDBMgr.GetTable< T >()->EndWrite();
	return 0;

}

//Part Delete
template<class T>
uint32 CDbStringMgr<T>::DeleteString(uint32 strID)
{
	int result = CMysqlTableCache<T>::DeleteEntryFromDB(strID);
	m_nextStrID = CMysqlTableCache<T>::GetMaxID() + 1;
	return m_nextStrID;
}

template<class T>
bool CDbStringMgr<T>::GetStringRow(uint32 strId, CStringRow & strRow)
{
	T strItem;
	
	if(CMysqlTableCache<T>::ReadEntryFromDB(strId, &strItem))
	{
		for(int i=0; i< LANG_MAX; i++)
		{
			utf82unicode(strItem.str[i].c_str(), strRow.m_strArray[i], CStringRow::MAX_STRING_LENGTH);
		}
		return true;
	}
	return false;
}

template<class T>
bool CDbStringMgr<T>::SaveStringRow(uint32 strId, const CStringRow & strRow)
{
	T strItem;
	strItem.Id = strId;

	char charArray[CStringRow::MAX_STRING_LENGTH * 3];
	for(int i=0; i< LANG_MAX; i++)
	{
		unicode2utf8(strRow.m_strArray[i], charArray, sizeof(charArray));
		strItem.str[i] = charArray;
	};

	CTableCache<T> * String_Table = (CTableCache<T> *)(sDBMgr.GetTable<T>());
	return String_Table->AddEntrySync(&strItem);
}

template<class T>
uint32 CDbStringMgr<T>::NewStringRow(const CStringRow & strRow)
{
	if(m_nextStrID == 0)
		m_nextStrID = CMysqlTableCache<T>::GetMaxID() + 1;
	if(SaveStringRow(m_nextStrID, strRow))
		return m_nextStrID++;
	return 0;
}
/*
template<class T>
const char* CDbStringMgr<T>::GetUtf8String(int strId, MULTILANG speLang)
{
	T strItem;

	if(CMysqlTableCache<T>::ReadEntryFromDB(strId, &strItem))
	{
		//utf82unicode(strItem.str[speLang].c_str(), strArray);
		return strItem.str[speLang].c_str();
	}
	return "";
}*/

#else


//template<class T>
//const char * CDbStringMgr<T>::GetUtf8String(int strId, MULTILANG speLang)
//{
//	CTableCache<T> * String_Table = (CTableCache<T> *)(sDBMgr.GetTable<T>());//->LoadData();
//	const T* templeteType = String_Table->GetEntry(strId);
//	if (templeteType)
//	{
//		if (templeteType->str[speLang].empty())
//		{
//			speLang = LANG_EN;
//		}
//
//		return templeteType->str[speLang].c_str();
//
//	}
//	return "";
//}


#endif

/* ------------------------------------------------------------------------
        Explicit definition for all classes
   ------------------------------------------------------------------------ */

//template class CDbStringMgr<String_Creature>;
//template class CDbStringMgr<String_Npc_SubName>;
//template class CDbStringMgr<String_Npc>;
//template class CDbStringMgr<String_Item>;
//template class CDbStringMgr<String_Quest>;
//template class CDbStringMgr<String_Quest_NPC_Emote>;
////template class CDbStringMgr<String_Spell>;
////template class CDbStringMgr<String_Talent>;
//template class CDbStringMgr<String_Skill>;
////template class CDbStringMgr<String_AI>;
//template class CDbStringMgr<String_StaticObject>;
template class CDbStringMgr<String_Ui>;
template class CDbStringMgr<String_Mail>;
//template class CDbStringMgr<String_Map>;

//template class CDbStringMgr<String_Achievement>;
