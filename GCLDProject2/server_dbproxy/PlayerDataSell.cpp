#include "PlayerData.h"

// PlayerData.cpp 部分实现

//////////////////////////////////////////////////////////////////////////
#include <boost/bind.hpp>
#include <boost/function.hpp>     
#include <boost/typeof/typeof.hpp>
#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"

#include "Item.pb.h"



void PlayerData::UpdateSellRandomProgram( const pb::GS2C_SellRandomProgram& msg )
{
//    SellRandomProgramTable * cell = m_SellRandomProgramTable.GetElement( msg.victim_type());
//    if ( cell)
//    {
//        SetIsDirty( DIRTY_SELL_RANDOM_PROGRAM);
//        ASSERT( cell->victim_type == msg.victim_type() );
//        cell->LoadFrom(msg);
//        m_SellRandomProgramTable.SetModifiedID( msg.victim_type());
//    }
//    else
//    {
//        /////////////////////////////////////////////////////////////
//        // for test
//#ifdef _DEBUG
//        std::string sql;
//        Utility::FormatString( sql, "select * from sell_random_program where player_id = %llu and victim_type = %d", GetPlayerId(), msg.victim_type() );
//        SellRandomProgramTableCacheType tmp;
//        std::vector<SellRandomProgramTable*> vecSellRandomProgramTable ;
//        if( tmp.SqlLoadAndAddData( sql.c_str(), &vecSellRandomProgramTable, FlagReload ) )
//        {
//            ASSERT( false);
//        }
//#endif
//        ////////////////////////////////////////////
//
//        SellRandomProgramTable aRandomProgram;
//        aRandomProgram.player_id = GetPlayerId();
//        aRandomProgram.LoadFrom(msg);
//
//        m_SellRandomProgramTable.AddAndAddToCache( &aRandomProgram);
//    }
}

void PlayerData::UpdateSellProgramInfo( const pb::GS2C_SellProgramInfo& msg )
{
    //SellProgramInfoTable * cell = m_SellProgramInfoTable.GetElement( msg.program_id());
    //if ( cell)
    //{
    //    SetIsDirty( DIRTY_SELL_PROGRAM_INFO);
    //    cell->LoadFrom(msg);
    //    m_SellProgramInfoTable.SetModifiedID( msg.program_id());
    //}
    //else
    //{
    //    SellProgramInfoTable aProgramInfo;
    //    aProgramInfo.player_id = GetPlayerId();
    //    aProgramInfo.LoadFrom(msg);

    //    m_SellProgramInfoTable.AddAndAddToCache( &aProgramInfo);
    //}

    //const pb::GS2C_SellRecordList& sellrecordlist = msg.records();
    //for ( int i = 0 ; i < sellrecordlist.sell_records_size(); ++i)
    //{
    //    const pb::GS2C_ItemSellRecord& recordMsg = sellrecordlist.sell_records(i);
    //    SellRecordTable * record = m_SellRecordTable.GetElement( recordMsg.sell_id());
    //    if ( record)
    //    {
    //        SetIsDirty( DIRTY_SELL_RECORD);
    //        record->LoadFrom(recordMsg);
    //        m_SellRecordTable.SetModifiedID( recordMsg.sell_id());
    //    }
    //    else
    //    {
    //        SellRecordTable aSellRecord;
    //        aSellRecord.player_id = GetPlayerId();
    //        aSellRecord.LoadFrom(recordMsg);

    //        m_SellRecordTable.AddAndAddToCache( &aSellRecord);
    //    }

    //}
}

bool PlayerData::save_mod_sell_random_program()
{
    //m_SellRandomProgramTable.SaveMod();
    return true;
}

bool PlayerData::save_mod_sell_program_info()
{
 /*   m_SellProgramInfoTable.SaveMod();*/
    return true;
}

bool PlayerData::save_mod_sell_record()
{
 /*   m_SellRecordTable.SaveMod();*/
    return true;
}

void PlayerData::WriteSellData( pb::GS2C_AllSellData& allSellData )
{
    //for ( BOOST_AUTO( it , m_SellRandomProgramTable.Begin());
    //    it != m_SellRandomProgramTable.End(); ++it )
    //{
    //    MsgTool::SaveToRepeatField( it->second, allSellData.mutable_random_programs());
    //}

    //for ( BOOST_AUTO( it , m_SellProgramInfoTable.Begin());
    //    it != m_SellProgramInfoTable.End(); ++it )
    //{
    //    MsgTool::SaveToRepeatField( it->second, allSellData.mutable_sell_program_infos());
    //}

    //for ( BOOST_AUTO(it , m_SellRecordTable.Begin());
    //    it != m_SellRecordTable.End(); ++it )
    //{
    //    MsgTool::SaveToRepeatField( it->second, allSellData.mutable_sell_records());
    //}
}
