

#ifndef __STRUCT_DEF_H__
#define __STRUCT_DEF_H__


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
uint64/time_t		't'
int64				'l'
uint64				'b'
short				'h'
*/

#include <stdarg.h>

#include <string>

#include "def/TypeDef.h"
#include <boost/smart_ptr.hpp>
struct ExtraDataStruct
{
public:
	ExtraDataStruct():is_modify(false),is_online(false)
	{
	}

public:
	bool		IsModify() { return is_modify;}
	void		SetModify() { is_modify = true;}
	void		ResetModify() { is_modify = false;}
	void        SetOnline() { is_online = true;}
	void        ResetOnline() { is_online = false;}
	bool        IsOnline()  { return is_online;}
private:
	bool        is_modify;
	bool        is_online;
} ;

union CombinId
{
public:
	~CombinId( void ) {}
	CombinId( void ): m_combinKey( 0 ){}
	CombinId(const CombinId& other ): m_combinKey(other.m_combinKey){}
	CombinId( const uint64 &type ): m_combinKey( type ){}
	explicit CombinId( uint32 index32, uint32 other32 )
	{
		m_keyInfo.m_index =index32 ;
		m_keyInfo.m_key =other32 ;
		assert( m_combinKey != 0 ) ;
	}
public:
	inline  bool operator<( const CombinId &other) const
	{
		return this->m_combinKey < other.m_combinKey;
	}

	//inline bool operator==( const CombinId &other) const
	//{
	//	return this->m_combinKey == other.m_combinKey;
	//}

	inline CombinId &operator++()
	{
		m_keyInfo.m_key++;
		return *this;
	}

	inline CombinId operator++(int n)
	{
		CombinId temp = *this;
		m_keyInfo.m_key++;
		return temp;
	}


	
public:
	operator uint64&( void ) {    return m_combinKey ;    }
	operator uint64( void ) const {    return m_combinKey ; }

public:
	uint64 m_combinKey ;
private:
	struct
	{
		uint32 m_key; // ตอ
		uint32 m_index; //
	}m_keyInfo;
} ;

struct BaseStruct
{
public:
	typedef uint64 IdType ;
	typedef uint64 Key;

public:
	inline static uint64 CalculateKey( uint32 partA, uint32 partB)
	{
		uint64 retValue =0 ;

		uint32 *pInt32 =reinterpret_cast< uint32* >( &retValue ) ;
		*pInt32 =partA ;
		++pInt32 ;
		*pInt32 =partB ;
		return retValue ;
	}

} ;



#define INIT_MEMBER_DATA \
	uint32 offset =0;\
	char* structpointer = (char*)this;\
	const char* pFormat = GetFormat();\
	for(uint32 i = 0; i< strlen(pFormat); i++ )\
		{\
		switch( pFormat[i])\
			{\
		case 'k':\
		case 'u':\
		*(uint32*)&structpointer[offset] = 0;\
		offset += sizeof(uint32);\
		break;\
			case 'i':\
			*(int32*)&structpointer[offset] = 0;\
			offset += sizeof(int32);\
			break;\
			case 'f':\
			*(float*)&structpointer[offset] = 0.0f;\
			offset += sizeof(float);\
			break;\
			case 's':\
			*(std::string*)&structpointer[offset] = "";\
			offset += sizeof(std::string);\
			break;\
			case 'c':\
			*(uint8*)&structpointer[offset] = 0;\
			offset += sizeof(uint8);\
			break;\
			case 'h':\
			*(uint16*)&structpointer[offset] = 0;\
			offset += sizeof(uint16);\
			break;\
			case 'l':\
			*(int64*)&structpointer[offset] = 0;\
			offset += sizeof(int64);\
			break;\
			case 'b':\
			*(uint64*)&structpointer[offset] = 0;\
			offset += sizeof(uint64);\
			break;\
			case 't':\
			*(int64*)&structpointer[offset] = 0;\
			offset += sizeof(int64);\
			break;\
			}\
		}

#define DECLARE_INCREASEID_FUCTION(idtype,keytype,id,idName)\
	typedef idtype IdType ;\
	typedef keytype Key ;\
	const IdType& GetIncreaseId( void ) const\
	{\
		return id ;\
	}\
	void SetDbIncreaseId(const IdType& id_)\
	{\
		id = id_;\
	}\
	static const char* GetDbIncreaseIdName()\
	{\
		return idName;\
	}

#define DECLARE_EXTRA_SIZE_BY_MEMBER(class_name,member_name) \
	static uint32 ExtraSize()\
	{\
	class_name* p = NULL;\
	uint32 offset = (uint32)((uint64)(&p->member_name));\
	return sizeof(class_name) -offset;\
	}



#define DECLARE_EXTRA_SIZE_ZERO \
	static uint32 ExtraSize()\
	{\
	return 0;\
	}

#define INIT_GAMEDATA(classname) \
	classname()\
	{\
	INIT_MEMBER_DATA; \
	};\
	DECLARE_EXTRA_SIZE_ZERO


#define INIT_GAMEDATA_AND_EXTRA( classname, extra_member ) \
	classname() \
	{ \
	INIT_MEMBER_DATA; \
	} ; \
	DECLARE_EXTRA_SIZE_BY_MEMBER( classname, extra_member )



#define TABLE_TYPEDEF( StructName ) \
	typedef CMysqlTableCache<StructName> StructName##CacheType;\
	typedef boost::shared_ptr<StructName##CacheType> StructName##Cache;\
	typedef StructName##CacheType::MapItr StructName##CacheMItr ;\
	typedef StructName##CacheType::VecItr StructName##CacheVItr ;



#endif


