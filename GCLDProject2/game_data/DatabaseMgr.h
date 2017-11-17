#ifndef _DATABASE_MGR_
#define _DATABASE_MGR_


#include "TableCache.h"
#include "ActiveDbTables.h"

#include "SimpleSingleton.h"

class CMySqlDatabase;
class DatabaseMgr :public SimpleSingleton<DatabaseMgr>
{
protected:
	CMySqlDatabase*								m_pWorldDB;

protected:
	#ifdef LOAD_DB_TAB
		#undef LOAD_DB_TAB
	#endif
	#define LOAD_DB_TAB(tab_name) CTableCache <tab_name> * Table_##tab_name;

protected:
	LOAD_ALL_DB_TABLES 
	LOAD_ALL_DB_STRING_TABLES
	LOAD_TOOL_TABLES

private:
	boost::mutex m_mutex;
	
public:
	DatabaseMgr();
	~DatabaseMgr();
	CMySqlDatabase*	GetWorldDB();


	bool	InitWorldDB(const char* pszHost, uint32 nPort, const char* pszUser, const char* pszPassword, const char* pszWorldDB);
	bool	InitCharacterDB(const char* pszHost, uint32 nPort, const char* pszUser, const char* pszPassword, const char* pszCharacterDB);
	bool	InitLogDB(const char* pszHost, uint32 nPort, const char* pszUser, const char* pszPassword, const char* pszLogDB );

	bool	SetupDBWorld();
	bool	SetupDBCharacter();
	bool	SetupDBLog();
	template < typename T > CTableCache< T >* GetTable();

	//add for skillEditor;
	template<typename T> CTableCache<T>* ReloadTable();

	//just for server side
	bool	LoadAllTables(LOAD_FLAG flagLoad = FlagNone);
	bool	LoadServerTables(LOAD_FLAG flagLoad = FlagNone);
	bool	LoadClientStringTables(MULTILANG curLanguage, LOAD_FLAG flagLoad = FlagNone );
	void	CloseTableZipArc();
	template < typename T > void DeleteTable();
};

#define sDBMgr DatabaseMgr::Instance()

#ifdef MYSQL_SUPPORT 
#define GET_TABLE(table) dynamic_cast< CMysqlTableCache<table> *>(sDBMgr.GetTable<table>());
#endif

#endif