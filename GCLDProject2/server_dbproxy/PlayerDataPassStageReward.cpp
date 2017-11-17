#include "Quest.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/PassStageRewardGoldTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_pass_stage_reward()
{
	m_PassStageRewardRecordTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerPassStageReward(pb::GxDB_Pass_Stage_Reward& msg)
{
	for (BOOST_AUTO(it, m_PassStageRewardRecordTable.Begin()); it != m_PassStageRewardRecordTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_pass_stage_info());
	}
}

Msg_Realize(SG2D_ACTIVITY_PASS_STAGE_REWARD_DIAMOND_UPDATE, pass_stage_reward_info)
{
	if(msg.operate_type() == pb::DATA_UPDATE)
	{
		if (PassStageRewardRecordTable* pCell = m_PassStageRewardRecordTable.GetElement(msg.id()))		//如果表中有数据，则更新
		{
			SetIsDirty(DIRTY_PASS_STAGE_REWARD);			//设置更新
			pCell->LoadFrom(msg);
			m_PassStageRewardRecordTable.SetModifiedID(msg.id());
		}
		else
		{
			PassStageRewardRecordTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_PassStageRewardRecordTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == pb::DATA_DEL)
	{
		if (PassStageRewardRecordTable* pCell = m_PassStageRewardRecordTable.GetElement(msg.id()))		//如果表中有数据，则更新
		{
			m_PassStageRewardRecordTable.DeleteEntry(pCell);
		}
	}
}

Msg_Realize(SG2D_PASS_STAGE_REWARD_DIAMOND_DELETE, GxDB_Pass_Stage_Reward_Delete)
{
	if(msg.operate_type() == pb::DATA_DEL)
	{
		if(!m_PassStageRewardRecordTable.Empty())
		{
			m_PassStageRewardRecordTable.RemoveSelfEntryFromCacheAndDb();
		}
	}
}