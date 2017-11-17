#pragma once
#include "structDef.h"

namespace pb{
    class GS2C_Altar_All_Info;
}

//set byte alignment 
#pragma pack(push,1)

struct AltarTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(AltarTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "altar"; }
    static const char* GetFormat()
    {
        return "bbcu"
            "ccubbc"
            "ccubbc"
            "ccubbc"
            "ccubbc"
            "ccubbc";
    }
    void SaveTo(pb::GS2C_Altar_All_Info& msg);
    void LoadFrom(const pb::GS2C_Altar_All_Info& msg);

    struct SubAltar
    {
        uint8   is_building_up;
        uint8   have_reward;
        uint32  study_times;
        uint64  last_study_time;
        uint64  take_free_time;
        uint8   reward_count;
    };
public:
    IdType      id;
    uint64      player_id;
    uint8       level;
    uint32      silver_coin_for_build;

    SubAltar    sub_altar_lst[5];
};


#pragma pack(pop)
