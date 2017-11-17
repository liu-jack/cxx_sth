
#include "../object/Player.h"
#include "../session/PlayerSession.h"
#include "../GameServer.h"

#include "memory_buffer/NetPack.h"
#include "../server_client/DbProxyServerClient.h"
#include "utility/TimeParser.h"
#include "data/PayItemTable.h"
#include "../object/PlayerDataManager.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "Web.pb.h"
#include "GameServer.pb.h"
#include "Fuben.pb.h"
#include "memory_buffer/NetPack.h"
#include "reward/reward.h"
#include "ActivityControl/ActivityMgr.h"
using namespace pb ;
#define MONTH_CARD_NORMAL	1
#define MONTH_CARD_ADVANCE	2
OpHandler(CMSG_TAKE_FIRST_PAY_REWARD)
{
#ifdef _MMO_SERVER_
	if(TestFlag(pb::PLAYER_FLAG_HAD_FIRST_PAY))
	{
		if(!TestFlag(pb::PLAYER_FLAG_HAD_TAKE_FIRST_PAY_REWARD))
		{
			IntV3Vec retMap;
			std::map<int,int> priMap;
			pb::StRewards rewardItems;
			if (sPlayerDataMgr.GetFirstPayRewards(retMap,priMap))
			{
				for ( IntV3Vec::const_iterator iterBeg = retMap.begin(); iterBeg != retMap.end(); ++iterBeg)
				{
					const IntV3& curItem = (*iterBeg);
					pb::StReward*  rwds = rewardItems.add_rewards();
					rwds->set_type(curItem.first);
					rwds->set_value(curItem.second);
				}
				CreateMailItem(pb::IR_IAP_REWARD_GET,retMap);
				std::map<int,int>::iterator iterPri = priMap.begin();
				for (; iterPri != priMap.end(); ++iterPri)
				{
					SetPlayerPrivilege((uint32)iterPri->first, (uint32)iterPri->second);
				}
				SetFlag(pb::PLAYER_FLAG_HAD_TAKE_FIRST_PAY_REWARD);
			}
			Send(pb::SMSG_TAKE_FIRST_PAY_REWARD_RSP,rewardItems);
		}
	}
#endif
}

OpHandler(CMSG_TAKE_VIP_REWARD)
{
	int forVip = 0;
	pack >> forVip ;
	if (IsTakeVipReward(forVip))
	{
		SendErrorCode(pb::ErrAlreadyTake);
	}
	else
	{
		if(TakedVipReward(forVip))
		{
#ifdef _MMO_SERVER_
			IntV3Vec retMap;
			pb::StRewards rewardItems;
			if (sPlayerDataMgr.GetRewardItems(forVip,retMap))
			{
				for ( IntV3Vec::const_iterator iterBeg = retMap.begin(); iterBeg != retMap.end(); ++iterBeg)
				{
					const IntV3& curItem = (*iterBeg);
					pb::StReward*  rwds = rewardItems.add_rewards();
					rwds->set_type(curItem.first);
					rwds->set_value(curItem.second);
				}
				CreateMailItem(pb::IR_VIP_REWARD_GET,retMap);
			}
			Send(pb::SMSG_TAKE_VIP_REWARD,rewardItems);
#endif
		}
		else
		{
			SendErrorCode(pb::ErrCommonFail);
		}
		
	}
}
#ifdef _MMO_SERVER_
OpHandler(CMSG_TAKE_MONTH_CARD_REWARD)
{
	pb::C2GS_Take_Month_Card_Reward msg;
	pack >> msg;
	pb::GS2C_Take_Month_Card_Reward_Rsp sendmsg;
	uint32 lastDays = 0;
	IntPairVec vec;
	sendmsg.set_card_type(msg.card_type());
	if(msg.card_type() == MONTH_CARD_NORMAL)
	{
		time_t normal_card_buy_time = GetBuyNormalMonthCardTime();
		if(sPlayerDataMgr.getMonthCardRewardAndLastDays(msg.card_type(),vec,lastDays) == 0)
		{
			if(sOS.TimeSeconds() > static_cast<uint64>(normal_card_buy_time + (lastDays-1) * ONE_DAY_SECOND + sActivityMgr.getDiffSecondFromNextDayZero(normal_card_buy_time)))
			{
				sendmsg.set_ret(2);
				Send(pb::SMSG_TAKE_MONTH_CARD_REWARD_RSP,sendmsg);
				return;
			}
			if(!sOS.IsSameDay(sOS.TimeSeconds(),GetTakeNormalMonthCardRewardTime()))
			{
				sReward.Change(*this,vec);
				for(IntPairVec::iterator it = vec.begin();it != vec.end();++it)
				{
					pb::StReward *reward = sendmsg.add_reward();
					reward->set_type(it->first);
					reward->set_value(it->second);
				}
				SetTakeNormalMonthCardRewardTime(sOS.TimeSeconds());
				sendmsg.set_ret(0);////success
			}
			else
			{
				sendmsg.set_ret(1);
			}
		}
		else
		{
			sendmsg.set_ret(3);
		}
	}
	else if(msg.card_type() == MONTH_CARD_ADVANCE)
	{
		time_t advance_card_buy_time = GetBuyAdvanceMonthCardTime();
		if(sPlayerDataMgr.getMonthCardRewardAndLastDays(msg.card_type(),vec,lastDays) == 0)
		{
			if(sOS.TimeSeconds() > static_cast<uint64>(advance_card_buy_time + (lastDays-1) * ONE_DAY_SECOND + sActivityMgr.getDiffSecondFromNextDayZero(advance_card_buy_time)))
			{
				sendmsg.set_ret(2);
				Send(pb::SMSG_TAKE_MONTH_CARD_REWARD_RSP,sendmsg);
				return;
			}
			if(!sOS.IsSameDay(sOS.TimeSeconds(),GetTakeAdvanceMonthCardRewardTime()))
			{
				sReward.Change(*this,vec);
				for(IntPairVec::iterator it = vec.begin();it != vec.end();++it)
				{
					pb::StReward *reward = sendmsg.add_reward();
					reward->set_type(it->first);
					reward->set_value(it->second);
				}
				SetTakeAdvanceMonthCardRewardTime(sOS.TimeSeconds());
				sendmsg.set_ret(0);///success
			}
			else
			{
				sendmsg.set_ret(1);
			}
		}
		else
		{
			sendmsg.set_ret(3);
		}
	}
	Send(pb::SMSG_TAKE_MONTH_CARD_REWARD_RSP,sendmsg);
	return;
}
#endif