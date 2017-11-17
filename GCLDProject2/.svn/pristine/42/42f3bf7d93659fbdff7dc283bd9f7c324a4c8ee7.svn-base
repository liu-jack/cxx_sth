#pragma once
#include "struct_base.h"
#include "ptr_container/PtrMap.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_MonsterCity
{
    INIT_GAMEDATA(DB_MonsterCity)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "invest_wild"; }
    static const char* GetFormat()
    {
        return 	"uuuuuusssu";
    }

    uint32          id;
    uint32          moster_city_level;
    uint32          donation_max;
    uint32          npc_id;
    uint32          npc_lv;
    uint32          npc_num;
    string          city_lst[3];
	uint32			increase_intimacy;
};


#pragma pack(pop)

class Table_MonsterCity {
    static PtrMap<uint32, Table_MonsterCity> m_map;
public:
    const DB_MonsterCity& m_db;
    std::vector<int> city_lst[3];

public:
    Table_MonsterCity(const DB_MonsterCity& db);

    static void Init();
    static const Table_MonsterCity* GetTable(uint32 moster_city_level);
};
#define sMonsterCityTable(friendVal) Table_MonsterCity::GetTable(friendVal)