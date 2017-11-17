



#ifndef INCLUDE_GUIDANCE_PROTO_SPECIAL_H
#define INCLUDE_GUIDANCE_PROTO_SPECIAL_H

#include "struct_base.h"
#pragma pack(push,1)

struct DB_GuidanceProtoSpecial
{
public:
	INIT_GAMEDATA(DB_GuidanceProtoSpecial)

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
		return "guidance_proto_special";
	}
	static const char* GetFormat() {
		return 
			"uuuuusuussusuuu";
	}

	uint32 id;
	uint32 groud_id;
	uint32 guidance_button_id;
	uint32 dialogue_id;
	uint32 ui_view_id;
	string ui_view_id2;
	uint32 arrow_direction;
	uint32 is_need_record;
	string record_type;
	string comment;
	uint32 main_ui_flag;
	string end_type;
	uint32 guidance_type;
	uint32 arrow_type;
	uint32 arrow_string_id;
};

#pragma pack(pop)

#endif ///INCLUDE_GUIDANCE_PROTO_SPECIAL_H