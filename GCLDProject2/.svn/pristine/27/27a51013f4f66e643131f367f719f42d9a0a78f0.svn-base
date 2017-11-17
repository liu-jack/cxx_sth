#include "Fuben.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/SeigeForceTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_seige_force()
{
	m_SeigeForceTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerSeigeForce(pb::GxDB_Seige_Force_Info_Pack& msg)
{
	for (BOOST_AUTO(it, m_SeigeForceTable.Begin()); it != m_SeigeForceTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_SEIGE_FORCE_DATA_UPDATE, Seige_Force_info)
{
	if (SeigeForceTable* pCell = m_SeigeForceTable.GetElement(msg.army_id()))		//如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_SEIGE_FORCE);			//设置更新
		pCell->LoadFrom(msg);
		m_SeigeForceTable.SetModifiedID(msg.army_id());
	}
	else
	{
		SeigeForceTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_SeigeForceTable.AddAndAddToCache(&cell);
	}
}