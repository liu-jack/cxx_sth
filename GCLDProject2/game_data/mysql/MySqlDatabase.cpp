#include "MySqlDatabase.h"

#ifdef MYSQL_SUPPORT

#include <stdarg.h>

#include "Logger.h"

#define _LOG NLOG

#ifndef _LOG
#define _LOG printf
#endif


typedef boost::mutex Mutex ;
typedef boost::mutex::scoped_lock CAutoLock ;


CMySqlConnection::CMySqlConnection(void)
{
	m_connection = NULL;
}

CMySqlConnection::~CMySqlConnection(void)
{
	Close();
}

bool CMySqlConnection::Connect(const char* Hostname, unsigned int port,
							 const char* Username, const char* Password, const char* DatabaseName)
{
	my_bool my_true = true;

	_LOG("Connecting to `%s`, database `%s`...\n", Hostname, DatabaseName);

	MYSQL* mysqlConn;
	mysqlConn = mysql_init( NULL );
	if( mysqlConn == NULL )
		return false;

	if(mysql_options(mysqlConn, MYSQL_SET_CHARSET_NAME, "utf8"))
		_LOG("MySQLDatabase: Could not set utf8 character set.");

	if (mysql_options(mysqlConn, MYSQL_OPT_RECONNECT, &my_true))
		_LOG("MySQLDatabase: MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.");

	m_connection = mysql_real_connect( mysqlConn, Hostname, Username, Password, DatabaseName, port, NULL, 0 );
	if( m_connection == NULL )
	{
		_LOG("MySQLDatabase: Connection failed due to: `%s`", mysql_error( mysqlConn ) );
		mysql_close(mysqlConn);
		return false;
	}
	return true;
}

void CMySqlConnection::Close()
{
	if (m_connection)
		mysql_close(m_connection);
	m_connection = NULL;
}

bool CMySqlConnection::CheckValid()
{
	return m_connection && (mysql_ping(m_connection) == 0);
}

bool CMySqlConnection::SelectDatabase(const char* pszDatabaseName)
{
	int nResult = m_connection ? mysql_select_db(m_connection, pszDatabaseName) : -1;
	return (nResult == 0);
}

std::string CMySqlConnection::EscapeString(std::string Escape)
{
	char a2[16384] = { 0 };
	std::string ret;
	if(m_connection && mysql_real_escape_string(m_connection, a2, Escape.c_str(), (unsigned long)Escape.length()) == 0)
		ret = Escape.c_str();
	else
		ret = a2;
	return std::string(ret);
}

bool CMySqlConnection::_Query(const char* strSql)
{	
	if ( !m_connection )
		return false;
	int result = mysql_query( m_connection,strSql);
	if(result > 0)
	{
		const char* pError = GetLastError() ;
		_LOG("%s/n",pError?pError:"Unknown Error!");
	}
	return (result == 0 ? true : false);
}

CMysqlRstPtr CMySqlConnection::QueryFormat(const char* szQueryString, ...)
{
	char sql[16384];
	va_list vlist;
	va_start(vlist, szQueryString);
	vsnprintf(sql, 16384, szQueryString, vlist);
	va_end(vlist);

	return Query(sql);
}

int CMySqlConnection::ExecuteFormat(const char* szQueryString, ...)
{
	char sql[16384];
	va_list vlist;
	va_start(vlist, szQueryString);
	vsnprintf(sql, 16384, szQueryString, vlist);
	va_end(vlist);

	return Execute(sql);
}

int CMySqlConnection::Execute(const char* szQueryString)
{
	bool isSuc = _Query(szQueryString);
	if ( !isSuc )
	{
		return -1;
	}
	return (int)mysql_affected_rows(m_connection);
}

CMysqlRstPtr CMySqlConnection::Query(const char* szQueryString)
{
	bool isSuc = _Query( szQueryString);
	if(!isSuc)
	{
		return NULL;
	}

	MYSQL_RES * pRes = mysql_store_result( m_connection );
	uint32 uRows = (uint32)mysql_affected_rows( m_connection );
	uint32 uFields = (uint32)mysql_field_count( m_connection );

	CMysqlRstPtr pRst(new CMysqlRst( pRes, uFields, uRows ));
	pRst->NextRow();
	return pRst;
}

uint64 CMySqlConnection::GetInsertID()
{
	return mysql_insert_id(m_connection);
}

 void CMySqlConnection::TransactionBegin()
{
	_Query("START TRANSACTION");
}
 void CMySqlConnection::Commit() 
{
	_Query("COMMIT");
}
 void CMySqlConnection::Rollback()
 {
	 _Query("ROLLBACK");
 }

 bool CMySqlConnection::Lock( const char* pTableName )
 {
	 char buff[1024];
	 sprintf(buff,"LOCK TABLES %s WRITE",pTableName);
	 _Query( buff );
	 return true;
 }

 bool CMySqlConnection::Unlock( )
 {
	 _Query( "UNLOCK TABLES " );
	 return true;
 }




const char* CMySqlConnection::GetLastError()
{
	if ( !m_connection )
		return "Invalid Connection";
	if(mysql_errno(m_connection))
	{
		return mysql_error(m_connection);
	}else{
		return NULL;
	}
}
CMySqlDatabase::CMySqlDatabase()
{
	_mutex = new Mutex;
}
CMySqlDatabase::~CMySqlDatabase()
{
	delete _mutex;
	_mutex = NULL;

	int nCount = _connections.size();
	for ( int iConn = 0; iConn < nCount; iConn ++)
	{
		delete _connections.at(iConn);
	}
	_connections.clear();
}


bool CMySqlDatabase::_NewConnection()
{
	CMySqlConnection * pConnection = new CMySqlConnection;
	if ( !pConnection->Connect(_host.c_str(),_port,_user.c_str(),_password.c_str(),_database.c_str()) )
	{
		delete pConnection;
		return false;
	}
	_connections.push_back(pConnection);
	_is_free.push_back(true);
	return true;
}

bool CMySqlDatabase::Connect(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName, uint32 connCount,uint32 connCountMax)
{
	_host = Hostname;
	_port = port;
	_database = DatabaseName;
	_user = Username;
	_password = Password;
	_default_connection_count = connCount;
	_max_connection_count = connCountMax;

	{
		CAutoLock lock(*_mutex);
		for (int i = 0; i < _default_connection_count; i ++)
		{
			if ( !_NewConnection() )
			{
				return false;
			}
		}
		return true;
	}
}


CMySqlConnection* CMySqlDatabase::GetConnection()
{
	CAutoLock lock(*_mutex);

	CMySqlConnection *pConn = NULL;
	int iConn = 0;
	int count = _connections.size();
	for (iConn = 0; iConn < count; iConn++) 
	{
		if (_is_free[iConn])
		{
			_is_free[iConn] = false;
			pConn = _connections.at(iConn);
			break;
		}
	}

	if (iConn == count)
	{
		bool isOk = _NewConnection();
		if ( isOk )
		{
			_is_free[iConn] = false;
			pConn = _connections.at(iConn);
		}
	}
	//assert(pConn);
	if ( pConn && !pConn->CheckValid() )
	{
		pConn->Close();
		bool isOk = pConn->Connect(_host.c_str(),_port,_user.c_str(),_password.c_str(),_database.c_str());
		if ( !isOk )
		{
			_LOG("Mysql connection reconnect faild!\n");
			PutConnection(pConn);
		}
	}
	return pConn;
}

bool CMySqlDatabase::PutConnection( CMySqlConnection*& pConn)
{
	CAutoLock lock(*_mutex);

	int count = _connections.size();
	for (int i = 0; i < count; i ++) 
	{
		if (_connections[i] == pConn)
		{
			assert(!_is_free[i]);
			_is_free[i] = true;
			pConn = NULL;
			return true;
		}
	}
	return false;
}

const char* CMySqlDatabase::GetHostName()
{
	return _host.c_str();
}
const char* CMySqlDatabase::GetDatabaseName()
{
	return _database.c_str();
}

#endif
