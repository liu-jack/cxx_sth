
#include "Quest.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/GrowUpQuestTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"


bool PlayerData::save_growupquest()
{
	m_GrowUpQuestTable.SaveMod();
	return true;
}

bool PlayerData::save_growupquestgroup()
{
	m_GrowUpQuestGroupTable.SaveMod();
	return true;
}

void PlayerData::WritePlayerGrowUpQuestInfo(pb::GxDB_Grow_Up_Quest& msg)
{
	for (BOOST_AUTO(it, m_GrowUpQuestTable.Begin()); it != m_GrowUpQuestTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_quest_info());
	}
}


void PlayerData::WritePlayerGrowUpQuestGroupInfo(pb::GxDB_Grow_Up_Quest_Group& msg)
{
	for (BOOST_AUTO(it, m_GrowUpQuestGroupTable.Begin()); it != m_GrowUpQuestGroupTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_group_info());
	}
}

Msg_Realize(SG2D_GROW_UP_QUEST_GROUP_UPDATE, GxDB_Grow_Up_Quest_Group_Info)
{
	if(msg.operate_type() == pb::DATA_UPDATE)
	{
		if(GrowUpQuestGroupTable* pcell = m_GrowUpQuestGroupTable.GetElement(msg.group_id()))
		{

			SetIsDirty(DIRTY_GROW_UP_QUEST_GROUP);			//设置更新
			pcell->LoadFrom(msg);
			m_GrowUpQuestGroupTable.SetModifiedID(msg.group_id());
		}
		else
		{
			GrowUpQuestGroupTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_GrowUpQuestGroupTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == pb::DATA_DEL)
	{
		if (GrowUpQuestGroupTable* pCell = m_GrowUpQuestGroupTable.GetElement(msg.group_id())) //如果表中有数据，则更新
		{
			m_GrowUpQuestGroupTable.DeleteEntry(pCell);
		}
	}
}


Msg_Realize(SG2D_GROW_UP_QUEST_UPDATE, GxDB_Grow_Up_Quest_Info)
{
	if(msg.operate_type() == pb::DATA_UPDATE)
	{
		if (GrowUpQuestTable* pCell = m_GrowUpQuestTable.GetElement(msg.quest_id())) //如果表中有数据，则更新
		{
			SetIsDirty(DIRTY_GROW_UP_QUEST);			//设置更新
			pCell->LoadFrom(msg);
			m_GrowUpQuestTable.SetModifiedID(msg.quest_id());
		}
		else
		{
			GrowUpQuestTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_GrowUpQuestTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == pb::DATA_DEL)
	{
		if (GrowUpQuestTable* pCell = m_GrowUpQuestTable.GetElement(msg.quest_id())) //如果表中有数据，则更新
		{
			m_GrowUpQuestTable.DeleteEntry(pCell);
		}
	}
}

Msg_Realize(SG2D_GROW_UP_QUEST_DELETE_INFO, GxDB_Grow_Up_Quest_Delete)
{
	if(msg.operate_type() == pb::DATA_DEL)
	{
		if(!m_GrowUpQuestTable.Empty())
		{
			m_GrowUpQuestTable.RemoveSelfEntryFromCacheAndDb();
		}
		if(!m_GrowUpQuestGroupTable.Empty())
		{
			m_GrowUpQuestGroupTable.RemoveSelfEntryFromCacheAndDb();
		}
	}
}