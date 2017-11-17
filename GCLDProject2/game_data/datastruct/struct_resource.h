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


struct DB_Resource
{
    INIT_GAMEDATA(DB_Resource)
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
		return "resource";
	}
	static const char* GetFormat()
	{
		return "hsu";
	}
    uint16      id;
    std::string comment;
    uint32      name_id;
};

#pragma pack(pop)