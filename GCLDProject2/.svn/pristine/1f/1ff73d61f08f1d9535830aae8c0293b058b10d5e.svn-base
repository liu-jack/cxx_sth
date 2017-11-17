

#pragma once

#include "struct_base.h"
#pragma pack(push,1)

struct DB_AddSoliderMatch{
public:

	INIT_GAMEDATA(DB_AddSoliderMatch)

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
		return "addsoldier_match";
	}
	static const char* GetFormat() {
		return 
			"uss";
	}

	uint32	id;
	string ranking;
	string reward;
};


struct DB_OccupationMatch{
public:
	INIT_GAMEDATA(DB_OccupationMatch)

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
		return "occupation_match";
	}
	static const char* GetFormat() {
		return 
			"uss";
	}

	uint32 id;
	string ranking;
	string reward;
};


struct DB_CountryMatch{
public:
	INIT_GAMEDATA(DB_CountryMatch)

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
		return "country_match";
	}
	static const char* GetFormat() {
		return 
			"uus";
	}

	uint32 id;
	uint32 country;
	string reward;
};


#pragma pack(pop)