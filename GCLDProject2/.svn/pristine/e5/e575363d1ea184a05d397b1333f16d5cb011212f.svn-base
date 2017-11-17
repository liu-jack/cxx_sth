

#include "../object/Player.h"
#include "reward/reward.h"
#include "Activity.pb.h"
#include "Table/ActivityMoneyToGoldTableMgr.h"
#include "Opcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "ActivityControl/ActivityMgr.h"

using namespace pb;
#ifdef _MMO_SERVER_
OpHandler(CMSG_TAKE_MONEY_TO_GOLD_AWARD)
{
	sActivityMgr.MarkTakeInActivity(pb::ACTIVITY_MONEY_TO_GOLD,GetGuid());
	C2GS_Recharge_rebate msg; pack >> msg;
	GS2C_Recharge_rebate_Rsp sendmsg;
	uint32 Id = msg.id();
	const ActivityRechargeRebate* rebate = sActivityRechargeRebateMgr.getActivityRechargeRebate(Id);
	if(rebate)
	{
		if(getCurrentActivityGold() >= rebate->NeedCharge())
		{
			if(!IsTakeReward(pb::PLAYER_FIELD_ACTIVITY_GOLD_TAKE_FLAG,Id))
			{
				TakeReward(pb::PLAYER_FIELD_ACTIVITY_GOLD_TAKE_FLAG,Id);
				sendmsg.set_ret(0);////success
				pb::StReward * reward = sendmsg.mutable_reward();
				reward->set_type(Reward::Coin_Gold);
				reward->set_value(rebate->Reward());
				sReward.Change(*this,reward->type(),reward->value());
			}
			else
			{
				sendmsg.set_ret(1);////has taken
			}
		}
		else
		{
			sendmsg.set_ret(2);///// gold is not enough
		}
	}
	else
	{
		sendmsg.set_ret(3);////common fail
	}
	Send(pb::SMSG_TAKE_MONEY_TO_GOLD_AWARD_RSP,sendmsg);
}
#endif
