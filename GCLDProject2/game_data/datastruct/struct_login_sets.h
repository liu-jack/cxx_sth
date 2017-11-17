#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_LoginSets{
public:

	INIT_GAMEDATA(DB_LoginSets)

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
		return "login_sets";
	}
	static const char* GetFormat() {
		return 
			"uusuuuss";
	}

	uint32	id;
	uint32 country_name_id;
	std::string cartoon_name;
	uint32 male_image_id;
	uint32 female_image_id;
	uint32 description_id;
	std::string initial_image;
	std::string country_image_name;
};


#pragma pack(pop)