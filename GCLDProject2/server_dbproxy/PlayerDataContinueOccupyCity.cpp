

#include "Fuben.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/ContinueOccupyCityTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"


bool PlayerData::save_continue_occupy_city()
{
	m_ContinueOccupyCityTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerContinueOccupyCity(pb::GxDB_ContinueOccupyCityInfo& msg)
{
	for (BOOST_AUTO(it, m_ContinueOccupyCityTable.Begin()); it != m_ContinueOccupyCityTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_CONTINUE_OCCUPY_CITY_UPDATE, BaseContinueMsg)
{	
	if (ContinueOccupyCityTable* pCell = m_ContinueOccupyCityTable.GetElement(msg.active_time()))		//如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_CONTINUE_CITY);			//设置更新
		pCell->LoadFrom(msg);
		m_ContinueOccupyCityTable.SetModifiedID(msg.active_time());
	}
	else
	{
		ContinueOccupyCityTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_ContinueOccupyCityTable.AddAndAddToCache(&cell);
	}
}