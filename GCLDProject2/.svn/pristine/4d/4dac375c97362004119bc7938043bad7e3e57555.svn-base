
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
//Item Table

struct C_Skill
{
	INIT_GAMEDATA(C_Skill)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	static const char* GetKeyName()
	{
		return "Id";
	}
	static const char* GetTableName()
	{
		return "c_skills";
	}
	static const char* GetFormat()
	{
		return "usus";
	}
	uint32 Id;
	std::string		skillname;
	uint32 maxlevel;
	std::string		desc;
};

#pragma pack(pop)