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
//set byte alignment 
#pragma pack(push,1)
struct Event_mail
{
public:
	INIT_GAMEDATA(Event_mail);

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
		return "mail_proto";
	}

	static const char* GetFormat()
	{
		return "usuus";
	}

	

public:
	uint32 id;
	string comment;
	uint32 title;
	uint32 content;
	string items;
};




#pragma pack(pop)
