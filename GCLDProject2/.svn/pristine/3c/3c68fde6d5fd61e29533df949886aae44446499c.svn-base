#include "PlayerData.h"

// PlayerData.cpp 部分实现

//////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"

#include "Stage.pb.h"
#include "InterServer.pb.h"



void PlayerData::WritePlayerStageLevelInfo(pb::GS2C_AllStageInfo& stageInfo)
{
    for (BOOST_AUTO( it, m_StageLevelTable.Begin()); it != m_StageLevelTable.End(); it++)
    {
        MsgTool::SaveToRepeatField( it->second, stageInfo.mutable_stage_level_infos());
    }

  /*  if ( m_EndlessLevelTable)
    {
        m_EndlessLevelTable->SaveTo( *stageInfo.mutable_endless_level());
    }*/
}

bool PlayerData::save_stage_level_record()
{
    m_StageLevelTable.SaveMod();
    m_EndlessLevelTable.SaveMod();
    return true;
}

void PlayerData::UpdateEndlessLevel( pb::SG2D_UpdataStageLevel& info )
{
    if ( m_EndlessLevelTable)
    {
        m_EndlessLevelTable->LoadFrom( info.info());
        SetIsDirty(DIRTY_STAGE_LEVEL);
    }
    else
    {
        EndlessLevelTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom( info.info());

        m_EndlessLevelTable.SetAndAddToCache(&cell);
    }
}


void PlayerData::UpdateStageLevelInfo( pb::SG2D_UpdataStageLevel& info )
{
    StageLevelTable* cell = m_StageLevelTable.GetElement( info.info().level_id());
    if ( cell)
    {
        SetIsDirty( DIRTY_STAGE_LEVEL);
        cell->LoadFrom( info.info());
        m_StageLevelTable.SetModifiedID( info.info().level_id());
    }
    else
    {
        StageLevelTable stageLevelInfo;

        stageLevelInfo.player_id = GetPlayerId();
        stageLevelInfo.LoadFrom(info.info());

        m_StageLevelTable.AddAndAddToCache( &stageLevelInfo);
    }
}
