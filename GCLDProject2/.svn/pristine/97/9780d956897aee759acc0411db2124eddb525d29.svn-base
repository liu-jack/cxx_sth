#pragma once
#include "struct_base.h"

#pragma pack(push,1)

struct DB_CountryRoad
{
	INIT_GAMEDATA(DB_CountryRoad)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "country_road"; }
	static const char* GetFormat() {
		return "us";
	}

	uint32 id;
	string value1;
};
#pragma pack(pop)