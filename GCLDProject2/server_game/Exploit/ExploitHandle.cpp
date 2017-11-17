/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "object/Player.h"

#include "item/LogicItemProto.h"
#include "item/ItemManager.h"
#include "memory_buffer/NetPack.h"
#include "Rank.pb.h"
#include "Opcode.pb.h"
#include "Enum.pb.h"
#include "Item.pb.h"
#include "BaseDefine.pb.h"
#include "ExploitMgr.h"
#include "../reward/reward.h"
#include "System.h"
#include "ExploitMgr.h"
#include "Technology/PlayerTechnology.h"
#include "../session/PlayerPool.h"
#include "character/CharacterStorage.h"
#include "seige_force/SeigeForceLog.h"
#include "BaoQi/PlayerBaoQiLog.h"
using namespace pb;
#define SHOW_MILITARY_NUMBER_PLAYER 200

//功勋榜
OpHandler(CMSG_EXPLOIT_RANK_LIST)
{
	ranksortMap::const_iterator iter = sExploitMgr.exploit_rank_map_.begin();
	pb::GS2C_Exploit_Rank_List msg;
	size_t rank_id = 1;
	if(LIKELY(!sExploitMgr.exploit_rank_map_.empty()))
	{
		for(; iter!= sExploitMgr.exploit_rank_map_.end();++iter)
		{
			if(iter->first.country_id_ == GetCountryId() && rank_id <= EXPLOIT_NUM && iter->first.ranknum_ != 0)
			{
				pb::ExploitRank *rank = msg.add_list();
				rank->set_id(rank_id);
				rank->set_name(iter->second);
				rank->set_lv (iter->first.level_);
				rank->set_vip_level(iter->first.vip_level_);
				rank->set_exploit(iter->first.ranknum_);
				rank->set_headiconid(iter->first.headIconId_);
				++rank_id;
			}
		}
	}
	else
	{
		pb::ExploitRank *rank = msg.add_list();

		rank->set_isnew(true);
		rank->set_id(rank_id);
		rank->set_name(CharName());
		rank->set_lv (GetLevel());
		rank->set_vip_level(GetVipLevel());
		rank->set_exploit(GetExploitValue());
		rank->set_headiconid(GetHeadIconId());
		//SetValueInt(pb::PLAYER_FIELD_EXPLOIT_RANK_NUMBER,1);
	}
	Send(SMSG_EXPLOIT_RANK_LIST,msg);
}

// 诱敌榜
OpHandler(CMSG_TRICK_RANK_LIST)
{
	ranksortMap::const_iterator iter = sExploitMgr.trick_rank_map_.begin();
	pb::GS2C_Trick_Rank_List msg;
	size_t i = 1;
	if(LIKELY(!sExploitMgr.trick_rank_map_.empty()))
	{
		for(; iter!= sExploitMgr.trick_rank_map_.end() ;++iter){
			if(GetCountryId() == iter->first.country_id_ && i <= EXPLOIT_NUM && iter->first.ranknum_ != 0)
			{
				pb::TrickRank *rank = msg.add_list();
				rank->set_id(i);
				rank->set_name(iter->second);
				//NLOG("name = %s",iter->name_);//
				rank->set_lv(iter->first.level_);
				rank->set_vip_level(iter->first.vip_level_);
				rank->set_trick(iter->first.ranknum_);
				rank->set_headiconid(iter->first.headIconId_);
				++i;
			}
		}
	}
	else
	{
		pb::TrickRank *rank = msg.add_list();
		rank->set_isnew(true);
		rank->set_id(i);
		rank->set_name(CharName());
		rank->set_vip_level(GetVipLevel());
		rank->set_lv (GetLevel());
		rank->set_trick(GetTrickValue());
		rank->set_headiconid(GetHeadIconId());
	}

	uint32 current_trick_value = GetTrickValue();
	while(GetAttrUint(pb::PLAYER_FIELD_TRICKREWARD_STEP) > trick_queue_.front().id_)
	{
		trick_queue_.pop_front();
	}
	if(LIKELY(!trick_queue_.empty()))
	{
		if(current_trick_value  == 0 )
		{
			pb::RankRewardStatus *Rank = msg.add_reward();
			Rank->set_step(trick_queue_.front().id_);
			Rank->set_reward(0);//没有领过奖
		}
		else if(current_trick_value > 0)
		{
			if(current_trick_value >= trick_queue_.front().trick_times_ )
			{
				pb::RankRewardStatus *rankrewardstatus = msg.add_reward();
				rankrewardstatus->set_step(trick_queue_.front().id_);
				rankrewardstatus->set_reward(0);//没有领过奖
			}
			else 
			{
				pb::RankRewardStatus *rankrewardstatus = msg.add_reward();
				rankrewardstatus->set_step(trick_queue_.front().id_ );
				rankrewardstatus->set_reward(0);//已经领过奖
			}
		}
		
	}
	else
	{
		pb::RankRewardStatus *rankrewardstatus = msg.add_reward();
		rankrewardstatus->set_step(trick_queue_size_);
		rankrewardstatus->set_reward(1);//已经领过奖
	}
	Send(SMSG_TRICK_RANK_LIST,msg);
}

//攻城榜
OpHandler(CMSG_OCCUPY_RANK_LIST)
{
	ranksortMap::const_iterator iter = sExploitMgr.occupy_rank_map_.begin();
	pb::GS2C_Occupy_Rank_List msg;
	size_t i = 1;
	if(!sExploitMgr.occupy_rank_map_.empty())
	{
		for(;iter!= sExploitMgr.occupy_rank_map_.end() ;++iter)
		{
			if(GetCountryId() == iter->first.country_id_ && i <= EXPLOIT_NUM && iter->first.ranknum_ != 0)
			{
				pb::OccupyRank *rank = msg.add_list();
				rank->set_id(i);
				rank->set_name(iter->second);
				rank->set_lv(iter->first.level_);
				rank->set_vip_level(iter->first.vip_level_);
				rank->set_highest(0);
				rank->set_occupy(iter->first.ranknum_);
				rank->set_headiconid(iter->first.headIconId_);
				++i;
			}
		}
	}
	else
	{
		pb::OccupyRank *rank = msg.add_list();
		rank->set_isnew(true);
		rank->set_id(i);
		rank->set_name(CharName());
		rank->set_vip_level(GetVipLevel());
		rank->set_lv (GetLevel());
		rank->set_occupy(GetOccupyValue());
		rank->set_highest(0);
		rank->set_headiconid(GetHeadIconId());
	}

	uint32 current_occupy_value = GetOccupyValue();
	while(GetAttrUint(pb::PLAYER_FIELD_OCCUPYREWARD_STEP) > occupy_queue_.front().id_)
	{
		occupy_queue_.pop_front();
	}
	if(!occupy_queue_.empty() )
	{
		if(current_occupy_value == 0)
		{
			pb::RankRewardStatus *Rank = msg.add_reward();
			Rank->set_step(occupy_queue_.front().id_);
			Rank->set_reward(0);//没有领过奖

		}
		else if(current_occupy_value > 0)
		{
			if(current_occupy_value >= occupy_queue_.front().occupy_nums_)
			{
				pb::RankRewardStatus *Rank = msg.add_reward();
				Rank->set_step(occupy_queue_.front().id_);
				Rank->set_reward(0);//没有领过奖

			}
			else
			{
				pb::RankRewardStatus *rankrewardstatus = msg.add_reward();
				rankrewardstatus->set_step(occupy_queue_.front().id_ );
				rankrewardstatus->set_reward(0);//已经领过奖
			}
		}
		
	}
	
	else
	{
		pb::RankRewardStatus *rankrewardstatus = msg.add_reward();
		rankrewardstatus->set_step(occupy_queue_size_);
		rankrewardstatus->set_reward(1);
	}
	Send(SMSG_OCCUPY_RANK_LIST,msg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace exploit
{
	struct drop_struct
	{
		uint32 value;
		uint32 strike;
		drop_struct()
			:value()
			,strike(){ }
	};
	typedef std::map<uint32,drop_struct> exploit_drop;

	uint32 SaveToItemMap(Player* player,const IntPairVec& vec,exploit_drop& item_map,exploit_drop& money_map,drop_struct& player_exp_struct)
	{
		LootList lootlist;
		if(!sExploitMgr.GetRankBoxReward(*player,LOOTTYPE_EXPLOIT_BOX,lootlist,true))
		{
			return 2;
		}
		const ItemMap& Items = lootlist.Items();
		const MoneyMap& moneys = lootlist.Moneys();
		//计算暴击
		uint32 strike = System::RandWeight(vec);
		for(MoneyMap::const_iterator iter = moneys.begin();iter != moneys.end();++iter)
		{
			if(!player->IsActivityStillActive(pb::ACTIVITY_HERO_REWARD))
			{
				if(iter->first == Reward::Beef)
				{
					continue;
				}
			}
			if(money_map.find(iter->first) == money_map.end())
			{
				if(iter->first == Reward::TroopsOder || iter->first == Reward::Beef)
				{
					money_map[iter->first].value = iter->second;
					money_map[iter->first].strike = 1;
				}
				else
				{
					money_map[iter->first].value = iter->second * strike;
					money_map[iter->first].strike = strike;
				}
			}
			else
			{
				if(iter->first == Reward::TroopsOder || iter->first == Reward::Beef)
				{
					money_map[iter->first].value += iter->second;
					money_map[iter->first].strike = 1;
				}
				else
				{
					money_map[iter->first].value += iter->second * strike;
					money_map[iter->first].strike = strike;
				}
			}
		}
		for(ItemMap::const_iterator iter = Items.begin();iter!= Items.end();++iter)
		{
			const LogicItemProto* proto = sItemMgr.Find(iter->first);
			if(proto)
			{
				if(proto->IsDecoration())
				{
					if(item_map.find(iter->first) == item_map.end())
					{
						item_map[iter->first].value = iter->second;
						item_map[iter->first].strike = 1;
					}
					else
					{
						item_map[iter->first].value += iter->second;
						item_map[iter->first].strike = 1;
					}
				}
				else
				{
					if(item_map.find(iter->first) == item_map.end())
					{
						item_map[iter->first].value = iter->second * strike;
						item_map[iter->first].strike = strike;
					}
					else
					{
						item_map[iter->first].value += iter->second * strike;
						item_map[iter->first].strike = strike;
					}
				}
			}
		}

		if(lootlist.GetPlayerExp() != 0)
		{
			player_exp_struct.value += lootlist.GetPlayerExp() * strike;
			player_exp_struct.strike = strike;
		}
		return 0;
	}
}///namespace exploit
OpHandler(CMSG_EXPLOIT_RANK_OPEN_BOX)
{
	using namespace exploit;
	pb::C2GS_Exploit_Rank_Open_Box cmsg;
	pack >> cmsg;
	pb::GS2C_Exploit_Rank_Open_Box msg;
	exploit_drop item_map;
	exploit_drop money_map;
	drop_struct player_exp_struct;
	int box_count = 0;
	IntPairVec pair_vec;
	pair_vec.push_back(std::make_pair(1,GET_BASE_DEF_UINT(pb::BD_EXPLOIT_BOX_CRITICAL_WEIGHT1)));
	pair_vec.push_back(std::make_pair(2,GET_BASE_DEF_UINT(pb::BD_EXPLOIT_BOX_CRITICAL_WEIGHT2)));
	pair_vec.push_back(std::make_pair(4,GET_BASE_DEF_UINT(pb::BD_EXPLOIT_BOX_CRITICAL_WEIGHT3)));
	pair_vec.push_back(std::make_pair(10,GET_BASE_DEF_UINT(pb::BD_EXPLOIT_BOX_CRITICAL_WEIGHT4)));
	if(cmsg.is_all())
	{
		if(cmsg.id() == 1)
		{
			box_count = GetAttrInt(pb::PLAYER_FIELD_EXPLOIT_RANK_NORMAL_BOX);
		}
		else if(cmsg.id() == 2)
		{
			box_count = GetAttrInt(pb::PLAYER_FIELD_EXPLOIT_RANK_ADVANCE_BOX);
		}
	}
	else
	{
		if(cmsg.id() == 1)
		{
			box_count = GetAttrInt(pb::PLAYER_FIELD_EXPLOIT_RANK_NORMAL_BOX) > 0 ? 1: 0;
		}
		else if(cmsg.id() == 2)
		{
			box_count = GetAttrInt(pb::PLAYER_FIELD_EXPLOIT_RANK_ADVANCE_BOX) > 0 ? 1 : 0;
		}
	}
	if(box_count == 0)
	{
		msg.set_ret(1);//box is not enough.
		Send(SMSG_EXPLOIT_RANK_OPEN_BOX,msg);
		return;
	}
	int i = 0;
	for(;i < box_count;++i)
	{
		uint32 result = 0;
		if((result = SaveToItemMap(this,pair_vec,item_map,money_map,player_exp_struct) ) != 0)
		{
			msg.set_ret(result);
			break;
		}
	}
	if(cmsg.id() == 1)
	{
		AddValue(pb::PLAYER_FIELD_EXPLOIT_RANK_NORMAL_BOX,-i);
	}
	else if(cmsg.id() == 2)
	{
		AddValue(pb::PLAYER_FIELD_EXPLOIT_RANK_ADVANCE_BOX,-i);
	}
	IntPairVec reward_vec;
	for(exploit_drop::iterator iter = item_map.begin();iter != item_map.end();++iter)
	{
		pb::ExploitRankBoxDrop* drop = msg.add_drop();
		drop->set_id(iter->first);
		drop->set_cnt(iter->second.value);
		reward_vec.push_back(std::make_pair(drop->id(),drop->cnt()));
		//sReward.Change(*this,drop->id(),drop->cnt());
		if(!cmsg.is_all())
			drop->set_strike(iter->second.strike);
	}
	for(exploit_drop::iterator iter = money_map.begin();iter != money_map.end();++iter)
	{
		pb::ExploitRankBoxDrop* drop = msg.add_drop();
		drop->set_id(iter->first);
		drop->set_cnt(iter->second.value);
		reward_vec.push_back(std::make_pair(drop->id(),drop->cnt()));
		if(!cmsg.is_all())
			drop->set_strike(iter->second.strike);
	}
	if(player_exp_struct.value != 0)
	{
		pb::ExploitRankBoxDrop* drop = msg.add_drop();
		drop->set_id(Reward::PlayerExp);
		drop->set_cnt(player_exp_struct.value);
		reward_vec.push_back(std::make_pair(drop->id(),drop->cnt()));
		if(!cmsg.is_all())
			drop->set_strike(player_exp_struct.strike);
	}
	Send(SMSG_EXPLOIT_RANK_OPEN_BOX,msg);
	sReward.Change(*this,reward_vec);
}


///zhoulunhao
OpHandler(CMSG_TRICK_RANK_GET_REWARD)
{
	
	pb::GS2C_Trick_Rank_Get_Reward msg;
	uint32 trickValue = GetTrickValue();
	if(!trick_queue_.empty() && trickValue >= trick_queue_.front().trick_times_)
	{
		pb::ExploitRankBoxDrop* drop = msg.mutable_drop();
		drop->set_id(trick_queue_.front().item_id_);
		drop->set_cnt(trick_queue_.front().item_num_);
		drop->set_strike(1);
		pb::RankRewardStatus *Rank = msg.add_reward();
		if(trick_queue_.front().id_ == trick_queue_size_)
		{
			Rank->set_step(trick_queue_.front().id_);
			Rank->set_reward(1);//已领取
		}
		else
		{
			Rank->set_step(trick_queue_.front().id_ + 1);
			SetValueInt(pb::PLAYER_FIELD_TRICKREWARD_STEP,trick_queue_.front().id_ + 1);
			Rank->set_reward(0);//未领取
		}
		
		Send(pb::SMSG_TRICK_RANK_GET_REWARD,msg);
		sReward.Change(*this,trick_queue_.front().item_id_,trick_queue_.front().item_num_);
		trick_queue_.pop_front();
	}
}

OpHandler(CMSG_OCCUPY_RANK_GET_REWARD)
{
	pb::GS2C_Occupy_Rank_Get_Reward msg;
	uint32 attackvalue = GetOccupyValue();
	if( !occupy_queue_.empty() && attackvalue >= occupy_queue_.front().occupy_nums_ )
	{
		pb::ExploitRankBoxDrop* drop = msg.mutable_drop();
		drop->set_id(occupy_queue_.front().item_id_);
		drop->set_cnt(occupy_queue_.front().item_num_);
		drop->set_strike(1);
		pb::RankRewardStatus *Rank = msg.add_reward();
		if(occupy_queue_.front().id_ == occupy_queue_size_)
		{
			Rank->set_step(occupy_queue_.front().id_);
			Rank->set_reward(1);//已领取
		}
		else
		{
			Rank->set_step(occupy_queue_.front().id_ + 1);
			SetValueInt(pb::PLAYER_FIELD_OCCUPYREWARD_STEP,occupy_queue_.front().id_ + 1);
			Rank->set_reward(0);//keyi领取
		}
		Send(pb::SMSG_OCCUPY_RANK_GET_REWARD,msg);
		sReward.Change(*this,occupy_queue_.front().item_id_,occupy_queue_.front().item_num_);
		occupy_queue_.pop_front();
	}
}

OpHandler(CMSG_MILITARY_POWER_RANK_LIST)
{
	GS2C_MilitaryPowerRankList msg;
	size_t i = 1;
	ranksortMap::const_iterator iter = sExploitMgr.military_power_rank_map_.begin();
	for(;iter != sExploitMgr.military_power_rank_map_.end() && iter->first.ranknum_ != 0;++iter,++i)
	{
		if(i <= SHOW_MILITARY_NUMBER_PLAYER)
		{
			pb::MilitaryPowerPack* pack_data = msg.add_military_power_list();
			pb::ShowData *show_data = pack_data->mutable_military_power_rank();
			show_data->set_rank(i);
			show_data->set_name(iter->second);
			show_data->set_lv(iter->first.level_);
			show_data->set_vip_lv(iter->first.vip_level_);
			show_data->set_value(iter->first.ranknum_);
			show_data->set_country_id(iter->first.country_id_);
			show_data->set_headiconid(iter->first.headIconId_);
			if(iter->first.playerid_ == GetGuid())
			{
				pb::MilitaryPowerPack* pack_data_self = msg.mutable_self_info();
				pb::ShowData *self_data = pack_data_self->mutable_military_power_rank();
				self_data->set_rank(i);
				self_data->set_name(iter->second);
				self_data->set_lv(iter->first.level_);
				self_data->set_vip_lv(iter->first.vip_level_);
				self_data->set_value(iter->first.ranknum_);
				self_data->set_country_id(iter->first.country_id_);
				self_data->set_headiconid(iter->first.headIconId_);
				m_characterStorage->SaveToMsg(*pack_data_self->mutable_char_msg());
				m_SeigeForceLog->SaveToMsg(*pack_data_self->mutable_siege_army_info());
				m_BaoQiLog->SaveToMsg(*pack_data_self->mutable_shenqi_msg());
				m_BaoQiLog->SaveToMsg(*pack_data->mutable_shenqi_msg());
				m_characterStorage->SaveToMsg(*pack_data->mutable_char_msg());
				m_SeigeForceLog->SaveToMsg(*pack_data->mutable_siege_army_info());
				continue;
			}
			Player *player = sPlayerPool.GetByPlayerId(iter->first.playerid_);
			if(player)
			{
				player->m_BaoQiLog->SaveToMsg(*pack_data->mutable_shenqi_msg());
				player->m_characterStorage->SaveToMsg(*pack_data->mutable_char_msg());
				player->m_SeigeForceLog->SaveToMsg(*pack_data->mutable_siege_army_info());
			}
		}
		else
		{
			break;
		}
		
	}
	if(!msg.has_self_info() && getMilitaryPower() != 0)
	{
		pb::MilitaryPowerPack* pack_data_self = msg.mutable_self_info();
		pb::ShowData* self_data = pack_data_self->mutable_military_power_rank();
		self_data->set_rank(i);
		self_data->set_name(CharName());
		self_data->set_lv(GetLevel());
		self_data->set_vip_lv(GetVipLevel());
		self_data->set_value(getMilitaryPower());
		self_data->set_country_id(GetCountryId());
		self_data->set_headiconid(GetHeadIconId());
		m_characterStorage->SaveToMsg(*pack_data_self->mutable_char_msg());
		m_SeigeForceLog->SaveToMsg(*pack_data_self->mutable_siege_army_info());
		m_BaoQiLog->SaveToMsg(*pack_data_self->mutable_shenqi_msg());
	}
	Send(SMSG_MILITARY_POWER_RANK_LIST_RSP,msg);
}