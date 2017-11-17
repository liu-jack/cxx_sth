#pragma once

#include <sstream>

#include "FileTableCache.h"
#include "mysql/MySqlDatabase.h"
#include "mysql/MysqlRst.h"

#include "def/MmoAssert.h"

extern bool g_ExitWhenError;

typedef boost::mutex::scoped_lock CAutoLock ;


template<class T>
class CMysqlTableCache : public CFileTableCache< T >
{
private:
	static CMySqlConnection*	m_pLockConn;
protected:
	bool LoadFieldNames();

	//interface
public:
	static CMySqlDatabase* GetDatabase()
	{
		//MASSERT( m_pDB ,"Invaliad database");
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
			}else{
				return NULL;
			}
		}
		return m_pLockConn;
	}

	static bool FinishConn(CMySqlConnection*& pConn)
	{
		if ( pConn != m_pLockConn)
			GetDatabase()->PutConnection(pConn);
		else
			pConn = NULL;
		return true;
	}

public:
	virtual bool AddEntrySync(T* pEntry)
	{
		bool isOk = this->WriteEntry(pEntry);
		if ( !isOk )
			return false;
		if (pEntry)
			return this->AddEntry( pEntry);
		return false;
	}

	virtual bool RemoveEntrySync( int iIdx )
	{
		if ( !T::HasIndex() )
		{
			return false;
		}
		bool isOk = this->DeleteEntry( iIdx );
		if ( !isOk )
			return false;
		return this->RemoveEntry( iIdx);
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
			pRst = pConn->Query(szSource);
		}else{
			strTablename =T::GetTableName() ;
			pRst = pConn->QueryFormat("SELECT * FROM %s", T::GetTableName());
		}

		if ( !CheckRecordset (pRst) )
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
			typename std::map< int, T >::iterator iter = this->m_mapTable.begin();
			while ( iter != this->m_mapTable.end())
			{
				uint64 insertID = WriteEntryToDB( &(iter->second) );
				if ( insertID == 0)
				{
					MASSERT(false,"Add Record Faild!");
				}
				iter++;
			}
		}else
		{
			int nCount = this->m_table.size();
			for ( int iEntry = 0 ; iEntry < nCount; iEntry++)
			{
				uint64 insertID = WriteEntryToDB( &this->m_table[iEntry] );
				if ( insertID == 0)
				{
					MASSERT(false,"Add Record Faild!");
				}
			}
		}
		
		return true;
	}


	virtual bool ReadEntry(uint32 nID, T* pEntry) const
	{
		return ReadEntryFromDB(nID, pEntry);
	}

	virtual bool WriteEntry(T* pEntry)
	{
		WriteEntryToDB(pEntry);
		return true;
	}

	virtual bool DeleteEntry(uint32 iEntry)
	{
		DeleteEntryFromDB(iEntry);
		return true;
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
	//static functions
public:

	static uint64 GetMaxFieldValue( const char* pszFieldName )
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return false;

		CMysqlRstPtr pRst = pConn->QueryFormat("select max(%s) from %s ", pszFieldName, T::GetTableName());
		if ( pRst && pRst->GetRowCount() > 0 && pRst->GetFieldCount() > 0)
		{
			Field* pField = pRst->Fetch();
			uint64 MaxID = pField->GetUInt64();

			FinishConn(pConn);
			return MaxID;
		}

		FinishConn(pConn);
		return 0;
	}

	static uint32 GetMaxID()
	{
		return GetMaxFieldValue( T::GetKeyName()  );
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
		pRst = pConn->QueryFormat("SELECT * FROM %s where %s = %d ", T::GetTableName(), T::GetKeyName(), nID);
		if ( !CheckRecordset (pRst) )
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
		//pRst->Delete();
		FinishConn(pConn);
		return ret;
	}


	static bool ReadEntryFromDB(uint64 nID, T* pEntry)
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return false;

		CMysqlRstPtr pRst;
		pRst = pConn->QueryFormat("SELECT * FROM %s where %s = %ul ", T::GetTableName(), T::GetLogicalKeyName(), nID);
		if ( !CheckRecordset (pRst) )
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
		//pRst->Delete();
		FinishConn(pConn);
		return ret;
	}


	/*
		insert a entry to database,return is success
	*/
	static uint64 WriteEntryToDB(const T* pEntry)
	{
		uint64 insertId = 0;
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return insertId;

		int iCount ;
		std::string strSql = EntryToSql(pEntry);
		iCount = pConn->Execute(strSql.c_str());
		if ( iCount != 0)
		{
			insertId = pConn->GetInsertID();
		}
		else
		{
			ASSERT(0 && pConn->GetLastError());
		}
		FinishConn(pConn);
		/*if( pEntry->HasIndex() )
		{
			if( pEntry->GetKey() == 0 )
				pEntry->GetKey() = insertId;
		}*/
		return insertId;
	}


	/*
		Delete a entry from database, reutrn count of deleted items.
	*/
	static uint32 DeleteEntryFromDB(uint32 nID )
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return false;

		uint32 iCount ;
		MASSERT( nID >= 0 ,"Key should >= 0" );
		iCount = pConn->ExecuteFormat("delete from %s where %s = %d",T::GetTableName(),T::GetKeyName(),nID);
		FinishConn(pConn);
		return iCount ;
	}
	
//protected:

	static bool	CheckRecordset( const CMysqlRstPtr& pRst)
	{
		if ( !pRst)
		{
			const char* pszError = GetConnection()?GetConnection()->GetLastError():NULL;
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
		#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)
			if (g_ExitWhenError)
			{
				printf ("Table:'%s' column not match, in db is %d, in src is %d\n", T::GetTableName(), nCols, nFormatLen);
				::exit(-1);
			}
			else
		#endif
			{
				printf ("Table:'%s' column not match, in db is %d, in src is %d\n", T::GetTableName(), nCols, nFormatLen);
				ASSERT(false);
			}
			return true;
		}

		//==================================================
		uint32 nRealSize = sizeof(T); 
		uint32 nSizeFormat = CTableCache<T>::SizeByFormat() +T::ExtraSize(); 
		if ( nRealSize != nSizeFormat )
		{
		#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)
			if (g_ExitWhenError)
			{
				printf ("Table:'%s' column not match, real size is %d, in format is %d\n", T::GetTableName(), nRealSize, nSizeFormat);
				::exit(-1);
			}
			else
		#endif
			{
				MASSERT(false,"nRealSize define not match ncolumns, ignore to continue...");
			}
			return true;
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
			case 'k':
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
				//MASSERT(false,"No Skip In Develop Step");
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

	static std::string EntryToSql( const T* pEntry)
	{
		CMySqlConnection* pConn = GetConnection();
		if ( !pConn )
			return "";

		std::stringstream ss;
		ss<<"REPLACE INTO ";
		ss<< T::GetTableName();
		ss<<" value (";
		//string strSql = "insert into "+ T::GetTableName() + 
		const char * p = T::GetFormat();
		char * structpointer = (char*)pEntry;
		uint32 offset = 0;
		for(; *p != 0; ++p)
		{
            switch(*p)
			{
			case 'k':
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
				
				ss << "\"" << pConn->EscapeString( *(std::string*)&structpointer[offset] ).c_str() << "\"";
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
		FinishConn(pConn);
		return ss.str();
	}
};

template<class T> CMySqlConnection*	CMysqlTableCache<T>::m_pLockConn = NULL;
