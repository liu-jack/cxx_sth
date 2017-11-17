
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "Opcode.pb.h"

#include "../object/Player.h"
#include "GrowUpQuest/Table/GrowUpQuestTableMgr.h"
#include "GrowUpQuest/GrowUpQuest.h"
#include "Quest.pb.h"
#include "memory_buffer/NetPack.h"
#include "reward/reward.h"


using namespace pb;

OpHandler(CMSG_GROW_UP_QUEST_ALL_INFO)
{
	GS2C_Grow_Up_Quest_All_Info msg;
	m_GrowUpQuest->SaveTo(*this,msg);
	Send(SMSG_GROW_UP_QUEST_ALL_INFO_RSP,msg);
}	

OpHandler(CMSG_GROW_UP_QUEST_TAKE_REWARD)
{
	C2GS_Grow_Up_Quest_Reward msg;
	pack >> msg;
	GS2C_Grow_Up_Quest_Reward_Res sendmsg;
	IntPairVec vec;
	if(m_GrowUpQuest->CanTakeReward(msg.type(),msg.quest_id()))
	{
		if(LIKELY(msg.type() == pb::REWARD_NORMAL))
		{
			if(sGrowUpQuestTableMgr.GetGrowUpProtoRewardById(msg.quest_id(),&vec))
			{
				sReward.Change(*this,vec);
				for(IntPairVec::iterator iter = vec.begin();iter != vec.end();++iter)
				{
					pb::StReward* reward = sendmsg.add_rewards();
					reward->set_type(iter->first);
					reward->set_value(iter->second);
				}
			}

			GrowUpQuestStructMap& grow_up_quest_struct =  m_GrowUpQuest->GetGrowUpQuestStructMap();
			grow_up_quest_struct[msg.quest_id()].is_take_reward = 1;
			PlayerGrowUpQuestDb::SendUpdateInfoToDb(*this,msg.quest_id(),grow_up_quest_struct[msg.quest_id()],pb::DATA_UPDATE);
			m_GrowUpQuest->SendUpdateDataToClient(*this,msg.quest_id());
		}
		else
		{
			if(sGrowUpQuestTableMgr.GetGrowUpGroupRewardById(msg.quest_id(),&vec))
			{
				sReward.Change(*this,vec);
				for(IntPairVec::iterator iter = vec.begin();iter != vec.end();++iter)
				{
					pb::StReward* reward = sendmsg.add_rewards();
					reward->set_type(iter->first);
					reward->set_value(iter->second);
				}
			}

			GrowUpQuestGroupStructMap& grow_up_quest_group_struct = m_GrowUpQuest->GetGrowUpQuestGroupStructMap();
			grow_up_quest_group_struct[msg.quest_id()].is_take_group_reward = 1;
			PlayerGrowUpQuestDb::SendUpdateGroupInfoToDb(*this,msg.quest_id(),grow_up_quest_group_struct[msg.quest_id()],pb::DATA_UPDATE);
			m_GrowUpQuest->SendUpdateGroupDataToClient(*this,msg.quest_id());
		}
		Send(SMSG_GROW_UP_QUEST_TAKE_REWARD_RSP,sendmsg);
		return;
	}
	else
	{
		Send(SMSG_GROW_UP_QUEST_TAKE_REWARD_RSP,sendmsg);
		return;
	}
}