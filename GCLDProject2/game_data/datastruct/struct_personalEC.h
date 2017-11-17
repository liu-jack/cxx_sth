#pragma once
#include "struct_base.h"
#include "ptr_container/PtrMap.h"
#pragma pack(push,1)

struct DB_PersonalEvent
{
	INIT_GAMEDATA(DB_PersonalEvent)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "events_for_personal"; }
	static const char* GetFormat() {
		return "uuus";
	}

	uint32          id;
	uint32          personal_event_type;
	uint32          index_id;
	string          quest_list;
};

struct DB_WorldEvent
{
	INIT_GAMEDATA(DB_WorldEvent)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "events_for_world"; }
	static const char* GetFormat() {
		return "uus";
	}

	uint32          id;
	uint32          refresh_time;
	string          city_area_id;
};

struct DB_EventQuestPool
{
	INIT_GAMEDATA(DB_EventQuestPool)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "events_quest_pool"; }
	static const char* GetFormat() {
		return "uuuuuuu";
	}

	uint32          id;
	uint32          name_id;
	uint32          description_id;
	uint32          type_id;
	uint32          value1;
	uint32          value2;
	uint32          quest_target;
};


struct DB_PersonalEventAward
{
	INIT_GAMEDATA(DB_PersonalEventAward)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "events_for_personal_reward"; }
	static const char* GetFormat() {
		return "uuss";
	}

	uint32          id;
	uint32          belong_id;
	string          level_area;
	string          reward_list;
};

struct DB_WorldEventAward
{
	INIT_GAMEDATA(DB_WorldEventAward)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "events_for_world_reward"; }
	static const char* GetFormat() {
		return "uuss";
	}

	uint32          id;
	uint32          belong_world_id;
	string          server_open_time;
	string          reward_list;
};


#pragma pack(pop)