#include "PlayerData.h"

// PlayerData.cpp 部分实现

//////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"

#include "Quest.pb.h"


void PlayerData::UpdateQuestCounter( const pb::GxDB_Behavior& questCounterMsg )
{
    QuestCounterTable* cell = m_QuestCounterTable.GetElement( questCounterMsg.behavior()) ;
    if ( cell)
    {
        SetIsDirty(DIRTY_QUEST_COUNTER);
        cell->LoadFrom(questCounterMsg);
        m_QuestCounterTable.SetModifiedID( questCounterMsg.behavior());
    }
    else
    {
        QuestCounterTable aQuestCounter;
        aQuestCounter.player_id = GetPlayerId();
        aQuestCounter.LoadFrom(questCounterMsg);

        m_QuestCounterTable.AddAndAddToCache( &aQuestCounter);
    }
}

void PlayerData::UpdateQuestLog( const pb::GxDB_Quest& questLog )
{
    QuestLogTable* cell = m_QuestLogTable.GetElement( questLog.quest_id()) ;
    if ( cell)
    {
        if ( questLog.is_deleted())
        {
            m_QuestLogTable.DeleteEntry(cell);
        }
        else
        {
            SetIsDirty(DIRTY_QUEST_LOG);
            cell->LoadFrom(questLog);
            m_QuestLogTable.SetModifiedID( questLog.quest_id());
        }

    }
    else
    {
        /////////////////////////////////////////////////////////////
        // for test
#ifdef _DEBUG
        std::string sql;
		Utility::FormatString( sql, "select * from %s where player_id = %llu and quest_id = %d", QuestLogTable::GetTableName(), GetPlayerId(), questLog.quest_id() );
        QuestLogTableCacheType tmp;
        std::vector<QuestLogTable*> vecQuestLogTable ;
        if( tmp.SqlLoadAndAddData( sql.c_str(), &vecQuestLogTable, FlagReload ) )
        {
            ASSERT( false);
        }
#endif

        QuestLogTable aQuestLog;
        aQuestLog.player_id = GetPlayerId();
        aQuestLog.LoadFrom(questLog); 
        m_QuestLogTable.AddAndAddToCache(&aQuestLog);

    }
}

void PlayerData::UpdateQuestComplete( const pb::GxDB_Quest& questId )
{
    if ( m_QuestCompleteTable)
    {
        m_QuestCompleteTable->SetFinishMask( questId.quest_id());
        SetIsDirty( DIRTY_QUEST_COMPLETE);
    }
    else{
        QuestCompleteTable qc;
        qc.player_id = GetPlayerId();
        qc.SetFinishMask( questId.quest_id());

        m_QuestCompleteTable.SetAndAddToCache(&qc);
    }
}

bool PlayerData::save_quest_complete()
{
    m_QuestCompleteTable.SaveMod();
    return true;
}

bool PlayerData::save_mod_quest_log()
{
    m_QuestLogTable.SaveMod();
    return true;
}

bool PlayerData::save_mod_quest_counter()
{
    m_QuestCounterTable.SaveMod();
    return true;
}

void PlayerData::WriteQuestList( pb::GS2C_QuestList& questListMsg )
{
    for ( BOOST_AUTO( it , m_QuestLogTable.Begin());
        it != m_QuestLogTable.End(); ++it)
    {
        MsgTool::SaveToRepeatField( it->second, questListMsg.mutable_quests());
    }

    if (m_QuestCompleteTable)
    {
        pb::FinishMask* finishMsg = questListMsg.mutable_finish_mask();
        if (finishMsg)
        {
            m_QuestCompleteTable->SaveTo( *finishMsg);
        }
    }

}

void PlayerData::WriteQuestCounter( pb::GxDB_BehaviorList& questCountersMsg )
{
    for ( BOOST_AUTO( it , m_QuestCounterTable.Begin());
        it != m_QuestCounterTable.End(); ++it)
    {
        MsgTool::SaveToRepeatField( it->second, questCountersMsg.mutable_behavior_list());
    }
}
