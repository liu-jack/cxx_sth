#include "TypeDef.h"
#include "ObjectDefines.h"


// 标志位使用32位 对应 ObjectType, 每个类型都有自己的一位
const uint32 ObjFlg[OBJECT_TYPE_COUNT] = {
	0x00000001,//TYPE_OBJECT            = 0,
	0x00000002,//TYPE_LOCATABLE         = 1,
	0x00000004,//TYPE_ITEM              = 2,
	0x00000008,//TYPE_UNIT              = 3,
	0x00000010,//TYPE_STATIC_OBJECT     = 4,
	0x00000020,//TYPE_DYNAMIC_OBJECT    = 5,
	0x00000040,//TYPE_CORPSE            = 6,
	0x00000080,//TYPE_CONTAINER         = 7,
	0x00000100,//TYPE_AREA_TRIGGER      = 8,
	0x00000200,//TYPE_PLAYER            = 9,
	0x00000400,//TYPE_CREATURE          = 10,
	0x00000800,//TYPE_ETHEREAL          = 11,
	
	////client only
	0x00001000,//TYPE_HERO              = 12,
	0x00002000,//TYPE_EFFECT            = 13,

	////pet
	0x00004000,//TYPE_PET               = 14,

	////virtual item
	0x00008000,//TYPE_VIRTUAL_ITEM      = 15,

	0x00010000,//TYPE_MAP_HOOK          = 16,
	0x00020000,//TYPE_DUNGEON_HOOK      = 17,
	0x00040000,//TYPE_ARENA_HOOK        = 18,
	0x00080000,//TYPE_BF_HOOK           = 19,

	0x00100000,//TYPE_ARENA_TEAM        = 20,

	////bullet
	0x00200000,//TYPE_BULLET            = 21,
} ;

// 继承关系表对,应 ObjectType, 如果该位为一，则属于该类型
const uint32 ObjectFlagTable[OBJECT_TYPE_COUNT] = {
	//TYPE_OBJECT            = 0,
	ObjFlg[TYPE_OBJECT],

	//TYPE_LOCATABLE         = 1,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE],

	//TYPE_ITEM              = 2,
	ObjFlg[TYPE_ITEM],

	//TYPE_UNIT              = 3,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_UNIT],

	//TYPE_STATIC_OBJECT     = 4,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_STATIC_OBJECT],

	//TYPE_DYNAMIC_OBJECT    = 5,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_DYNAMIC_OBJECT],

	//TYPE_CORPSE            = 6,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_CORPSE],

	//TYPE_CONTAINER         = 7,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_CONTAINER],

	//TYPE_AREA_TRIGGER      = 8,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_AREA_TRIGGER],

	//TYPE_PLAYER            = 9,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_UNIT] | ObjFlg[TYPE_PLAYER],

	//TYPE_CREATURE          = 10,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_UNIT] | ObjFlg[TYPE_CREATURE],

	//TYPE_ETHEREAL          = 11,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_ETHEREAL],

	////client only
	//TYPE_HERO              = 12,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_UNIT] | ObjFlg[TYPE_PLAYER] | ObjFlg[TYPE_HERO],

	//TYPE_EFFECT            = 13,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_UNIT] | ObjFlg[TYPE_EFFECT],

	////pet
	//TYPE_PET               = 14,
	ObjFlg[TYPE_OBJECT] | ObjFlg[TYPE_LOCATABLE] | ObjFlg[TYPE_UNIT] | ObjFlg[TYPE_CREATURE] | ObjFlg[TYPE_PET],

	////virtual item
	//TYPE_VIRTUAL_ITEM      = 15,
	ObjFlg[TYPE_VIRTUAL_ITEM],

	//TYPE_MAP_HOOK          = 16,
	ObjFlg[TYPE_MAP_HOOK],

	//TYPE_DUNGEON_HOOK      = 17,
	ObjFlg[TYPE_DUNGEON_HOOK] | ObjFlg[TYPE_MAP_HOOK],

	//TYPE_ARENA_HOOK        = 18,
	ObjFlg[TYPE_ARENA_HOOK] | ObjFlg[TYPE_MAP_HOOK],

	//TYPE_BF_HOOK           = 19,
	ObjFlg[TYPE_BF_HOOK] | ObjFlg[TYPE_MAP_HOOK],

	//TYPE_ARENA_TEAM        = 20,
	ObjFlg[TYPE_ARENA_TEAM],

	////bullet
	//TYPE_BULLET            = 21,
	ObjFlg[TYPE_BULLET],

} ;


// 获取类型对应的继承关系
uint32 GetTypeFlag( uint32 type )
{
	return ObjectFlagTable[type] ;
}

uint32 Guid::Guid32( const Guid &guid )
{
	return guid.m_bigInfo.m_guid32 ;
}

uint32 Guid::Other32( const Guid &guid )
{
	return guid.m_bigInfo.m_other32 ;
}

uint32 Guid::ServerId( const Guid &guid )
{
	return guid.m_smallInfo.m_serverId ;
}

uint32 Guid::RegionId( const Guid &guid )
{
	return guid.m_smallInfo.m_regionId ;
}


//#define GET_TYPE(guid)                  ((uint32)((guid)>>32))
uint32 Guid::Type( const Guid &guid )
{
	return guid.m_smallInfo.m_type ;
}

//#define GET_LOW_GUID(guid)              ((uint32)(guid))
uint32 Guid::LowGuid( const Guid &guid )
{
	return guid.m_smallInfo.m_lowGuid ;
}

//#define IS_KIND_OF(tSrcType,tDesType)   (((tSrcType)&(tDesType))==(tDesType))
bool Guid::IsKindOf( uint32 srcType, uint32 dstType )
{
	return ( ObjectFlagTable[srcType] & ObjectFlagTable[dstType] ) == ObjectFlagTable[dstType] ;
}

//#define IS_TYPE(tSrcType,tDesType)      ((tSrcType)==(tDesType))
bool Guid::IsType( uint32 srcType, uint32 dstType )
{
	return srcType == dstType ;
}


bool operator < ( const Guid &lhs, const Guid &rhs )
{
	return lhs.m_guid < rhs.m_guid ;
}
