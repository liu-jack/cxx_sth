
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



struct SoundProto
{
	INIT_GAMEDATA(SoundProto)

	static const bool HasIndex()
	{
		return true;
	}
	//index key
	const uint32 GetKey() const
	{
		return id;
	}
	//index key field name
	static const char* GetKeyName()
	{
		return "id";
	}
	//table name
	static const char* GetTableName()
	{
		return "sound_proto";
	}
	//struct format
	static const char* GetFormat()
	{
		return "ususssuffff" ;
	}


	uint32      id ;
	std::string description ;
	uint32      flag ;
	std::string sound_main ;
	std::string sound_sub_01 ;
	std::string sound_sub_02 ;
	uint32      priority_id ;
	float       volume ;
	float       pitch ;
	float       distance_min_3d ;
	float       distance_max_3d ;
};


#pragma pack(pop)