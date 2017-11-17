#include "PlayerData.h"

// PlayerData.cpp 部分实现

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "InterServer.pb.h"
#include "data/FuBenTable.h"
#include "Fuben.pb.h"
#include "boost/typeof/typeof.hpp"



bool PlayerData::save_fuben_record()
{
    m_FuBenBaseTable.SaveMod();
    //m_FuBenResourceTable.SaveMod();
    m_FuBenPassStageTable.SaveMod();
	m_BranchFuBenTable.SaveMod();
    return true;
}

bool PlayerData::save_mod_crusade()
{
	m_StageCrusadeTable.SaveMod();
	return true;
}

void PlayerData::WritePlayerFuBenInfo(pb::GS2C_Fuben_All_Info& msg)
{
    if (m_FuBenBaseTable)
    {
        msg.set_cur_fuben_id(m_FuBenBaseTable->cur_fuben_id);
        msg.set_cur_stage_id(m_FuBenBaseTable->cur_stage_id);
		for(size_t i = 0;i < ARRAY_SIZE(m_FuBenBaseTable->NR_stars);++i)
		{
			msg.add_nr_stars(m_FuBenBaseTable->NR_stars[i]);
		}
    }
    for (BOOST_AUTO(it, m_FuBenHideTable.Begin()); it != m_FuBenHideTable.End(); ++it)
    {
        MsgTool::SaveToRepeatField(it->second, msg.mutable_hide_fuben_infos());
    }
    for (BOOST_AUTO(it, m_FuBenPassStageTable.Begin()); it != m_FuBenPassStageTable.End(); ++it)
    {
        MsgTool::SaveToRepeatField(it->second,msg.mutable_pass_stage_info());
    }
	for (BOOST_AUTO(it, m_BranchFuBenTable.Begin()); it != m_BranchFuBenTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second,msg.mutable_branch_info());
	}

	if(m_StageCrusadeTable)
	{
		m_StageCrusadeTable->SaveTo(*msg.mutable_crusade_info_db());
	}
}
Msg_Realize(SG2D_UPDATA_FUBEN_BASE, SG2D_UpdataFuBenBase)
{
    if (m_FuBenBaseTable)
    {
        m_FuBenBaseTable->LoadFrom(msg);
        SetIsDirty(DIRTY_FUBEN_INFO);
    }
    else
    {
        FuBenBaseTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_FuBenBaseTable.SetAndAddToCache(&cell);
    }
}
// Msg_Realize(SG2D_UPDATA_FUBEN_RESOURCE, SG2D_UpdataFuBenResource)
// {
//     if (FuBenResourceTable* pCell = m_FuBenResourceTable.GetElement(msg.info().stage_id()))
//     {
//         SetIsDirty(DIRTY_FUBEN_INFO);
//         pCell->LoadFrom(msg.info());
//         m_FuBenResourceTable.SetModifiedID(msg.info().stage_id());
//     }
//     else
//     {
//         FuBenResourceTable cell;
//         cell.player_id = GetPlayerId();
//         cell.LoadFrom(msg.info());
// 
//         m_FuBenResourceTable.AddAndAddToCache(&cell);
//     }
// }
Msg_Realize(SG2D_UPDATA_FUBEN_PASSSTAGE, Stage_Info)
{
    if (FuBenPassStageTable* pCell = m_FuBenPassStageTable.GetElement(msg.stage_type()))
    {
        SetIsDirty(DIRTY_FUBEN_INFO);
        pCell->LoadFrom(msg);
        m_FuBenPassStageTable.SetModifiedID(msg.stage_type());
    }
    else
    {
        FuBenPassStageTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);

        m_FuBenPassStageTable.AddAndAddToCache(&cell);
    }
}

Msg_Realize(SG2D_CRUSADE_UPDATE, GxDB_Combat_Crusade)
{
	if(m_StageCrusadeTable)
	{
		m_StageCrusadeTable->LoadFrom(msg);
		SetIsDirty(DIRTY_CRUSADE);
	}
	else
	{
		StageCrusadeTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_StageCrusadeTable.SetAndAddToCache(&cell);
	}
}
Msg_Realize(SG2D_BRANCH_FUBEN_UPDATE, BranchFubenInfo)
{
	if (BranchFuBenTable* pCell = m_BranchFuBenTable.GetElement(msg.stage_id()))
	{
		SetIsDirty(DIRTY_FUBEN_INFO);
		pCell->LoadFrom(msg);
		m_BranchFuBenTable.SetModifiedID(msg.stage_id());
	}
	else
	{
		BranchFuBenTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);

		m_BranchFuBenTable.AddAndAddToCache(&cell);
	}
}
Msg_Realize(SG2D_HIDE_FUBEN_UPDATE, HideFubenInfo)
{
	if (FuBenHideTable* pCell = m_FuBenHideTable.GetElement(msg.hide_stage_id()))
	{
		SetIsDirty(DIRTY_FUBEN_INFO);
		pCell->LoadFrom(msg);
		m_FuBenHideTable.SetModifiedID(msg.hide_stage_id());
	}
	else
	{
		FuBenHideTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);

		m_FuBenHideTable.AddAndAddToCache(&cell);
	}
}


