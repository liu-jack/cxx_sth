#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_CountryUp
{
    INIT_GAMEDATA(DB_CountryUp)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return id;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "country_up";
    }
    static const char* GetFormat()
    {
        return 	"uuuuuu"
                "uuuu"
                "uuuuu";
    }

    uint32      id;
    uint32      exp;
    uint32      need_donate_times;
    uint32      donate_cost;
    uint32      donate_cd;
    uint32      clear_donate_cd_cost;

    uint32      savage_id;
    uint32      savage_lv;
    uint32      savage_num;
	uint32      need_kill_num;

    uint32      suppress_monster_npc_id;
    uint32      suppress_monster_npc_lv;
    uint32      defence_borderland_npc_id;
    uint32      defence_borderland_npc_lv;
    uint32      wild_max_intimacy;
};
struct DB_CountryGuardUp
{
    INIT_GAMEDATA(DB_CountryGuardUp)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return id;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "country_guard_up";
    }
    static const char* GetFormat()
    {
        return  "uuuuuuuu";
    }

    uint32  id;
    uint32  level;
    uint32  need_exp;
	uint32  country1_fog_id;
	uint32  country2_fog_id;
	uint32  country3_fog_id;
	uint32  expedition_lv;
	uint32  lifeguard_lv;

};
struct DB_CountryQuest
{
    INIT_GAMEDATA(DB_CountryQuest)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return id;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "country_quest";
    }
    static const char* GetFormat()
    {
        return  "usu" "ususs" "uuuu";
    }

    uint32  id;
    string  comment;
    uint32  name_id;

    uint32  weight;
    string  start_time;
    uint32  last_time;
    string  player_quest_id1;
    string  player_quest_id2;

    uint32  npc_id[3];
    uint32  npc_num;
};
struct DB_Attack_Defense_Quest
{
    INIT_GAMEDATA(DB_Attack_Defense_Quest)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "attack_defense_quest"; }
    static const char* GetFormat() {
        return
            "usus";
    }

    uint32          id;
    string          comment;
    uint32          name_id;
    string          rank;
};

struct DB_Country_Suppress
{
    INIT_GAMEDATA(DB_Country_Suppress)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "country_repress_wild"; }
    static const char* GetFormat() {
        return
            "uusssuuu";
    }

    uint32      id;
    uint32      stage;
    string      target_cities[3];
    uint32      last_time;
    uint32      reward_country_exp;
    uint32      npc_num;
};
struct DB_Country_DefenceBorderland
{
    INIT_GAMEDATA(DB_Country_DefenceBorderland)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "country_defense_frontier"; }
    static const char* GetFormat() {
        return
            "uusssuuuu";
    }

    uint32      id;
    uint32      stage;
    string      target_cities[3];
    uint32      last_time;
    uint32      reward_country_exp;
    uint32      npc_num;
	uint32		defence_num;
};

#pragma pack(pop)