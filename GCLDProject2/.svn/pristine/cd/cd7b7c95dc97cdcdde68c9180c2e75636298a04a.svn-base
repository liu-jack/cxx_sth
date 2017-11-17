
#ifndef __DATA_GAMEDATA_CONV_H__
#define __DATA_GAMEDATA_CONV_H__


#ifdef FORCE_MYSQL_TABLE_CACHE
#include "mysqldata.h"
#endif


#include "FileTableCache.h"
#include "datastruct/struct.h"
#include "DatabaseMgr.h"


#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)
	#define LOADTABLE(x) m_mutex.lock(); (x)->LoadData(); m_mutex.unlock();
#else
	#define LOADTABLE(x) 
#endif

#define RELOADTABLE(x) m_mutex.lock();(x)->LoadData(NULL,FlagReload); m_mutex.unlock();


#ifdef MYSQL_SUPPORT
#include "mysql/MySqlDatabase.h"
#include "MysqlTableCache.h"
#endif

#endif // __DATA_GAMEDATA_H__
