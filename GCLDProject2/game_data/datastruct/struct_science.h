#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Science
{
    INIT_GAMEDATA(DB_Science)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "science_proto"; }
    static const char* GetFormat() {
        return
            "usuuuu"
            "suss"
            "uuuu";
    }

    uint32          id;
    string          comment;
	uint32			type_id;
    uint32          name_id;
    uint32          description_id;
	uint32          unlock_description_id;

    string          unlock_trigger_id;
    uint32          invest_need_star;
    string          every_star_cost;
    string          attr_reward;

    uint32          study_need_time;
    uint32          value1;
    uint32          value2;
    uint32          value3;
};


#pragma pack(pop)