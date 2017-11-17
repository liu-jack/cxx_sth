#pragma once
#include "struct_base.h"
#pragma pack(push,1)


struct DB_TeachMapProto
{
	INIT_GAMEDATA(DB_TeachMapProto)
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
		return "teach_proto";
	}
	static const char* GetFormat()
	{
		return 	"uuususssssu";
	}

	uint32		id;
	uint32		questid;
	uint32		teach_type;
	string		teach_value;
	uint32		is_fresh;
	string		city_initialize;
	string		city_attack_npc;
	string		city_defense_npc;
	string		teach_soldiers;
	string		guidance_type;
	uint32		show_skill_guidance;

};

struct DB_TeachWorld
{
	INIT_GAMEDATA(DB_TeachWorld)
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
		return "teach_world";
	}
	static const char* GetFormat()
	{
		return 	"uus";
	}

	uint32		id;
	uint32		countryid;
	string teach_map;

};

struct DB_Teach_Soldier
{
	INIT_GAMEDATA(DB_Teach_Soldier)
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
		return "teach_soldier";
	}
	static const char* GetFormat()
	{
		return 	"uuu";
	}

	uint32		id;
	uint32		sourceid;
	uint32		level;
};

#pragma pack(pop)