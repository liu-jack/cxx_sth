
#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ActivityXilian
{
public:

	INIT_GAMEDATA(DB_ActivityXilian)

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
		return "activity_xilian";
	}
	static const char* GetFormat() {
		return 
			"uus";
	}

	uint32	id;
	uint32 need_times;
	std::string reward_list;
};

#pragma pack(pop)