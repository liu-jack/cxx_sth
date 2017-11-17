#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Altar
{
    INIT_GAMEDATA(DB_Altar)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "altar_proto"; }
    static const char* GetFormat() {
        return 	
            "uuusuuu"
            "ssu"
            "uuuu"
            "uuu";
    }

    uint32          id;
    uint32          altar_level;
    uint32          need_level;
    string          comment;
    uint32          name_id;
    uint32          study_times;
    uint32          study_cost;

    string          reward;
    string          max_reward;
    uint32          study_cd;

    uint32          level_cost_typ;
    uint32          level_cost_num;
    uint32          level_cost_typ2;
    uint32          level_cost_num2;

    uint32          special_effect;
    uint32          building_up_cost;
    uint32          free_add_times;
};


#pragma pack(pop)