

#include "Rank.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/CongratulateTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_congratulate()
{
	m_CongratulateTable.SaveMod();
	return true;
}
void PlayerData::WritePlayerCongratulate(pb::GxDB_CongratulateRewardLog& msg)
{
	for (BOOST_AUTO(it, m_CongratulateTable.Begin()); it != m_CongratulateTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_CONGRATULATE_UPDATE, GxDB_CongratulateInfo)
{
	if(msg.operate_type() == pb::DATA_UPDATE)
	{
		if (CongratulateTable* pCell = m_CongratulateTable.GetElement(msg.table_id()))		//������������ݣ������
		{
			SetIsDirty(DIRTY_CONGRATULATE);			//���ø���
			pCell->LoadFrom(msg);
			m_CongratulateTable.SetModifiedID(msg.table_id());
		}
		else
		{
			CongratulateTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_CongratulateTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == pb::DATA_DEL)
	{
		if (CongratulateTable* pCell = m_CongratulateTable.GetElement(msg.table_id()))		//������������ݣ������
		{
			m_CongratulateTable.DeleteEntry(pCell);
		}
	}
}


bool PlayerData::save_congratulate_been_con()
{
	m_CongratulateBeenCongratulatedTable.SaveMod();
	return true;
}
void PlayerData::WritePlayerCongratulateBeenCon(pb::GxDB_CongratulateBeenCongratualted& msg)
{
	for (BOOST_AUTO(it, m_CongratulateBeenCongratulatedTable.Begin()); it != m_CongratulateBeenCongratulatedTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_CONGRATULATE_BEEN_CON_UPTATE, C2GS_Congratulate_Accept)
{

	if(msg.operate_type() == pb::DATA_UPDATE)
	{	
		if (CongratulateBeenCongratulatedTable* pCell = m_CongratulateBeenCongratulatedTable.GetElement(msg.table_id()))		//������������ݣ������
		{
			SetIsDirty(DIRTY_CONGRATULATE_BEEN_CON);			//���ø���
			pCell->LoadFrom(msg);
			m_CongratulateBeenCongratulatedTable.SetModifiedID(msg.table_id());
		}
		else
		{
			CongratulateBeenCongratulatedTable cell;
			cell.player_id = GetPlayerId();
			cell.LoadFrom(msg);
			m_CongratulateBeenCongratulatedTable.AddAndAddToCache(&cell);
		}
	}
	else if(msg.operate_type() == pb::DATA_DEL)
	{
		if (CongratulateBeenCongratulatedTable* pCell = m_CongratulateBeenCongratulatedTable.GetElement(msg.table_id()))		//������������ݣ������
		{
			m_CongratulateBeenCongratulatedTable.DeleteEntry(pCell);
		}
	}	
	
}