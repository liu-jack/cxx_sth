#include "PlayerBehaviorCounter.h"

#include "../QuestManager.h"

#include "BehaviorCounterBase.h"
#include "DailyBehaviorCounter.h"
#include "NormalBehaviorCounter.h"

#include "server_client/DbProxyServerClient.h"
#include "Quest.pb.h"
#include "ServerOpcode.pb.h"
#include "utility/MsgTool.h"




PlayerBehaviorCounter::PlayerBehaviorCounter( QuestLogger* questLogger)
: m_pQuestlogger( questLogger)
{

}

void PlayerBehaviorCounter::TriggerBehavior(Player* pPlayer, uint32 category1, uint32 category2, uint32 category3, uint32 condition, int count /*= 1*/)
{
    BehaviorType behavior(category1, category2, category3, condition); 
    
    behavior.m_eventType = EVENT_TYPE_DAILY;
    if ( TriggerBehavior(pPlayer, behavior,count))
    {
        NotifyBehaviorTriggered(pPlayer,behavior);
    }

    behavior.m_eventType = EVENT_TYPE_NORMAL;
    if ( TriggerBehavior(pPlayer, behavior, count))
    {
        NotifyBehaviorTriggered(pPlayer,behavior);
    } 
}

bool PlayerBehaviorCounter::GetCount( const BehaviorType& behavior, uint32& outCount) const
{
    outCount = 0;
    if ( IsBehaviorHaveCounter(behavior))
    {
        BHV_Records::const_iterator iter = m_records.find( behavior.GetUint64());
        if (iter !=  m_records.end())
        {
            if (BehaviorCounterBase* counter = iter->second)
            {
                outCount = counter->GetCount();
				return true;
			}
        }
    }
    return false;
}

bool PlayerBehaviorCounter::TriggerBehavior(Player* pPlayer,  const BehaviorType &behavior , int count/* = 1*/)
{
    if ( !IsBehaviorHaveCounter(behavior))
    {
        return false;
    }    

    BehaviorCounterBase* counter = NULL;
    BHV_Records::iterator it = m_records.find( behavior.GetUint64());
    if ( it == m_records.end())
    {
        counter = CreateCounter(behavior);
        if (counter != NULL)
        {
            m_records[behavior.GetUint64()] = counter;
        }
    }
    else
    {
        counter = it->second;
    }

    bool result = false;
    if ( counter != NULL)
    {
        result = counter->TrggerBehavior( behavior, count);
        if ( result )
        {
            pb::GxDB_Behavior msg;
            counter->SaveTo( msg);
            sDbProxyClient.SendPlayerPack( *pPlayer, pb::SG2D_UPDATE_BEHAVIOR_COUNTER, msg);
        }           
    }
    return result;
}

BehaviorCounterBase* PlayerBehaviorCounter::CreateCounter( const BehaviorType &behavior  )
{
    if ( behavior.m_eventType == EVENT_TYPE_DAILY)
    {
        return new DailyBehaviorCounter(behavior);
    }
    else if ( behavior.m_eventType == EVENT_TYPE_NORMAL)
    {
        return new NormalBehaviorCounter(behavior);
    }
    return NULL;
}

void PlayerBehaviorCounter::NotifyBehaviorTriggered(Player*player, const BehaviorType &behavior )
{
    if ( m_pQuestlogger)
    {
         m_pQuestlogger->UpdateQuest(player, behavior);                                           
    }
}

bool PlayerBehaviorCounter::IsBehaviorHaveCounter(const BehaviorType& behavior)
{
    return sQuestMgr.IsBehaviorHaveCounter(behavior);
}



void PlayerBehaviorCounter::LoadFrom( const pb::GxDB_BehaviorList& bListMsg )
{
    for ( int i = 0; i < bListMsg.behavior_list_size(); ++i)
    {
        const pb::GxDB_Behavior& behaviorMsg = bListMsg.behavior_list(i);
        BehaviorCounterBase* counter = CreateCounter( BehaviorType(behaviorMsg.behavior()) );
        if (counter)
        {
            m_records[ behaviorMsg.behavior()] = counter;
            counter->LoadFrom(behaviorMsg);
        }
    }
}

void PlayerBehaviorCounter::SaveTo( pb::GxDB_BehaviorList& bListMsg ) const
{
    for ( BHV_Records::const_iterator it = m_records.begin(); it != m_records.end(); ++it)
    {
        MsgTool::SaveToRepeatField( it->second, bListMsg.mutable_behavior_list());
    }
}



