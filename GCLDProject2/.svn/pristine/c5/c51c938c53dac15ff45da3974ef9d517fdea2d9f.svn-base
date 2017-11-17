#pragma once
#include "struct_base.h"
#include "ptr_container/PtrMap.h"
#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Buff
{
    INIT_GAMEDATA(DB_Buff)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "buff"; }
    static const char* GetFormat() {
        return "usuusuufffuuuu";
    }

    uint32          id;
    string          comment;
    uint32          name_id;
    uint32          description_id;
    string          image_name;
    uint32          buff_level;
    uint32          buff_type;
    float           value1;
    float           value2;
    float           value3;
    uint32          last_time;
    uint32          target;
    uint32          is_save;
	uint32          effect_id;
};

struct DB_Build_buff
{
    INIT_GAMEDATA(DB_Build_buff)
        static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "build_buff"; }
    static const char* GetFormat() {
        return
            "uuuuuu";
    }

    uint32          id;
    uint32          build_type;
    uint32          table_type;
    uint32          buff_id;
    uint32          effect_last_days;
    uint32          cost;
};

#pragma pack(pop)


class Build_buffTable {
    static PtrMap<uint32, Build_buffTable>    m_map;

    const DB_Build_buff&       m_dbProto;
public:
    Build_buffTable(const DB_Build_buff& db) : m_dbProto(db) {};
    uint32 GetMoudle() const { return m_dbProto.build_type; }
    uint32 GetBuffId() const { return m_dbProto.buff_id; }
    uint32 GetEffectSecTime() const { return m_dbProto.effect_last_days * 86400; }
    uint32 GetCost() const { return m_dbProto.cost; }

    static void Init();
    static const Build_buffTable* GetTable(uint32 proid);
};

class PlayerBuffTable {
    static PtrMap<uint32, PlayerBuffTable>    m_map;

    const DB_Buff&       m_dbProto;
public:
    enum TargetEnum {
        Player  = 1,
        Char    = 2,
        All     = 3,
    };

    PlayerBuffTable(const DB_Buff& db) : m_dbProto(db) {};
    uint32 GetBuffLv() const { return m_dbProto.buff_level; }
    uint32 GetBuffId() const { return m_dbProto.id; }
    uint32 GetBuffTyp() const { return m_dbProto.buff_type; }
    uint32 GetTarget() const { return m_dbProto.target; }
    uint32 TimeLong() const { return m_dbProto.last_time; }
    bool   IsSave() const { return m_dbProto.is_save == 1; }
	float GetBuffValue1() const { return m_dbProto.value1; }
	float GetBuffValue2() const { return m_dbProto.value2; }
    float GetBuffValue3() const { return m_dbProto.value3; }
    static void Init();
    static const PlayerBuffTable* GetTable(uint32 proid);
};