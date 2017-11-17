#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_CountryInvest
{
    INIT_GAMEDATA(DB_CountryInvest)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "country_invest_quest"; }
    static const char* GetFormat() {
        return
            "uuuuuuuu";
    }

    uint32      id;
    uint32      invest_stage;
    uint32      invest_item;
    uint32      invest_cost;
    uint32      invest_reward;
	uint32      invest_upper;
	uint32      country_exp;
    uint32      stage_last_time;
};

struct DB_CountryInvestTitle
{
    INIT_GAMEDATA(DB_CountryInvestTitle)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "country_invest_title"; }
    static const char* GetFormat() {
        return
            "usuu";
    }

    uint32      id;
    string      comment;
    uint32      name_id;
    uint32      need_invest_amounts;
};

#pragma pack(pop)