#ifndef PlayerBehaviorCounter_h__
#define PlayerBehaviorCounter_h__

#include "def/TypeDef.h"
#include "../QuestDef.h"
#include "../QuestLogger.h"
#include "BehaviorCounterBase.h"

#include "ptr_container/PtrHashMap.h"


union BehaviorType;
class QuestLogger;
class Player;

namespace pb
{
    class GxDB_BehaviorList;
}


class PlayerBehaviorCounter
{
    typedef PtrHashMap< uint64 ,BehaviorCounterBase> BHV_Records;
public:
    static BehaviorCounterBase* CreateCounter(const BehaviorType &behavior);
    static bool IsBehaviorHaveCounter(const BehaviorType& behavior);
public:
    explicit PlayerBehaviorCounter( QuestLogger* questLogger);
    void TriggerBehavior(Player* pPlayer, uint32 category1, uint32 category2, uint32 category3 = 0, uint32 condition = 0, int count = 1);    
    bool GetCount( const BehaviorType& behavior, uint32& outCount) const;

    void LoadFrom( const pb::GxDB_BehaviorList& bListMsg);
    void SaveTo( pb::GxDB_BehaviorList& bListMsg) const;

private:
    bool TriggerBehavior(Player* pPlayer, const BehaviorType &behavior , int count = 1);
    void NotifyBehaviorTriggered( Player* player,const BehaviorType &behavior);

private:
    BHV_Records   m_records;
    QuestLogger* m_pQuestlogger;
};


#endif // PlayerBehaviourCounter_h__
