/*
Author: szz
Desc:	This is a sample sync mysql access class,
		sample and clear,just for editor.

*/

#pragma once

#ifdef MYSQL_SUPPORT

#include <boost/thread/mutex.hpp>

#include "MysqlRst.h"

#include "def/TypeDef.h"

class CMySqlConnection
{
	MYSQL* m_connection;
public:
	CMySqlConnection(void);
	virtual ~CMySqlConnection(void);

	virtual bool Connect(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName);
	virtual bool SelectDatabase(const char* pszDatabaseName);
	virtual void Close() ;
	virtual bool CheckValid();

	virtual std::string EscapeString(std::string Escape);

	virtual CMysqlRstPtr QueryFormat(const char* QueryString, ...);
	virtual int ExecuteFormat(const char* QueryString, ...);

	virtual CMysqlRstPtr Query(const char* QueryString);
	virtual int Execute(const char* QueryString);
	virtual uint64 GetInsertID();

	virtual void TransactionBegin();
	virtual void Commit() ;
	virtual void Rollback();

	virtual bool Lock( const char* pTableName );
	virtual bool Unlock( );

	const char* GetLastError();
	const std::string GetHostName();
	const std::string GetDatabaseName();
protected:
	bool	_Query(const char* strSql);
	CMysqlRstPtr _FetchRecordset();
};


/*
Update!!!!!
CMySqlDatabase not a singlton now.
call DatabaseMgr::init to init database, 
example:
	DatabaseMgr::newInstance();
	sDBMgr.Init("beiwks3717",3306,"root","123","mmo_db","mmo_char");
*/
class CMySqlDatabase
{
private:
	boost::mutex *_mutex;
	short		  _port;
	std::string   _host;
	std::string   _database;
	std::string   _user;
	std::string   _password;

	int _default_connection_count;
	int _max_connection_count;

	std::vector< CMySqlConnection* > _connections;
	std::vector< bool >				 _is_free;

	bool _NewConnection();
public:
	CMySqlDatabase();
	virtual ~CMySqlDatabase();

	const char* GetHostName();
	const char* GetDatabaseName();

	bool Connect(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName, uint32 connCount = 1,uint32 connCountMax = 5);

	CMySqlConnection* GetConnection();
	bool			  PutConnection( CMySqlConnection*& pConn);

};

#endif
//#define sMysql (&g_con)
