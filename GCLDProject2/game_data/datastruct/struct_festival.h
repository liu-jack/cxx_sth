
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

struct Festival
{
	INIT_GAMEDATA(Festival)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return ID;
	}
	static const char* GetKeyName()
	{
		return "ID";
	}
	static const char* GetTableName()
	{
		return "festival";
	}
	static const char* GetFormat()
	{
		return "us";
	}
	
	uint32 ID;
	std::string festivalName;
};

#pragma pack(pop)