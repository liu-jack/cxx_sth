#include "QuestManager.h"
#include "DbTool.h"
#include "def/MmoAssert.h"
#include "QuestDef.h"
#include "datastruct/struct_quest.h"
#include "QuestProto.h"
#include "Logger.h"



void QuestManager::Init()
{
    FOREACH_DB_ITEM( DBproto, DB_QuestProto)
    {
        uint32 protoID = DBproto->Id;
        QuestProto * proto = new QuestProto(*DBproto);
        m_questProtoMap[ protoID] = proto;
    }

    FOREACH_DB_ITEM( DBreward, DB_QuestReward)
    {
        uint32 questId = DBreward->QuestId;
                
        QuestProto* proto = const_cast<QuestProto*>(GetQuestProto(questId));
        if (proto)
        {
            proto->m_dbRewards.push_back(DBreward);
        }
    }

    for (QuestProtoMap::iterator iter = m_questProtoMap.begin();
        iter != m_questProtoMap.end(); ++iter)
    {
        QuestProto * proto = iter->second;
        m_behaviorSet.insert( proto->GetBehaviorType());

        if ( proto->NextQuest() != 0)
        {
            MMO_ASSERT( !proto->IsRepeatable() && "Repeatable quest has next quest ?? ");
            QuestProto * next_quest = m_questProtoMap.pfind( proto->NextQuest());
            if ( next_quest != NULL)
            {
                next_quest->SetHasPreQuest(true);
            }
        }

        if (proto->IsRepeatable())
        {
            m_repeatableQuests.insert( proto->ID());
        }
    }

    for (QuestProtoMap::iterator iter = m_questProtoMap.begin();
        iter != m_questProtoMap.end(); ++iter)
    {
        QuestProto * proto = iter->second;
        if ( !proto->HasPreQuest())
        {
			if(proto->IsMainQuest())
			{
				m_firstMainQuests.insert( proto->ID());
			}
			else if(proto->IsBranchQuest())
			{
				m_firstBranchQuests.insert(proto->ID());
			}
        }
    }


    FOREACH_DB_ITEM( DBQuestCounter, DB_QuestCounter)
    {
        uint32 counterType 
            = DBQuestCounter->category_3 
            | DBQuestCounter->category_2 << 8
            | DBQuestCounter->category_1 << 16
            | DBQuestCounter->event_type << 24;
        m_counterTypeSet.insert( counterType);
    }
}

const QuestProto* QuestManager::GetQuestProto( QuestID protoID ) const
{
    return m_questProtoMap.pfind(protoID);
}

bool QuestManager::IsBehaviorHaveCounter( const BehaviorType& behavior )
{
    // 两者均有记录说明需要保存。
    bool hasQuest = m_behaviorSet.find( behavior) !=  m_behaviorSet.end();  // 任务设计表中有记录
    bool hasCounter = m_counterTypeSet.find( behavior.m_counterType ) != m_counterTypeSet.end();  // DB_QuestCounter 表中有该记录
    return hasQuest && hasCounter;
}

const QuestIDSet& QuestManager::GetFirstMainQuests() const
{
    return m_firstMainQuests;
}

const QuestIDSet& QuestManager::GetRepeatableQuests() const
{
    return m_repeatableQuests;
}

const QuestIDSet& QuestManager::GetFirstBranchQuests() const
{
	return m_firstBranchQuests;
}

const QuestID QuestManager::GetMaxQuestId() const
{
	QuestID max_quest_id = 0;
	QuestProtoMap::const_iterator iter = m_questProtoMap.begin();
	for (;iter != m_questProtoMap.end(); ++iter){
		if(iter->second->NextQuest() > max_quest_id)
			max_quest_id = iter->second->NextQuest();
	}
	return max_quest_id;
}

const QuestProtoMap& QuestManager::GetQuestProtoMap() const
{
	return m_questProtoMap;
}

const QuestID QuestManager::FindNearestId( const uint32 FromId ) const
{
	QuestProto* quest_proto = m_questProtoMap.pfind(FromId);
	if(quest_proto) 
		return quest_proto->ID();
	else
	{
		QuestProtoMap::const_iterator iter = m_questProtoMap.begin();
		for(;iter != m_questProtoMap.end();++iter)
		{
			QuestProtoMap::const_iterator old_iter = iter;
			QuestProtoMap::const_iterator new_iter = ++old_iter;
			if(new_iter != m_questProtoMap.end())
			{
				if(iter->first < FromId && new_iter->first > FromId)
					return iter->first;
			}
			else
				return iter->first;
		}
		return iter->first;
	}
}