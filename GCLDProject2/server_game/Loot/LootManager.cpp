#include "memory_buffer/NetPack.h"


#include "LootManager.h"
#include "LootFeatureLogic.h"
#include "LootFilter.h"
#include "LootFilterGroup.h"
#include "LootList.h"
#include "ILootPackage.h"

#include "DatabaseMgr.h"
#include "DbTool.h"
#include "def/MmoAssert.h"
#include "Logger.h"
#include "common/System.h"
#include "utility/TimeParser.h"

LootManager::LootManager(void)
{
}

LootManager::~LootManager(void)
{
}

void LootManager::Init()
{
	LoadDb_LootFeatures();
	LoadDb_LootFeatureItems();
	LoadDb_LootRelations();
}

void LootManager::LoadDb_LootFeatures()
{
	FOREACH_DB_ITEM(pFeature, LootFeature)
	{
		if (!LootFeatureLogic::IsValid(pFeature))
		{
			ELOG ("Invalid data in '%s' id:%d", LootFeature::GetTableName(), pFeature->id);
			continue;
		}
		LootFeatureLogic* logicFeature = new LootFeatureLogic(pFeature);
		m_features[pFeature->id] = logicFeature;
		AddFilterFeature(logicFeature);
	}
}

LootFilter* LootManager::InnerFindLootFilter( int filterId ) const
{
	FilterLootMap::const_iterator it = m_filters.find(filterId);
	if (it == m_filters.end())
	{
		return NULL;
	}

	return it->second;
}

void LootManager::AddFilterFeature( LootFeatureLogic* feature)
{
	int filterId = feature->FilterId();
	LootFilter* filter = InnerFindLootFilter(filterId);

	if (filter == NULL)
	{
		const Loot_Filter* dbFilter = DbTool::Get<Loot_Filter>(filterId);
		if (dbFilter == NULL)
		{
			ELOG ("Invalid filter id in '%s' id:%d", LootFeature::GetTableName(), feature->Id());
			return;
		}
		filter = new LootFilter(dbFilter);
		m_filters[filterId] = filter;
        
	}
	
	filter->AddFeature(feature);
}

void LootManager::LoadDb_LootFeatureItems()
{
	FOREACH_DB_ITEM(pFeatureItem, LootFeatureItems)
	{
		if (!LootFeatureItemLogic::IsValid(pFeatureItem))
		{
			ELOG ("Invalid data in '%s' id:%d", LootFeatureItems::GetTableName(), pFeatureItem->id);
			continue;
		}

		FeatureMap::iterator featureIt = m_features.find(pFeatureItem->feature_id);
		if (featureIt == m_features.end())
		{
			continue;
		}            
		featureIt->second->AddItem(pFeatureItem);
	}

    for ( FeatureMap::iterator it = m_features.begin(); it != m_features.end(); ++it)
    {
        LootFeatureLogic* feature = it->second;
        feature->CalculateItemRate();
    }


}

void LootManager::LoadDb_LootRelations()
{
	FOREACH_DB_ITEM(pRelation, LootProto)
	{
		int type = pRelation->lootType;
		int entry = pRelation->victimID; 
		if (type <= LOOTTYPE_NONE || type >= LOOTTYPE_COUNT || entry < 0 )
		{
			ELOG ("Invalid type in 'loot' id:%d", pRelation->Id);
			continue;
		}

		uint32 filterId = pRelation->lootFilterID;
		LootFilter* filter = InnerFindLootFilter(filterId);
		if (filter == NULL)
		{
			ELOG ("Not found filter id in 'loot' id:%d", pRelation->Id);
			continue;
		}

		AddFeature(type, entry, filter);

        if ( type == LOOTTYPE_LOTTERY && filter->LootBoxType() == Loot_Filter::FixCorrect )
        {
            MMO_ASSERT(m_lotteryId2Filter.find( entry) == m_lotteryId2Filter.end());
            m_lotteryId2Filter[entry] = filter->FilterId();
        }
	}

    for ( int i = 0 ; i < LOOTTYPE_COUNT; ++i )
    {
        EntryIdLootMap& id2group = m_loots[i];
        for ( EntryIdLootMap::iterator it = id2group.begin(); it != id2group.end(); ++it)
        {
            LootFilterGroup* filtergroup = it->second;
            filtergroup->InitFiltersPriority();
        }
    }
}

void LootManager::AddFeature( int type, int entry, LootFilter* filter )
{
	//NLOG(" addfeature type = %d",type);
	LootFilterGroup* pGroup = NULL;
	EntryIdLootMap::iterator it = m_loots[type].find(entry);

	if (it == m_loots[type].end())
	{
		//NLOG("type = %d",type);
		pGroup = new LootFilterGroup();
		m_loots[type][entry] = pGroup;
	}
	else
	{
		pGroup = it->second;
	}

	pGroup->AddFilter(filter);
}

LootFilterGroup* LootManager::FindLootFilterGroup( int type, int entry ) const
{
	MMO_ASSERT (type > LOOTTYPE_NONE && type < LOOTTYPE_COUNT );

	//NLOG("LootManager::FindLootFilterGroup type=%d,id=%d",type,entry);
	const EntryIdLootMap& lootMap = m_loots[type];
	EntryIdLootMap::const_iterator it = lootMap.find(entry);
	if (it == lootMap.end())
	{
		return NULL;
	}

	return it->second;
}

bool LootManager::HasLoot( int giverType, int entry ) const
{
	LootFilterGroup* pGroup = FindLootFilterGroup(giverType, entry);
	return pGroup != NULL;
}

const LootFilter* LootManager::FindLootFilter( int filterId ) const
{
	return InnerFindLootFilter(filterId);
}

bool LootManager::FillLootList(Player* player, LootParam& lootParam, LootList& lootList ,bool is_exploit) const
{
	//NLOG("LootManager::FillLootList");
    LootFilterGroup* pGroup = FindLootFilterGroup( lootParam.victim_type, lootParam.victim_id);	//根据道具id和类型
    if (pGroup)
    {
        pGroup->FillLoot(player,&lootList, lootParam,is_exploit);
        return true;
    }
    else
    {
        return false;
    }
}




bool LootManager::NeedRollRecord( int filterId ) const
{
    const LootFilter* lootfilter = FindLootFilter(filterId);
    if ( lootfilter == NULL)
        return false;
    return lootfilter->NeedRollRecord();
}

uint32 LootManager::GetLotteryFixCorrectFilter( uint32 lotteryId ) const
{
    LotteryId2FixCorrectFilter::const_iterator it = m_lotteryId2Filter.find( lotteryId ) ;
	if ( it != m_lotteryId2Filter.end() )
	{
		return it->second;
	}

    return 0;
}


