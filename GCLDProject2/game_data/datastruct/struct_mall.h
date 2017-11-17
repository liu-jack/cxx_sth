
#pragma once
#include <string>
#include "struct_base.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
index string		'k'
*/


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct MallProtoBase
{
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return item_id;
	}
	static const char* GetKeyName()
	{
		return "item_id";
	}  
    static const char* GetFormat()
	{
		return "uuuuuu";
	}
    uint32 item_id;
	uint32 mall_tag;
    uint32 bulk_count;
    uint32 price;
    uint32 limit_type;
    uint32 limit_count;
};

struct MallLaurelProto : public MallProtoBase
{
	INIT_GAMEDATA(MallLaurelProto)
	static const char* GetTableName()
	{
		return "sell_mall_laurel";
	}
};


struct MallDiamondProto : public MallProtoBase
{
	INIT_GAMEDATA(MallDiamondProto)
	static const char* GetTableName()
	{
		return "sell_mall_diamond";
	}
};

#pragma pack(pop)