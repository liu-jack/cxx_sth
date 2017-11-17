#include "struct_quest.h"


bool DB_QuestProto::IsRepeatable() const
{
    return quest_type == QUEST_TYPE_DAILY 
        || quest_type == QUEST_TYPE_ACHIEVEMENT_DAILY
        || quest_type == QUEST_TYPE_VIP_DAILY;
}

bool DB_QuestProto::IsSequentQuest() const
{
    return quest_type == QUEST_TYPE_MAIN 
        || quest_type == QUEST_TYPE_ACHIEVEMENT
        || quest_type == QUEST_TYPE_VIP
        || quest_type == QUEST_TYPE_VIP_ACHIEVEMENT
        || quest_type == QUEST_TYPE_BRANCH;
}

bool DB_QuestProto::IsQuestBranch() const
{
	return quest_type == QUEST_TYPE_BRANCH;
}

bool DB_QuestProto::IsMainQuest() const
{
	return quest_type == QUEST_TYPE_MAIN;
}
