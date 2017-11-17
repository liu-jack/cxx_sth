#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ActivityGeneralRewardBox{
public:

	INIT_GAMEDATA(DB_ActivityGeneralRewardBox)

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
		return "activity_general_reward_box";
	}
	static const char* GetFormat() {
		return 
			"uuu";
	}

	uint32	id;
	uint32 reward_item_id;
	uint32 reward_item_num;
};
struct DB_ActivityGeneralRewardProto{
public:

	INIT_GAMEDATA(DB_ActivityGeneralRewardProto)

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
		return "activity_general_reward_proto";
	}
	static const char* GetFormat() {
		return 
			"uusuus";
	}

	uint32	id;
	uint32 troops_id;
	string cost;
	uint32 reward_box_first;
	uint32 reward_troops_exp;
	string reward_critical_weight;
};

#pragma pack(pop)