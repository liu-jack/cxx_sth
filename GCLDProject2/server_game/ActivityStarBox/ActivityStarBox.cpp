/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "ActivityStarBox.h"
#include "Activity.pb.h"
#include "Table/ActivityStarBoxTableMgr.h"
#include "object/Player.h"

ActivityStarBoxLog::ActivityStarBoxLog()
{
	Init();
}

ActivityStarBoxLog::~ActivityStarBoxLog()
{

}

void ActivityStarBoxLog::SaveTo( pb::GxDB_ActivityStarBox& msg )
{
	for(ActivityStarBoxStructMap::iterator Iter = activity_star_box_struct_map_.begin();Iter != activity_star_box_struct_map_.end();++Iter)
	{
		pb::GxDB_ActivityStarBoxInfo* info = msg.add_info();
		info->set_table_id(Iter->first);
		info->set_is_take_reward(Iter->second.is_take_reward);
		info->set_last_take_reward_time(Iter->second.last_take_rewrd_time);
		info->set_left_times(Iter->second.npc_left_reward_times);
		info->set_npc_is_opened(Iter->second.npc_is_opened);
	}
}

void ActivityStarBoxLog::SaveTo(Player*player, pb::GS2C_ActivityStarBoxInfo& msg )
{
	uint32 curr_score = player->GetActivityStarBoxExp();
	uint32 id = sActivityStarBoxMgr.GetId(player);
	//uint32 cur_level = player->GetActivityStarBoxLevel();
	if(id == 0) return;
	const ActivityStarBox* ac_box = sActivityStarBoxMgr.GetActivityStarBox(id);
	if(!ac_box) 
	{
		return;
	}
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	msg.set_next_level_need_wait_time(ac_box->NextLvNeedWaitTime());
	msg.set_npc_is_opened(ref.npc_is_opened);
	msg.set_left_times( ref.npc_left_reward_times);
	msg.set_last_take_reward_time(ref.last_take_rewrd_time);
	msg.set_table_id(id);
	if(IsLvHasTakeReard(sActivityStarBoxMgr.GetMaxStarLevel()))
		msg.set_is_all_take_reward(1);
	else
		msg.set_is_all_take_reward(0);
}
void ActivityStarBoxLog::LoadFrom( const pb::GxDB_ActivityStarBox& msg )
{
	for(int i = 0;i< msg.info_size();i++)
	{
		const pb::GxDB_ActivityStarBoxInfo& info = msg.info(i);
		ActivityStarBoxStruct& ref = activity_star_box_struct_map_[info.table_id()];
		ref.is_take_reward = info.is_take_reward();
		ref.last_take_rewrd_time = info.last_take_reward_time();
		ref.npc_left_reward_times = info.left_times();
		ref.npc_is_opened = info.npc_is_opened();
	}
}

void ActivityStarBoxLog::Init()
{
	activity_star_box_struct_map_.clear();
	const ActivityStarBoxMap& star_box_map = sActivityStarBoxMgr.GetStarBoxMap();
	for(ActivityStarBoxMap::const_iterator iter = star_box_map.begin();iter != star_box_map.end();++iter)
	{
		ActivityStarBoxStruct& ref = activity_star_box_struct_map_[iter->first];
		if(iter->second->NextLvNeedWaitTime() != 0)
		{
			ref.npc_left_reward_times = iter->second->RewardTimes();
		}
		else
		{
			ref.npc_left_reward_times = 0;
		}

		ref.is_take_reward = 0;
		ref.last_take_rewrd_time = 0;
		ref.npc_is_opened = 0;
		ref.star_level = iter->second->StarLevel();
	}
}

ActivityStarBoxStructMap& ActivityStarBoxLog::GetActivityStarBoxStructMap()
{
	return activity_star_box_struct_map_;
}

void ActivityStarBoxLog::UpdateTakeRewardToDb( const uint32 id )
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	ref.is_take_reward = 1;
}


bool ActivityStarBoxLog::CanUpdateBoxLevel(const uint32 id)
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	if(ref.is_take_reward == 1)
		return true;
	else
		return false;
}

bool ActivityStarBoxLog::UpdateBoxLevel(Player* player,const uint32 id)
{
	const ActivityStarBox* ac_box = sActivityStarBoxMgr.GetActivityStarBox(id);
	if(!ac_box) 
	{
		return false;
	}
	if(CanUpdateBoxLevel(id))
	{
		if(ac_box->NextLvNeedWaitTime() == 0)
		{
			player->AddValueLimitByLimit(pb::PLAYER_FIELD_ACTIVITY_STAR_BOX_LEVEL,1,sActivityStarBoxMgr.GetMaxStarLevel());
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void ActivityStarBoxLog::UpdateNpcTakeRewardTimes( const uint32 id )
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	const ActivityStarBox* ac_box = sActivityStarBoxMgr.GetActivityStarBox(id);
	if(!ac_box) 
	{
		return;
	}
	if(ac_box->NextLvNeedWaitTime() != 0)
	{
		ref.npc_left_reward_times -= 1;
		if(ref.npc_left_reward_times < 0  )
		{
			ref.npc_left_reward_times = 0;
		}
	}
}

uint32 ActivityStarBoxLog::GetLeftTimesOfNpcTakeReward( const uint32 id )
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	return ref.npc_left_reward_times;
}

void ActivityStarBoxLog::UpdateLastTakeRewardTime( const uint32 id )
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	ref.last_take_rewrd_time = sOS.GetRealTime();
}

void ActivityStarBoxLog::UpdateNpcIsOpened( const uint32 id )
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	ref.npc_is_opened = 1;
}

bool ActivityStarBoxLog::HasTakeReward( const uint32 id )
{
	ActivityStarBoxStruct& ref = activity_star_box_struct_map_[id];
	return ref.is_take_reward == 1;
}

bool ActivityStarBoxLog::IsLvHasTakeReard( const uint32 lv )
{
	for(ActivityStarBoxStructMap::iterator Iter = activity_star_box_struct_map_.begin();Iter != activity_star_box_struct_map_.end();++Iter)
	{
		if(Iter->second.star_level == lv)
		{
			if(Iter->second.is_take_reward)
				return true;
		}
	}
	return false;
}

void ActivityStarBoxLog::Clear()
{
	Init();
}
