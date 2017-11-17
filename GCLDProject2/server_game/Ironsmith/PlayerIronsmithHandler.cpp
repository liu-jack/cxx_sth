#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Smithy.pb.h"
#include "PlayerIronsmithLog.h"
#include "Table/IronsmithTableMgr.h"
using namespace pb;

OpHandler(CMSG_SMITHY_ALL_INFO)
{
    GS2C_Smithy_All_Info msg;
    m_SmithyLog->SaveTo(msg);
    Send(SMSG_SMITHY_ALL_INFO, msg);
}
OpHandler(CMSG_SMITHY_SMELTER_DO)
{
    C2GS_Smithy_Smelter_Do msg; pack >> msg;

    for (int i = 0; i < msg.item_id_size(); ++i)
    {
        m_SmithyLog->DoSmelter(*this, msg.idx(), msg.item_id(i));

		GS2C_Smithy_All_Info msgBack;
		m_SmithyLog->SaveTo(msgBack);
		Send(SMSG_SMITHY_ALL_INFO, msgBack);
    }
}
OpHandler(CMSG_SMITHY_SUB_SMELTER_LEVELUP)
{
    C2GS_Smithy_Sub_Smelter_LevelUp msg; pack >> msg;

    m_SmithyLog->SubSmelterLevelUp(*this, msg.idx());

	GS2C_Smithy_All_Info msgBack;
	m_SmithyLog->SaveTo(msgBack);
	Send(SMSG_SMITHY_ALL_INFO, msgBack);
}
OpHandler(CMSG_SMITHY_BUILD_BY_TOKEN)
{
    m_SmithyLog->BuildByToken(*this);

	GS2C_Smithy_All_Info msgBack;
	m_SmithyLog->SaveTo(msgBack);
	Send(SMSG_SMITHY_ALL_INFO, msgBack);
}
OpHandler(CMSG_SMITHY_BUILD_BY_SILVER)
{
    C2GS_Smithy_Build_By_Silver msg; pack >> msg;
    m_SmithyLog->BuildBySilver(*this, msg.is_fast());

	GS2C_Smithy_All_Info msgBack;
	m_SmithyLog->SaveTo(msgBack);
	Send(SMSG_SMITHY_ALL_INFO, msgBack);
}
OpHandler(CMSG_SMITHY_LEVELUP_TO_OPEN_SUB_SMELTER)
{
    m_SmithyLog->LevelUpToOpenSubSmelter(*this);

	GS2C_Smithy_All_Info msgBack;
	m_SmithyLog->SaveTo(msgBack);
	Send(SMSG_SMITHY_ALL_INFO, msgBack);
}