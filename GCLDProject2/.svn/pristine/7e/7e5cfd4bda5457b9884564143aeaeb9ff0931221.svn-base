#include <list>
#include "LootFeatureLogic.h"
#include "../item/Item.h"
#include "datastruct/struct_loot.h"
#include "datastruct/struct_item.h"
#include "LootList.h"
#include "System.h"
#include "def/MmoAssert.h"
#include "Logger.h"

LootFeatureLogic::LootFeatureLogic( const LootFeature* pFeature )
:m_db(pFeature)
{
}

LootFeatureLogic::~LootFeatureLogic(void)
{
}

uint32 LootFeatureLogic::Id() const
{
	return m_db->id;
}

uint32 LootFeatureLogic::FilterId() const
{
	return m_db->filter_id;
}

void LootFeatureLogic::AddItem( const LootFeatureItems* pFeatureItem )
{
	if (!pFeatureItem || HasItem(pFeatureItem->id))
	{
		return;
	}

	LootFeatureItemLogic* temp = new LootFeatureItemLogic(pFeatureItem);
	m_items[pFeatureItem->id] = temp;
}

bool LootFeatureLogic::HasItem( uint32 itemId ) const
{
	if (m_items.find(itemId) != m_items.end())
	{
		return true;
	}

	return false;
}

bool LootFeatureLogic::IsValid(const LootFeature* pFeature)
{
	return pFeature->filter_id > 0
		&& pFeature->require_lv_min >= 0
		&& pFeature->require_lv_min <= pFeature->require_lv_max
		&& pFeature->require_vip >= 0
		;
}

void LootFeatureLogic::Fill( LootList& lootList,bool isCountLoot,bool is_exploit) const
{
	NLOG("LootFeatureLogic::Fill");
	MMO_ASSERT(IsValid(m_db));
	PickItemBySeparateChance(lootList,isCountLoot,is_exploit); // 按权重出物品， 必出一件
}

void LootFeatureLogic::PickItemBySeparateChance( LootList &lootList,bool isCountLoot ,bool is_exploit) const
{
	//NLOG("LootFeatureLogic::PickItemBySeparateChance lootList.size=%d",lootList.Size());
    float dice = System::Randf(0.0f, 1.0f );
	//NLOG("dice=%f",dice);
    uint32 count = 0;
	for (ItemMap::const_iterator it = m_items.begin(); it!=m_items.end(); ++it)
	{
		const LootFeatureItemLogic* item = it->second;

		/*if (lootList.HasItem(item->Id()))
		{
			continue;
		}*/

		if (item->TryRandomPick(dice))
        {
			if (item->IsMoney())
			{
				if(is_exploit)
				{
					lootList.AddMoney(item->Id(), item->GeneCount());
				}
				else
				{
					lootList.AddMoney(item->MoneyType(), item->GeneCount());
				}
			}
			else
			{
				if (isCountLoot)
				{
					lootList.SetItem(item->Id(),item->GeneCount());
				}
				else
				{
					lootList.AddItem(item->Id(), item->GeneCount());
				}
			}
            ++count;
            break;
		}

	}
    MMO_ASSERT( count > 0);
}

bool LootFeatureLogic::IsMatchLevel( int giverLv, int vipLv) const
{
	if (giverLv <= 0  || vipLv < 0)
	{
		return true;
	}

	bool bMatchlevel = IsMatchRequireMinLv(giverLv) && IsMatchRequireMaxLv(giverLv);
	bool bMatchvip = vipLv >= (int)m_db->require_vip;
	return bMatchlevel && bMatchvip;
}


bool LootFeatureLogic::IsMatchRequireMinLv( int level ) const
{
	if ( m_db->require_lv_min != 0 )
	{
        if ( (int)m_db->require_lv_min > level)
        {
            return false;
        }
	}
	return true;
}

bool LootFeatureLogic::IsMatchRequireMaxLv( int level ) const
{
    if ( m_db->require_lv_min != 0 )
    {
        if ( (int)m_db->require_lv_max < level)
        {
            return false;
        }
    }
    return true;
}

void LootFeatureLogic::CalculateItemRate()
{
    float sumWeight = 0;

	for ( ItemMap::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        LootFeatureItemLogic* featureItem = it->second;
        sumWeight += featureItem->m_dbItem->item_weight;
    }
    MMO_ASSERT( sumWeight > 0);
    if (sumWeight == 0)
    {
        ELOG( "Sum Weight equal to 0 ?! LootFeatureLogic::Id() : %d", Id() );
        return;
    }
    float weight = 0;
    for ( ItemMap::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        LootFeatureItemLogic* featureItem = it->second;
        float weightBefore = weight;
        weight += featureItem->m_dbItem->item_weight;
        LootFeatureItems* m_dbItem = const_cast<LootFeatureItems*>(featureItem->m_dbItem);

        m_dbItem->weight_min_rate = weightBefore / sumWeight;
        m_dbItem->weight_max_rate = weight / sumWeight;
    }

}
