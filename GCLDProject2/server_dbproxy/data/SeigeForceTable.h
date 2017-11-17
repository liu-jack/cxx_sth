
#ifndef _SEIGE_FORCE_TABLE_H
#define _SEIGE_FORCE_TABLE_H
#include "structDef.h"

namespace pb
{
	class Seige_Force_info;
}
#pragma pack(push,1)

struct SeigeForceTable : public BaseStruct {
public:

	INIT_GAMEDATA(SeigeForceTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return army_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "army_id"; }
	static const char* GetTableName(){ return "seige_force_record"; }
	static const char* GetFormat()
	{
		return "bb""uuuuuu"
			"uuuuuuuuuuuu"
			"uuuuu";
	}
	void SaveTo(pb::Seige_Force_info& msg);
	void LoadFrom(const pb::Seige_Force_info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32      army_id;
	uint32		level;
	uint32		left_times;
	uint32		cur_all_times;
	uint32		cur_exp;
	uint32		skill_id;
	uint32		specific_ids[12];
	uint32		terrains[5];
};

#pragma pack(pop)

#endif