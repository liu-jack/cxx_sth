#include <memory>
#include "LootFilterGroup.h"
#include "LootFilter.h"
#include "LootList.h"
#include "Logger.h"
#include "object/Player.h"
#include "ILootRoundRecordProvider.h"



LootFilterGroup::LootFilterGroup()
: m_isPriorityMod( false)
{

}


void LootFilterGroup::AddFilter( LootFilter* filter )
{
	m_filters.push_back(filter);
}

LootList* LootFilterGroup::GenerateLoot( Player* player,LootParam& param ) const
{
    std::auto_ptr<LootList> lootList (new LootList());
    FillLoot(player,lootList.get(), param); 
    if (lootList->IsEmpty())
    {
        return NULL;
    }

    return lootList.release();
}

void LootFilterGroup::GenerateLoot(Player*player, LootParam& param, LootList& lootList ) const
{
    FillLoot(player, &lootList, param);
}

void LootFilterGroup::FillLoot(Player*player, LootList* lootList, LootParam& param ,bool is_exploit) const
{
	//NLOG("LootFilterGroup::FillLoot");
    bool haveEverMatch = false;

    for (LootFilterArray::const_iterator it = m_filters.begin(); it != m_filters.end(); ++it)
    {
		//NLOG("LootFilterGroup::FillLoot round");
        const LootFilter* filter = *it;
        if ( !filter->IsMatchTime())
            continue;

        bool match = false;
        ILootRoundRecord* roundRecord = NULL;

        if ( (IsPriorityMod() && !haveEverMatch) || !IsPriorityMod())
        {
            int rollCount = 0, rollSuccessCount = 0;
            if (param.provider)
            {
                roundRecord = param.provider->GetRoundRecord( param.victim_type, param.victim_id, filter->FilterId());
                if (roundRecord)
                {
                    rollCount = roundRecord->RoundRollCount();
                    rollSuccessCount = roundRecord->RoundRollSuccessCount();
                }
            }        
            match = filter->IsMatchRate( rollCount, rollSuccessCount, param.loot_correct_count);
			//NLOG("LootFilterGroup::FillLoot rollCount:%d rollSuccessCount:%d loot_correct_count:%d",rollCount,rollSuccessCount,param.loot_correct_count);
        }

        if ( match)
        {
            filter->Fill( *lootList, param.victim_lv, param.player_vip_lv,param.is_count_loot,is_exploit);
            haveEverMatch = match;
			//match = false;
        }

        if ( param.provider)
        {
            param.provider->AddRoundRecord( player,param.victim_type, param.victim_id, filter->FilterId(),
                match, filter->RoundCorrectMod(), filter->RoundCorrectCount());
        }
    }
}

bool LootFilterGroup::IsPriorityMod() const
{
    return m_isPriorityMod;
}

namespace{
    bool CompareFilter( const LootFilter* lhs, const LootFilter* rhs)
    {
        return lhs->Priority() < rhs->Priority();
    }
}

void LootFilterGroup::InitFiltersPriority()
{
    LootFilterArray::size_type size = m_filters.size();
    if ( size == 0 || size == 1)
    {
        return;
    }
    for ( LootFilterArray::size_type i = 0; i < size - 1; ++i)
    {
        if ( (m_filters[i]->Priority()) != (m_filters[i+1]->Priority()))
        {
            m_isPriorityMod = true;
            break;
        }
    }
    if ( IsPriorityMod())
    {
        std::sort( m_filters.begin(), m_filters.end(), CompareFilter);
    }
}

