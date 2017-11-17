/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#ifndef __ACTIVITY_MGR__H
#define __ACTIVITY_MGR__H


#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"
#include "InterServer.pb.h"
#include "utility/Utility.h"
#include "Activity.pb.h"

struct cur_activity
{	
	uint32 activity_id_;
	uint32 open_way_;
	time_t start_date_;
	time_t end_date_;
	time_t last_show_date_;
	uint32 need_than_greater_time_;
	uint32 need_than_less_time_;
	std::string stage_area_;
	cur_activity(uint32 activity_id,uint32 open_way,time_t start_date,time_t end_date,time_t last_show_date,
		uint32 need_than_greater_time,uint32 need_than_less_time,std::string stage_area)
		:activity_id_(activity_id),open_way_(open_way),start_date_(start_date),end_date_(end_date),last_show_date_(last_show_date)
		,need_than_greater_time_(need_than_greater_time),need_than_less_time_(need_than_less_time),stage_area_(stage_area)
	{

	}

	bool operator==(const cur_activity& other) const
	{
		return activity_id_ == other.activity_id_ && open_way_ == other.open_way_ && start_date_ == other.start_date_ &&end_date_ == other.end_date_ 
			&& last_show_date_ == other.last_show_date_
			&& need_than_greater_time_ == other.need_than_greater_time_
			&& need_than_less_time_ == other.need_than_less_time_ && stage_area_ == other.stage_area_;
	}
	bool operator!=(const cur_activity& other) const
	{
		return !operator==(other);
	}
};

struct server_open_time
{
	uint32 server_id_;
	time_t open_time_;
	server_open_time(uint32 server_id,time_t open_time)
		:server_id_(server_id),open_time_(open_time)
	{

	}
//TODO:MAYBE we can declare those functions as friend.
	bool operator==(const server_open_time& other) const
	{
		return server_id_ == other.server_id_ && open_time_ == other.open_time_;
	}
	bool operator!=(const server_open_time& other) const
	{
		return !operator==(other);
	}
};

struct activity_proto
{
	uint32 reward_type_;
	uint32 act_enum_id_;
	activity_proto(uint32 reward_type,uint32 act_enum_id)
		:reward_type_(reward_type),act_enum_id_(act_enum_id)
	{

	}
	bool operator==(const activity_proto& other) const
	{
		return reward_type_ == other.reward_type_ &&  act_enum_id_ == other.act_enum_id_;
	}
	bool operator!=(const activity_proto& other) const
	{
		return !operator==(other);
	}
};

struct activity_reward
{
	uint32			activity_id_;
	uint32			cost_price_;
	uint32			is_has_discount_;
	uint32			cost_discount_;
	uint32			condition_;
	string			reward_list_;
	IntPair			p_;
	activity_reward(uint32 activity_id,uint32 cost_price,uint32 is_has_discount,uint32 cost_discount,uint32 condition,string reward_list)
		:activity_id_(activity_id),cost_price_(cost_price),is_has_discount_(is_has_discount),cost_discount_(cost_discount),condition_(condition),reward_list_(reward_list)
	{
		Utility::SplitStr(reward_list_,p_,'|');
	}
	bool operator==(const activity_reward& other) const
	{
		return activity_id_ == other.activity_id_ && cost_price_ == other.cost_price_ && is_has_discount_ == other.is_has_discount_ &&cost_discount_ == other.cost_discount_ 
			&& condition_ == other.condition_ && reward_list_ == other.reward_list_;
	}
	bool operator!=(const activity_reward& other) const
	{
		return !operator==(other);
	}
};

struct ActivityPack
{
	std::map<uint64,cur_activity> cur_activity_map_;
	std::map<uint64,activity_proto> activity_proto_map_;
	std::map<uint64,server_open_time> server_open_time_map_;
	std::map<uint64,activity_reward> activity_reward_map_;
};

template <class T1,class T2>  
void InsertOrUpdateItem( std::map<T1, T2>& mapList, T1 key, T2 tValue )  
{  
	typename std::map<T1, T2>::iterator iter = mapList.find( key );  
	if ( iter != mapList.end() )  
	{  
		iter->second = tValue;   
		return;  
	}  
	else  
	{  
		mapList.insert( iter, std::make_pair( key, tValue ) );  
	}  
} 

///此文件中返回的time_t时间都是当地时间戳
class ActivityMgr:public SimpleSingleton<ActivityMgr>
{
public:
	static ActivityMgr& Instance(){ static ActivityMgr T; return T; }
private:
	ActivityPack activity_log_pack_;
	int cur_server_id_;
	int cur_region_id_;
	std::vector<std::set<uint64> > activity_num_peopel_;
public:
	void Init();
	void LoadFrom(const pb::SC2G_activity_package& msg);
	void UpdateInfo(const pb::SC2G_activity_package& msg);
	void EraseOutOfDate();
	time_t  GetOpenTimeById(const uint32 server_id) const;
	bool ActivityIdIsValid(const uint32 activity_id) const;
	uint32 GetRewardType(const uint32 activity_id) const;
	const ActivityPack& GetActivityPack() const;
	int GetCurServerId() const;
	bool IsCurTimeBetweenOffTime(std::map<uint64,cur_activity>::const_iterator &iter,time_t cur_time,time_t open_time) const;
#ifdef _MMO_SERVER_
	bool IsActivityActive(const uint32 activity_id,std::pair<time_t,time_t>* pair_time = NULL);
#endif

	time_t getDiffSecondFromNextDayZero(const time_t& server_open_time);
	void GetActivityStartAndEndTime(std::map<uint64,cur_activity>::const_iterator &iter,std::pair<time_t,time_t>& pair_time);
	void MarkTakeInActivity(const int activity_id,const uint64 playerId);
	const char *get_activity_name(const int activity_id );
	size_t GetActivityPeopleCount(const int activity_id);
	void addToEventLog(int paramCount, ...);
	void UpdateToEventLog();
};

#define sActivityMgr (ActivityMgr::Instance())
#endif