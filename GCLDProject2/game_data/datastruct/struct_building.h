#pragma once
#include "struct_base.h"

#pragma pack(push,1)
#define  MAXIMPERIALREBUILDINVESTITEMS  3
struct DB_Module
{
	INIT_GAMEDATA(DB_Module)
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
		return "module";
	}
	static const char* GetFormat()
	{
		return 	"uuuuuuus";
	}

	uint32          id;//module id
	uint32          name_id;
	uint32          mission_req;
	uint32          warn;
	uint32          time_type;
	uint32          income_type;
	uint32          rebuild_id;
	string          image_name;

};

struct DB_BuildingProto
{
    INIT_GAMEDATA(DB_BuildingProto)
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
        return "building";
    }
    static const char* GetFormat()
    {
        return 	"uuuuuuuusss";
    }

    uint32          id;//building id
	uint32          module_id;
    uint32          name_id;
    uint32          level_req;
	uint32          rebuild_order;
	uint32          need_people_num;
	uint32          need_times;
	uint32          resource_add;
	string			zerg_build_image;
	string			terran_build_image;
	string			protoss_build_image;
};



struct DB_BuildingLevelUp
{
    INIT_GAMEDATA(DB_BuildingLevelUp)
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
        return "building_up";
    }
    static const char* GetFormat()
    {
        return 	"uuuuuuuuuuu";
    }

    uint32 id;
	uint32 build_id;
	uint32 lv;
    uint32 coin_req;
    uint32 wood_req;
    uint32 time_req;
	uint32 growtype_id;
    uint32 growspeed;
	uint32 limit_raise;
    uint32 honor_reward;
    uint32 exp_reward;
};

struct DB_ImperialRebuild
{
	INIT_GAMEDATA(DB_ImperialRebuild)
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
		return "imperial_rebuild";
	}
	static const char* GetFormat()
	{
		return 	"uuuuuuuuu";
	}

	uint32 id;
	uint32 need_item[MAXIMPERIALREBUILDINVESTITEMS];
	uint32 need_item_num[MAXIMPERIALREBUILDINVESTITEMS];
	uint32 need_labor;
	uint32 need_times;
};

struct DB_FunctionOpen
{
	INIT_GAMEDATA(DB_FunctionOpen)
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
		return "function_open";
	}
	static const char* GetFormat()
	{
		return 	"uuu";
	}

	uint32          id;//module id
	uint32          function_id;
	uint32          trigger_id;
};

struct DB_FunctionOpenOrder
{
	INIT_GAMEDATA(DB_FunctionOpenOrder)
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
		return "function_open_order";
	}
	static const char* GetFormat()
	{
		return 	"uu";
	}

	uint32          id;//module id
	uint32          function_id;
};

#pragma pack(pop)