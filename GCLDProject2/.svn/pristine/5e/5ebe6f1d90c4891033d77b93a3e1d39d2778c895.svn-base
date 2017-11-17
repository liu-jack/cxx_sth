
#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_DailyQuest{
public:

	INIT_GAMEDATA(DB_DailyQuest)

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
		return "daily_quest";
	}
	static const char* GetFormat() {
		return 
			"uuuuuuuu";
	}

	uint32	id;
	uint32 type_id;//任务类型
	uint32 val1;
	uint32 val2;
	uint32 reward_score;//奖励积分
	uint32 trigger_id;//开启trigger
	uint32 name_id;//任务名称id
	uint32 skip_id;//跳转界面id
};


struct DB_DailyQuestReward{
public:
	INIT_GAMEDATA(DB_DailyQuestReward)

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
		return "daily_quest_reward";
	}
	static const char* GetFormat() {
		return 
			"uuu";
	}

	uint32 id;
	uint32 reward_score;//奖励积分点
	uint32 unlock_level;//奖励开启条件
};

#pragma pack(pop)