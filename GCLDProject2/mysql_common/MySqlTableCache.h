#pragma once

#include <sstream>
#include <boost/numeric/conversion/cast.hpp>

#include "FileTableCache.h"
#include "mysql/MySqlDatabase.h"
#include "mysql/MySqlRst.h"

#include "def/MmoAssert.h"
#include "utility/Utility.h"
#include "MySqlSingleton.h"
#include "structDef.h"

typedef boost::mutex::scoped_lock CAutoLock ;


template<class T>
class CMysqlTableCache : public FileTableCache< T >
{
private:
	static CMySqlConnection*	m_pLockConn;

protected:
	bool LoadFieldNames();

public:
	typedef typename T::IdType           IdType ;
	typedef typename CTableCache< T >::K K ;

	//interface
public:
	static CMySqlDatabase* GetDatabase()
	{
		MASSERT( CTableCache<T>::m_pDB ,"Invaliad database");
		return CTableCache<T>::m_pDB;
	}

	//Use GetConnection/FinishConn pairs
	static CMySqlConnection* GetConnection()
	{
		if ( !m_pLockConn )
		{
			if (GetDatabase())
			{
				return GetDatabase()->GetConnection();
			}
			else
			{
				return NULL;
			}
		}
		return m_pLockConn;
	}

	static bool FinishConn(CMySqlConnection*& pConn)
	{
		if ( pConn != m_pLockConn)
		{
			GetDatabase()->PutConnection(pConn);
		}
		else
		{
			pConn = NULL;
		}

		return true;
	}

public:

	//load from recordset
	/*
	 * 加载对应的数据
	 *
	 * @param : szSource : 指定sql语句，如果不指定，默认加载这个表格对应的所有数据。
	 * @param : pAddVec  : 指定后可以获取所有读取的数据。
	 *
	 * @return : 是否成功
	 */
	virtual bool SqlLoadAndAddData( const char* szSource = NULL, std::vector<T*> *pAddVec =NULL, LOAD_FLAG flagLoad = FlagNone,MULTILANG curLanguage = LANG_EN)
	{
		bool bResult =false ;

		CAutoLock lock((this->m_mutex));

		if ( this->GetNumRows() > 0 && !(flagLoad&FlagReload)  )
		{
			return bResult;
		}

		if (flagLoad&FlagCleanup)
		{
			this->Cleanup();
		}

		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
		{
			ASSERT( false ) ;
			return bResult;
		}

		string strTablename ;// Tips: 暂时添加报错的时候方便定位错误 By CaiBingjie 2014-12-9
		CMysqlRstPtr pRst;
		if ( szSource != NULL)
		{
			pRst = pConn->Query( szSource, strlen( szSource ) );
		}
		else
		{
			strTablename =T::GetTableName() ;
			pRst = pConn->QueryFormat("SELECT * FROM %s", T::GetTableName());
		}

		if ( !CheckRecordset (pRst, pConn) )
		{
			FinishConn(pConn);
			return bResult;
		}

		this->m_nCols = pRst->GetFieldCount();

		//load field names
		//this->m_strColNames.clear();
		//for (uint32 iCol = 0; iCol < this->m_nCols; iCol++)
		//{
		//	this->m_strColNames.push_back( pRst->GetFieldName(iCol) );
		//}

		T object;

		if ( pRst->GetRowCount() > 0)
		{
			if( pAddVec != NULL )
			{
				pAddVec->reserve( pRst->GetRowCount() ) ;
			}

			bResult =true ;

			Field* pField;
			do 
			{
				pField = pRst->Fetch();

				// TODO: By CaiBingjie 优化提示：
				//       这里可以使用在 CTableCache< T >内部加一个map< dbUniqueId, T > memMap 数组
				//       然后使用 map< Key, T* >, 所谓平时使用，那么这里就不用每读一个都复制一个了，
				//       T object 可以用  memMap 返回。然后将指针放给 CTableCache< T >::AddEntry() 去添加。
				//T object;

				ReadFieldEntry(pField,&object,this->m_nCols );
				T *pAddObj =CTableCache< T >::AddEntry(&object);
				if( pAddVec != NULL )
				{
					pAddVec->push_back( pAddObj ) ;
				}

				this->m_nRows ++;

				//if (this->m_nRows > MAX_ITEM_LIMIT )
				//{
				//	break;
				//}

			}while (pRst->NextRow());
		}

		//pRst->Delete();
		FinishConn(pConn);

		printf("=====> LoadAndAddData mysqlTableCache = [%s]\n", T::GetTableName());

		CMysqlTableCache< T >::PrepareData(0);

		return bResult;
	}

	//load from recordset
	virtual bool LoadData(const char* szSource = NULL,LOAD_FLAG flagLoad = FlagNone,MULTILANG curLanguage = LANG_EN)
	{
		CAutoLock lock((this->m_mutex));

		if ( this->GetNumRows() > 0 && !(flagLoad&FlagReload)  )
			return true;

		if (flagLoad&FlagCleanup)
			this->Cleanup();
		
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
		{
			ASSERT( false ) ;
			return false;
		}

		string strTablename ;// Tips: 暂时添加报错的时候方便定位错误 By CaiBingjie 2014-12-9
		CMysqlRstPtr pRst;
		if ( szSource != NULL)
		{
			pRst = pConn->Query(szSource,strlen( szSource ));
		}else{
			strTablename =T::GetTableName() ;
			pRst = pConn->QueryFormat("SELECT * FROM %s", T::GetTableName());
		}

		if ( !CheckRecordset (pRst, pConn) )
		{
			ASSERT( false ) ;
			FinishConn(pConn);
			return false;
		}

		this->m_nCols = pRst->GetFieldCount();

		//load field names
		//this->m_strColNames.clear();
		//for (uint32 iCol = 0; iCol < this->m_nCols; iCol++)
		//{
		//	this->m_strColNames.push_back( pRst->GetFieldName(iCol) );
		//}
		if ( pRst->GetRowCount() > 0)
		{
			Field* pField;
			do 
			{
				pField = pRst->Fetch();
				T object;
				ReadFieldEntry(pField,&object,this->m_nCols );
				CTableCache< T >::AddEntry(&object);
				this->m_nRows ++;
				//if (this->m_nRows > MAX_ITEM_LIMIT )
				//{
				//	break;
				//}
			}while (pRst->NextRow());
		}
		
		//pRst->Delete();
		FinishConn(pConn);
		printf("=====> LoadData mysqlTableCache = [%s]\n", T::GetTableName());

		CMysqlTableCache< T >::PrepareData(0);

		return true;
	}
	

	virtual bool DumpToMysql(const char* szDest = NULL )
	{
		if ( T::HasIndex() )
		{
			typename std::map< K, T >::iterator iter = this->m_mapTable.begin();
			while ( iter != this->m_mapTable.end())
			{
				uint64 insertID = WriteEntryToDB( &(iter->second), true );
				if ( insertID == 0)
				{
					MASSERT(false,"Add Record Faild!");
				}
				iter++;
			}
		}
		else
		{
			int nCount = this->m_vecTable.size();
			for ( int iEntry = 0 ; iEntry < nCount; iEntry++)
			{
				uint64 insertID = WriteEntryToDB( &this->m_vecTable[iEntry], true );
				if ( insertID == 0)
				{
					MASSERT(false,"Add Record Faild!");
				}
			}
		}

		return true;
	}

	/*
	 * 添加入内存 且 保存进数据库的操作, 用在添加一条记录，且将记录加载进内存( 如：新建账号 )
	 *
	 * @param : pEntry : 表格一行数据的对应结构体指针
	 * @param : isSync : 是否同步操作，默认 false 为非同步操作
	 *
	 * @return : 加入后，数据保存的地址指针
	 */
	virtual T* SaveAndAddEntry(T* pEntry, bool isSync =false )
	{
		if( isSync )
		{
			uint64 insertId =WriteEntryToDB(pEntry, isSync) ;
			ASSERT( pEntry->id == insertId ) ;
			if( pEntry->id != insertId )
			{
				return NULL ;
			}

			return this->AddEntry( pEntry ) ;
		}
		else
		{
			T *pRet =this->AddEntry( pEntry ) ;
			if( pRet == NULL )
			{
				return pRet ;
			}
			
			AsyncWriteEntryToDB( pEntry ) ;

			return pRet ;
		}
	}

	/*
	 * 仅将数据保存进数据库的操作( 例：只新建角色，但是角色不进入游戏，所以不需要加载 )
	 *
	 * @param : pEntry : 表格一行数据的对应结构体指针
	 * @param : isSync : 是否同步操作，默认 false 为非同步操作
	 *
	 * @return : 加入后，数据保存的地址指针
	 */
	virtual uint64 WriteEntry(T* pEntry, bool isSync =false )
	{
		if( pEntry == NULL || pEntry->id == 0 )
		{
			ASSERT( false ) ;
			return 0 ;
		}

		return WriteEntryToDB(pEntry, isSync);
	}


	// 同步
	virtual bool DeleteEntrySync( T* pEntry )
	{
		return DeleteEntryFromDB( pEntry->id, true ) > 0 ;
	}

	// 异步
	virtual void DeleteEntryAsync( T* pEntry )
	{
		DeleteEntryFromDB( pEntry->id, false ) ;
	}



	/*
	 * 从内存中移除数据 且 同时 删除数据库里面的数据
	 *
	 * @param : pEntry : 表格一行数据的对应结构体指针
	 * @param : isSync : 是否同步操作，默认 false 为非同步操作
	 *
	 * @return : 是否成功
	 */
	virtual bool RemoveAndDeleteEntry( T* pEntry, bool isSync =false )
	{
		if( !T::HasIndex() || pEntry == NULL )
		{
			return false;
		}

		bool isOk = DeleteEntryFromDB( pEntry->id, isSync ) > 0 ;
		if( isSync && !isOk )
		{
			return false;
		}

		return this->RemoveKeyEntry( pEntry );
	}

	/*
	 * 从内存中移除所有数据 且 同时 清空表
	 *
	 * @param : isSync : 是否同步操作，默认 false 为非同步操作
	 *
	 * @return : 是否成功
	 */
	virtual bool RemoveAndDeleteAllEntry(bool isSync =false )
	{
		if( !T::HasIndex())
		{
			return false;
		}

		bool isOk = DeleteAllEntryFromDB(isSync ) > 0 ;
		if( isSync && !isOk )
		{
			return false;
		}

		 this->Cleanup();
		 return true;
	}

	/*
	 * 从内存中移除数据 且 同时保存一次数据( 游戏一般不用这个，因为在下线的时候放入缓存已经保存了一次，所以从内存移除已经不需要再保存 )
	 *
	 * @param : pEntry : 表格一行数据的对应结构体指针
	 * @param : isSync : 是否同步操作，默认 false 为非同步操作
	 *
	 * @return : 是否成功
	 */
	virtual bool RemoveAndSaveData( T *pEntry, bool isSync =false )
	{
		bool isOk = this->WriteEntry( pEntry, isSync ) > 0 ;
		if( isSync && !isOk )
		{
			return isOk ;
		}

		return this->RemoveEntryByKey( pEntry->GetKey() ) ;
	}


	//!!not thread safe, don't use in multithread.
	virtual bool BeginWrite()
	{
		m_pLockConn = GetConnection();
		if ( !m_pLockConn )
			return false;

		m_pLockConn->Lock( T::GetTableName() );
		return true;
	}

	//!!not thread safe, don't use in multithread.
	virtual bool EndWrite()
	{
		m_pLockConn->Unlock();
		GetDatabase()->PutConnection(m_pLockConn);
		return true;
	}


	/*virtual bool ReadEntry(typename T::IdType &id, T* pEntry) const
	{
		return ReadEntryFromDB(id, pEntry);
	}*/



	//static functions
public:
	static uint64 GetMaxFieldValue(uint32 index, const char* pszFieldName )
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return false;

		CombinId min_guid( index,0 ) ;
		CombinId max_guid( index,-1) ;

		uint64 MaxID = 0;
		CMysqlRstPtr pRst= pConn->QueryFormat("select max(`%s`) from `%s` WHERE %s > %llu AND %s < %llu", pszFieldName, T::GetTableName(),pszFieldName, static_cast<uint64>(min_guid), pszFieldName, static_cast<uint64>(max_guid));
		if ( pRst && pRst->GetRowCount() > 0 && pRst->GetFieldCount() > 0)
		{
			Field* pField = pRst->Fetch();
			MaxID = pField->GetUInt64();
			//pRst->Delete();
		}
		else
		{
			MaxID = min_guid;
		}

		if (MaxID < min_guid)
		{
			MaxID = min_guid;
		}
		//ASSERT( false );
		FinishConn(pConn);
		return MaxID;
	}

	static uint64 QueryMaxID(uint32 index)
	{
		return GetMaxFieldValue(index, T::GetDbIncreaseIdName() );
	}

	static IdType& GetMaxId( void )
	{
		static IdType s_maxId =0 ;
		return s_maxId ;
	}

	static void SetMaxId( const IdType &id )
	{
		GetMaxId() =id ;
	}

	static const IdType& IncreaseMaxId( void )
	{
		return ++GetMaxId() ;
	}

	static void InitMaxId(  uint32 dbproid)
	{
		GetMaxId() = boost::numeric_cast<IdType>( QueryMaxID(dbproid));
	}

	static bool QueryIsTure( const char* pszFieldName, ... )
	{
		char buffer[512];
		va_list vlist;
		va_start(vlist, pszFieldName);
		unsigned int retValut =vsnprintf(buffer, 512, pszFieldName, vlist);
		va_end(vlist);

		if( retValut > 512 )
		{
			ASSERT( false ) ;
			return false ;
		}

		bool isTrue =false ;

		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
		{
			ASSERT( false ) ;
			return isTrue;
		}

		CMysqlRstPtr pRst= pConn->Query( buffer, retValut ) ;
		if ( pRst && pRst->GetRowCount() > 0 && pRst->GetFieldCount() > 0)
		{
			isTrue =true ;
		}
		FinishConn(pConn);

		return isTrue;
	}


	/*
		read a entry from database,return is success
	*/
	static bool ReadEntryFromDB(uint32 nID, T* pEntry)
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return false;

		CMysqlRstPtr pRst;
		pRst = pConn->QueryFormat("SELECT * FROM %s where %s = %d ", T::GetTableName(), T::GetDbIncreaseIdName(), nID);
		if ( !CheckRecordset (pRst, pConn) )
		{
			FinishConn(pConn);
			return false;
		}

		bool ret = false;
		if ( pRst->GetRowCount() > 0)
		{
			Field* pField = pRst->Fetch();
			ReadFieldEntry(pField,pEntry, pRst->GetFieldCount() );
			ret = true;
		}

		FinishConn(pConn);
		return ret;
	}


	static bool ReadEntryFromDB(uint64 nID, T* pEntry)
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return false;

		CMysqlRstPtr pRst;
		pRst = pConn->QueryFormat("SELECT * FROM %s where %s = %llu ", T::GetTableName(), T::GetDbIncreaseIdName(), nID);
		if ( !CheckRecordset (pRst, pConn) )
		{
			FinishConn(pConn);
			return false;
		}

		bool ret = false;
		if ( pRst->GetRowCount() > 0)
		{
			Field* pField = pRst->Fetch();
			ReadFieldEntry(pField,pEntry, pRst->GetFieldCount() );
			ret = true;
		}

		FinishConn(pConn);
		return ret;
	}

	/*
	insert a entry to database,return is success
	*/
	static uint64 WriteEntryToDB(const T* pEntry, bool isSync )
	{
		uint64 insertId = 0;
		if( pEntry == NULL )
		{
			ASSERT( false ) ;
			return insertId ;
		}

		if( isSync )
		{
			CMySqlConnection* pConn = GetConnection();
			if ( !pConn )
			{
				ASSERT( false ) ;
				return insertId;
			}

			std::stringstream ss ;
			EntryToSql( ss, pEntry );

			int iCount = pConn->Execute( ss.rdbuf()->str() );
			if ( iCount != 0)
			{
				insertId = pConn->GetInsertID();
			}
			else
			{
				ASSERT(0 && pConn->GetLastError());
			}

			FinishConn(pConn);
		}
		else
		{
			std::stringstream ss ;
			EntryToSql( ss, pEntry );
			sDatabase.AddAsyncQuery( ss.rdbuf()->str() ) ;
		}

		return insertId;
	}

	static bool AsyncWriteEntryToDB( T* pEntry )
	{
		if( pEntry == NULL || pEntry->id == 0)
		{
			ASSERT( false ) ;
			return false ;
		}

		std::stringstream ss ;
		EntryToSql( ss, pEntry );
		sDatabase.AddAsyncQuery( ss.rdbuf()->str() ) ;

		return true ;
	}

	static uint32 DeleteEntryFromDB( uint32 id, bool isSync )
	{
		MASSERT( id >= 0 ,"Key should >= 0" );

		std::string sql ;
		Utility::FormatString( sql, "DELETE FROM %s WHERE %s = %d", T::GetTableName(), T::GetDbIncreaseIdName(), id ) ;

		uint32 value =0 ;
		if( isSync )
		{
			CMySqlConnection* pConn = GetConnection();
			if ( !pConn )
			{
				return 0;
			}

			value =pConn->Execute( sql.c_str(), sql.size() ) ;
			FinishConn(pConn);
		}
		else
		{
			sDatabase.AddAsyncQuery( sql ) ;
		}

		return value ;
	}

	/*
	Delete all entry from database, reutrn count of deleted items.
	*/
	static uint32 DeleteAllEntryFromDB( bool isSync )
	{
		std::string sql ;
        Utility::FormatString( sql, "TRUNCATE TABLE %s", T::GetTableName()) ;
		uint32 value =0 ;
		if( isSync )
		{
			CMySqlConnection* pConn = GetConnection();
			if ( !pConn )
			{
				return 0;
			}

			value =pConn->Execute( sql.c_str(), sql.size() ) ;
			FinishConn(pConn);
		}
		else
		{
			sDatabase.AddAsyncQuery( sql ) ;
		}

		return value ;
	}


	static bool SqlLoadTempData( const char* szSource = NULL, std::vector<T*> *pAddVec =NULL)
	{
		bool bResult =false ;

		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
		{
			ASSERT( false ) ;
			return bResult;
		}
		
		CMysqlRstPtr pRst;
		if ( szSource != NULL)
		{
			pRst = pConn->Query( szSource, strlen( szSource ) );
		}
		else
		{
			pRst = pConn->QueryFormat("SELECT * FROM %s", T::GetTableName());
		}

		if ( !CheckRecordset (pRst, pConn) )
		{
			FinishConn(pConn);
			return bResult;
		}
		uint32 m_nCols = pRst->GetFieldCount();
		T object;
		if ( pRst->GetRowCount() > 0)
		{
			if( pAddVec != NULL )
			{
				pAddVec->reserve( pRst->GetRowCount() ) ;
			}
			bResult =true ;
			Field* pField;
			do 
			{
				pField = pRst->Fetch();
				ReadFieldEntry(pField,&object,m_nCols );
				pAddVec->push_back( object ) ;

			}while (pRst->NextRow());
		}
		FinishConn(pConn);
		return bResult;
	}

	/*
	Delete a entry from database, reutrn count of deleted items.
	*/
	static uint32 DeleteEntryFromDB( uint64 id, bool isSync )
	{
		MASSERT( id >= 0 ,"Key should >= 0" );

		std::string sql ;
        Utility::FormatString( sql, "DELETE FROM %s WHERE %s = %llu", T::GetTableName(), T::GetDbIncreaseIdName(), id ) ;

		uint32 value =0 ;
		if( isSync )
		{
			CMySqlConnection* pConn = GetConnection();
			if ( !pConn )
			{
				return 0;
			}

			value =pConn->Execute( sql.c_str(), sql.size() ) ;
			FinishConn(pConn);
		}
		else
		{
			sDatabase.AddAsyncQuery( sql ) ;
		}

		return value ;
	}

	//protected:

	static bool CheckRecordset( const CMysqlRstPtr& pRst, CMySqlConnection *pConn )
	{
		if ( !pRst && pConn != NULL )
		{
			const char* pszError =pConn->GetLastError() ;
			if ( pszError )
			{
				MASSERT(false,"Mysql Error" );
			}

			return false;
		}

		uint32 nCols = pRst->GetFieldCount();
		uint32 nFormatLen = strlen( T::GetFormat() ) ; 
		if ( nCols != nFormatLen )
		{
			printf ("Table:'%s' column not match, in db is %d, in src is %d\n", T::GetTableName(), nCols, nFormatLen);
			ASSERT(false);
			return false;
		}

		//==================================================
		uint32 nRealSize = sizeof(T); 
		uint32 nSizeFormat = CTableCache<T>::SizeByFormat() +T::ExtraSize(); 
		if ( nRealSize != nSizeFormat )
		{
			MASSERT(false, "nRealSize define not match ncolumns, ignore to continue...");
			return false;
		}

		return true;
	}

	static void ReadFieldEntry(Field * fields, T * Allocated,uint32 nCols, bool reload = false )
	{
		const char * p = T::GetFormat();
		char * structpointer = (char*)Allocated;
		uint32 offset = 0;
		Field * f = fields;

		for(uint32 iCol = 0; p[iCol] != 0 && iCol < nCols; ++iCol, ++f)
		{
			char format = p[iCol];
			switch(format)
			{
			case 'u':	// Unsigned integer
				*(uint32*)&structpointer[offset] = f->GetUInt32();
				offset += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				*(int32*)&structpointer[offset] = f->GetInt32();
				offset += sizeof(int32);
				break;

			case 'l':	// Signed integer 64
				*(int64*)&structpointer[offset] = f->GetInt64();
				offset += sizeof(int64);
				break;

			case 'b':	// Unsigned integer 64
				*(uint64*)&structpointer[offset] = f->GetUInt64();
				offset += sizeof(uint64);
				break;

			case 's':	// std::string
				*(std::string*)&structpointer[offset] = f->GetString();
				offset += sizeof(std::string);
				break;

			case 'x':	// Skip
				//MASSERT(false, "No Skip In Develop Step");
				break;

			case 'f':	// Float
				*(float*)&structpointer[offset] = f->GetFloat();
				offset += sizeof(float);
				break;

			case 'c':	// Char
				*(uint8*)&structpointer[offset] = f->GetUInt8();
				offset += sizeof(uint8);
				break;

			case 'h':	// Short
				*(uint16*)&structpointer[offset] = f->GetUInt16();
				offset += sizeof(uint16);
				break;

			case 't':	// time
				*(int64*)&structpointer[offset] = f->GetTimeT();
				offset += sizeof(int64);
				break;

			default:	// unknown
				printf("%s:unknown field type in string: %d\n", T::GetTableName(), format);
				break;
			}
		}
	};

	static void EntryToSql( std::stringstream &ss, const T* pEntry)
	{
		//ss.str( "" ) ;

		ss<<"REPLACE INTO ";
		ss<< T::GetTableName();
		ss<<" value (";
		const char * p = T::GetFormat();
		char * structpointer = (char*)pEntry;
		uint32 offset = 0;
		for(; *p != 0; ++p)
		{
			switch(*p)
			{
			case 'u':	// Unsigned integer
				ss <<  *(uint32*)&structpointer[offset];
				offset += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				ss << *(int32*)&structpointer[offset];
				offset += sizeof(int32);
				break;

			case 'l':	// Signed integer 64
				ss << *(int64*)&structpointer[offset];
				offset += sizeof(int64);
				break;

			case 'b':	// Unsigned integer 64
				ss << *(uint64*)&structpointer[offset];
				offset += sizeof(uint64);
				break;

			case 's':	// Null-terminated string
				ss << "\"" << (*(std::string*)&structpointer[offset])/*.c_str()*/ << "\"";

				// Tips: 如果需要使用 EscapeString() 那么可有上一层调用将 pConn 传入。
				//ss << "\"" << pConn->EscapeString( *(std::string*)&structpointer[offset] ).c_str() << "\"";

				offset += sizeof(std::string);
				break;

			case 'x':	// Skip
				//MASSERT(false,"No Skip In Develop Step");
				break;

			case 'f':	// Float
				ss << *(float*)&structpointer[offset];
				offset += sizeof(float);
				break;

			case 'c':	// Char
				ss << (uint32)(*(uint8*)&structpointer[offset]);
				offset += sizeof(uint8);
				break;

			case 'h':	// Short
				ss << (uint32)(*(uint16*)&structpointer[offset]);
				offset += sizeof(uint16);
				break;

			case 't':	// time 
				{
					string strtime( "1970-01-01 08:00:00" );
					TimeT2String(*(int64*)&structpointer[offset], strtime);
					ss << "\"" << strtime.c_str() << "\"";
					offset += sizeof(int64);
				}
				break;

			default:	// unknown
				printf("%s:unknown field type in string: %d\n", T::GetTableName(), *p);
				break;
			}
			if ( *(p+1) != 0 )
			{
				ss << ",";
			}else{
				ss << ")";
			}
		}
	}
};

template<class T> CMySqlConnection*	CMysqlTableCache<T>::m_pLockConn = NULL;
