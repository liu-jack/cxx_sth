
#include "DatabaseMgr.h"

#ifdef MYSQL_SUPPORT 
	#include "mysqldata.h"
	#define TableStorage CMysqlTableCache
#else
	#include "gamedata.h"
	//#include "common/data_reader/PackPatchReader.h"
	#define TableStorage CFileTableCache
#endif

#define SET_CHAR_DB(x) TableStorage<x>::SetDB(GetCharacterDB())
#define SET_WORLD_DB(x) TableStorage<x>::SetDB(GetWorldDB())
#define SET_LOG_DB(x) TableStorage<x>::SetDB(GetLogDB())


//#if defined(_DEBUG) && defined(_MSC_VER) && defined(_MMO_SERVER_) && !defined(_TOOL)
//	#ifndef new
//		#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//	#endif
//#endif

//////////////////////////////////////////////////////////////////////////
bool g_ExitWhenError = false;



//////////////////////////////////////////////////////////////////////////
#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)
bool s_IsCacheFileTable = true;
#else
bool s_IsCacheFileTable = false;
#endif

#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) Table_##tab_name = new TableStorage<tab_name>();

DatabaseMgr::DatabaseMgr()
{
	LOAD_ALL_DB_TABLES 
	LOAD_ALL_DB_STRING_TABLES
	LOAD_TOOL_TABLES

	m_pWorldDB		= NULL;
}
	

#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) SET_WORLD_DB(tab_name);

bool	DatabaseMgr::SetupDBWorld()
{
	LOAD_ALL_DB_TABLES 
	LOAD_ALL_DB_STRING_TABLES
	LOAD_TOOL_TABLES
	return true;
}

bool	DatabaseMgr::SetupDBCharacter()
{
	return true;
}
bool DatabaseMgr::SetupDBLog()
{
	return true;
}

bool DatabaseMgr::InitWorldDB(const char* pszHost, uint32 nPort, const char* pszUser, const char* pszPassword, const char* pszWorldDB)
{
#ifdef MYSQL_SUPPORT
	printf ( "Connect database:%s:%d \n", pszHost, nPort ) ;
	m_pWorldDB = new CMySqlDatabase;
	bool isOk = m_pWorldDB->Connect(pszHost,nPort,pszUser,pszPassword,pszWorldDB);
	if ( !isOk )
	{
		printf("connect db fail,host:%s,user:%s,pwd:%s", pszHost, pszUser, pszPassword);
		return false;
	}
	SetupDBWorld();
#else
	printf ( "Use Loacal File Table ~! \n" ) ;
#endif
	return true;
}


#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) delete Table_##tab_name;

DatabaseMgr::~DatabaseMgr()
{
	LOAD_ALL_DB_TABLES 
	LOAD_ALL_DB_STRING_TABLES
	LOAD_TOOL_TABLES
#ifdef MYSQL_SUPPORT
	delete	m_pWorldDB;
#endif
}


bool	DatabaseMgr::LoadAllTables(LOAD_FLAG flagLoad )
{
	bool isOk = LoadServerTables( flagLoad );
	return isOk;
}

#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) \
	isOk &=Table_##tab_name->LoadData(NULL, flagLoad);\
	MASSERT(isOk,"Load table "#tab_name" error!");

bool DatabaseMgr::LoadServerTables(LOAD_FLAG flagLoad )
{
	bool isOk = true;

	LOAD_ALL_DB_TABLES;
	LOAD_ALL_DB_STRING_TABLES;

	MASSERT(isOk,"Load table error!");
	return isOk;
}


#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) \
	isOk &=Table_##tab_name->LoadData(NULL, flagLoad, curLanguage);\
	MASSERT(isOk,"Load table "#tab_name" error!");

bool DatabaseMgr::LoadClientStringTables(MULTILANG curLanguage, LOAD_FLAG flagLoad)
{
	bool isOk = true;

	LOAD_ALL_DB_STRING_TABLES;

	MASSERT(isOk, "Error: Load client string tables failed!");
	return isOk;
}



CMySqlDatabase*	DatabaseMgr::GetWorldDB()
{
	return m_pWorldDB;
}


#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) \
	template <>\
	CTableCache<tab_name>* DatabaseMgr::GetTable<tab_name>()\
	{\
		LOADTABLE(Table_##tab_name);\
		return Table_##tab_name;\
	}

LOAD_ALL_DB_TABLES 
LOAD_ALL_DB_STRING_TABLES
LOAD_TOOL_TABLES

#undef LOAD_DB_TAB
#define LOAD_DB_TAB(tab_name) \
	template <>\
	void DatabaseMgr::DeleteTable<tab_name>()\
	{\
		delete Table_##tab_name;\
	}

LOAD_ALL_DB_TABLES 
LOAD_ALL_DB_STRING_TABLES
LOAD_TOOL_TABLES

//////////////////////////////////////////////////////////////////////////
// PrepareData

//template<>
//bool CTableCache<Prefix>::PrepareData(int iCurLanguage)
//{
//	CTableCache<String_Item> * Table_String_Item = (CTableCache<String_Item> *)(sDBMgr.GetTable<String_Item>());//->LoadData();
//	Table_String_Item->LoadData();
//	////////////////////////////////////////////////////////////////////////////////////
//	//prepare item
//	CTableCache<Prefix>::iterator iter = Begin();
//	std::string strItemName;
//	String_Item item;
//	while(iter != End() )
//	{
//		bool bSuccess =  Table_String_Item->GetEntry( iter->second.NameStringID, &item );
//		if(bSuccess)
//		{
//			strItemName = item.str[iCurLanguage];
//		}
//		else
//		{
//			strItemName = "Unknown";
//		}
//		iter->second.Name = strItemName;
//		iter++;
//	}
//	//Table_String_Item->Cleanup();
//
//	return true;
//}
//
//template<>
//bool CTableCache<Item_Prefix>::PrepareData(int iCurLanguage)
//{
//	map<uint32, set<uint32> > tmpMap;   // map< item_id, set<prefix_id> >
//	int iIdx = 0;
//	const Item_Prefix* pItem_prefix;
//	while( (pItem_prefix = GetEntry(iIdx++)) != NULL )
//	{
//		tmpMap[pItem_prefix->item_id].insert(pItem_prefix->prefix_id);
//	}
//
//	// prepare prefixVec for each ItemPrototype
//	CTableCache<ItemPrototype>* pItemProTable = (CTableCache<ItemPrototype>*)(sDBMgr.GetTable<ItemPrototype>());
//	CTableCache<Prefix>* pTable_Prefix = (CTableCache<Prefix>*)(sDBMgr.GetTable<Prefix>());
//
//	CTableCache<ItemPrototype>::iterator iter = pItemProTable->Begin();
//	for( ; iter != pItemProTable->End(); ++iter )
//	{
//		map<uint32, set<uint32> >::iterator mapit = tmpMap.find(iter->first);
//		if( mapit != tmpMap.end() )
//		{
//			set<uint32>& tmpset = mapit->second;
//			for( set<uint32>::iterator setit = tmpset.begin(); setit != tmpset.end(); ++setit )
//			{
//				const Prefix* pPrefix = pTable_Prefix->GetEntry( *setit );
//				if( pPrefix != NULL )
//					iter->second.prefixVec.push_back( pPrefix );
//			}
//		}
//	}
//
//	//after setup ItemPrototype's prefixVec, needn't self table, remove it from memory
//	//Cleanup();
//
//	return true;
//}
//
//template<> 
//bool CTableCache<ItemPrototype>::PrepareData(int iCurLanguage)
////bool DatabaseMgr::_prepareItemPrototype(int iCurLanguage)
//{
//	CTableCache<String_Item> * Table_String_Item = (CTableCache<String_Item> *)(sDBMgr.GetTable<String_Item>());//->LoadData();
//	Table_String_Item->LoadData();
//	////////////////////////////////////////////////////////////////////////////////////
//	//prepare item
//	CTableCache<ItemPrototype>::iterator iter = Begin();
//	std::string strItemName ;
//	String_Item item;
//	while(iter != End() )
//	{
//		bool bSuccess = Table_String_Item->GetEntry( iter->second.NameStringID , &item);
//		if(bSuccess)
//		{
//#ifdef MYSQL_SUPPORT
//			strItemName = item.str[iCurLanguage];
//#else
//			strItemName = item.str;
//#endif
//		}
//		else
//		{
//			strItemName = "Unknown";
//		}
//		iter->second.strName = strItemName;
//		//iter->second.exchange = "1000*3+8*5+2*1+78*9+76542*9876+109*9999";
//		iter->second.ParseExchange();
//		iter++;
//	}
//	//Table_String_Item->Cleanup();
//
//	return true;
//}

//template<> 
//bool CTableCache<PlayerCreateInfo>::PrepareData(int iCurLanguage)
////bool DatabaseMgr::_preparePlayerCreateInfo(int iCurLanguage)
//{
//	PlayerCreateInfo_spells spell;
//	PlayerCreateInfo_skills skill;
//	char buff[128];
//	TableStorage< PlayerCreateInfo >::iterator iter = Begin();
//	for( ;iter != End();iter++ )
//	{
//		PlayerCreateInfo& info = iter->second;
//
//		TableStorage< PlayerCreateInfo_spells >	playerSpells;
//		//sprintf(sql, "select * from playercreateinfo_spells where `index` = %d",info.index);
//		playerSpells.LoadData();
//		//prepare spells
//		uint32 nSpellCount = playerSpells.GetNumRows();
//		for( uint32 iSpell = 0; iSpell < nSpellCount; iSpell ++ )
//		{
//			const PlayerCreateInfo_spells* pSpell = playerSpells.GetEntry( iSpell );
//			if( !pSpell || pSpell->index != info.index )
//			{
//				continue;
//			}
//			sprintf(buff,"%d;",pSpell->spell_id );
//			info.spells += (char*)&buff;
//		}
//
//		TableStorage< PlayerCreateInfo_skills >	playerSkill;
//		//sprintf(sql, "select * from playercreateinfo_skills where `index` = %d",info.index);
//		playerSkill.LoadData();
//		//prepare bars
//		uint32 nSkillCount = playerSkill.GetNumRows();
//		for( uint32 iSkill = 0; iSkill < nSkillCount; iSkill ++ )
//		{
//			const PlayerCreateInfo_skills* pSkill = playerSkill.GetEntry( iSkill );
//			if( !pSkill || pSkill->index != info.index )
//			{
//				continue;
//			}
//
//			sprintf(buff,"%d,%d,%d;",pSkill->skill_id,pSkill->level,pSkill->max_level);
//			info.skills += buff;
//		}
//
//
//
//		TableStorage< PlayerCreateInfo_items >	playerItems;
//		//char sql[2048];
//		//sprintf(sql, "select * from playercreateinfo_items where `index` = %d",info.index);
//		//playerItems.LoadData(sql);
//		playerItems.LoadData();
//		//prepare bars
//		uint32 nItemCount = playerItems.GetNumRows();
//		for( uint32 iItem = 0; iItem< nItemCount; iItem++ )
//		{
//			const PlayerCreateInfo_items* pItem  = playerItems.GetEntry( iItem);
//			if( !pItem || pItem->index != info.index )
//			{
//				continue;
//			}
//
//			info.items.push_back(*pItem);
//		}
//
//
//		TableStorage< PlayerCreateInfo_bars >	playerBars;
//		//sprintf(sql, "select * from playercreateinfo_bars where `index` = %d order by button asc",info.index);
//		playerBars.LoadData();
//		//prepare bars
//		std::map<uint32, const PlayerCreateInfo_bars* > mapBar;
//		uint32 nBarCount = playerBars.GetNumRows();
//		for( uint32 iBarData = 0; iBarData < nBarCount; iBarData ++ )
//		{
//			const PlayerCreateInfo_bars* pBar  = playerBars.GetEntry( iBarData );
//			if( !pBar || pBar->index != info.index )
//			{
//				continue;
//			}
//			mapBar[ pBar->btn ] = pBar;
//
//		}
//
//		std::map<uint32, const PlayerCreateInfo_bars* >::iterator iter = mapBar.begin();
//		uint32 iBar = 0;
//		for( ; iter != mapBar.end() ;  iBar++ )
//		{
//			const PlayerCreateInfo_bars* pBar = iter->second;
//			sprintf(buff,"%d,%d,%d;",0,0,0);
//			if ( pBar->btn == iBar )
//			{
//				sprintf(buff,"%d,%d,%d;",pBar->action,pBar->type,pBar->misc);
//				iter ++;
//			}
//			info.bars += buff;
//		}
//	}
//	return true;
//}

//template<> 
//bool CTableCache<Spell_Skillset>::PrepareData(int iCurLanguage)
////bool DatabaseMgr::_prepareSpellData(int iCurLanguage)
//{
//	CTableCache<String_Spell> * Table_String_Spell = (CTableCache<String_Spell> *)(sDBMgr.GetTable<String_Spell>());//->LoadData();
//	Table_String_Spell->LoadData();
//	//////////////////////////////////////////////////////////////////////////
//	TableStorage< Spell_Skillset >::iterator iter = Begin();
//	for( ;iter != End();iter++ )
//	{
//
//		const String_Spell* pString =  Table_String_Spell->GetEntry( iter->second.refStrNameID );
//		if( pString )
//		{
//			iter->second.strName = pString->str[iCurLanguage];
//		}
//	}
//	//	Table_String_Spell->Cleanup();
//	return true;
//}

