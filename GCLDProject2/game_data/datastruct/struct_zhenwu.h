

#pragma once
///ÕþÎñ
#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ArmyGovText
{
public:

	INIT_GAMEDATA(DB_ArmyGovText)

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
		return "army_gov_text";
	}
	static const char* GetFormat() {
		return 
			"uuuuusu";
	}

	uint32	id;
	uint32 left_text_id;
	uint32 right_text_id;
	uint32 title_id;
	uint32 weight;
	string image_name;
	uint32 right_critical_text_id;
};

struct DB_ArmyGovRewardType
{
public:

	INIT_GAMEDATA(DB_ArmyGovRewardType)

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
		return "army_gov_reward_type";
	}
	static const char* GetFormat() {
		return 
			"uuuuu";
	}

	uint32	id;
	uint32 reward_id;
	uint32 which_side;//0 both side ,1 only right side
	uint32 reward_type;//
	uint32 weight;
};


struct DB_ArmyGovRewardNum
{
public:

	INIT_GAMEDATA(DB_ArmyGovRewardNum)

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
		return "army_gov_reward_num";
	}
	static const char* GetFormat() {
		return 
			"usuuu";
	}

	uint32	id;
	string level_stage;
	uint32 reward_type;//
	uint32 base_reward_num;
	uint32 weight;
};

struct DB_ArmyGovRewardCritical
{
public:

	INIT_GAMEDATA(DB_ArmyGovRewardCritical)

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
		return "army_gov_reward_critical";
	}
	static const char* GetFormat() {
		return 
			"uuuu";
	}

	uint32	id;
	uint32 critical_time;
	uint32 cost_diamond;//
	uint32 weight;
};

struct DB_ArmyGovLoyalReward
{
public:

	INIT_GAMEDATA(DB_ArmyGovLoyalReward)

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
		return "army_gov_loyal_reward";
	}
	static const char* GetFormat() {
		return 
			"uss";
	}

	uint32	id;
	string level_stage;
	string reward_list;
};



#pragma pack(pop)