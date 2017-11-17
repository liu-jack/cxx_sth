#include "MapFog.h"
#include "datastruct/struct_city.h"
#include "utility/Utility.h"


MapFog::MapFog(const DB_MapFog& db) : m_dbMapFog(db)
{
    Utility::SplitStr(m_dbMapFog.open_list, m_openCity, ',');
}
uint16 MapFog::Id() const { return m_dbMapFog.id; }
uint32 MapFog::CountryId() const { return m_dbMapFog.country; }
uint32 MapFog::BattleCityId() const { return m_dbMapFog.battle_city; }
uint32 MapFog::StageId() const { return m_dbMapFog.stageId; }
uint32 MapFog::ShowLevel() const { return m_dbMapFog.show_lv; }

