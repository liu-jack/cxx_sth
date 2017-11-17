

#include "Activity.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/ZhengWuTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"


bool PlayerData::save_zheng_wu()
{
	m_ZhengWuTable.SaveMod();
	return true;
}
void PlayerData::WritePlayerZhengWu(pb::GxDB_GovAffairsInfo& msg)
{
	if(m_ZhengWuTable)
	{
		m_ZhengWuTable->SaveTo(msg);
	}
}

Msg_Realize(SG2D_ZHENGWU_UPDATE, GxDB_GovAffairsInfo)
{
	if(m_ZhengWuTable)
	{
		m_ZhengWuTable->LoadFrom(msg);
		SetIsDirty(DIRTY_ZHENG_WU);
	}
	else
	{
		ZhengWuTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_ZhengWuTable.SetAndAddToCache(&cell);
	}
}
