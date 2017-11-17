#include "PlayerData.h"

// PlayerData.cpp 部分实现

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "data/MarketTable.h"
#include "Market.pb.h"



bool PlayerData::save_market_record()
{
    m_MarketTable.SaveMod();
    return true;
}
void PlayerData::WritePlayerMarketInfo(pb::GS2C_Market_All_Info& msg)
{
    if (m_MarketTable)
    {
        m_MarketTable->SaveTo(msg);
    }
}
Msg_Realize(SG2D_MARKET_COLLECT, GS2C_Market_Collect)
{
    if (m_MarketTable)
    {
        m_MarketTable->LoadFrom(msg);
        SetIsDirty(DIRTY_MARKET_INFO);
    }
    else
    {
        MarketTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_MarketTable.SetAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_MARKET_BLACK, GS2C_Market_Black)
{
    if (m_MarketTable)
    {
        m_MarketTable->LoadFrom(msg);
        SetIsDirty(DIRTY_MARKET_INFO);
    }
    else
    {
        MarketTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_MarketTable.SetAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_MARKET_BUY, GS2C_Market_Buy)
{
    if (m_MarketTable)
    {
        m_MarketTable->LoadFrom(msg);
        SetIsDirty(DIRTY_MARKET_INFO);
    }
    else
    {
        MarketTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_MarketTable.SetAndAddToCache(&cell);
    }
}
Msg_Realize(SG2D_MARKET_MYSTERY, GS2C_Market_Mystery_Shop)
{
    if (m_MarketTable)
    {
        m_MarketTable->LoadFrom(msg);
        SetIsDirty(DIRTY_MARKET_INFO);
    }
    else
    {
        MarketTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_MarketTable.SetAndAddToCache(&cell);
    }
}