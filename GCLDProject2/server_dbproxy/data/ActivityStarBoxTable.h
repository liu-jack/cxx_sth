
#pragma once
#include "structDef.h"
namespace pb
{
	class GxDB_ActivityStarBoxInfo;
}

//set byte alignment 
#pragma pack(push,1)

struct ActivityStarBoxTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(ActivityStarBoxTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return table_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "table_id"; }
	static const char* GetTableName(){ return "activity_star_box_record"; }
	static const char* GetFormat()
	{
		return "bbb""uuuu";
	}
	void SaveTo(pb::GxDB_ActivityStarBoxInfo& msg);
	void LoadFrom(const pb::GxDB_ActivityStarBoxInfo& msg);
public:
	IdType      id;
	uint64      player_id;
	uint64		last_take_rewrd_time;
	uint32		table_id;
	uint32		is_take_reward;
	uint32		npc_is_opened;
	uint32		npc_left_reward_times;
};

#pragma pack(pop)