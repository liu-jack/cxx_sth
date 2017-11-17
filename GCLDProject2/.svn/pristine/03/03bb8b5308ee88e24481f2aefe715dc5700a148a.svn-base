#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Country_PK_Rank
{
    INIT_GAMEDATA(DB_Country_PK_Rank)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "country_pk_rank"; }
    static const char* GetFormat() {
        return
            "usus";
    }

    uint32      id;
    string      comment;
    uint32      name_id;
    string      rank;
};

#pragma pack(pop)