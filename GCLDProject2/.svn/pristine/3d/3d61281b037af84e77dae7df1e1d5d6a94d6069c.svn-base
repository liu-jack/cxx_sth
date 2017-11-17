#include "struct_panzer.h"
#include "DbTool.h"

PtrMap<uint32, PanzerTable> PanzerTable::m_map;

void PanzerTable::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_Panzer)
    {
        m_map[ptr1->id] = new PanzerTable(*ptr1);
    }
}
const PanzerTable* PanzerTable::GetTable(uint32 proId)
{
    return m_map.pfind(proId);
}