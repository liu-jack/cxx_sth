#include "../object/Player.h"			//（必备）
#include "FubenLog.h"

#include "memory_buffer/NetPack.h"		//消息包（必备）
#include "utility/MsgTool.h"			//（必备）

#include "Opcode.pb.h"				//cs指令号枚举
#include "Fuben.pb.h"				//指令消息体内容
#include "FubenStage.h"
#include "FubenDataMgr.h"
#include "../reward/reward.h"		//副本奖励
#include "Combat.pb.h"
#include "ServerOpcode.pb.h"

using namespace pb;

#define  PRE_JUDGE_INIT_FUBEN_LOG  if(!m_fubenLog.get()) { return;}


//获得所有副本信息
OpHandler(CMSG_ALL_FUBEN_INFO)
{
    PRE_JUDGE_INIT_FUBEN_LOG;
    GS2C_Fuben_All_Info msg;
    m_fubenLog->SaveTo(this,msg);
    Send(SMSG_ALL_FUBEN_INFO, msg);
}

//进入关卡
OpHandler(CMSG_ENTER_STAGE)
{
    PRE_JUDGE_INIT_FUBEN_LOG;
    C2GS_Enter_Stage msg; pack >> msg;

    const uint32 clientStageID = msg.stage_id();
    
    //收集武将上阵数据
    std::vector<uint32> heroIds;
    for (int i = 0; i < msg.hero_id_size(); ++i)
    {
        heroIds.push_back(msg.hero_id(i));
    }
    m_fubenLog->EnterStage(*this, clientStageID, heroIds);
}

//拾取资源
OpHandler(CMSG_TAKE_RESOURCE)
{
//     PRE_JUDGE_INIT_FUBEN_LOG;
//     C2GS_TakeResource msg; pack >> msg;
// 	GS2C_TakeResource sendmsg;
// 	IntPairVec reward;
// 	m_fubenLog->TakeResource(*this, msg.stage_id(),sendmsg,reward);
// 	Send(pb::SMSG_TAKE_RESOURCE_RSP,sendmsg);//显示资源副本的奖励
// 	sReward.Change(*this,reward);//升级
}

//重置资源
OpHandler(CMSG_RESET_RESOURCE)
{
//     PRE_JUDGE_INIT_FUBEN_LOG;
//     C2GS_ResetResource msg; pack >> msg;
// 	GS2C_ResetResource sendmsg;
//     m_fubenLog->ResetResource(*this, msg.stage_id(),sendmsg);
// 	Send(pb::SMSG_RESET_RESOURCE_RSP,sendmsg);
}

OpHandler(CMSG_Select_Crusade)
{
	PRE_JUDGE_INIT_FUBEN_LOG;
	C2GS_Select_Crusade msg; pack >> msg;
	GS2C_Select_Ret retMsg;

	m_fubenLog->SelectCrusade(*this, msg.crusade_id(),retMsg);

	Send(SMSG_Select_Ret, retMsg);
}
///获取当前的远征信息
OpHandler(CMSG_Current_Crusade)
{
	GS2C_Combat_Crusade msg;
	m_fubenLog->SaveTo(msg);
	Send(SMSG_Current_Crusade, msg);
}
OpHandler(CMSG_TAKE_BRANCH_FUBEN_STAR)
{
	C2GS_take_branch_fuben_star msg; pack >> msg;
	GS2C_take_branch_fuben_star_rsp sendmsg;
	m_fubenLog->TakeBranchReward(*this,msg.branch_stageid(),sendmsg);
	Send(SMSG_TAKE_BRANCH_FUBEN_STAR_RSP, sendmsg);
}
