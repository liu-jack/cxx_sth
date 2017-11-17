/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Technology.pb.h"
#include "PlayerTechnology.h"
using namespace pb;

OpHandler(CMSG_TECHNOLOGY_ALL_INFO)
{
    GS2C_Technology_All_Info msg;
    m_Technology->SaveTo(*this, msg);
    Send(SMSG_TECHNOLOGY_ALL_INFO, msg);
}
OpHandler(CMSG_TECHNOLOGY_UPSTAR)
{
    C2GS_Technology_UpStar req; pack >> req;
	GS2C_Technology_UpStar msg;
    m_Technology->UpStar(*this, req.id(), msg);
	Send(pb::SMSG_TECHNOLOGY_UPSTAR_RSP,msg);
}

OpHandler(CMSG_TECHNOLOGY_STUDY)
{
	C2GS_Technology_Study req;pack >> req;
	GS2C_Technology_Study msg;
	if(m_Technology->Study(this,req.id(),msg))
	{
		Send(pb::SMSG_TECHNOLOGY_STUDY_RSP,msg);
		NLOG("Send  SMSG_TECHNOLOGY_STUDY_RSP msg ");
		return;
	}
	NLOG("CMSG_TECHNOLOGY_STUDY failed");
}
OpHandler(CMSG_TECHNOLOGY_SPEEDUP)
{
	GS2C_Technology_SpeedUp msg;
    C2GS_Technology_SpeedUp req; pack >> req;
    m_Technology->SpeedUp(*this,req.id(),msg);
	Send(pb::SMSG_TECHNOLOGY_SPEEDUP_RSP,msg);
}
