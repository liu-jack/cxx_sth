#include "struct_soldier_food.h"
#include "DbTool.h"

PtrMap<uint32, SoldierFoodTable> SoldierFoodTable::m_map;

void SoldierFoodTable::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_SoldierFood)
    {
        m_map[ptr1->level] = new SoldierFoodTable(*ptr1);
    }
}
const SoldierFoodTable* SoldierFoodTable::GetTableByLv(uint32 level)
{
    return m_map.pfind(level);
}