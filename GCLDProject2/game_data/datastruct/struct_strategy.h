

#ifndef INCLUDE_STRUCT_STRATEGY_H
#define INCLUDE_STRUCT_STRATEGY_H
#include "struct_base.h"
#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_strategy_world_map
{
	INIT_GAMEDATA(DB_strategy_world_map)
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
		return "strategy_world_map";
	}
	static const char* GetFormat()
	{
		return "uususssuu";
	}
	uint32     id;
	uint32		label_id;
	string     label_title_id;
	uint32     label_content__id;
	string     label_content_text_id;
	string		image_name;
	string		content_title_id;
	uint32 content_title_star;
	uint32 skip_id;

};
struct DB_strategy_become_stronger
{
	INIT_GAMEDATA(DB_strategy_become_stronger)
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
		return "strategy_become_stronger";
	}
	static const char* GetFormat()
	{
		return "uususssuu";
	}
	uint32     id;
	uint32		label_id;
	string     label_title_id;
	uint32     label_content__id;
	string     label_content_text_id;
	string		image_name;
	string		content_title_id;
	uint32 content_title_star;
	uint32 skip_id;
};
struct DB_strategy_country_grow
{
	INIT_GAMEDATA(DB_strategy_country_grow)
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
		return "strategy_country_grow";
	}
	static const char* GetFormat()
	{
		return "uususssuu";
	}
	uint32     id;
	uint32		label_id;
	string     label_title_id;
	uint32     label_content__id;
	string     label_content_text_id;
	string		image_name;
	string		content_title_id;
	uint32 content_title_star;
	uint32 skip_id;
};
struct DB_strategy_cross_war
{
	INIT_GAMEDATA(DB_strategy_cross_war)
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
		return "strategy_cross_war";
	}
	static const char* GetFormat()
	{
		return "uususssuu";
	}
	uint32     id;
	uint32		label_id;
	string     label_title_id;
	uint32     label_content__id;
	string     label_content_text_id;
	string		image_name;
	string		content_title_id;
	uint32 content_title_star;
	uint32 skip_id;
};
struct DB_strategy_game
{
	INIT_GAMEDATA(DB_strategy_game)
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
		return "strategy_game";
	}
	static const char* GetFormat()
	{
		return "uus";
	}
	uint32     id;
	uint32     label_name_id;
	string     comment;
};
struct DB_strategy_quick_level
{
	INIT_GAMEDATA(DB_strategy_quick_level)
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
		return "strategy_quick_level";
	}
	static const char* GetFormat()
	{
		return "uususssuu";
	}
	uint32     id;
	uint32		label_id;
	string     label_title_id;
	uint32     label_content__id;
	string     label_content_text_id;
	string		image_name;
	string		content_title_id;
	uint32 content_title_star;
	uint32 skip_id;
};
#pragma pack(pop)

#endif  ////INCLUDE_STRUCT_STRATEGY_H
