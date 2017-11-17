/*
A cache of entry table.
*/
#ifndef __DATA_TABLECACHE_H__
#define __DATA_TABLECACHE_H__

#include <map>
#include <vector>

#include "datastruct/struct.h"

#include "def/TypeDef.h"
#include "def/MmoAssert.h"

#include "lock_free/Mutex.h"


//#define MAX_ITEM_LIMIT 1000
enum LOAD_FLAG
{
	FlagNone			= 0x0000,
	FlagReload			= 0x0001,
	FlagCleanup			= 0x0002,
	FlagCleanupReload	= FlagCleanup|FlagReload,
};

class CMySqlDatabase;
template<class T>
class CTableCache
{   
protected:
	static CMySqlDatabase*		m_pDB;
public:
	static void SetDB(CMySqlDatabase* pDatabase)
	{
		m_pDB = pDatabase;
	}

protected:
	//std::vector< std:: string > m_strColNames;
	uint32 m_nRows;
	uint32 m_nCols;
	std::vector< T >	m_table;
	std::map< int, T >	m_mapTable;

protected:
	Mutex m_mutex;

public:
	typedef typename std::map< int, T >::iterator iterator;
	typedef typename std::map< int, T >::const_iterator const_iterator;
	iterator Begin() { return m_mapTable.begin();}
	iterator End() { return m_mapTable.end();}

	iterator Find(int iKey)
	{
		if ( T::HasIndex() )
		{
			iterator iter = m_mapTable.find( iKey );
			return iter;
		}else{
			return m_mapTable.end();	
		}
	}

	CTableCache()
	{
		m_nCols = strlen(T::GetFormat());
		m_nRows = 0;
	}

	virtual ~CTableCache()
	{
		Cleanup();
	}

	void Cleanup()
	{
		m_nCols = strlen(T::GetFormat());
		m_nRows = 0;
		m_table.clear();
		m_mapTable.clear();
	}
	//load from file
	//virtual bool Load(const char * szSource = NULL) = 0;
	virtual uint32 GetNumRows() const
	{
		return m_nRows;
	}
	//just for table with key
	bool FindEntryByKey(int iKey) const
	{
		if ( T::HasIndex() )
		{
			const_iterator iter = m_mapTable.find( iKey );
			return (iter != m_mapTable.end());
		}else{
			return false;	
		}
	}

	virtual T* GetEntryBySN (int sn )
	{
		if( T::HasIndex() )
		{
			if( m_mapTable.empty() || sn < 0 || sn >=  (int)(m_mapTable.size() ) )
			{
				return NULL ;
			}

			iterator iter = m_mapTable.begin() ;
			std::advance( iter, sn ) ;
			return &iter->second ;
		}
		else
		{
			if( sn < 0 || sn >= (int)m_table.size() )
			{
				return NULL ;
			}

			return &m_table[sn] ;
		}

		return NULL ;
	};

	/*
	iIdx [IN]:
	HasIndex: the index key of the table
	NoIndex : the location in table
	entry[OUT]:
	value to get
	return:true if finded, false if not find.
	*/
	virtual bool GetEntry(int iIdx,T* pEntry) const
	{
		if ( T::HasIndex() )
		{
			if (m_mapTable.empty())
				return false;
			const_iterator iter = m_mapTable.find( iIdx );
			if (  iter ==  m_mapTable.end() )
			{
				return false;
			}else
			{
				*pEntry = iter->second;
				return true;
			}
		}
		else{
			if (iIdx < 0 || iIdx >=  (int)m_table.size())
			{
				return false;
			}
			*pEntry = m_table[iIdx];
		}
		return true;
	}
	virtual bool GetEntryBySN(int sn,T* pEntry) const {
		if ( T::HasIndex() )
		{
			if (m_mapTable.empty() || sn < 0 || sn >=  (int)(m_mapTable.size()))
				return false;
			const_iterator iter = m_mapTable.begin();
			for(int i=0; i<sn; i++)
			{
				if (iter != m_mapTable.end())
				{
					++iter;
				}
			}
			*pEntry = iter->second;
			return true;

		}
		else{
			if (sn < 0 || sn >=  (int)m_table.size())
			{
				return false;
			}
			*pEntry = m_table[sn];
		}
		return true;
	};
	//add an entry to table.if has key,the old one will be replaced.
	virtual bool AddEntry( T* pEntry)
	{
		bool bHasIndex = T::HasIndex();
		if ( bHasIndex )
		{
			m_mapTable[ pEntry->GetKey()] = *pEntry;
		}else{
			m_table.push_back(*pEntry);
		}
		return true;
	}

	//remove an Entry from the table
	virtual bool RemoveEntry(int iIdx )
	{
		if ( T::HasIndex() )
		{
			iterator iter = m_mapTable.find( iIdx );
			if (  iter ==  m_mapTable.end() )
			{
				return false;
			}else
			{
				m_mapTable.erase(iter);
			}
		}
		else{
			if (iIdx < 0 || iIdx >=  (int)m_table.size())
			{
				return false;
			}
			m_table.erase(m_table.begin() + iIdx);
		}
		return true;

	}

	//for in game,quick get entry
	virtual const T* GetEntry( int iIdx ) const
	{
		if ( T::HasIndex() )
		{
			if (m_mapTable.empty())
			{
				return NULL;
			}

			const_iterator iter = m_mapTable.find( iIdx );
			if (  iter ==  m_mapTable.end() )
			{
				return NULL;
			}else
			{
				return &(iter->second);
			}
		}
		else{
			if (iIdx < 0 || iIdx >=  (int)m_table.size())
			{
				return NULL;
			}
			return &m_table[iIdx];
		}
	}


	//interface 
public:

	virtual bool LoadData(const char* szSource = NULL,LOAD_FLAG flagLoad = FlagNone,MULTILANG curLanguage = LANG_EN)
	{
		ASSERT(false);
		return false;
	};

	bool PrepareData(int iCurLanguage)
	{
		return true;
	};

	virtual bool DumpToMysql(const char* szDest = NULL )
	{
		ASSERT(false);
		return false;
	}

	virtual bool DumpToFile(const char* szDest = NULL )
	{
		ASSERT(false);
		return false;
	}

	virtual bool ReadEntry(uint32 nEntryID, T* pEntry) const
	{
		ASSERT(false);
		return false;
	}

	virtual bool WriteEntry(T* pEntry)
	{
		ASSERT(false);
		return false;
	}

	virtual bool DeleteEntry(uint32 iEntry)
	{
		ASSERT(false);
		return false;
	}

	virtual bool AddEntrySync(T* pEntry)
	{
		ASSERT(false&&"just for mysql");
		return false;
	}

	virtual bool RemoveEntrySync( int iIdx )
	{
		ASSERT(false&&"just for mysql");
		return false;
	}

	virtual bool BeginWrite()
	{
		return true;
	}

	virtual bool EndWrite()
	{
		return true;
	}

	static bool UnpackEntry(ByteBuffer* pBuff, T* pEntry)
	{
		const char * pFormat =  T::GetFormat();
		char * structpointer = (char*)pEntry;
		//uint32 offset = 0;
		for(; *pFormat != 0; ++pFormat)
		{
			switch(*pFormat)
			{
			case 'k':
			case 'u':	// Unsigned integer
				*pBuff>>(*(uint32*)structpointer);
				structpointer += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				*pBuff>>(*(int32*)structpointer);
				structpointer += sizeof(int32);
				break;

			case 'l':	// Signed integer 64
				*pBuff>>(*(int64*)structpointer);
				structpointer += sizeof(int64);
				break;

			case 'b':	// Unsigned integer 64
				*pBuff>>(*(uint64*)structpointer);
				structpointer += sizeof(uint64);
				break;

			case 'f':	// Float
				*pBuff>>(*(int32*)structpointer);
				structpointer += sizeof(float);
				break;

			case 'c':	// Char
				*pBuff>>(*(char*)structpointer);
				structpointer += sizeof(char);
				break;

			case 'h':	// Short
				*pBuff>>(*(short*)structpointer);
				structpointer += sizeof(short);
				break;

			case 's':	// std::string
				*pBuff>>(*(std::string*)structpointer);
				structpointer += sizeof(std::string);
				break;

			case 't':	// time
				*pBuff>>(*(int64*)structpointer);
				structpointer += sizeof(int64);
				break;

            case 'x':	// ignore
                break;

			default:	// unknown
				printf("%s:unknown field type in string: %d\n", T::GetTableName(), *pFormat);
				break;
			}
		}
		return true;
	}

	static bool PackEntry(const T* pEntry, ByteBuffer* pBuff)
	{ 
		const char * pFormat = T::GetFormat();
		char * structpointer = (char*)pEntry;
		uint32 offset = 0;
		for(; *pFormat != 0; ++pFormat)
		{
			switch(*pFormat)
			{
			case 'k':
			case 'u':	// Unsigned integer
				*pBuff <<  *(uint32*)&structpointer[offset];
				offset += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				*pBuff << *(int32*)&structpointer[offset];
				offset += sizeof(int32);
				break;

			case 'l':	// Signed integer 64
				*pBuff << *(int64*)&structpointer[offset];
				offset += sizeof(int64);
				break;

			case 'b':	// Unsigned integer 64
				*pBuff << *(uint64*)&structpointer[offset];
				offset += sizeof(uint64);
				break;

			case 's':	// Null-terminated string
				{
					*pBuff << *(std::string*)&structpointer[offset];
					offset += sizeof(std::string);
				}
				break;

			case 'f':	// Float
				*pBuff << *(float*)&structpointer[offset];
				offset += sizeof(float);
				break;

			case 'c':	// Char
				*pBuff << *(uint8*)&structpointer[offset];
				offset += sizeof(uint8);
				break;

			case 'h':	// Short
				*pBuff << *(uint16*)&structpointer[offset];
				offset += sizeof(uint16);
				break;

			case 't':	// time
				*pBuff << *(int64*)&structpointer[offset];
				offset += sizeof(int64);
				break;
            case 'x':	// ignore
                break;

			default:	// unknown
				printf("%s:unknown field type in string: %d\n", T::GetTableName(), *pFormat);
				break;
			}
		}
		return true;
	}

	static bool cmpentry(const T* pEntry, const T* pEntry2)
	{ 
		const char * pFormat = T::GetFormat();
		char * structpointer = (char*)pEntry;
		char * structpointer2 = (char*)pEntry2;
		uint32 offset = 0;
		for(; *pFormat != 0; ++pFormat)
		{
			switch(*pFormat)
			{
			case 'k':
			case 'u':	// Unsigned integer
				if(*(uint32*)&structpointer[offset] != *(uint32*)&structpointer2[offset])
					return false;
				offset += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				if(*(int32*)&structpointer[offset] != *(int32*)&structpointer2[offset])
					return false;
				offset += sizeof(int32);
				break;

			case 'l':	// Signed integer 64
				if(*(int64*)&structpointer[offset] != *(int64*)&structpointer2[offset])
					return false;
				offset += sizeof(int64);
				break;

			case 'b':	// Unsigned integer 64
				if(*(uint64*)&structpointer[offset] != *(uint64*)&structpointer2[offset])
					return false;
				offset += sizeof(uint64);
				break;

			case 's':	// Null-terminated string
				{
					if(*(std::string*)&structpointer[offset] != *(std::string*)&structpointer2[offset])
						return false;
					offset += sizeof(std::string);
				}
				break;

			case 'f':	// Float
				if(*(float*)&structpointer[offset] != *(float*)&structpointer2[offset])
					return false;
				offset += sizeof(float);
				break;

			case 'c':	// Char
				if(*(uint8*)&structpointer[offset] != *(uint8*)&structpointer2[offset])
					return false;
				offset += sizeof(uint8);
				break;

			case 'h':	// Short
				if(*(uint16*)&structpointer[offset] != *(uint16*)&structpointer2[offset])
					return false;
				offset += sizeof(uint16);
				break;

			case 't':	// time
				if(*(int64*)&structpointer[offset] != *(int64*)&structpointer2[offset])
					return false;
				offset += sizeof(int64);
				break;

            case 'x':	// ignore
                break;

			default:	// unknown
				printf("%s:unknown field type in string: %d\n", T::GetTableName(), *pFormat);
				break;
			}
		}
		return true;
	}

	static uint32 SizeByFormat()
	{ 
		const char * pFormat = T::GetFormat();
		uint32 offset = 0;
		for(; *pFormat != 0; ++pFormat)
		{
			switch(*pFormat)
			{
			case 'k':
			case 'u':	// Unsigned integer
				offset += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				offset += sizeof(int32);
				break;

			case 'l':	// Signed integer 64
				offset += sizeof(int64);
				break;

			case 'b':	// Unsigned integer 64
				offset += sizeof(uint64);
				break;

			case 's':	// Null-terminated string
				offset += sizeof(std::string);
				break;

			case 'f':	// Float
				offset += sizeof(float);
				break;

			case 'c':	// Char
				offset += sizeof(uint8);
				break;

			case 'h':	// Short
				offset += sizeof(uint16);
				break;

			case 't':	// time
				offset += sizeof(int64);
				break;

            case 'x':	// ignore
                break;

			default:	// unknown
				printf("%s:unknown field type in string: %d\n", T::GetTableName(), *pFormat);
				break;
			}
		}
		return offset;
	}

	int Lock()
	{
		m_mutex.lock();
		return 1 ;
	}

	int Unlock()
	{
		m_mutex.unlock();
		return 1 ;
	}
};

template<class T> CMySqlDatabase* CTableCache<T>::m_pDB = NULL;

#endif // __DATA_TABLECACHE_H__
