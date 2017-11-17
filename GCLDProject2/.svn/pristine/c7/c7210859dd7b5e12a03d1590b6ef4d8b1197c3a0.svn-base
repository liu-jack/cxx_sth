


#pragma once
	
#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_SignUp{
public:

	INIT_GAMEDATA(DB_SignUp)

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
		return "daily_sign";
	}
	static const char* GetFormat() {
		return 
			"ussssuuuuu";
	}

	uint32	id;
	std::string base_reward;
	std::string base_reward_num;
	std::string extra_reward;
	std::string extra_reward_num;
	uint32 need_vip;
	uint32 power_num;
	uint32 sup_sign_cost;
	uint32 base_reward_tips_id;
	uint32 extra_reward_tips_id;
};


#pragma pack(pop)



