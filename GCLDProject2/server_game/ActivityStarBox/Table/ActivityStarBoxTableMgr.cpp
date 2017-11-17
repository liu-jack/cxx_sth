/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "ActivityStarBoxTableMgr.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "object/Player.h"

ActivityStarBox::ActivityStarBox( const struct DB_ActivityStarBox& activity_star_box )
	:activity_star_box_(activity_star_box)
{
	Utility::SplitStr2(activity_star_box_.reward_list,reward_list_);
	Utility::SplitStr(activity_star_box_.level_range,level_range_,',');
}

uint32 ActivityStarBox::Id() const
{
	return activity_star_box_.id;
}

uint32 ActivityStarBox::StarLevel() const
{
	return activity_star_box_.star_level;
}

uint32 ActivityStarBox::NeedScore() const
{
	return activity_star_box_.need_score;
}

uint32 ActivityStarBox::RewardTimes() const
{
	return activity_star_box_.reward_times;
}

uint32 ActivityStarBox::QuickLvUpCost() const
{
	return activity_star_box_.quick_levelup;
}	

uint32 ActivityStarBox::NextLvNeedWaitTime() const
{
	return activity_star_box_.next_level_need_wait_time;
}

const IntPairVec& ActivityStarBox::GetRewardList() const
{
	return reward_list_;
}

const std::vector<int>& ActivityStarBox::GetLevelRange() const
{	
	return level_range_;
}
void ActivityStarBoxMgr::Init()
{
	max_star_level_ = 0;
	FOREACH_DB_ITEM(activity_star_box,DB_ActivityStarBox)
	{
		uint32 Id = activity_star_box->id;
		ActivityStarBox* _activity_star_box = new ActivityStarBox(*activity_star_box);
		activity_star_box_map_[Id] = _activity_star_box;
		if(_activity_star_box->StarLevel() > max_star_level_)
		{
			max_star_level_ = _activity_star_box->StarLevel();
		}
	}
}

const uint32 ActivityStarBoxMgr::GetId(Player* player ) const
{
	int32 curr_lv = player->GetLevel();
	uint32 box_level = player->GetActivityStarBoxLevel();
	for(ActivityStarBoxMap::const_iterator iter = activity_star_box_map_.begin();iter != activity_star_box_map_.end();++iter)
	{
		if(iter->second->StarLevel() == box_level )
		{
			const std::vector<int>& level_range = iter->second->GetLevelRange();
			if(curr_lv >= level_range[0] && curr_lv <= level_range[1])
			{
				return iter->first;
			}		
		}
	}
	return activity_star_box_map_.begin()->first;
}


const ActivityStarBoxMap& ActivityStarBoxMgr::GetStarBoxMap() const
{
	return activity_star_box_map_;
}

const ActivityStarBox* ActivityStarBoxMgr::GetActivityStarBox( const uint32 id ) const
{
	return activity_star_box_map_.pfind(id);
}

const uint32 ActivityStarBoxMgr::GetMaxStarLevel() const
{
	return max_star_level_;
}

