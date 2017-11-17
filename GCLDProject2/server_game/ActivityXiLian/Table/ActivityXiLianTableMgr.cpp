/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "ActivityXiLianTableMgr.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "object/Player.h"

ActivityXilian::ActivityXilian( const struct DB_ActivityXilian& activity_xilian )
:activity_xilian_(activity_xilian)
{
	Utility::SplitStr2(activity_xilian_.reward_list,reward_list_);
}

uint32 ActivityXilian::Id() const
{
	return activity_xilian_.id;
}

const uint32 ActivityXilian::NeedTimes() const
{
	return activity_xilian_.need_times;
}

const IntPairVec& ActivityXilian::GetRewardList() const
{
	return reward_list_;
}


void ActivityXilianMgr::Init()
{
	FOREACH_DB_ITEM(activity_xilian,DB_ActivityXilian)
	{
		uint32 Id = activity_xilian->id;
		ActivityXilian* _activity_xilian= new ActivityXilian(*activity_xilian);
		activity_xilian_map_[Id] = _activity_xilian;
	}
}

ActivityXilian* ActivityXilianMgr::GetActivityXilian( const uint32 id )
{
	return activity_xilian_map_.pfind(id);
}

const ActivityXilianMap& ActivityXilianMgr::GetActivityXilianMap() const
{
	return activity_xilian_map_;
}
