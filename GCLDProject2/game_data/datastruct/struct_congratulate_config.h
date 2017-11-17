
#ifndef INCLUDE_CONGRATULATE_CONFIG_H
#define INCLUDE_CONGRATULATE_CONFIG_H

#include "struct_base.h"
#pragma pack(push,1)

struct DB_CongratulateConfig
{
public:
	INIT_GAMEDATA(DB_CongratulateConfig)

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
		return "congratulate_config";
	}
	static const char* GetFormat() {
		return 
			"uuuuuussu";
	}

	uint32 id;
	uint32 congratulate_type;
	uint32 cat1;
	uint32 before_rank;
	uint32 congratulate_id;
	uint32 be_congratulate_id;
	string congratulate_reward_id;
	string be_congratulate_reward_id;
	uint32 get_congratulate_id;
};

#pragma pack(pop)

#endif