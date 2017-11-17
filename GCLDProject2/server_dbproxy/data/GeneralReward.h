#ifndef __GENERAL_REWARD__H
#define __GENERAL_REWARD__H

#include "structDef.h"

namespace pb
{
	class GxDB_Hero_Reward_info;
	class WannaBeStrongerInfo;
}
#pragma pack(push,1)


struct GeneralRewardTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(GeneralRewardTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return charId; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "charId"; }
	static const char* GetTableName(){ return "general_reward"; }
	static const char* GetFormat()
	{
		return "bb""uu";
	}
	void SaveTo(pb::GxDB_Hero_Reward_info& msg);
	void LoadFrom(const pb::GxDB_Hero_Reward_info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32      charId;
	uint32		IsHasFirstReward;
};


struct WannaBeStrongerTable : public BaseStruct {
public:

	INIT_GAMEDATA(WannaBeStrongerTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return group_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "group_id"; }
	static const char* GetTableName(){ return "wanna_be_stronger_record"; }
	static const char* GetFormat()
	{
		return "bb""ucc"/*"uuuuuuuuuuuuuuu"*/;
	}
	void SaveTo(pb::WannaBeStrongerInfo& msg);
	void LoadFrom(const pb::WannaBeStrongerInfo& msg);
// 	struct stronger_base_info
// 	{
// 		uint32 quest_id;
// 		uint32 cur_value1_;
// 		uint32 need_value1_;
// 		uint32 need_value2_;
// 		uint32 quest_type;
// 	};
public:
	IdType      id;
	uint64      player_id;
	uint32		group_id;
	uint8		is_achieve;
	uint8		is_take_reward;
	//stronger_base_info base_info[3];
};
#pragma pack(pop)




#endif