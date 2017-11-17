#include "PlayerData.h"

// PlayerData.cpp 部分实现

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "data/BaoQiTable.h"
#include "BaoQi.pb.h"



bool PlayerData::save_bao_qi_record()
{
    m_BaoQiTable.SaveMod();
    return true;
}
void PlayerData::WritePlayerBaoQiInfo(pb::GS2C_BaoQi_All_Info& msg)
{
    if (m_BaoQiTable)
    {
        m_BaoQiTable->SaveTo(msg);
    }
}
Msg_Realize(SG2D_BAO_QI_ALL_INFO, GS2C_BaoQi_All_Info)
{
    if (m_BaoQiTable)
    {
        m_BaoQiTable->LoadFrom(msg);
        SetIsDirty(DIRTY_BAO_QI_INFO);
    }
    else
    {
        BaoQiTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_BaoQiTable.SetAndAddToCache(&cell);
    }
}
