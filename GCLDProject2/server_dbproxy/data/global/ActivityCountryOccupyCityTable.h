#pragma once
#include "structDef.h"

namespace pb{
	class Activity_Occupy_City_Num;
}

//set byte alignment 
#pragma pack(push,1)

struct ActivityCountryOccupyCityTable : public BaseStruct {
public:

	INIT_GAMEDATA(ActivityCountryOccupyCityTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return country_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "country_id"; }
	static const char* GetTableName(){ return "activity_country_occupy_city"; }
	static const char* GetFormat()
	{
		return "buuuu";
	}

	void LoadFrom(const pb::Activity_Occupy_City_Num& msg);
	void SaveTo( pb::Activity_Occupy_City_Num& msg);
	IdType id;
	uint32 server_id;
	uint32 region_id;
	uint32 country_id;
	uint32 city_num;
};




#pragma pack(pop)