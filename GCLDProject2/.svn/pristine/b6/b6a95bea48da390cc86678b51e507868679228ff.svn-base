#pragma once
#include "structDef.h"

namespace pb{
    class GS2C_Dungeon_Base;
    class CatchHero;
}

//set byte alignment 
#pragma pack(push,1)

struct DungeonTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(DungeonTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "dungeon_base"; }
    static const char* GetFormat()
    {
        return "bb"
            "cuccbb"
            "cccu";
    }
    void SaveTo(pb::GS2C_Dungeon_Base& msg);
    void LoadFrom(const pb::GS2C_Dungeon_Base& msg);

public:
    IdType      id;
    uint64      player_id;

    uint8   m_CaneLv;
    uint32  m_CaneExp;
    uint8   m_freeTryCane;
    uint8   m_TempCaneLv;
    uint64  m_TempCaneLvTime;
    uint64  m_CatchHeroTime;

    uint8   m_TodayCatchHeroCnt;  //今天抓了几个
    uint8   m_CatchHeroCntForExp; //连续抓了几个人
    uint8   m_DungeonLv;
    uint32  m_SilverCoinForBuild;
};
struct DungeonHeroTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(DungeonHeroTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "id"; }
    static const char* GetTableName(){ return "dungeon_hero"; }
    static const char* GetFormat()
    {
        return "bbubc";
    }
    void SaveTo(pb::CatchHero& msg);
    void LoadFrom(const pb::CatchHero& msg);

public:
    IdType      id;
    uint64      player_id;
    uint32      hero_tid;
    uint64      owner_id;
    uint8       is_caned;
};


#pragma pack(pop)
