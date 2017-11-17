
#ifndef MYSQL_SINGLETION_H__
#define MYSQL_SINGLETION_H__

#include "Logger.h"
#include "OS.h"
#include "Thread.h"
#include "SimpleSingleton.h"
#include "lock_free/LockFreeQueue.h"

#include "structDef.h"
#include "def/MmoAssert.h"
#include "mysql/MySqlDatabase.h"

class MySqlSingletion : public SimpleSingleton<MySqlSingletion>, public CMySqlDatabase
{
private:
	friend class SimpleSingleton<MySqlSingletion> ;

private:
	MySqlSingletion( void )
	{
	}

	virtual ~MySqlSingletion( void )
	{
        DeleteAllPtrInLFQueue(_strBuff );
        DeleteAllPtrInLFQueue(_queryStr);

		_workThread.Stop() ;
		NLOG( "~MySqlSingletion( void )" ) ;
	}

public:

	void Start( void )
	{
		_workThread.Start( boost::bind( &MySqlSingletion::WorldThread, this ) ) ;
	}

	void WorldThread( void )
	{
		while( _workThread.IsStart() )
		{
			int64  count   = 0;
			uint64 begTime =sOS.TimeMS() ;

			std::string *pStr ;
			while( _queryStr.Dequeue( pStr ) )
			{
				++count ;
				CMySqlConnection *pConnect =GetConnection();
				if( pConnect != NULL )
				{
					int result =pConnect->JustQuery( pStr->c_str(), pStr->size() ) ;

					if (result != 0)
					{
						WLOG( "!!!!!!Sql: %s Size: %d JustQuery Error result not 0 is: %d ", pStr->c_str(), pStr->size(),result) ;
						PutConnection( pConnect );
						continue;
					}
					//ASSERT( result == 0 ) ; 
				}
				PutConnection( pConnect );
				_strBuff.Enqueue( pStr ) ;
			}

			uint64 endTime =sOS.TimeMS() ;

			int64 diffTime =endTime - begTime ;
			if( count > 0 && diffTime > count * 100 )
			{
				WLOG( "Sql thread taker %u query use %lld ms", count, diffTime ) ;
			}
			else
			{
				_workThread.Sleep( 1 ) ;
			}
		}

		NLOG( "MySql thread will stop, release all memory.......!" ) ;
	}

	void AddAsyncQuery( std::string &sql )
	{
		std::string *pStr =NULL ;
		if( !_strBuff.Dequeue( pStr ) )
		{
			pStr =new std::string() ;
		}

		pStr->swap( sql ) ;
		_queryStr.Enqueue( pStr ) ;
	}


	void AddAsyncQuery( const std::string &sql )
	{
		std::string *pStr =NULL ;
		if( !_strBuff.Dequeue( pStr ) )
		{
			pStr =new std::string() ;
		}

		pStr->assign( sql ) ;
		_queryStr.Enqueue( pStr ) ;
	}

	bool SyncExcute( std::string &sql )
	{
		bool isOk =false ;

		CMySqlConnection *pConnect =GetConnection();
		if( pConnect != NULL )
		{
			isOk =pConnect->JustQuery( sql.c_str(), sql.size() ) == 0 ;
		}

		PutConnection( pConnect );

		return isOk ;
	}

public:
	typedef ServerQueue< std::string* > QueryBufferTable ;

private:
	CThread           _workThread ;
	QueryBufferTable  _strBuff ;
	QueryBufferTable  _queryStr ;
} ;


#define sDatabase (MySqlSingletion::Instance())

#endif

