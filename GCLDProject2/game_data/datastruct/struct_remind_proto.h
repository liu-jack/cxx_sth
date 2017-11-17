#pragma once

#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_RemindProto{
public:

	INIT_GAMEDATA(DB_RemindProto)

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
		return "remind_proto";
	}
	static const char* GetFormat() {
		return 
			"uuuusu";
	}

	uint32		id;
	uint32		type_id;
	uint32		end_type;
	uint32		skip_id;
	std::string	image_name;
	uint32		name_id;
};


#pragma pack(pop)