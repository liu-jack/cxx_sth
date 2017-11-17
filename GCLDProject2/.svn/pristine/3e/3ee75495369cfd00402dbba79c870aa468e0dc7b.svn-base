#include "PlayerData.h"

// PlayerData.cpp 部分实现

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "data/AltarTable.h"
#include "Altar.pb.h"



bool PlayerData::save_altar_record()
{
    m_AltarTable.SaveMod();
    return true;
}
void PlayerData::WritePlayerAltarInfo(pb::GS2C_Altar_All_Info& msg)
{
    if (m_AltarTable)
    {
        m_AltarTable->SaveTo(msg);
    }
}
Msg_Realize(SG2D_ALTAR_ALL_INFO, GS2C_Altar_All_Info)
{
    if (m_AltarTable)
    {
        m_AltarTable->LoadFrom(msg);
        SetIsDirty(DIRTY_ALTAR_INFO);
    }
    else
    {
        AltarTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_AltarTable.SetAndAddToCache(&cell);
    }
}
