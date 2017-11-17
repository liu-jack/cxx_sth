#include "LootFilter.h"

#include "OS.h"
#include "LootList.h"
#include "datastruct/struct_loot.h"
#include "DatabaseMgr.h"
#include "System.h"
#include "LootManager.h"
#include "def/MmoAssert.h"
#include "Logger.h"
#include "utility/TimeParser.h"
#include "FixFakeRandom/FixFakeRandomBitSet.h"

LootFilter::LootFilter(const Loot_Filter* filter)
: m_db(filter)
{
}

LootFilter::~LootFilter(void)
{
}


uint32 LootFilter::FilterId() const
{
    return m_db->Id;
}


void LootFilter::AddFeature(LootFeatureLogic* feature)
{
	MMO_ASSERT(feature);

	if (FindFeature(feature->Id()))
	{
		return; 		
	}
	m_features.push_back(feature);
}

LootFeatureLogic* LootFilter::FindFeature( uint32 featureId )
{
	for (FeatureArray::iterator it =m_features.begin(); it != m_features.end(); ++it)
	{
		LootFeatureLogic* feature = *it;
		if (feature->Id() == featureId)
		{
			return feature;
		}
	}

	return NULL;
}

bool LootFilter::IsMatchRate( int rollCount, int rollSeccessCount, int outerCorrectCount/* = 0*/) const
{
    if (LootBoxType() == (uint32)Loot_Filter::Normal)
    {
        return true;
    }

    int correctCount = RoundCorrectCount(outerCorrectCount);
    int correctMod = RoundCorrectMod();

    if (LootBoxType() == (uint32)Loot_Filter::Correct)
	{
		if (correctCount == 0)
		{
			return false;
		}
        // 已出次数 大于 必出次数
		if ( rollSeccessCount >= correctMod)
		{
			return false;
		}
	
        // 周期剩余次数 等于 剩余必出次数
		if (correctCount - (rollCount + 1) == correctMod - rollSeccessCount)
		{
			return true;
		}
	}
    else if ( LootBoxType() == (uint32)Loot_Filter::FixCorrect)
    {
        if ( sFixFakeRD.IsHitFixCorrect(correctCount, correctMod, rollCount))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int Chance = (int)(m_db->loot_random_rate * (float)RAND_FACTOR);
    int dice = System::Rand(0, RAND_FACTOR);
    if (dice > Chance)
    {
        return false;
    }

	return true;
}

void LootFilter::Fill( LootList& lootList,  int lv, int vipLv,bool isCountLoot,bool is_exploit) const
{
	//NLOG("LootFilter::Fill");
	for (FeatureArray::const_iterator it =m_features.begin(); it != m_features.end(); ++it)
	{
		LootFeatureLogic* feature = *it;
		if (!feature->IsMatchLevel(lv, vipLv))
		{
			continue;
		}
		feature->Fill(lootList,isCountLoot,is_exploit);
	}
}

uint32 LootFilter::Priority() const
{
    // TODO
    return m_db->priority;
}

int LootFilter::RoundCorrectCount( int outerCorrectCount /*= 0*/ ) const
{
    if ( m_db->loot_correct_count != 0)
    {
        return  m_db->loot_correct_count;
    }
    else
    {
        return outerCorrectCount;
    }
}

int LootFilter::RoundCorrectMod() const
{
    return m_db->loot_correct_mod;
}

bool LootFilter::NeedRollRecord() const
{
    if ( LootBoxType() == (uint32)Loot_Filter::Correct 
        || LootBoxType() == (uint32)Loot_Filter::FixCorrect)
    {
        if ( RoundCorrectMod() != 0 )
        {
            return true;
        }
    }
    
    return false;
}

bool LootFilter::IsMatchTime() const
{
    // 日期判断
    time_t nowDateTime =  sOS.GetRealTime() ;
    if( m_db->lootDateEnd != m_db->lootDateBegin )
    {
        if( nowDateTime < m_db->lootDateBegin || nowDateTime >= m_db->lootDateEnd )
        {
            return false ;
        }
    }

    // 时间判断

    if(  m_db->lootTimeEnd != m_db->lootTimeBegin )
    {
        time_t endTime = JustGetTime( m_db->lootTimeEnd ) ;
        time_t nowTime = JustGetTime( nowDateTime ) ;
        time_t begTime = JustGetTime( m_db->lootTimeBegin ) ;
        if( nowTime < begTime || nowTime > endTime )
        {
            return false ;
        }
    }

    return true;
}

uint32 LootFilter::LootBoxType() const
{
    return m_db->loot_option;
}

