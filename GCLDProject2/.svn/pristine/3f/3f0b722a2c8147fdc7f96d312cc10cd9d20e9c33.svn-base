#include "AltarTableMgr.h"
#include "DbTool.h"
#include "datastruct/struct_altar.h"
#include "utility/Utility.h"
#include "System.h"


AltarTable::AltarTable(const DB_Altar& db)
    : m_db(db)
{
    Utility::SplitStr2(m_db.reward, m_RewardWeight);
    Utility::SplitStr2(m_db.max_reward, m_MaxRewardWeight);
}
int AltarTable::RandAddSoldierTimes(int level) const
{
    if (level >= (int)sAltarTableMgr.GetMaxAltarLv())
    {
        return System::RandWeight(m_RewardWeight);
    }
    else
    {
        return System::RandWeight(m_MaxRewardWeight);
    }
}

uint32 AltarTable::AltarStudyCd()
{
	return m_db.study_cd;
}
void AltarTableMgr::Init()
{
    m_MaxAltarLv = 0;

    FOREACH_DB_ITEM(ptr1, DB_Altar)
    {
        m_AltarTable[ptr1->altar_level] = new AltarTable(*ptr1);

        if (ptr1->altar_level > m_MaxAltarLv) m_MaxAltarLv = ptr1->altar_level;
    }
}
const AltarTable* AltarTableMgr::GetAltar(int lv)
{
    return m_AltarTable.pfind(lv);
}
