#pragma once

#include "struct_base.h"
#pragma pack(push,1)

struct DB_CharSiegeArmy{
public:

	INIT_GAMEDATA(DB_CharSiegeArmy)

	static const bool HasIndex() {
		return true;
	}
	const uint32 GetKey() const {
		return armyId;
	}
	static const char* GetKeyName() {
		return "armyId";
	}
	static const char* GetTableName() {
		return "char_siege_army";
	}
	static const char* GetFormat() {
		return 
			"uuuuuusus"
			"ffffffffffff"
			"usu";
	}

	uint32	armyId;
	uint32 nameId;
	uint32 headId;
	uint32 displayId;
	uint32 skillId;
	uint32 line_count;
	string specificId;
	uint32 fightTimes;
	string fightTerrain;
	float baseValue[12];
	uint32 unlock_type;
	string unlock_cost;
	uint32 discount_last_time;//hour
};

struct DB_SiegeArmySpecific{
public:

	INIT_GAMEDATA(DB_SiegeArmySpecific)

		static const bool HasIndex() {
			return true;
	}
	const uint32 GetKey() const {
		return specificId;
	}
	static const char* GetKeyName() {
		return "specificId";
	}
	static const char* GetTableName() {
		return "siege_army_specific";
	}
	static const char* GetFormat() {
		return 
			"uuus";
	}

	uint32	specificId;
	uint32	describeId;
	uint32	specificType;
	string	specificParameter;
};

struct DB_SiegeArmyUp{
public:

	INIT_GAMEDATA(DB_SiegeArmyUp)

	static const bool HasIndex() {
			return true;
	}
	const uint32 GetKey() const {
		return index;
	}
	static const char* GetKeyName() {
		return "index";
	}
	static const char* GetTableName() {
		return "siege_army_up";
	}
	static const char* GetFormat() {
		return 
			"uuuuussuu";
	}

	uint32	index;
	uint32	armyId;
	uint32	level;
	uint32	levelExp;
	uint32	developExp;
	string	developMaterial;
	string	critical;
	uint32	damage;
	uint32  fight_value;
};

#pragma pack(pop)