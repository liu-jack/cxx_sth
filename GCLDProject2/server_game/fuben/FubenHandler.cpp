#include "../object/Player.h"			//���ر���
#include "FubenLog.h"

#include "memory_buffer/NetPack.h"		//��Ϣ�����ر���
#include "utility/MsgTool.h"			//���ر���

#include "Opcode.pb.h"				//csָ���ö��
#include "Fuben.pb.h"				//ָ����Ϣ������
#include "FubenStage.h"
#include "FubenDataMgr.h"
#include "../reward/reward.h"		//��������
#include "Combat.pb.h"
#include "ServerOpcode.pb.h"

using namespace pb;

#define  PRE_JUDGE_INIT_FUBEN_LOG  if(!m_fubenLog.get()) { return;}


//������и�����Ϣ
OpHandler(CMSG_ALL_FUBEN_INFO)
{
    PRE_JUDGE_INIT_FUBEN_LOG;
    GS2C_Fuben_All_Info msg;
    m_fubenLog->SaveTo(this,msg);
    Send(SMSG_ALL_FUBEN_INFO, msg);
}

//����ؿ�
OpHandler(CMSG_ENTER_STAGE)
{
    PRE_JUDGE_INIT_FUBEN_LOG;
    C2GS_Enter_Stage msg; pack >> msg;

    const uint32 clientStageID = msg.stage_id();
    
    //�ռ��佫��������
    std::vector<uint32> heroIds;
    for (int i = 0; i < msg.hero_id_size(); ++i)
    {
        heroIds.push_back(msg.hero_id(i));
    }
    m_fubenLog->EnterStage(*this, clientStageID, heroIds);
}

//ʰȡ��Դ
OpHandler(CMSG_TAKE_RESOURCE)
{
//     PRE_JUDGE_INIT_FUBEN_LOG;
//     C2GS_TakeResource msg; pack >> msg;
// 	GS2C_TakeResource sendmsg;
// 	IntPairVec reward;
// 	m_fubenLog->TakeResource(*this, msg.stage_id(),sendmsg,reward);
// 	Send(pb::SMSG_TAKE_RESOURCE_RSP,sendmsg);//��ʾ��Դ�����Ľ���
// 	sReward.Change(*this,reward);//����
}

//������Դ
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
///��ȡ��ǰ��Զ����Ϣ
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
