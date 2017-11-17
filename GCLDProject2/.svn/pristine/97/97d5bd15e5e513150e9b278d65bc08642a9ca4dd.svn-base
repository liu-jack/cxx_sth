
#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_GrowUpQuestProto
{
	INIT_GAMEDATA(DB_GrowUpQuestProto)

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
		return "growup_quest_proto";
	}
	static const char* GetFormat() {
		return 
			"uuuuuss";
	}
	
	uint32 id;
	uint32 quest_group;
	uint32 quest_type;
	uint32 val1;
	uint32 description_id;
	std::string reward;
	std::string skip_id;
};

struct DB_GrowUpQuestGroup
{
	INIT_GAMEDATA(DB_GrowUpQuestGroup)

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
		return "growup_quest_group";
	}
	static const char* GetFormat() {
		return 
			"uuuus";
	}

	uint32 id;
	uint32 quest_group;
	uint32 open_time;
	uint32 group_name_id;
	std::string reward;
};

#pragma pack(pop)