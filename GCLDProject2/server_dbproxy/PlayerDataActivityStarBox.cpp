
#include "Activity.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/ActivityStarBoxTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_activity_star_box()
{
	m_ActivityStarBoxTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerActivityStarBox(pb::GxDB_ActivityStarBox& msg)
{
	for (BOOST_AUTO(it, m_ActivityStarBoxTable.Begin()); it != m_ActivityStarBoxTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_ACTIVITY_STAR_BOX_UPDATE, GxDB_ActivityStarBoxInfo)
{
	if(msg.operate_type() == pb::DATA_UPDATE)
	{
		if (ActivityStarBoxTable* pCell = m_ActivityStarBoxTable.GetElement(msg.table_id()))		//如果表中有数据，则更新
		{
			SetIsDirty(DIRTY_ACTIVITY_STAR_BOX);			//设置更新
			pCell->LoadFrom(msg);
			m_ActivityStarBoxTable.SetModifiedID(msg.table_id());
		}
		else
		{
			ActivityStarBoxTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_ActivityStarBoxTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == pb::DATA_DEL)
	{
		if (ActivityStarBoxTable* pCell = m_ActivityStarBoxTable.GetElement(msg.table_id()))		//如果表中有数据，则更新
		{
			m_ActivityStarBoxTable.DeleteEntry(pCell);
		}
	}	
}

