#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_MarketBlack
{
    INIT_GAMEDATA(DB_MarketBlack)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "market_black"; }
    static const char* GetFormat()
    {
        return 	"uuuuuuuu";
    }

    uint32          id;
    uint32          exchange_type;
    uint32          exchange_num;
    uint32          get_type1;
    uint32          get_num1;
    uint32          get_type2;
    uint32          get_num2;
    uint32          add_cd;
};
struct DB_MarketBuy
{
    INIT_GAMEDATA(DB_MarketBuy)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "market_buy"; }
    static const char* GetFormat()
    {
        return 	"uuuuuuuu";
    }

    uint32          id;
    uint32          cost_type;
    uint32          cost_num;
    uint32          reward_type;
    uint32          reward_num;
    uint32          weight;
    uint32          quality;
    uint32          trigger;
};
struct DB_MarketCollect
{
    INIT_GAMEDATA(DB_MarketCollect)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "market_collect"; }
    static const char* GetFormat()
    {
        return 	"usuuuuus";
    }

    uint32          id;
    string          comment;
    uint32          build_module;
    uint32          collect_item_id;
    uint32          collect_value;
    uint32          collect_cost;
    uint32          collect_is_free;
    string          collect_critical; //±©»÷£º²»±©»÷£¬2±¶£¬4±¶£¬10±¶
};
struct DB_MysteryShop //ÉñÃØÉÌÈË
{
    INIT_GAMEDATA(DB_MysteryShop)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "mysteroy_businessman"; }
    static const char* GetFormat()
    {
        return 	"usuuuuuuu";
    }

    uint32          id;
    string          comment;
    uint32          name_id;
    uint32          open_lv;
    uint32          cost_typ;
    uint32          cost_val;
    uint32          cost_need_times;
    uint32          give_typ;
    uint32          give_val;
};

struct DB_ActivityObsidianCollect //Ìú¿óÕ÷ÊÕ
{
	INIT_GAMEDATA(DB_ActivityObsidianCollect)
	static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "activity_obsidian_collect"; }
	static const char* GetFormat()
	{
		return 	"uusuu";
	}

	uint32          id;
	uint32			need_collect_times;
	std::string		reward_list;
	uint32			description_id;
	uint32			is_auto_use;
};

#pragma pack(pop)