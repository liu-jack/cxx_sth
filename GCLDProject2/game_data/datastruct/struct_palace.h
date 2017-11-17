#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Palace_Proto
{
    INIT_GAMEDATA(DB_Palace_Proto)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "palace_proto"; }
    static const char* GetFormat()
    {
        return 	
            "usuuu"
            "uuuuuuuuu"
            "uusssusui"
			"i"
            ;
    }
    bool IsLowOffice() const { return aide_num == 0; }

    uint32          id;
    string          comment;
    uint32          post_name_id;
    uint32          palace_id;
    uint32          aide_num;

    uint32          master_add;
    uint32          aide_add;
    uint32          master_iron_add;
    uint32          aide_iron_add;
    uint32          master_draught_reward;
    uint32          master_silver_salary;
    uint32          aide_silver_salary;
    uint32          master_iron_salary;
    uint32          aide_iron_salary;

    uint32          monster_id;
    uint32          monster_level;
	string			zerg_building_image;
	string			terran_building_image;
	string			protoss_building_image;
	uint32			palance_level;
	string			achievement_reward;
	uint32			belong_area;
	int32			build_location_x;
	int32			build_location_y;
};

#pragma pack(pop)