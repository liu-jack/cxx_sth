

#pragma  once

#include "structDef.h"

namespace pb
{
	class GxDB_Sign_info;
}
#pragma pack(push,1)

struct SignUpTable : public BaseStruct {
public:
	
	INIT_GAMEDATA(SignUpTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return day; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "day"; }
	static const char* GetTableName(){ return "sign_up_record"; }
	static const char* GetFormat()
	{
		return "bb""uuu";
	}
	void SaveTo(pb::GxDB_Sign_info& msg);
	void LoadFrom(const pb::GxDB_Sign_info& msg);
public:
	IdType      id;
	uint64      player_id;
	uint32      day;
	uint32		is_signed;
	uint32		sign_type;
};




#pragma pack(pop)