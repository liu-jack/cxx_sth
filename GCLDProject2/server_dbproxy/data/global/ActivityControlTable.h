#pragma once
#include "structDef.h"

namespace pb{
	class SC2G_activity_proto;
	class SC2G_activity_plan;
	class SC2G_server_open_time;
	class SC2G_activity_reward;
}

//set byte alignment 
#pragma pack(push,1)

struct ActivityProtoTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(ActivityProtoTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return id; }
	void SetDbIncreaseId(const uint32& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "id"; }
	static const char* GetTableName(){ return "activity_proto"; }
	static const char* GetFormat()
	{
		return "buu";
	}

	void LoadFrom(const pb::SC2G_activity_proto& msg);
	void SaveTo( pb::SC2G_activity_proto& msg);
	bool IsModified(const pb::SC2G_activity_proto& msg);
	IdType id;
	uint32 act_enum_type;
	uint32 reward_type;
};
struct ActivityPlanTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(ActivityPlanTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return id; }
	void SetDbIncreaseId(const uint32& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "id"; }
	static const char* GetTableName(){ return "activity_plan"; }
	static const char* GetFormat()
	{
		return "buuuuutttsuu";
	}
	void LoadFrom(const pb::SC2G_activity_plan& msg);
	void SaveTo( pb::SC2G_activity_plan& msg);
	bool IsModified(const pb::SC2G_activity_plan& msg);
	IdType			id;
	uint32			activity_id;
	uint32			open_way;
	uint32			start_time;
	uint32			last_time;
	uint32			show_time;
	time_t			start_date;
	time_t			end_date;
	time_t			end_show_date;
	std::string		stage_area;
	uint32			need_greater_than_open;
	uint32			need_less_than_open;

};
struct ServerOpenTimeTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(ServerOpenTimeTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return id; }
	void SetDbIncreaseId(const uint32& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "id"; }
	static const char* GetTableName(){ return "server_open_time"; }
	static const char* GetFormat()
	{
		return "but";
	}

	void LoadFrom(const pb::SC2G_server_open_time& msg);
	void SaveTo( pb::SC2G_server_open_time& msg);
	bool IsModified(const pb::SC2G_server_open_time& msg);
	IdType			id;
	uint32			server_id;
	time_t			open_time;
};


struct ActivityRewardTable:public BaseStruct
{
public:
	
	INIT_GAMEDATA(ActivityRewardTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return id; }
	void SetDbIncreaseId(const uint32& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "id"; }
	static const char* GetTableName(){ return "activity_reward"; }
	static const char* GetFormat()
	{
		return "buuuuus";
	}

	void LoadFrom(const pb::SC2G_activity_reward& msg);
	void SaveTo( pb::SC2G_activity_reward& msg);
	bool IsModified(const pb::SC2G_activity_reward& msg);
	IdType			id;
	uint32			activity_id;
	uint32			cost_price;
	uint32			is_has_discount;
	uint32			cost_discount;
	uint32			condition;
	string			reward_list;
};

struct ActivityInfoLogTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(ActivityInfoLogTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return player_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "player_id"; }
	static const char* GetTableName(){ return "activity_info_log"; }
	static const char* GetFormat()
	{
		return "buubuuut";
	}

	//void LoadFrom(const pb::SC2G_server_open_time& msg);
	
	IdType			id;
	uint32			server_id;
	uint32			region_id;
	uint64			player_id;
	uint32			activity_id;
	uint32			target_value;
	uint32			is_finished;
	time_t			finish_time;
};



#pragma pack(pop)