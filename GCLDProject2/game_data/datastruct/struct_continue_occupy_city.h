
#ifndef INCLUDE_STRUCT_CONTINUE_OCCUPY_CITY_H
#define INCLUDE_STRUCT_CONTINUE_OCCUPY_CITY_H

#include "struct_base.h"

#pragma pack(push,1)

struct DB_ContinueOccupyCity
{
	INIT_GAMEDATA(DB_ContinueOccupyCity)
	static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "country_serial_reward"; }
	static const char* GetFormat() {
		return 	"usfuu";
	}
	uint32 id;
	string country_count;
	float probability;
	uint32 serial_count;
	uint32 rewrad_count;
};


#pragma pack(pop)

#endif ////INCLUDE_STRUCT_CONTINUE_OCCUPY_CITY_H