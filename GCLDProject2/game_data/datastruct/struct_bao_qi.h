#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_BaoQi
{
    INIT_GAMEDATA(DB_BaoQi)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "baoqi_up"; }
    static const char* GetFormat() {
        return
            "uu"
            "uuuus"
            "uuuus"
            "uuuus"
            "uuuus"
            "uuuus"
            "uuuus";
    }
    struct Property
    {
        uint32          attr_id;
        uint32          attr_value;
        uint32          up_cost;
        uint32          click_value;
        string          critical_weight;
    };

    uint32          id;
    uint32          baoqi_level;
    Property        pro_lst[6];
};
struct DB_BaoQi_Unlock
{
    INIT_GAMEDATA(DB_BaoQi_Unlock)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "baoqi_unlock"; }
    static const char* GetFormat() {
        return
            "uuusu"
            "uuuuuuuuu";
    }

    uint32          id;
    uint32          baoqi_id;
    uint32          name_id;
    string          image_name;
    uint32          unlock_trigger;

    uint32          creat_need_item1;
    uint32          item1_num;
    uint32          buy_item1_cost;
    uint32          creat_need_item2;
    uint32          item2_num;
    uint32          creat_need_item3;
    uint32          item3_num;
    uint32          punching_cost_iron;
	uint32			punching_cost_item;
};
struct DB_BaoQi_UpStar
{
    INIT_GAMEDATA(DB_BaoQi_UpStar)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "baoqi_upstar"; }
    static const char* GetFormat() {
        return
            "uu"
            "uuuuuu"
            "u";
    }

    uint32          id;
    uint32          star_need_level;
    uint32          baoqi_value[6];
    uint32          star_tips_id;
};


#pragma pack(pop)