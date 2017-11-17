#pragma once
#include "structDef.h"

namespace pb{
	class GxDB_World_Achievement_Info;
}

//set byte alignment 
#pragma pack(push,1)

struct WorldFightAchievementTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(WorldFightAchievementTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return city_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "city_id"; }
	static const char* GetTableName(){ return "world_fight_achievement"; }
	static const char* GetFormat()
	{
		return "bbuuu";
	}
	void SaveTo(pb::GxDB_World_Achievement_Info& msg);
	void LoadFrom(const pb::GxDB_World_Achievement_Info& msg);

public:
	IdType      id;
	uint64      player_id;
	uint32		city_id;
	uint32		type;
	uint32		state;
};

#pragma pack(pop)