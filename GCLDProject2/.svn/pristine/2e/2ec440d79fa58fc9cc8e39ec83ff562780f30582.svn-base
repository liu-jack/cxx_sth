
#pragma once

#include "structDef.h"

namespace pb
{
	class GxDB_Grow_Up_Quest_Info;
	class GxDB_Grow_Up_Quest_Group_Info;
}
#pragma pack(push,1)

struct GrowUpQuestTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(GrowUpQuestTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return quest_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "quest_id"; }
	static const char* GetTableName(){ return "grow_up_quest_record"; }
	static const char* GetFormat()
	{
		return "bb""uuuuuu";
	}
	void SaveTo(pb::GxDB_Grow_Up_Quest_Info& msg);
	void LoadFrom(const pb::GxDB_Grow_Up_Quest_Info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32      quest_id;
	uint32		group_id;
	uint32		quest_type;
	uint32      cur_value;
	uint32		is_finished;
	uint32		is_take_reward;
};

struct GrowUpQuestGroupTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(GrowUpQuestGroupTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return group_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "group_id"; }
	static const char* GetTableName(){ return "grow_up_quest_group_record"; }
	static const char* GetFormat()
	{
		return "bb""uuu";
	}
	void SaveTo(pb::GxDB_Grow_Up_Quest_Group_Info& msg);
	void LoadFrom(const pb::GxDB_Grow_Up_Quest_Group_Info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32		group_id;
	uint32		is_finished;
	uint32		is_take_group_reward;
};


#pragma pack(pop)