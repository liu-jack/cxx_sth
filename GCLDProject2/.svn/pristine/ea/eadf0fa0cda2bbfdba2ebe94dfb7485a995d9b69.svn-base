

#include "Activity.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/GeneralReward.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_general_reward()
{
	m_GeneralRewardTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerGeneralReward(pb::GxDB_Hero_Reward& msg)
{
	for (BOOST_AUTO(it, m_GeneralRewardTable.Begin()); it != m_GeneralRewardTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_ACTIVITY_GENERAL_REWARD_UPDATE, GxDB_Hero_Reward_info)
{
	if(msg.operate_type() == pb::DATA_UPDATE)
	{
		if (GeneralRewardTable* pCell = m_GeneralRewardTable.GetElement(msg.charid()))		//如果表中有数据，则更新
		{
			SetIsDirty(DIRTY_GENERAL_REWARD);			//设置更新
			pCell->LoadFrom(msg);
			m_GeneralRewardTable.SetModifiedID(msg.charid());
		}
		else
		{
			GeneralRewardTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_GeneralRewardTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == DATA_DEL)
	{
		if (GeneralRewardTable* pCell = m_GeneralRewardTable.GetElement(msg.charid()))		//如果表中有数据，则更新
		{
			m_GeneralRewardTable.DeleteEntry(pCell);
		}
	}
	
}
////////////////////////////////

bool PlayerData::save_wanna_be_stronger()
{
	m_WannaBeStrongerTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerWannaBeStronger(pb::GxDB_WannaBeStrongerInfoAllInfo& msg)
{
	for (BOOST_AUTO(it, m_WannaBeStrongerTable.Begin()); it != m_WannaBeStrongerTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_WANNA_BE_STRONGER_UPDATE, WannaBeStrongerInfo)
{
	if (WannaBeStrongerTable* pCell = m_WannaBeStrongerTable.GetElement(msg.group_id())) //如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_WANNA_BE_STRONGER);			//设置更新
		pCell->LoadFrom(msg);
		m_WannaBeStrongerTable.SetModifiedID(msg.group_id());
	}
	else
	{
		WannaBeStrongerTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_WannaBeStrongerTable.AddAndAddToCache(&cell);
	}
}
