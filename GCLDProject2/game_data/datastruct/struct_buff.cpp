#include "struct_buff.h"
#include "DbTool.h"

PtrMap<uint32, Build_buffTable> Build_buffTable::m_map;
PtrMap<uint32, PlayerBuffTable> PlayerBuffTable::m_map;

void Build_buffTable::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_Build_buff)
    {
        m_map[ptr1->id] = new Build_buffTable(*ptr1);
    }
}
const Build_buffTable* Build_buffTable::GetTable(uint32 proId)
{
    return m_map.pfind(proId);
}

void PlayerBuffTable::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_Buff)
	{
		m_map[ptr1->id] = new PlayerBuffTable(*ptr1);
	}
}
const PlayerBuffTable* PlayerBuffTable::GetTable(uint32 proId)
{
	return m_map.pfind(proId);
}