
#include <string>

#ifdef WIN32

#include <TCHAR.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif

#include "gamedata.h"
#include "ActiveDbTables.h"

#include "Logger.h"

extern bool g_ExitWhenError;

#undef LOAD_DB_TAB
#define LOAD_DB_TAB(structName) \
	CMysqlTableCache<structName> table##structName;\
	if (!table##structName.LoadData())\
	{\
		printf("Failed when load data from <%s>", #structName);\
		return false;\
	}\
	if (!table##structName.DumpToFile())\
	{\
		printf("Failed when dump file for <%s>", #structName);\
		return false;\
	}

bool ConvertServerData()
{
	LOAD_ALL_DB_TABLES;
	LOAD_ALL_DB_STRING_TABLES;

	return true;
}


bool ConvertData()
{
#ifdef WIN32
	SetCurrentDirectory( _T("..\\data") );
#else
	chdir( "./data" ) ;
#endif

	LOAD_ALL_DB_TABLES;
	LOAD_ALL_DB_STRING_TABLES;

	return true;
}

int main(int argc, char *argv[])
{
	sDBMgr;

	string host("192.168.6.141");
	string user("dbdump");
	string password("123456");
	string db("mb_db");
	switch( argc )
	{
	case 1:
		break;
	case 2:
		host = argv[1];
		break;
	case 4:
		host = argv[1];
		user = argv[2];
		password = argv[3];
		break;
	case 5:
		host = argv[1];
		user = argv[2];
		password = argv[3];
		db = argv[4];
		break;
	default:
		printf("不带参数 或者  game_data_conv.exe host user password db.\n");
		g_ExitWhenError = true;
		exit(-1);
		break;
	}

	if (!sDBMgr.InitWorldDB(host.c_str(),3306,user.c_str(),password.c_str(),db.c_str()))
	{
		printf("Failed when connect database:%s \n", host.c_str());
		return -1;
	}

	printf("converting server tables from:%s...\n",host.c_str());
	if (!ConvertServerData())
	{
		return -1;
	}
	printf("Done!\n");

	return 0;
}

