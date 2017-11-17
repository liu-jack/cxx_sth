#include "DungeonTable.h"
#include "InterServer.pb.h"
#include "Dungeon.pb.h"

void DungeonTable::SaveTo(pb::GS2C_Dungeon_Base& msg)
{
    msg.set_dngeon_levle(m_DungeonLv);
    msg.set_silver_for_build(m_SilverCoinForBuild);
    msg.set_cane_levle(m_CaneLv);
    msg.set_cane_exp(m_CaneExp);
    msg.set_free_try_cane(m_freeTryCane);
    msg.set_cane_temp_levle(m_TempCaneLv);
    msg.set_cane_temp_time(m_TempCaneLvTime);
    msg.set_today_catch_hero_cnt(m_TodayCatchHeroCnt);
    msg.set_catch_hero_cnt_for_exp(m_CatchHeroCntForExp);
    msg.set_catch_hero_time(m_CatchHeroTime);
}
void DungeonTable::LoadFrom(const pb::GS2C_Dungeon_Base& msg)
{
    m_CaneLv = msg.cane_levle();
    m_CaneExp = msg.cane_exp();
    m_freeTryCane = msg.free_try_cane();
    m_TempCaneLv = msg.cane_temp_levle();
    m_TempCaneLvTime = msg.cane_temp_time();
    m_CatchHeroTime = msg.catch_hero_time();
    m_TodayCatchHeroCnt = msg.today_catch_hero_cnt();
    m_CatchHeroCntForExp = msg.catch_hero_cnt_for_exp();
    m_DungeonLv = msg.dngeon_levle();
    m_SilverCoinForBuild = msg.silver_for_build();
}
void DungeonHeroTable::SaveTo(pb::CatchHero& msg)
{
    msg.set_player_id(owner_id);
    msg.set_hero_tid(hero_tid);
    msg.set_is_caned(is_caned);
}
void DungeonHeroTable::LoadFrom(const pb::CatchHero& msg)
{
    owner_id = msg.player_id();
    hero_tid = msg.hero_tid();
    is_caned = msg.is_caned();
}