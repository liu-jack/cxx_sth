#pragma once
#include "struct_base.h"

#pragma pack(push,1)

struct DB_SpecialEffect
{
	INIT_GAMEDATA(DB_SpecialEffect)
	static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "special_effect_proto"; }
	static const char* GetFormat() {
		return "usffuusu";
	}

	uint32 id;			
	string effect_name;
	float time1;
	float time2;
	uint32 mount_position;
	uint32 effect_type;
	string effect_site;
	uint32 sound_id;
};
#pragma pack(pop)