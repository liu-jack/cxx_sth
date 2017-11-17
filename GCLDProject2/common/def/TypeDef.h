#ifndef TYPE_DEF_H_
#define TYPE_DEF_H_

//type define without irrlicht
#define FLAG_SHIFT 16
#define SHIFT_4		4
#define SHIFT_8		8
#define SHIFT_12	12
#define SHIFT_16	16
#define SHIFT_24	24
#define SHIFT_32	32


/////////////////////////////////////////////////
//typedef
typedef  unsigned char	BYTE;
typedef  unsigned int	UINT, uint;
/* Use correct types for x64 platforms, too */

#ifdef __GNUC__
typedef signed long long int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned int DWORD;
typedef unsigned char byte;
#else
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
typedef unsigned long DWORD;
#endif

union InstanceGUID
{
	InstanceGUID(uint64 id = 0)
	{
		Guid = id;
	}

	inline bool operator<( const InstanceGUID& other ) const
	{
		return Guid < other.Guid;
	}

	inline bool operator==(const InstanceGUID &other) const
	{
		return Guid == other.Guid;
	}

	inline bool operator!=(const InstanceGUID &other) const
	{
		return Guid != other.Guid;
	}

	inline bool operator==(uint64 id) const
	{
		return Guid == id;
	}

	inline bool operator!=(uint64 id) const
	{
		return Guid != id;
	}

	uint64 Guid;
	struct
	{
		uint16 GsId;
		uint16 MapId;
		uint32 SeqId;
	};
};

#if defined(__amd64__)||defined(__amd64)||defined(__x86_64__)||defined(__x86_64)||defined(_M_X64)
typedef uint64 dword_ptr;
#else
typedef uint32 dword_ptr;
#endif 


typedef	uint32				U32;
typedef	uint16				U16;
typedef	uint8				U8;
typedef	int32				S32;
typedef	int16				S16;
typedef	int8				S8;
typedef float				F32;
typedef double				F64;

typedef	uint32				u32;
typedef	uint16				u16;
typedef	uint8				u8;
typedef	int32				s32;
typedef	int16				s16;
typedef	int8				s8;
typedef float				f32;
typedef double				f64;

//#include <string>

typedef int GLfixed;

/* Define NULL pointer value */
/*
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
*/
 

#ifdef __GNUC__
#if !defined(IPHONEOS)
#define stricmp strcasecmp
#endif
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#else

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define atoll _atoi64
#define ltoa  _ltoa
#endif

#ifdef __GNUC__
#	define LIKELY( _x ) \
		__builtin_expect( !!( _x ), 1 )
#	define UNLIKELY( _x ) \
 		__builtin_expect( !!( _x ), 0 )
#else
#	define LIKELY( _x ) \
		(!!(_x))
#	define UNLIKELY( _x ) \
		(!!(_x))
#endif




#ifndef SOCKET
typedef uint32 SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#endif


// ANSI C .h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>



// STL .h
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <deque>

#include <limits>
#include <set>
#include <utility>//std::swap
#include <string>
//using namespace std;///Don't do this at home.Kids.
///Maybe we can include <ext/vstring.h> and <ext/vstring_fwd.h> instead of <string>
using std::string;
/**/
#ifdef __GNUC__
#include <ext/hash_map>
#include <ext/hash_set>
// #include <ext/vstring.h>
// #include <ext/vstring_fwd.h>
#else
#include <hash_map>
#include <hash_set>
/*#include <string>*/
#endif

// #if defined(__GNUC__)
// #ifdef USE_STD_STRING
// using std::string;
// #else ///!USE_STD_STRING
// typedef __gnu_cxx::__sso_string string;
// #endif
// #else
// using std::string;
// #endif

#if COMPILER == COMPILER_MICROSOFT
	#if _MSC_VER < 1310 || _MSC_VER >= 1600
		#define HM_NAMESPACE std
	#else
		#define HM_NAMESPACE stdext
	#endif
#else
#define HM_NAMESPACE __gnu_cxx
#endif



// CPU endian
#ifndef BIG_ENDIAN
#define LITTLE_ENDIAN 1
#endif

#if !LITTLE_ENDIAN
#define _BITSWAP16(x) (((x << 8) & 0xff00) | ((x >> 8) & 0x00ff))

#define _BITSWAP32(x) (((x << 24) & 0xff000000) | ((x <<  8) & 0x00ff0000) | \
	((x >>  8) & 0x0000ff00) | ((x >> 24) & 0x000000ff))

#define _BITSWAP64(x) (((x << 56) & 0xff00000000000000ULL) | ((x << 40) & 0x00ff000000000000ULL) | \
	((x << 24) & 0x0000ff0000000000ULL) | ((x <<  8) & 0x000000ff00000000ULL) | \
	((x >>  8) & 0x00000000ff000000ULL) | ((x >> 24) & 0x0000000000ff0000ULL) | \
	((x >> 40) & 0x000000000000ff00ULL) | ((x >> 56) & 0x00000000000000ffULL))
#else
#define _BITSWAP16(x) (x)
#define _BITSWAP32(x) (x)
#define _BITSWAP64(x) (x)
#endif

struct FTableHeader
{
	uint8		FLAG[8]; 
	uint32		nVersion;
	uint32		nRows;
	uint32		nCols;		
	uint32		nOffsetIndex;
	uint32		nOffsetFormat;
	uint32		nOffsetEntry;
	uint32		nEntrySize;
	uint32		nOffsetStrTable;
	uint32		nLengthStrTable;
};
#define	TABLE_STR_FLAG		"FTABLE"
#define TABLE_VERSION		 0x100000

#define DIR_BASE 0,0,-1
#define VEC3_BASE VEC3(0, 0, -1)
// --- usefull key defines
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))
#endif
#define STATIC_ASSERT(exp, name) typedef int dummy##name [(exp) ? 1 : -1]
#define STATIC_ASSERT_ARRAY_LENGTH(arr, len) STATIC_ASSERT(sizeof(arr)/sizeof(arr[0])==(len), ___sizeErr##arr)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Begin, Guid 区域
/*
 * uint64 <=> Guid
 * 8  type       对象类型
 * 11 regionID   区ID
 * 13 serverID   服务器ID
 * 32 guid       对象guid
 *
 */
// 最大值
const uint32  cMaxGuidType      =0x00000000000000FF ; // 255
const uint32  cMaxRegonId       =0x0000000000000FFE ; // 4096
const uint32  cMaxServerId      =0x0000000000001FFF ; // 8191
const uint32  cMaxGuid          =0x00000000FFFFFFFF ; // 4294967295
/////////--------------------------------------------------------------
////Those macros make Classes can't COPY AND ASSIGNMEMT.
#define DISALLOW_ASSIGN(Typename)\
	void operator=(const Typename&)
	
#define DISALLOW_COPY_AND_ASSIGN(Typename) \
	Typename(const Typename&);\
	DISALLOW_ASSIGN(Typename)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(Typename)\
	Typename();					\
	DISALLOW_COPY_AND_ASSIGN(Typename)
//////----------------------------------------------------------------
//const string::string(const string&);
// 自动兼容 uint64
union Guid
{
public:
	~Guid( void ) {}
	Guid( void ) : m_guid( 0 ) {}
	Guid( const int64 &guid ) : m_guid( guid ) {    assert( m_guid != 0 ) ;    }
	Guid( const uint64 &guid ) : m_guid( guid ) {    assert( m_guid != 0 ) ;    }

	explicit Guid( uint32 guid32, uint32 other32 )
	{
		m_bigInfo.m_guid32 =guid32 ;
		m_bigInfo.m_other32 =other32 ;
		assert( m_guid != 0 ) ;
	}

	explicit Guid(uint32 type, uint32 regionId, uint32 serverId,uint32 lowGuid ) 
	{
		m_smallInfo.m_type     =type;
		m_smallInfo.m_regionId =regionId;
		m_smallInfo.m_serverId =serverId;
		m_smallInfo.m_lowGuid  =lowGuid ;
		assert( m_guid != 0 ) ;
	}


	// 检测作用
	Guid( const int &guid ) : m_guid( static_cast<uint64>( guid ) ) {    if( m_guid != 0 ) {   assert( false ) ;   }    }
	Guid( const short &guid ) : m_guid( static_cast<uint64>( guid ) ) {    if( m_guid != 0 ) {   assert( false ) ;   }    }
	
	Guid( const uint8 &guid ) : m_guid( static_cast<uint64>( guid ) ) {    if( m_guid != 0 ) {   assert( false ) ;   }    }
	Guid( const uint16 &guid ) : m_guid( static_cast<uint64>( guid ) ) {    if( m_guid != 0 ) {   assert( false ) ;   }    }
	Guid( const uint32 &guid ) : m_guid( static_cast<uint64>( guid ) ) {    if( m_guid != 0 ) {   assert( false ) ;   }    }

public:
	operator uint64&( void ) {    return m_guid ;    }

	operator uint64( void ) const {    return m_guid ;    }

public:
	uint64 m_guid ;

private:
	struct SmallInfo
	{
		uint64 m_lowGuid:32;  // 0000000000000000000000000000000011111111111111111111111111111111
		uint64 m_serverId:16; // 0000000000000000111111111111111100000000000000000000000000000000
		uint64 m_regionId:8;  // 0000000011111111000000000000000000000000000000000000000000000000
		uint64 m_type:8;      // 1111111100000000000000000000000000000000000000000000000000000000 
	} m_smallInfo ;

	struct BigInfo
	{
		uint32 m_guid32 ;
		uint32 m_other32 ;
	} m_bigInfo ;

public:
	static uint32 Guid32( const Guid &guid ) ;
	static uint32 Other32( const Guid &guid ) ;

	static uint32 ServerId( const Guid &guid ) ;
	static uint32 RegionId( const Guid &guid ) ;

	static uint32 Type( const Guid &guid ) ;
	static uint32 LowGuid( const Guid &guid ) ;

	static bool IsType( uint32 srcType, uint32 dstType ) ;
	static bool IsKindOf( uint32 srcType, uint32 dstType ) ;
} ;

bool operator < ( const Guid &lhs, const Guid &rhs ) ;


typedef Guid ObjGUID;
//const static ObjGUID ObjGUID_NULL = 0;

// 获取类型对应的继承关系
uint32 GetTypeFlag( uint32 type ) ;


#define GET_TYPE(guid)                  Guid::Type( (guid) )
#define GET_LOW_GUID(guid)              Guid::LowGuid( (guid) )
#define IS_TYPE(tSrcType,tDesType)      Guid::IsType( (tSrcType), (tDesType) )
#define IS_KIND_OF(tSrcType,tDesType)   Guid::IsKindOf( (tSrcType), (tDesType) )

#ifdef _MSC_VER // maybe check the specific version, too...
    #define SSCANF sscanf_s
    #define SPRINTF sprintf_s
#define SWPRINTF	swprintf_s
#else
    #define SSCANF sscanf
    #define SPRINTF sprintf
#define SWPRINTF swprintf
#endif

struct IntV3
{
	int first;
	int second;
	string third;
};

struct IntV4
{
	int first;
	int second;
	int third;
	int fourth;
};
typedef std::string    ProductId ;
typedef uint32 ItemId;
typedef std::map<ItemId, uint32> ItemID2Count;
typedef std::map<uint32, uint32> CurrType2Count;
typedef std::pair<int, int>     IntPair;
typedef std::pair<uint64, string>  BstrPair;
typedef std::pair<string, string>  StringPair;
typedef std::vector< IntPair >  IntPairVec;
typedef std::vector< IntV3 >  IntV3Vec;
typedef std::map<int,StringPair>  ISpairMap;
typedef std::vector<IntPairVec> VecIntPairvec;
typedef std::vector< IntV4 >  IntV4Vec;
typedef std::vector<int> VecInt;
typedef std::vector<uint32> VecUint;
// End, Guid 区域
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif//TYPE_DEF_H_
