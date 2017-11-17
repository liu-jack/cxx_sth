#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Altar.pb.h"
#include "PlayerAltarLog.h"
#include "Table/AltarTableMgr.h"
using namespace pb;

OpHandler(CMSG_ALTAR_ALL_INFO)
{
    GS2C_Altar_All_Info msg;
    m_AltarLog->SaveTo(msg);
    Send(SMSG_ALTAR_ALL_INFO, msg);
}
OpHandler(CMSG_ALTAR_STUDY)
{
    C2GS_Altar_Study msg; pack >> msg;

    m_AltarLog->Study(*this, msg.altar_lv());
}
OpHandler(CMSG_ALTAR_TAKE_REWARD)
{
    C2GS_Altar_Take_Reward msg; pack >> msg;

    m_AltarLog->TakeReward(*this, msg.altar_lv());
}
OpHandler(CMSG_ALTAR_SUB_ALTAR_LEVELUP)
{
    C2GS_Altar_Sub_Altar_LevelUp msg; pack >> msg;

    m_AltarLog->SubAltarLevelUp(*this, msg.altar_lv());
}
OpHandler(CMSG_ALTAR_BUILD_BY_TOKEN)
{
    m_AltarLog->BuildAltar_Token(*this);
}
OpHandler(CMSG_ALTAR_BUILD_BY_SILVER)
{
    C2GS_Altar_Build_By_Silver msg; pack >> msg;

    m_AltarLog->BuildAltar_Silver(*this, msg.is_fast());
}
OpHandler(CMSG_ALTAR_LEVELUP_TO_OPEN_SUB_ALTAR)
{
    m_AltarLog->LevelUpToOpenSubAltar(*this);
}
OpHandler(CMSG_ALTAR_STUDY_ONEKEY)
{
	uint32 maxLv = sAltarTableMgr.GetMaxAltarLv();
	for (uint32 i = 1; i <= maxLv; ++i)
	{
		m_AltarLog->Study(*this, i);
	}
}
OpHandler(CMSG_ALTAR_TAKE_REWARD_ONEKEY)
{	
	//if(m_AltarLog->isAllAwardsCanTake(*this))
	//{
		uint32 maxLv = sAltarTableMgr.GetMaxAltarLv();
		for (uint32 i = 1; i <= maxLv; ++i)
		{
			m_AltarLog->TakeReward(*this, i);
		}
	//}
}