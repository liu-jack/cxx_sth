#pragma once
#include "struct_base.h"

#pragma pack(push,1)

struct DB_CostSetProto
{
	INIT_GAMEDATA(DB_CostSetProto)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "cost_set_proto"; }
	static const char* GetFormat() {
		return "usuu";
	}

	uint32 id;
	string commments;
	uint32 type_id;
	uint32 content_id;
};
#pragma pack(pop)