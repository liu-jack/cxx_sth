

#include "World.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/WorldFightTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_world_fight_achievement()
{
	m_WorldFightAchievementTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerWorldFightAchievement(pb::GxDB_World_Achievement& msg)
{
	for (BOOST_AUTO(it, m_WorldFightAchievementTable.Begin()); it != m_WorldFightAchievementTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_WORLD_FIGHT_ACHIEVEMENT_UPDATE, GxDB_World_Achievement_Info)
{
	if (WorldFightAchievementTable* pCell = m_WorldFightAchievementTable.GetElement(msg.city_id()))		//如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_WORLD_FIGHT_ACHIEVEMENT);			//设置更新
		pCell->LoadFrom(msg);
		m_WorldFightAchievementTable.SetModifiedID(msg.city_id());
	}
	else
	{
		WorldFightAchievementTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_WorldFightAchievementTable.AddAndAddToCache(&cell);
	}
}
