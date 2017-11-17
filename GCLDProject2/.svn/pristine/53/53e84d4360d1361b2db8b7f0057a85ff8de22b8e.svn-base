

#pragma once
#include "struct_base.h"
#pragma pack(push,1)


struct DB_WorldFightAchievement
{
	INIT_GAMEDATA(DB_WorldFightAchievement)
		static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "world_fight_achievement";
	}
	static const char* GetFormat()
	{
		return 	"uuuuuuss";
	}

	uint32		id;
	uint32		city_id;
	uint32		belong_area;
	uint32		zerg_order;
	uint32		terran_order;
	uint32		protoss_order;

	string attack_reward;
	string occupy_reward;

};

struct DB_WorldFightAchievementBox
{
	INIT_GAMEDATA(DB_WorldFightAchievementBox)
		static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "world_fight_achievement_box";
	}
	static const char* GetFormat()
	{
		return 	"uuuss";
	}

	uint32		id;
	uint32		attack_city_need_num;
	uint32		occupy_city_need_num;
	string		attack_city_reward;
	string		occupy_city_reward;

};

#pragma pack(pop)