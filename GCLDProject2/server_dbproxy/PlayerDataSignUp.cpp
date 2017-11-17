
#include "Quest.pb.h"
#include "PlayerData.h"
#include "utility/MsgTool.h"
#include "data/SignUpTable.h"
#include "data/DbProxyDataMgr.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_signup()
{
	m_SignUpTable.SaveMod();
	return true;
}


void PlayerData::WritePlayerSignUp(pb::GxDB_Sign_Up& msg)
{
	for (BOOST_AUTO(it, m_SignUpTable.Begin()); it != m_SignUpTable.End(); ++it)
	{
		MsgTool::SaveToRepeatField(it->second, msg.mutable_info());
	}
}

Msg_Realize(SG2D_SIGN_UP_UPDATE, GxDB_Sign_info)
{
	if (SignUpTable* pCell = m_SignUpTable.GetElement(msg.dayth()))		//如果表中有数据，则更新
	{
		SetIsDirty(DIRTY_SIGN_UP);			//设置更新
		pCell->LoadFrom(msg);
		m_SignUpTable.SetModifiedID(msg.dayth());
	}
	else
	{
		SignUpTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_SignUpTable.AddAndAddToCache(&cell);
	}
}