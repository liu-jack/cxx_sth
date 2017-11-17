#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "BaoQi.pb.h"
#include "PlayerBaoQiLog.h"
#include "Table/BaoQiTableMgr.h"
#include "../CrossLogic/PlayerBaoQiLogDB.h"
using namespace pb;

OpHandler(CMSG_BAO_QI_ALL_INFO)
{
    GS2C_BaoQi_All_Info msg;
    m_BaoQiLog->SaveTo(msg);
    Send(SMSG_BAO_QI_ALL_INFO, msg);
}
OpHandler(CMSG_BAO_QI_CREATE)
{
    C2GS_BaoQi_Create req; pack >> req;
	GS2C_BaoQi_Create sendmsg;
    sendmsg.set_iscreatesuccess(m_BaoQiLog->Create(*this, req.id(),sendmsg));
	Send(SMSG_BAO_QI_CREATE,sendmsg);
}
OpHandler(CMSG_BAO_QI_BUY_ITEM)
{
    C2GS_BaoQi_Buy_Item req; pack >> req;

    m_BaoQiLog->BuyDrawing(*this, req.id());

}
OpHandler(CMSG_BAO_QI_ADD_EXP)
{
    C2GS_BaoQi_AddExp req; pack >> req;

    int addExp = m_BaoQiLog->AddExp(*this, req.id());

    PlayerBaoQiLogDB::SendAllInfoToDb(*this, *m_BaoQiLog);

    GS2C_BaoQi_Exp_Inc_History msg;
    msg.set_id(req.id());
    msg.add_inc_val_lst(addExp);
    Send(SMSG_BAO_QI_EXP_INC_HISTORY, msg);
	UpdateAllDailyQuestInfo(BAOQI_ZHUZAO,1,0);//每日任务
}
OpHandler(CMSG_BAO_QI_LEVEL_UP)
{
    C2GS_BaoQi_LevelUp req; pack >> req;

    m_BaoQiLog->LevelUp(*this, req.id());
}
OpHandler(CMSG_BAO_QI_STAR_UP)
{
    C2GS_BaoQi_StarUp req; pack >> req;

    m_BaoQiLog->StarUp(*this, req.id());
}
OpHandler(CMSG_BAO_QI_PUNCH)
{
    C2GS_BaoQi_Punch req; pack >> req;

    if (m_BaoQiLog->Punch(*this, req.id()))
    {
        NetPack pack(SMSG_BAO_QI_PUNCH);
        Send(pack);
    }
}
OpHandler(CMSG_BAO_QI_KNIT_GEM)
{
    C2GS_BaoQi_KnitGem req; pack >> req;

    if (m_BaoQiLog->KnitGem(*this, req.id(), req.gem()))
    {
        NetPack pack(SMSG_BAO_QI_KNIT_GEM);
        Send(pack);
    }
}
OpHandler(CMSG_BAO_QI_UNKNIT_GEM)
{
    C2GS_BaoQi_UnKnitGem req; pack >> req;

    if (m_BaoQiLog->UnKnitGem(*this, req.id()))
    {
        NetPack pack(SMSG_BAO_QI_UNKNIT_GEM);
        Send(pack);
    }
}