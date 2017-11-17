
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
	uint32 type_id;//��������
	uint32 val1;
	uint32 val2;
	uint32 reward_score;//��������
	uint32 trigger_id;//����trigger
	uint32 name_id;//��������id
	uint32 skip_id;//��ת����id
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
	uint32 reward_score;//�������ֵ�
	uint32 unlock_level;//������������
};

#pragma pack(pop)