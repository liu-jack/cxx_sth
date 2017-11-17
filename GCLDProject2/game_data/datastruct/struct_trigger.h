#pragma once
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
#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Trigger
{
    INIT_GAMEDATA(DB_Trigger)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "world_events_trigger";
	}
	static const char* GetFormat()
	{
		return "uuuu";
	}
    uint32     id;
    uint32     type;
    uint32     value1;
    uint32     value2;
};

#pragma pack(pop)