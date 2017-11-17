
#pragma once
#include "structDef.h"
namespace pb
{
	class pass_stage_reward_info;
}

//set byte alignment 
#pragma pack(push,1)

struct PassStageRewardRecordTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(PassStageRewardRecordTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return table_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "table_id"; }
	static const char* GetTableName(){ return "pass_stage_record"; }
	static const char* GetFormat()
	{
		return "bb""uuuu";
	}
	void SaveTo(pb::pass_stage_reward_info& msg);
	void LoadFrom(const pb::pass_stage_reward_info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32		table_id;
	uint32		is_take_reward;
	uint32		stage_level;
	uint32		stage_is_passed;
};

#pragma pack(pop)