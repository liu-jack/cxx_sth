#pragma once
#include "structDef.h"

namespace pb{
	class SG2D_PersonalEC;
}

#pragma pack(push,1)

struct PersonalECTable : public BaseStruct {
public:

	INIT_GAMEDATA(PersonalECTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return player_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "player_id"; }
	static const char* GetTableName(){ return "personal_event_chain"; }
	static const char* GetFormat()
	{
		return "bbbuuuuuu";
	}
	void SaveTo(pb::SG2D_PersonalEC& msg);
	void LoadFrom(const pb::SG2D_PersonalEC& msg);

public:
	IdType      id;
	uint64      player_id;
	uint64		last_fresh_time;
	uint32      current_step;
	uint32      is_take_chain_award;
	uint32		current_event_id;
	uint32		city_id;
	uint32		current_completion;
	uint32		is_take_event_award;
};

#pragma pack(pop)