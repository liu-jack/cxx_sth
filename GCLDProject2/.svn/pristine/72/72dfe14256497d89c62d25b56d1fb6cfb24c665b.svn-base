#pragma once
#include "struct_base.h"
#include "ptr_container/PtrMap.h"
#pragma pack(push,1)

//Notice£ºÔÚActiveDbTables.hÖÐ×¢²á

struct DB_Panzer
{
    INIT_GAMEDATA(DB_Panzer)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "panzer"; }
    static const char* GetFormat() {
        return
            "usuuuu";
    }

    uint32          id;
    string          comment;
	uint32          npc_id;
	uint32          description_id;
	uint32          buy_cost;
	uint32          trigger_id;
};

#pragma pack(pop)

class PanzerTable {
	static PtrMap<uint32, PanzerTable>    m_map;

	const DB_Panzer&       m_dbProto;
public:
	PanzerTable(const DB_Panzer& db) : m_dbProto(db) {};
	uint32 BuyCost() const { return m_dbProto.buy_cost; }
	uint32 TrigerId() const { return m_dbProto.trigger_id; }
	uint32 NpcId() const { return m_dbProto.npc_id; }

	static void Init();
	static const PanzerTable* GetTable(uint32 proId);
};