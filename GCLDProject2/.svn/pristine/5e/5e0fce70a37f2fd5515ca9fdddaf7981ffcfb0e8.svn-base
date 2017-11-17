#pragma once


#include "struct_base.h"
#pragma pack(push,1)

struct DB_SkipProto
{
	INIT_GAMEDATA(DB_SkipProto)

	static const bool HasIndex() {
		return true;
	}
	const uint32 GetKey() const {
		return id;
	}
	static const char* GetKeyName() {
		return "id";
	}
	static const char* GetTableName() {
		return "skip_proto";
	}
	static const char* GetFormat() {
		return 
			"uuuuuu";
	}

	uint32 id;
	uint32 is_change_sence;
	uint32 id_type;
	uint32 target_id;
	uint32 ui_index;
	uint32 skip_name_id;

};

#pragma pack(pop)