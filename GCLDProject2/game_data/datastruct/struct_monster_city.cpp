#include "struct_monster_city.h"
#include "utility/Utility.h"
#include "DbTool.h"

PtrMap<uint32, Table_MonsterCity> Table_MonsterCity::m_map;

Table_MonsterCity::Table_MonsterCity(const DB_MonsterCity& db)
    : m_db(db)
{
    for (int i = 0; i < ARRAY_SIZE(db.city_lst); ++i)
    {
        Utility::SplitStr(db.city_lst[i], city_lst[i], ',');
    }
}
void Table_MonsterCity::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_MonsterCity)
    {
        m_map[ptr1->moster_city_level] = new Table_MonsterCity(*ptr1);
    }
}
const Table_MonsterCity* Table_MonsterCity::GetTable(uint32 monster_city_level)
{
    return m_map.pfind(monster_city_level);
}