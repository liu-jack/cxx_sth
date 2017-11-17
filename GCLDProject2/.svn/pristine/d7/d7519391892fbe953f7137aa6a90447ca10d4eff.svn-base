
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "object/Player.h"
#include "WannaBeStronger/WannaBeStrongerLog.h"
#include "WannaBeStronger/Table/WannaBeStrongerTable.h"
#include "CrossLogic/PlayerGeneralRewardDB.h"
#include "reward/reward.h"
#include "Opcode.pb.h"
#include "character/CharacterStorage.h"
#include "BaoQi/PlayerBaoQiLog.h"
#include "item/ItemArray.h"

WannaBeStrongerLog::WannaBeStrongerLog()
{
	const StrongerQuestMap& stronger_quest_map = sStrongerTableMgr.GetStrongerQuestMap();
	for(StrongerQuestMap::const_iterator iter = stronger_quest_map.begin();iter != stronger_quest_map.end();++iter)
	{
		WannaBeStrongerInfoMap::iterator itera = wanna_be_stronger_map_.find(iter->second->QuestGroup());
		if(itera != wanna_be_stronger_map_.end())
		{
			itera->second.wanna_be_strong_base_info_[iter->second->QuestId()] = WannaBeStrongBaseInfo_s(0,iter->second->Val1(),iter->second->Val2(),iter->second->QuestType());
		}
		else
		{
			wanna_be_stronger_map_[iter->second->QuestGroup()] =  BeStrongerInfo(iter->second->QuestId(),0,iter->second->Val1(),iter->second->Val2(),iter->second->QuestType());
		}
	}
}

void WannaBeStrongerLog::SaveTo(Player& player,pb::GS2C_WannaBeStrongerAllInfo& msg)
{
	for(WannaBeStrongerInfoMap::iterator iter = wanna_be_stronger_map_.begin();iter != wanna_be_stronger_map_.end();++iter)
	{
		pb::WannaBeStrongerInfo* info = msg.add_info();
		info->set_group_id(iter->first);
		bool flag = true;
		for(WannaBeStrongBaseInfoMap::iterator iter1= iter->second.wanna_be_strong_base_info_.begin();iter1 != iter->second.wanna_be_strong_base_info_.end();++iter1)
		{
			pb::WannaBeStrongerBaseInfo* base_info = info->add_base_info();
			base_info->set_quest_id(iter1->first);
			if(iter1->second.quest_type_ == WANNA_BE_S_TYPE_CHARACTER_ZHAO_MU)
			{
				uint32 char_count = player.m_characterStorage->GetCountBAMatchLevel(iter1->second.need_value2_);
				iter1->second.cur_value_ = char_count;
				bool finished = (char_count >= iter1->second.need_value1_);
				flag &= finished;
			}
			else if(iter1->second.quest_type_ == WANNA_BE_S_TYPE_UNLOCK_CHARACTER)
			{
				uint32 char_size = player.m_characterStorage->GetCountOfContainer();
				iter1->second.cur_value_ = char_size;
				bool finished = (char_size >= iter1->second.need_value1_);
				flag &= finished;
			}
			else if(iter1->second.quest_type_ == WANNA_BE_S_TYPE_EQUIP_QUAILTY)
			{
				uint32 equip_count = player.m_bag->GetGreaterQualityEquipOfSuitCount(iter1->second.need_value2_);
				iter1->second.cur_value_ = equip_count;
				bool finished = (equip_count >= iter1->second.need_value1_);
				flag &= finished;
			}
			else if(iter1->second.quest_type_ == WANNA_BE_S_TYPE_SHENQI_LEVEL)
			{
				uint32 count_of_baoqi = player.m_BaoQiLog->GetCountOfBaoQi(iter1->second.need_value2_);
				iter1->second.cur_value_ = count_of_baoqi;
				bool finished = (count_of_baoqi >= iter1->second.need_value1_);
				flag &= finished;
			}
			else if(iter1->second.quest_type_ == WANNA_BE_S_TYPE_EQUIP_MIJI)
			{
				uint32 equip_count = player.m_bag->GetHasMijiEquipCount(iter1->second.need_value2_);
				iter1->second.cur_value_ = equip_count;
				bool finished = (equip_count >= iter1->second.need_value1_);
				flag &= finished;
			}
			base_info->set_cur_value(iter1->second.cur_value_);
			base_info->set_need_value1(iter1->second.need_value1_);
			base_info->set_need_value2(iter1->second.need_value2_);
			base_info->set_quest_type(iter1->second.quest_type_);
		}
		iter->second.is_achieve_ = flag;
		info->set_is_achieve(iter->second.is_achieve_);
		info->set_is_take_reward(iter->second.is_take_reward_);
		//WannaBeStrongerDB::SendUpdateInfoToDb(player,*info);
	}
}


void WannaBeStrongerLog::SaveTo(const uint32 groupId,const BeStrongerInfo &ref,pb::WannaBeStrongerInfo& msg)
{
	msg.set_group_id(groupId);
	msg.set_is_achieve(ref.is_achieve_);
	msg.set_is_take_reward(ref.is_take_reward_);
// 	for(WannaBeStrongBaseInfoMap::const_iterator iter = ref.wanna_be_strong_base_info_.begin();
// 		iter != ref.wanna_be_strong_base_info_.end();++iter)
// 	{
// 		pb::WannaBeStrongerBaseInfo* base_info = msg.add_base_info();
// 		base_info->set_quest_id(iter->first);
// 		base_info->set_cur_value(iter->second.cur_value_);
// 		base_info->set_need_value1(iter->second.need_value1_);
// 		base_info->set_need_value2(iter->second.need_value2_);
// 		base_info->set_quest_type(iter->second.quest_type_);
// 	}
}

void WannaBeStrongerLog::LoadFrom(const pb::GxDB_WannaBeStrongerInfoAllInfo& msg)
{
	for(int i = 0;i < msg.info_size();++i)
	{
		const pb::WannaBeStrongerInfo &info = msg.info(i);
		WannaBeStrongerInfoMap::iterator itera = wanna_be_stronger_map_.find(info.group_id());
		if(LIKELY(itera != wanna_be_stronger_map_.end()))
		{
			itera->second.is_achieve_ = info.is_achieve();
			itera->second.is_take_reward_ = info.is_take_reward();
		}
		else
		{
			wanna_be_stronger_map_[info.group_id()] =  BeStrongerInfo();
		}
	}
}

void WannaBeStrongerLog::UpdateWannaBeStrongerData(Player& player)
{
	player.Send(pb::SMSG_WANNA_BE_STRONGER_UPDATE);
}


void WannaBeStrongerLog::TakeRewards(Player& player,const uint32 groupId,pb::GS2C_WannaBeStrongerRewardRes& msg)
{
	const IntPairVec* reward = sStrongerTableMgr.GetRewards(groupId);
	if(reward == NULL) return;
	BeStrongerInfo& ref = wanna_be_stronger_map_[groupId];
	if(ref.is_achieve_ && !ref.is_take_reward_)
	{
		for(IntPairVec::const_iterator iter = reward->begin();iter != reward->end();++iter)
		{
			pb::StReward * reward_ = msg.add_rewards();
			reward_->set_type(iter->first);
			reward_->set_value(iter->second);
		}
		ref.is_take_reward_ = true;
	}
	player.Send(pb::SMSG_WANNA_BE_STRONGER_TAKE_REWARD_RSP,msg);
	WannaBeStrongerDB::SendUpdateInfoToDb(player,groupId,ref);
	sReward.Change(player,*reward);
}


