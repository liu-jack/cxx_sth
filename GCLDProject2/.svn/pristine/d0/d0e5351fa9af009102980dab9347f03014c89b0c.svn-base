#ifndef IQuestLogger_h__
#define IQuestLogger_h__
#include "QuestDef.h"

#if 0
namespace pb{
    class GxDB_Quest;
    class GS2C_QuestList;
    class GS2C_LootList;
}
class Player;
class IQuestLogger
{
public:
    virtual ~IQuestLogger(){};
    virtual void AfterLoadLogger(Player* player) = 0;
    
    virtual void UpdateQuest(Player*player,  const BehaviorType& behavior) = 0;
    virtual void UpdateQuest(Player*player,  uint32 category1, uint32 category2, uint32 category3 = 0, uint32 condition = 0) = 0;
    virtual void UpdateQuestCategory(Player*player,  uint32 categroy1) = 0;
    virtual void UpdateQuestCategory(Player*player,  uint32 categroy1, uint32 category2) = 0;

    virtual bool TryAddQuest(Player*player, uint32 questId) = 0;
    virtual int TakeQuestReward(Player* player, uint32 questId , pb::GS2C_LootList& msg,uint32 &player_exp) = 0;
    virtual void Load(Player*player, const pb::GS2C_QuestList& questList) = 0 ;
    virtual void Save(Player*player, pb::GS2C_QuestList& questList) = 0;

    virtual void HandleGM(Player*player, QUEST_GM_CMD gmCmd, int param) = 0;

	virtual bool GetQuestFinished(QuestID id) = 0;
	virtual bool CheckCategory(uint32 category1,uint32 category2,uint32 category3) = 0;
	virtual bool    HasQuest( QuestID questId) const  = 0;
	virtual int GetQuestRewards(Player* player, uint32 questId , pb::GS2C_LootList& msg,uint32 &player_exp) = 0;
};
#endif
#endif // IQuestLogger_h__



