#include "object/Player.h"
#include "memory_buffer/NetPack.h"
#include "Opcode.pb.h"
#include "TeachMap.pb.h"
#include "TeachMap/TeachMapMgr.h"
#include "GameServer.pb.h"

OpHandler(CMSG_ASK_TEACH_REC)
{
	pb::GS2C_Get_Teach_Rec sendmsg;
	sTeachMapMgr.DealAskTeachRec(this,sendmsg);
	Send(pb::SMSG_ASK_TEACH_REC, sendmsg);
}

OpHandler(CMSG_REQUEST_TEACH)
{
	pb::C2GS_Request_Teach msg; pack >> msg;
	pb::GS2C_Answer_Teach sendMsg;
 	sTeachMapMgr.DealRequestTeachMap(this, msg, sendMsg);
	Send(pb::SMSG_ANSWER_TEACH,sendMsg);
}

OpHandler(CMSG_RUSH_TEACH)
{
	pb::C2GS_Rush_Teach msg; pack >> msg;
	pb::GS2C_Rush_Teach_Ret sendMsg;
	sTeachMapMgr.DealRushTeachMap(this, msg, sendMsg);
	Send(pb::SMSG_RUSH_TEACH_RET,sendMsg);

	if(sendMsg.ret() == pb::Teach_Common_success)	//突进成功，发送更新城市消息
	{
		pb::GS2C_Get_Teach_Rec sendmsg;
		sTeachMapMgr.DealAskTeachRec(this,sendmsg);
		Send(pb::SMSG_ASK_TEACH_REC, sendmsg);
	}
}

OpHandler(CMSG_FRESH_TEACH_TASK)
{
	sTeachMapMgr.FreshTeachTask(this);

	pb::NUll_Ret retmsg;
	Send(pb::SMSG_NULL_RET,retmsg);
}