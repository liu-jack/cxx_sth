

#include "Palace.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/global/PalaceTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"


bool PlayerData::save_palace_achievement()
{
	m_PalaceOfficeAchievement.SaveMod();
	return true;
}

void PlayerData::WritePlayerPalaceAchievement(pb::GxDB_PalaceAchievement& msg)
{
	for (BOOST_AUTO(it, m_PalaceOfficeAchievement.Begin()); it != m_PalaceOfficeAchievement.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_PALACE_ACHIEVEMENT_INFO, PalaceAchievementInfo)
{
	if (PalaceOfficeAchievement* pCell = m_PalaceOfficeAchievement.GetElement(msg.office_id()))		//如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_PALACE_ACHIEVEMENT);			//设置更新
		pCell->LoadFrom(msg);
		m_PalaceOfficeAchievement.SetModifiedID(msg.office_id());
	}
	else
	{
		PalaceOfficeAchievement cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_PalaceOfficeAchievement.AddAndAddToCache(&cell);
	}
}