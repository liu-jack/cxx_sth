#ifndef QuestProto_h__
#define QuestProto_h__

#include "def/TypeDef.h"
#include "QuestDef.h"
struct DB_QuestProto;
struct DB_QuestReward;



class QuestProto
{
    friend class QuestManager;
public:
    QuestProto( const DB_QuestProto& dbQuestProto );

    uint32  ID() const;
    uint32  QuestType() const;  
    const BehaviorType& GetBehaviorType() const;
    uint32  TargetValue() const;
    uint32  NextQuest() const;
    bool    IsRepeatable() const;
    bool    IsSequentQuest() const; //
	bool	IsBranchQuest() const;
	bool	IsMainQuest() const;
    bool    HasPreQuest() const;
	void	SetHasPreQuest(bool value);
    bool    MatchVipRequire( uint32 vipLv) const;
    bool    MatchLevelRequire( uint32 level) const;
    const RewardList& GetRewardList() const;
	bool CheckCategory(uint32 category1,uint32 category2,uint32 category3) const;
private:
    const DB_QuestProto&                m_dbQuestProto;
    RewardList                          m_dbRewards;
    BehaviorType                        m_behaviorType;
    bool                                m_hasPreQuest;
};


#endif // QuestProto_h__
