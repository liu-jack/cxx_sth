#include "MapBox.h"
#include "datastruct/struct_city.h"
#include "utility/Utility.h"

MapBox::MapBox(const DB_MapBox& db) : m_dbMapBox(db)
{
    Utility::SplitStr(m_dbMapBox.reward_lst, m_RewardLst);
}
uint32 MapBox::Id() const { return m_dbMapBox.id; }
