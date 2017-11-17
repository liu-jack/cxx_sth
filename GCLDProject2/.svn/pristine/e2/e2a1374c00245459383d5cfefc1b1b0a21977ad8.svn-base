#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Open_Frontier
{
    INIT_GAMEDATA(DB_Open_Frontier)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "country_open_frontier"; }
    static const char* GetFormat() {
        return
            "uuuuu";
    }

    uint32      id;
    uint32      stage;
    uint32      stage_last_time;
    uint32      need_cities;
    uint32      reward_country_exp;
};

#pragma pack(pop)