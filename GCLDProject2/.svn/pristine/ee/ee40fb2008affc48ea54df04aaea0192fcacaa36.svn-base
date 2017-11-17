/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "Opcode.pb.h"
#include "Quest.pb.h"
#include "../object/Player.h"
#include "DailyQuest/DailyQuest.h"
#include "Loot/LootManager.h"
#include "Loot/LootList.h"
#include "DailyQuest/Table/DailyQuestTableMgr.h"
#include "reward/reward.h"
#include "memory_buffer/NetPack.h"

OpHandler(CMSG_DAILY_QUEST_ALL_INFO)
{
	pb::GS2C_Daily_Quest_All_Info msg;
	m_DailyQuest->SaveTo(*this,msg);
	Send(pb::SMSG_DAILY_QUEST_ALL_INFO_RSP,msg);
}

OpHandler(CMSG_DAILY_QUEST_TAKE_REWARD)
{
	pb::C2GS_Daily_Quest_Reward msg;
	pack >> msg;
	pb::GS2C_Daily_Quest_Reward_Res reward_msg;
	if(LIKELY(IsTakeDailyQuestReward(msg.reward_id())))//没有领奖
	{
		TakeDailyQuestReward(msg.reward_id());//设置位标志
		//给奖励
		
		LootList lootlist;
		if(!sDailyQuestTableMgr.GetQuestReward(*this,LOOTTYPE_DAILY_QUEST_REWARD,lootlist,msg.reward_id()))
		{
			Send(pb::SMSG_DAILY_QUEST_TAKE_REWARD_RSP,reward_msg);
			return;
		}
		const ItemMap& Items = lootlist.MutableItem();
		const MoneyMap& moneys = lootlist.MutableMoneys();
		for(MoneyMap::const_iterator iter = moneys.begin();iter != moneys.end();++iter)
		{
			pb::StReward* reward = reward_msg.add_rewards();
			sReward.Change(*this,iter->first,iter->second );
			reward->set_type(iter->first);
			reward->set_value(iter->second);
		}
		for(ItemMap::const_iterator iter = Items.begin();iter!= Items.end();++iter)
		{
			pb::StReward* reward = reward_msg.add_rewards();
			sReward.Change(*this,iter->first,iter->second);
			reward->set_type(iter->first);
			reward->set_value(iter->second);
		}
		Send(pb::SMSG_DAILY_QUEST_TAKE_REWARD_RSP,reward_msg);
		return;
	}
	else
	{
		Send(pb::SMSG_DAILY_QUEST_TAKE_REWARD_RSP,reward_msg);
		return;
	}
}
