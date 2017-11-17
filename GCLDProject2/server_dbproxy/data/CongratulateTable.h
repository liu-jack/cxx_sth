


#ifndef INCLUDE_CONGRATULATE_TABLE_H
#define INCLUDE_CONGRATULATE_TABLE_H


#include "structDef.h"

namespace pb
{
	class GxDB_CongratulateInfo;
	class C2GS_Congratulate_Accept;
}
#pragma pack(push,1)

struct CongratulateTable : public BaseStruct {
public:

	INIT_GAMEDATA(CongratulateTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return table_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "table_id"; }
	static const char* GetTableName(){ return "congratulate_record"; }
	static const char* GetFormat()
	{
		return "bb""uuub";
	}
	void SaveTo(pb::GxDB_CongratulateInfo& msg);
	void LoadFrom(const pb::GxDB_CongratulateInfo& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32      table_id;
	uint32		is_achieve;
	uint32		been_congratulated_count;
	uint64		active_time;
};

struct CongratulateBeenCongratulatedTable : public BaseStruct {
public:

	INIT_GAMEDATA(CongratulateBeenCongratulatedTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return table_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "table_id"; }
	static const char* GetTableName(){ return "congratulate_been_con"; }
	static const char* GetFormat()
	{
		return "bbbb""uuuu";
	}
	void SaveTo(pb::C2GS_Congratulate_Accept& msg);
	void LoadFrom(const pb::C2GS_Congratulate_Accept& msg);
public:
	IdType      id;
	uint64      player_id;
	uint64		to_player_id;
	uint64		active_time;
	uint32      table_id;
	uint32		rank;
	uint32		suit_id;
	uint32		congratulate_tpe;
};


#pragma pack(pop)



#endif  ///INCLUDE_CONGRATULATE_TABLE_H