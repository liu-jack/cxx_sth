
#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ActivityStarBox{
public:

	INIT_GAMEDATA(DB_ActivityStarBox)

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
		return "activity_star_box";
	}
	static const char* GetFormat() {
		return 
			"usuusuuu";
	}

	uint32	id;
	std::string level_range;
	uint32 star_level;
	uint32 need_score;
	std::string reward_list;//½±Àø
	uint32 reward_times;//
	uint32 quick_levelup;//
	uint32 next_level_need_wait_time;//
};

#pragma pack(pop)