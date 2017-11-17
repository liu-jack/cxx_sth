#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Dungeon
{
    INIT_GAMEDATA(DB_Dungeon)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "dungeon_proto"; }
    static const char* GetFormat() {
        return "uuuuuuus";
    }
    uint32          id;
    uint32          dungeon_level;
    uint32          need_level;
    uint32          up_cost;
    uint32          second_cost_id;
    uint32          second_cost_num;
    uint32          special_ability;
    string          comment;
};
struct DB_Cane
{
    INIT_GAMEDATA(DB_Cane)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "cane_proto"; }
    static const char* GetFormat() {
        return "uuuuuuuuu";
    }
    uint32          id;
    uint32          level;
    uint32          base_exp;
    uint32          exp_add;
    uint32          escape_time;
    uint32          need_exp;
    uint32          need_people;
    uint32          need_gold;
    uint32          try_gold;
};

#pragma pack(pop)