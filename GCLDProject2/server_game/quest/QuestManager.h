#ifndef QuestManager_h__
#define QuestManager_h__

#include "def/TypeDef.h"
#include "QuestDef.h"
#include "SimpleSingleton.h"
#include "QuestProto.h"
#include "ptr_container/PtrMap.h"

class QuestProto;
union BehaviorType;

typedef PtrMap< QuestID, QuestProto> QuestProtoMap;
class QuestManager : public SimpleSingleton<QuestManager>
{
    friend class SimpleSingleton< QuestManager>;
	typedef std::set< BehaviorType > BehaviorSet;
	typedef std::set< uint32 > CounterTypeSet;
public:
    ~QuestManager(){}
    void Init();
    const QuestProto* GetQuestProto( QuestID protoID) const; 
    bool IsBehaviorHaveCounter( const BehaviorType& behavior);
    const QuestIDSet& GetFirstMainQuests() const;
    const QuestIDSet& GetRepeatableQuests() const;
	const QuestIDSet& GetFirstBranchQuests() const;
	const QuestID GetMaxQuestId() const;
	const QuestProtoMap& GetQuestProtoMap() const;
	const QuestID FindNearestId(const uint32 FromId) const;///向下取第一个存在的Id
private:
    QuestManager(){};

    QuestIDSet                  m_firstMainQuests; // 主线任务的第一个任务
    QuestIDSet                  m_repeatableQuests;
	QuestIDSet					m_firstBranchQuests;//分支任务的第一个任务

    QuestProtoMap               m_questProtoMap;
    BehaviorSet                 m_behaviorSet;      // 需要记录的behavior
    CounterTypeSet              m_counterTypeSet;   // 需要记录的counter_type
};

#define sQuestMgr QuestManager::Instance()


#endif // QuestManager_h__
