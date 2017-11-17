#pragma once
#include "structDef.h"

namespace pb{
	class GxDB_GovAffairsInfo;
}

//set byte alignment 
#pragma pack(push,1)

struct ZhengWuTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(ZhengWuTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return player_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "player_id"; }
	static const char* GetTableName(){ return "zhengwu"; }
	static const char* GetFormat()
	{
		return "bbuuuuuuuus";
	}
	void SaveTo(pb::GxDB_GovAffairsInfo& msg);
	void LoadFrom(const pb::GxDB_GovAffairsInfo& msg);

public:
	IdType      id;
	uint64      player_id;
	uint32      left_id;
	uint32      right_id;
	uint32      title_id;
	uint32      left_reward_type;
	uint32      left_reward_count;
	uint32		right_reward_type;
	uint32		right_reward_count;
	uint32		cost_diamond;
	string		image_name;
};

#pragma pack(pop)
