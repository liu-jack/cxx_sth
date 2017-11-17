/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "Table/ActivityXiLianTableMgr.h"
#include "Activity.pb.h"
#include "reward/reward.h"
#include "../object/Player.h"
#include "Opcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "ActivityControl/ActivityMgr.h"
using namespace pb;


OpHandler(CMSG_ACTIVITY_XILIAN_TAKE_REWARD)
{
	sActivityMgr.MarkTakeInActivity(pb::ACTIVITY_XILIAN,GetGuid());
	C2GS_XILIAN_TAKE_REWARD msg;
	pack >> msg;
	GS2C_XILIAN_TAKE_REWARD_RSP sendmsg;
	const uint32 id = static_cast<const uint32>(msg.reward_type());
	ActivityXilian* xilian = sActivityXilianMgr.GetActivityXilian(id);
	if(IsXilianActive())
	{
		if(xilian)
		{
			if(GetActivityXiLianValue() < xilian->NeedTimes())
			{
				sendmsg.set_ret(pb::XILIAN_NOT_ENOUGH_TIME);
				sendmsg.set_reward_type(msg.reward_type());
			}
			else
			{
				if(IsTakeXilianReward(id))
				{
					TakeXilianReward(id);
					const IntPairVec& pair_vec = xilian->GetRewardList();
					for(IntPairVec::const_iterator iter = pair_vec.begin();iter != pair_vec.end();++iter)
					{
						pb::StReward * reward = sendmsg.add_reward();
						reward->set_type(iter->first);
						reward->set_value(iter->second);
					}
					sReward.Change(*this,pair_vec);
					sendmsg.set_ret(pb::XILIAN_SUCCESS);
					sendmsg.set_reward_type(msg.reward_type());
				}
				else
				{
					sendmsg.set_ret(pb::XILIAN_HAS_TAKE);
					sendmsg.set_reward_type(msg.reward_type());
				}
			}
		}
		Send(pb::SMSG_ACTIIVTY_XILIAN_TAKE_REWARD_RSP,sendmsg);
	}
}


OpHandler(CMSG_ACTIVITY_XILIAN_TAKE_REWARD_OF_TEN)
{
	GS2C_XILIAN_TAKE_TEN_REWARD msg;
	uint32 xilian_ten = GetAttrUint(pb::PLAYER_FIELD_XILIAN_TEN_TIMES);
	uint32 count = xilian_ten / (GET_BASE_DEF_UINT(pb::BD_ACTIVITY_XILIAN_EXTRA_REWARD_TIMES));
	xilian_ten %= (GET_BASE_DEF_UINT(pb::BD_ACTIVITY_XILIAN_EXTRA_REWARD_TIMES));
	SetValueInt(pb::PLAYER_FIELD_XILIAN_TEN_TIMES,xilian_ten);
	AddCurrency(pb::IR_CYCLE_GET,eExtrmePurify,count);
	msg.set_count(count);
	Send(pb::SMSG_ACTIVITY_XILIAN_TAKE_REWARD_OF_TEN_RSP,msg);
}