/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "object/Player.h"
#include "Opcode.pb.h"
#include "GovAffairsHandler.h"
#include "table/GovAffairsTableMgr.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "CrossLogic/PlayerZhengWuDB.h"
#include "GovernmentAffairs/GovAffairsLog.h"

using namespace pb;

OpHandler(CMSG_ZHENGWU_CHOOSE_WIDGET)
{
	pb::GS2C_Show_Dialog msg;
	sGovAffairsTableMgr.GetRandomTextId(this,msg);
	Send(pb::SMSG_ZHENGWU_CHOOSE_WIDGET_RSP,msg);
#ifdef _MMO_SERVER_
	m_GovAffairsLog->loadFrom(msg);
	PlayerZhengWuDb::SendUpdateInfoToDb(*this);
#endif
}

OpHandler(CMSG_ZHENGWU_TAKE_REWARD)
{
	pb::C2GS_Take_Reward msg; pack >> msg;
	sGovAffairsTableMgr.TakeReward(this,msg);
#ifdef _MMO_SERVER_
	m_GovAffairsLog->reset();
	PlayerZhengWuDb::SendUpdateInfoToDb(*this);
#endif
}

OpHandler(CMSG_ZHENGWU_TAKE_MINZHONG_REWARD)
{
	pb::GS2C_Take_MinZhong_Reward_Rsp msg;
	sGovAffairsTableMgr.TakeMinZhongReward(this,msg);
	Send(pb::SMSG_ZHENGWU_TAKE_MINZHONG_REWRAD_RSP,msg);
}