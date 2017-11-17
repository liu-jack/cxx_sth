

#pragma  once


///Ω·Ωª√˚ À
#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ActivityVisitFamousReward
{
public:

	INIT_GAMEDATA(DB_ActivityVisitFamousReward)

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
		return "activity_visit_famous_reward";
	}
	static const char* GetFormat() {
		return 
			"uus";
	}

	uint32	id;
	uint32 need_renown;
	string reward_list;
};

struct DB_ActivityVisitFamousNpc
{
public:

	INIT_GAMEDATA(DB_ActivityVisitFamousNpc)

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
		return "activity_visit_famous_npc";
	}
	static const char* GetFormat() {
		return 
			"usuuu";
	}

	uint32	id;
	string reward_list;
	uint32 country_id;
	uint32 weight;
	uint32 amy_hz;//npc hz
};


struct DB_ActivityVisitFamousEvent
{
public:

	INIT_GAMEDATA(DB_ActivityVisitFamousEvent)

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
		return "activity_visit_famous_event";
	}
	static const char* GetFormat() {
		return 
			"uuusu";
	}

	uint32	id;
	uint32 event_id;
	uint32 weight;
	string left_image;
	uint32 image_name_id;
};

#pragma pack(pop)
