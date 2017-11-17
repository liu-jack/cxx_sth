

#ifndef INCLUDE_CONTINUE_OCCUPY_CITY_H
#define INCLUDE_CONTINUE_OCCUPY_CITY_H

#include "structDef.h"

namespace pb{
	class BaseContinueMsg;
}

//set byte alignment 
#pragma pack(push,1)

struct ContinueOccupyCityTable : public BaseStruct {
public:

	INIT_GAMEDATA(ContinueOccupyCityTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint64 GetLogicalKey(void) const{ return active_time; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "active_time"; }
	static const char* GetTableName(){ return "serial_city"; }
	static const char* GetFormat()
	{
		return "bbbbu";
	}
	void SaveTo(pb::BaseContinueMsg& msg);
	void LoadFrom(const pb::BaseContinueMsg& msg);

public:
	IdType      id;
	uint64      player_id;
	uint64		active_time;
	uint64		finish_time;
	uint32		serial_city_num;
};

#pragma pack(pop)















#endif