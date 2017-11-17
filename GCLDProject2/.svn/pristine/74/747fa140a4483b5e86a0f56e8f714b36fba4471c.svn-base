#include "PlayerData.h"

// PlayerData.cpp 部分实现

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "data/DungeonTable.h"
#include "Dungeon.pb.h"
#include "boost/typeof/typeof.hpp"



bool PlayerData::save_dungeon_record()
{
    m_DungeonTable.SaveMod();
    m_DungeonHeroTable.SaveMod();
    return true;
}
void PlayerData::WritePlayerDungeonInfo(pb::GS2C_Dungeon_All_Info& msg)
{
    if (m_DungeonTable)
    {
        m_DungeonTable->SaveTo(*msg.mutable_base_info());
    }
    for (BOOST_AUTO(it, m_DungeonHeroTable.Begin()); it != m_DungeonHeroTable.End(); ++it)
    {
        MsgTool::SaveToRepeatField(it->second, msg.mutable_catch_hero());
    }
}
static DungeonHeroTable* FindDungeonHero(CachePtrMap< uint64, DungeonHeroTable>& map, const pb::CatchHero& hero)
{
    DungeonHeroTable* pCell = NULL;
    CachePtrMap< uint64, DungeonHeroTable>::Iter it = map.Begin();
    for (; it != map.End(); ++it)
    {
        pCell = it->second;
        if (pCell->hero_tid == hero.hero_tid() && 
            pCell->owner_id == hero.player_id())
        {
            return pCell;
        }
    }
    return NULL;
}
Msg_Realize(SG2D_DUNGEON_BASE_INFO, GS2C_Dungeon_Base)
{
    if (m_DungeonTable)
    {
        m_DungeonTable->LoadFrom(msg);
        SetIsDirty(DIRTY_DUNGEON_INFO);
    }
    else
    {
        DungeonTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_DungeonTable.SetAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_DUNGEON_ADD_HERO, GS2C_Dungeon_Add_Hero)
{
    DungeonHeroTable cell;
    cell.player_id = GetPlayerId();
    cell.LoadFrom(msg.hero());
    m_DungeonHeroTable.AddAndAddToCache(&cell);
}
Msg_Realize(SG2D_DUNGEON_DEL_HERO, GS2C_Dungeon_Del_Hero)
{
    if (DungeonHeroTable* pCell = FindDungeonHero(m_DungeonHeroTable, msg.hero()))
    {
        m_DungeonHeroTable.DeleteEntry(pCell);
    }
}