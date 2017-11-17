#include "LootFeatureItemLogic.h"
#include "datastruct/struct_loot.h"
#include "item/LogicItemProto.h"
#include "item/ItemManager.h"
#include "Logger.h"
#include "common/System.h"

// reviewed by zy

LootFeatureItemLogic::LootFeatureItemLogic(const LootFeatureItems* pItem)
:m_dbItem(pItem)
,m_isMoney(false)
,m_moneyType(0)
{
	const LogicItemProto* proto = sItemMgr.Find(pItem->item_id);
	if (proto)
	{
		m_isMoney = proto->IsCurrency();
		if (m_isMoney)
		{
			m_moneyType = proto->category_3;
		}
	}
}

LootFeatureItemLogic::~LootFeatureItemLogic(void)
{
}

uint32 LootFeatureItemLogic::Id() const
{
	return m_dbItem->item_id;
}

//uint32 LootFeatureItemLogic::Chance() const
//{
//	return (uint32)(m_dbItem->item_weight * (float)RAND_FACTOR);
//}

bool LootFeatureItemLogic::IsValid( const LootFeatureItems* pItem )
{
	return pItem->feature_id >=0
		&& pItem->item_id >0
		&& pItem->item_weight >= 0.01f
		&& pItem->count_min >=0
		&& pItem->count_max >= pItem->count_min
		;
}

uint32 LootFeatureItemLogic::GeneCount() const
{
	return System::Rand(m_dbItem->count_min, m_dbItem->count_max +1);
}

bool LootFeatureItemLogic::TryRandomPick(float curRate) const
{
	//int dice = System::Rand(0, RAND_FACTOR);
	//return dice < (int)Chance();
	if (curRate >= m_dbItem->weight_min_rate && curRate < m_dbItem->weight_max_rate)
	{
		return true;
	}
	return false;
}


