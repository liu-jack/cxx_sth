#include "DungeonTableMgr.h"
#include "DbTool.h"

void DungeonTableMgr::Init()
{
    m_MaxDungeonLv = 0;

    FOREACH_DB_ITEM(ptr1, DB_Cane)
    {
        m_CaneLst[ptr1->level] = ptr1;
    }
    FOREACH_DB_ITEM(ptr2, DB_Dungeon)
    {
        m_DungeonLst[ptr2->dungeon_level] = ptr2;

        if (ptr2->dungeon_level > m_MaxDungeonLv) {
            m_MaxDungeonLv = ptr2->dungeon_level;
        }
    }
}
const DB_Cane* DungeonTableMgr::GetCane(uint32 lv)
{
    return m_CaneLst.pfind(lv);
}
const DB_Dungeon* DungeonTableMgr::GetDungeon(uint32 lv)
{
    return m_DungeonLst.pfind(lv);
}