#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "activity/ActMgr.pb.h"
#include "Country.pb.h"
#include "ActMgr.h"
#include "Investment/Investment.h"
#include "OccupyCity/OccupyCity.h"
#include "Personal/PersonalTask.h"
#include "Combat.pb.h"
#include "Defence/DefenceBorderland.h"

using namespace pb;

OpHandler(CMSG_ACTIVITY_ASK)
{
	GS2C_ActMgr_Begin msg;
	sActMgr.GetActivityList(this,msg);
	Send(SMSG_ACTIVITY_RET,msg);
}

OpHandler(CMSG_COUNTRY_TASK_INVEST)
{
	GS2C_Country_Task_Insvest msg;
	sInvestment.SaveTo(msg);
	Send(SMSG_COUNTRY_TASK_INVEST,msg);
}

OpHandler(CMSG_COUNTRY_INVEST_PLAYER)
{
	GS2C_Country_Invest_Player msg;
	sInvestment.SaveTo(msg,*this);
	Send(SMSG_COUNTRY_INVEST_PLAYER, msg);
}

OpHandler(CMSG_COUNTRY_TASK_INVEST_TASK_REWARD)
{
	C2GS_Country_Invest_Take_Reward req; pack >> req;
	GS2C_Country_Invest_Take_Reward msg;
	sInvestment.TakeReward(req.stage(),*this,msg);
	Send(SMSG_COUNTRY_INVEST_TASK_REWARD_RET, msg);
}

OpHandler(CMSG_COUNTRY_INVEST_DONATE)
{
	C2GS_Country_Invest_Donate req; pack >> req;
	GS2C_Country_Invest_Donate_Ret msg;
	sInvestment.GiveMoney(req.stage(), *this, msg);
	Send(SMSG_COUNTRY_INVEST_DONATE,msg);
}

OpHandler(CMSG_COUNTRY_TASK_INVEST_CLEAR_CD)
{
	GS2C_Country_Invest_Clear_CD_Ret msg;
	sInvestment.ClearInvestCD(*this,msg);
	Send(SMSG_COUNTRY_INVEST_CLEAR_CD_RET,msg);
}

OpHandler(CMSG_COUNTRY_TAST_OCCUPY)
{
	GS2C_Country_Task_Occupy msg;
	sOccupyCity.SaveTo(msg);
	Send(SMSG_COUNTRY_TAST_OCCUPY, msg);
}

OpHandler(CMSG_COUNTRY_OCCUPY_PLAYER)
{
	GS2C_Country_Occupy_Player msg;
	sOccupyCity.SaveTo(msg,*this);
	Send(SMSG_COUNTRY_OCCUPY_PLAYER, msg);
}

OpHandler(CMSG_COUNTRY_OCCUPY_TAKE_REWARD)
{
	C2GS_Country_Occupy_Take_Reward req; pack >> req;
	GS2C_Country_Occupy_Take_Reward msg;
	sOccupyCity._TakeRewards(*this,req.stage(),msg);
	Send(SMSG_COUNTRY_OCCUPY_TAKE_REWARD, msg);
}

OpHandler(CMSG_PERSONAL_TASK)
{
	GS2C_PersonalTask_Info msg;
	sPersonTask.SaveTo(msg,*this);
	Send(SMSG_PERSONAL_TASK, msg);
}

OpHandler(CMSG_PERSONAL_TASK_ONE)
{
	C2GS_PersonalTask_One req; pack >> req;
	OneTaskInfo msg;
	sPersonTask.SaveTo(msg,*this,req.taskid());
	Send(SMSG_PERSONAL_TASK_ONE, msg);
}

OpHandler(CMSG_PERSONAL_TAKE_AWARD)
{
	C2GS_PersonalTask_Take_Award req; pack >> req;
	GS2C_PersonalTask_Take_Award msg;
	sPersonTask._TakeRewards(*this,req.taskid(),msg);
	Send(SMSG_PERSONAL_TAKE_AWARD, msg);

	if(msg.issuccess())
	{
		OneTaskInfo onemsg;
		sPersonTask.SaveTo(onemsg,*this,req.taskid());
		Send(SMSG_PERSONAL_TASK_ONE, onemsg);
	}
}

OpHandler(CMSG_COUNTRY_TASK_DEFENCEBORDERLAND)
{
	GS2C_Country_Task_DefenceBorderland msg;
	sDefenceBorderland.SaveTo(msg);
	Send(SMSG_COUNTRY_TASK_DEFENCEBORDERLAND, msg);
}

OpHandler(CMSG_COUNTRY_DEFENCEBORDERLAND_PLAYER)
{
	GS2C_Country_DefenceBorderland_Player msg;
	sDefenceBorderland.SaveTo(msg,*this);
	Send(SMSG_COUNTRY_DEFENCEBORDERLAND_PLAYER, msg);
}

OpHandler(CMSG_COUNTRY_DEFENCEBORDERLAND_TAKE_REWARD)
{
	C2GS_Country_DefenceBorderland_Take_Reward req; pack >> req;
	GS2C_Country_DefenceBorderland_Take_Reward msg;
	sDefenceBorderland.TakeReward(req.stage(),*this,msg);
	Send(SMSG_COUNTRY_DEFENCEBORDERLAND_TAKE_REWARD, msg);
}

OpHandler(CMSG_DEFENCEBORDERLAND_CALL_UP)
{
	C2GS_Combat_Call_Request req; pack >> req;
	GS2C_Combat_Call_Request msg;

	sDefenceBorderland.MonsterCallRequest(*this,req,req.destcityid(),msg);

	Send(SMSG_DEFENCEBORDERLAND_CALL_UP,msg);
}