
#include "Quest.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/DailyQuestTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_dailyquest()
{
	m_DailyQuestTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerDailyQuestInfo(pb::GxDB_Daily_Quest& msg)
{
	for (BOOST_AUTO(it, m_DailyQuestTable.Begin()); it != m_DailyQuestTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_quest_info());
	}
}

Msg_Realize(SG2D_DAILY_QUEST_UPDATE, GxDB_Daily_Quest_Info)
{
	if (DailyQuestTable* pCell = m_DailyQuestTable.GetElement(msg.quest_id()))		//如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_DAILYQUEST);			//设置更新
		pCell->LoadFrom(msg);
		m_DailyQuestTable.SetModifiedID(msg.quest_id());
	}
	else
	{
		DailyQuestTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_DailyQuestTable.AddAndAddToCache(&cell);
	}
}