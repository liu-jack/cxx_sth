#pragma once
#include "struct_base.h"
#include "ptr_container/PtrMap.h"

#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_SoldierFood
{
    INIT_GAMEDATA(DB_SoldierFood)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
        return level;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "soldier_food";
	}
	static const char* GetFormat()
	{
		return "uf";
	}

    uint32   level;
    float   need_food;
};

#pragma pack(pop)

class SoldierFoodTable {
    static PtrMap<uint32, SoldierFoodTable>    m_map;

    const DB_SoldierFood&       m_dbProto;
public:
    SoldierFoodTable(const DB_SoldierFood& db) : m_dbProto(db) {};
    float NeedFood() const { return m_dbProto.need_food; }

    static void Init();
    static const SoldierFoodTable* GetTableByLv(uint32 level);
};
