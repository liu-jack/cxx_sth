


#ifndef INCLUDE_LOTTERY_DAILY_H
#define INCLUDE_LOTTERY_DAILY_H
#include "struct_base.h"

#pragma pack(push,1)

struct DB_LotteryDaily
{
	INIT_GAMEDATA(DB_LotteryDaily)
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
		return "daily_login";
	}
	static const char* GetFormat()
	{
		return 	"usussussus";
	}
	uint32 id;
	string first_reward;
	uint32 first_weight;
	string first_image;
	string second_reward;
	uint32 second_weight;
	string second_image;
	string third_reward;
	uint32 third_weight;
	string third_image;
};

struct DB_StrongerQuest
{
	INIT_GAMEDATA(DB_StrongerQuest)
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
		return "stronger_quest";
	}
	static const char* GetFormat()
	{
		return 	"uuuuuuus";
	}
	uint32 id;
	uint32 quest_group;
	uint32 quest_type;
	uint32 val1;
	uint32 val2;
	uint32 description_id;
	uint32 skip_id;
	string image_name;
};
struct DB_StrongerGroup
{
	INIT_GAMEDATA(DB_StrongerGroup)
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
		return "stronger_group";
	}
	static const char* GetFormat()
	{
		return 	"uus";
	}
	uint32 id;
	uint32 open_level;
	string reward_list;
};


#pragma pack(pop)

#endif //INCLUDE_LOTTERY_DAILY_H