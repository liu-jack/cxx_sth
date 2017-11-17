
#ifndef __DAILY_QUEST_TABLE__H
#define __DAILY_QUEST_TABLE__H

#include "structDef.h"

namespace pb
{
	class GxDB_Daily_Quest_Info;
}
#pragma pack(push,1)

struct DailyQuestTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(DailyQuestTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return questid; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "questid"; }
	static const char* GetTableName(){ return "dailyquestinfo"; }
	static const char* GetFormat()
	{
		return "bb""uuu";
	}
	void SaveTo(pb::GxDB_Daily_Quest_Info& msg);
	void LoadFrom(const pb::GxDB_Daily_Quest_Info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32      questid;
	uint32      finish_times;
	uint32		is_finished;;
};

#pragma pack(pop)




#endif