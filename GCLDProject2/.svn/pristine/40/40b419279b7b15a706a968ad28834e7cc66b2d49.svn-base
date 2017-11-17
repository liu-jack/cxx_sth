
#include "Opcode.pb.h"

#include "../object/Player.h"
#include "Quest.pb.h"
#include "memory_buffer/NetPack.h"
#include "reward/reward.h"
#include "Table/SignUpTableMgr.h"
#include "SignUp/SignUp.h"

using namespace pb;

//签到
OpHandler(CMSG_SIGN_UP)
{
	pb::GS2C_Sign_Res sendmsg;
	IntV3Vec retMap;
	uint32 day = GetDaysFromRegist();
	SignUp* sign_up = NULL;
	uint32 has_sign_day = GetDailySignDay();//签到总天数
	uint32 retroactive_day = GetDailyRetroactiveDay();//补签天数
	
	if(day > sSignUpTableMgr.GetDailySignLastLongDay())
	{
		sendmsg.set_ret(pb::SIGN_OTHER_REASON);
		Send(pb::SMSG_SIGN_UP_RSP,sendmsg);
		return;
	}
	
	SignUpStructMap& struct_map = m_SignUpLog->GetSignUpStructMap();
	if(LIKELY(struct_map[day].is_signed == 0))
	{
		m_SignUpLog->UpdateSigned(day,pb::MORMAL_SIGN);
		m_SignUpLog->SendToDB(*this,day);
	}
	else
	{
		if(has_sign_day >= day )
		{
			sendmsg.set_ret(pb::SIGN_CANT_RETROACTIVE);
			Send(pb::SMSG_SIGN_UP_RSP,sendmsg);
			return;
		}
		sign_up = sSignUpTableMgr.GetSignUp(retroactive_day + 1);
		ASSERT(sign_up != NULL);
		//按照天数扣除钻石
		if(!sReward.Change(*this,Reward::Coin_Gold,-sign_up->SupSignCost()))
		{
			sendmsg.set_ret(pb::SIGN_DIAMONDS_NOT_ENOUGH);
			Send(pb::SMSG_SIGN_UP_RSP,sendmsg);
			return;
		}
		AddDailyRetroactiveDay(1);

		m_SignUpLog->UpdateSigned(has_sign_day,pb::RETROACTIVE);
		m_SignUpLog->SendToDB(*this,has_sign_day);
	}
	AddDailySignDay(1);
	
	NLOG("sign day is [%u],retroactive day is [%u]",GetDailySignDay(),GetDailyRetroactiveDay());
	sign_up = sSignUpTableMgr.GetSignUp(has_sign_day + 1);
	ASSERT(sign_up != NULL);
	const std::string& base_reward = sign_up->BaseReward();
	const std::string& base_reward_num = sign_up->BaseRewardNum();
	if(LIKELY(sSignUpTableMgr.ParseItems(base_reward,base_reward_num,retMap)))
	{
		uint32 power_num = 1;
		if(GetVipLevel() >= static_cast<int32>(sign_up->NeedVip()))
		{
			if(sign_up->PowerNum() > power_num) power_num = sign_up->PowerNum();
		}
		for(IntV3Vec::const_iterator iterBeg = retMap.begin(); iterBeg != retMap.end(); ++iterBeg)
		{
			const IntV3& curItem = (*iterBeg);
			pb::StReward *reward = sendmsg.add_rewards();
			reward->set_type(curItem.first);
			reward->set_value(curItem.second * power_num);
		}
		CreateMailItem(pb::IR_SIGN_UP_BASE_REWARD_GET,retMap);
	}
	else
	{
		NLOG("NO BASEREWARD!!!! ParseItems failed ");
	}
	sendmsg.set_ret(pb::SIGN_SUCCESS);
	Send(pb::SMSG_SIGN_UP_RSP,sendmsg);
	return;
}

OpHandler(CMSG_EXTRA_REWARD)
{
	pb::C2GS_Take_Extra_Reward msg;
	pack >> msg;
	pb::GS2C_Take_Extra_Reward_res sendmsg;
	size_t no_get_reward = msg.day();
	SignUp* sign_up = sSignUpTableMgr.GetSignUp(no_get_reward);
	ASSERT(sign_up != NULL);
	//额外奖励
	const std::vector<int>& extra_reward = sign_up->GetExtraReward();
	const std::vector<int>& extra_reward_num = sign_up->GetExtraRewardNum();
	if(LIKELY(IsTakeDailySignExtraReward(no_get_reward)))
	{
		TakeDailySignExtraReward(no_get_reward);
		size_t i = 0;
		for(std::vector<int>::const_iterator iter = extra_reward.begin();iter != extra_reward.end() && i < extra_reward_num.size();++iter)
		{
			if(*iter == 0 || extra_reward_num[i] == 0 ) continue;
			pb::StReward *reward = sendmsg.add_rewards();
			reward->set_type(*iter);
			reward->set_value(extra_reward_num[i]);
			sReward.Change(*this,reward->type(),reward->value());
			++i;
		}
		sendmsg.set_ret(pb::EX_GET_SUCCESS);
		Send(pb::SMSG_EXTRA_REWARD_RSP,sendmsg);
		return;
	}
	else
	{
		sendmsg.set_ret(pb::EX_HAS_GET_REWARD);
		Send(pb::SMSG_EXTRA_REWARD_RSP,sendmsg);
		return;
	}
}

OpHandler(CMSG_UPDATE_DATE)
{
	pb::GS2C_Update_date sendmsg;
	uint32 day = GetDaysFromRegist();
	sendmsg.set_day(day);
	SignUpStructMap& struct_map = m_SignUpLog->GetSignUpStructMap();
	sendmsg.set_is_signed(struct_map[day].is_signed);
	Send(pb::SMSG_UPDATE_DATE_RSP,sendmsg);
	return;
}
