#include "QuestProto.h"
#include "datastruct/struct_quest.h"
#include "utility/Utility.h"

QuestProto::QuestProto(  const DB_QuestProto& db )
:   m_dbQuestProto( db)
,   m_behaviorType( db.objective_category_1,
                   db.objective_category_2,
                   db.objective_category_3,
                   db.objective_condition_1,
                   db.IsRepeatable() ? EVENT_TYPE_DAILY : EVENT_TYPE_NORMAL)
,   m_hasPreQuest( false)
{

}

const BehaviorType& QuestProto::GetBehaviorType() const
{
    return m_behaviorType;
}

uint32 QuestProto::TargetValue() const
{
    return m_dbQuestProto.objective_value;
}

uint32 QuestProto::QuestType() const
{
    return m_dbQuestProto.quest_type;
}

uint32 QuestProto::ID() const
{
    return m_dbQuestProto.Id;
}

uint32 QuestProto::NextQuest() const
{
    return m_dbQuestProto.quest_next;
}

bool QuestProto::IsRepeatable() const
{
    return m_dbQuestProto.IsRepeatable();
}

bool QuestProto::IsSequentQuest() const
{
    return m_dbQuestProto.IsSequentQuest();
}

bool QuestProto::IsBranchQuest() const
{
	return m_dbQuestProto.IsQuestBranch();
}

bool QuestProto::IsMainQuest() const
{
	return m_dbQuestProto.IsMainQuest();
}

bool QuestProto::HasPreQuest() const
{
    return m_hasPreQuest;
}

void QuestProto::SetHasPreQuest(bool value)
{
	if(m_hasPreQuest != value)
		m_hasPreQuest = value;
}

const RewardList& QuestProto::GetRewardList() const
{
    return m_dbRewards;
}

bool QuestProto::CheckCategory(uint32 category1,uint32 category2,uint32 category3) const
{
	return m_behaviorType.m_cat1 == category1 && m_behaviorType.m_cat2 == category2 && m_behaviorType.m_cat3 == category3;
}

bool QuestProto::MatchVipRequire( uint32 vipLv ) const
{
    return vipLv >= m_dbQuestProto.quest_unlock_vip;
}

bool QuestProto::MatchLevelRequire( uint32 level ) const
{
    return level >= m_dbQuestProto.quest_unlock_lv;
}
