#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Smelter
{
    INIT_GAMEDATA(DB_Smelter)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "smelter_proto"; }
    static const char* GetFormat() {
        return 	
            "uuus"
            "uuuuuu";
    }

    uint32          id;
    uint32          field;
    uint32          smelter_star;
    string          comment;
    uint32          smelter_id;
    uint32          smelt_times;
    uint32          need_item;
    uint32          need_item_num;
    uint32          get_iron_num;
    uint32          levelup_need_gold;
};

struct DB_Smithy
{
    INIT_GAMEDATA(DB_Smithy)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "smithy_proto"; }
    static const char* GetFormat() {
        return
            "uuuuuuuuuu";
    }

    uint32          id;
    uint32          smithy_level;
    uint32          need_level;
    uint32          open_field;
    uint32          open_smelter_id;
    uint32          levelup_need_item;
    uint32          levelup_need_item_num;
    uint32          levelup_need_candidate_item;
    uint32          levelup_need_candidate_item_num;
	uint32			name_id;
};


#pragma pack(pop)