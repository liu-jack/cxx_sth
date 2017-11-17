#include "PlayerData.h"

// PlayerData.cpp 部分实现

#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "data/SmithyTable.h"
#include "Smithy.pb.h"


bool PlayerData::save_smithy()
{
	m_SmithyTable.SaveMod();
	return true;
}

void PlayerData::WriteSmithyInfo( pb::GS2C_Smithy_All_Info &data )
{
	if (m_SmithyTable)
	{
		m_SmithyTable->SaveTo(data);
	}
}

Msg_Realize(SG2D_UPDATE_SMITHY, pb::GS2C_Smithy_All_Info)
{
	if (m_SmithyTable)
	{
		m_SmithyTable->LoadFrom(msg);
		SetIsDirty(DIRTY_SMITHY);
	}
	else
	{
		SmithyTable cell;
		cell.player_id = GetPlayerId();
		cell.LoadFrom(msg);
		m_SmithyTable.SetAndAddToCache(&cell);
	}
}
