#ifndef QuestRecord_h__
#define QuestRecord_h__

#include "def/TypeDef.h"
#include "memory_buffer/MemoryPool.h"


class QuestProto;
union BehaviorType;
class IRecordProvider;

namespace pb
{
    class GxDB_Quest;
}


class QuestRecord
{
    DECLARE_OBJ_POOL(QuestRecord)
public:
    QuestRecord( const QuestProto& proto ,const IRecordProvider& provider);
    bool IsFinished() const;
    uint32 TargetValue() const;
    bool IsRepeatable() const;
    bool IsSequentQuest() const;
    bool IsActived() const;
    uint32 GetObjectiveValue() const;
    inline const QuestProto& Proto() const { return m_proto; };
    void SetActiveTime( time_t t);
    void SetInactiveUntilNextPeriod();
    void ResetActiveTime();
    bool IsMatchVipLv( int vipLv) const;
    bool IsMatchLevel( int level) const;
	uint64 GetActiveTime() const;
	bool CheckCategory(uint32 category1,uint32 category2,uint32 category3) const;
	


    void SaveTo( pb::GxDB_Quest& questMsg) const;
    void LoadFrom( const pb::GxDB_Quest& questMsg);

private:
    const QuestProto&       m_proto;
    time_t                  m_activeTime;
    const IRecordProvider&  m_provider;

};








#endif // QuestRecord_h__
