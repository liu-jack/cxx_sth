#ifndef QuestLogger_h__
#define QuestLogger_h__

//#include "IQuestLogger.h"
#include "boost/scoped_ptr.hpp"
#include "QuestRecord.h"
#include "QuestDef.h"
#include "ptr_container/PtrHashMap.h"
#include "ptr_container/PtrMap.h"
#include "boost/dynamic_bitset.hpp"


class Player;
class QuestRecord;
class QuestProto;
class LootList;

namespace pb{
    class GxDB_Quest;
    class GS2C_QuestList;
    class GS2C_LootList;
}


class QuestLogger 
    ///: public IQuestLogger
{

   typedef PtrMap< QuestID, QuestRecord> QuestRecords;


public:
    QuestLogger( /*Player* pPlayer*/);

    static QuestRecord* CreateQuestInst( const QuestProto& proto ,const IRecordProvider& provider);
    static QuestRecord* CreateQuestInst( const pb::GxDB_Quest& questMsg, const IRecordProvider& provider);

     void AfterLoadLogger(Player* player);
     void UpdateQuest(Player*player, const BehaviorType& behavior);
     void UpdateQuest( Player*player,uint32 category1, uint32 category2, uint32 category3 = 0, uint32 condition = 0);
     void UpdateQuestCategory(Player*player, uint32 categroy1) ;
     void UpdateQuestCategory(Player*player, uint32 categroy1, uint32 category2) ;


     bool TryAddQuest(Player*player, QuestID questId);
     int  TakeQuestReward(Player*player, QuestID questId, pb::GS2C_LootList& msg,uint32 &player_exp);

     void Load(Player*player, const pb::GS2C_QuestList& questList);
     void Save(Player*player, pb::GS2C_QuestList& questList) ;
     void HandleGM(Player*player, QUEST_GM_CMD gmCmd, int param);
	 bool GetQuestFinished( QuestID questId);
	 bool    HasQuest( QuestID questId) const;
	//粗暴完成任务
	 int GetQuestRewards(Player* player, uint32 questId , pb::GS2C_LootList& msg,uint32 &player_exp);
    enum SEND_MOD
    {
        SEND_MOD_CLIENT = 0x0001,
        SEND_MOD_DB_PROXY = 0x0002,
        SEND_MOD_ALL = SEND_MOD_CLIENT | SEND_MOD_DB_PROXY,
    };

	void    SendUpdateMsg(Player*player, QuestID questId , SEND_MOD mode = SEND_MOD_ALL);
	bool CheckCategory(uint32 category1,uint32 category2,uint32 category3 );
private:
    bool    TryAddQuestSilently(Player* player,QuestID questId);

    bool    CanAddQuest( QuestID questID) const;
    bool    _AddQuest(Player*player, const QuestProto *proto);
    void    _TakeQuestReward( Player* player,QuestRecord* record , pb::GS2C_LootList& msg,uint32 &player_exp);

    void    AddQuestRecord( QuestRecord * questRecord);
    void    MarkQuestCompleted(Player*player,QuestID questId,const QuestProto& proto);
	void	MarkQuestUnCompleted( QuestID questId );
	void    RemoveQuest(Player*player, QuestID questId );
    void    FillDBReward(Player* player, const DB_QuestReward* dbReward , LootList& lootList );

	bool HasQuestCompleted( QuestID questId) const;

    void    SendQuestCompletedMsg(Player*player, QuestID questId);
    void    SendDeleteMsg(Player*player, QuestID questId );
	
    void    SendUpdateMsg(Player* player, const QuestRecord * record , SEND_MOD mode = SEND_MOD_ALL);
	void MarkAllPreQuestFinished(Player* player,const QuestID id);

	bool ParseItems(const std::string& equip_id, const uint32 count,IntV3Vec& retV3 ) const;
private:
    QuestRecord* MutableQuestRecord( QuestID questId);
    const QuestRecord* GetQuestRecord( QuestID questId) const;
    
private:
    //Player*         m_pPlayer;   
    QuestRecords    m_questRecords; 
    boost::dynamic_bitset<uint32> m_questCompleted;
};


#endif // QuestLogger_h__
