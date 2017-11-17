#include "BaoQiTableMgr.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "System.h"

static const int Critical_Multi[4] = {1, 2, 4, 10};

BaoQiTable::BaoQiTable(const DB_BaoQi& db) : m_db(db)
{
    std::vector<int> vecWeight;
    for (int i = 0; i < BaoQi_MaxCnt; ++i)
    {
		vecWeight.clear();
        Utility::SplitStr(m_db.pro_lst[i].critical_weight, vecWeight, ',');

        MMO_ASSERT(vecWeight.size() == ARRAY_SIZE(Critical_Multi));

        for (size_t j = 0; j < ARRAY_SIZE(Critical_Multi); ++j)
        {
			m_CriticalWeight[i].push_back(std::make_pair(Critical_Multi[j], vecWeight[j]));
        }
    }
}


void BaoQiTableMgr::Init()
{
    m_BaoQiMaxLv = 0;
    FOREACH_DB_ITEM(ptr1, DB_BaoQi)
    {
        m_BaoQiMap[ptr1->baoqi_level] = new BaoQiTable(*ptr1);

        if (ptr1->baoqi_level > m_BaoQiMaxLv) m_BaoQiMaxLv = ptr1->baoqi_level;
    }
    FOREACH_DB_ITEM(ptr2, DB_BaoQi_Unlock)
    {
        m_UnlockMap[ptr2->baoqi_id] = ptr2;
    }
    FOREACH_DB_ITEM(ptr3, DB_BaoQi_UpStar)
    {
        m_UpStarMap[ptr3->id] = ptr3;
    }
}
const DB_BaoQi::Property* BaoQiTableMgr::GetBaoQi(int id, int lv)
{
    if (id < 0 || id >= BaoQi_MaxCnt) return NULL;

    if (const BaoQiTable* table = m_BaoQiMap.pfind(lv))
    {
        return &table->m_db.pro_lst[id];
    }
    return NULL;
}
const DB_BaoQi_Unlock* BaoQiTableMgr::GetBaoQiUnlock(int id)
{
    return m_UnlockMap.pfind(id);
}
const DB_BaoQi_UpStar* BaoQiTableMgr::GetBaoQiUpStar(int tid)
{
    return m_UpStarMap.pfind(tid);
}
int BaoQiTableMgr::RandCriticaMulti(int id, int lv,int value/* = 0 */)
{
    if (id < 0 || id >= BaoQi_MaxCnt)
    {
        return 0;
    }
    if (const BaoQiTable* table = m_BaoQiMap.pfind(lv))
    {
		IntPairVec pairvalue = table->m_CriticalWeight[id];
		if(value != 0)
		{
			for(IntPairVec::iterator iter = pairvalue.begin();iter != pairvalue.end();++iter)
			{
				iter->second += value;
			}
		}
        return System::RandWeight(pairvalue);
    }
	return 0;
}

