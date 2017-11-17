#pragma once
#include "struct_base.h"

#pragma pack(push,1)

struct DB_Function_Notice
{
	INIT_GAMEDATA(DB_Function_Notice)
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
		return "function_notice";
	}
	static const char* GetFormat()
	{
		return 	"uuusuuu";
	}

	uint32          id;
	uint32          begin_show_level;
	uint32          end_show_level;
	std::string		image_name;
	uint32			button_name_id;
	uint32			button_content_id;
	uint32			button_down_name_id;

};

#pragma pack(pop)