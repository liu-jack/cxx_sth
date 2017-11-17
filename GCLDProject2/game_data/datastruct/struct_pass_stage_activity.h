#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ActivityStageRewardDiamond{
public:

	INIT_GAMEDATA(DB_ActivityStageRewardDiamond)

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
		return "activity_stage_reward_diamond";
	}
	static const char* GetFormat() {
		return 
			"uuusuu";
	}

	uint32	id;
	uint32 stage_level_id;
	uint32 name_id;
	std::string image_name;
	uint32 reward_item_id;
	uint32 reward_item_num;
};


#pragma pack(pop)