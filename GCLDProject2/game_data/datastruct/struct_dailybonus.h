#pragma once
#include <string>
#include "struct_base.h"

//set byte alignment 
#pragma pack(push,1)

struct DailyBonus
{
public:
    DailyBonus();
	DECLARE_EXTRA_SIZE_ZERO
    static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "daily_bonus";
	}
	static const char* GetFormat()
	{
		return "usuuuuusssssssssssssssuu";
	}
	
	uint32 Id;
    std::string desc;
    uint32 enable;
    uint32 trigger_value_min;
    uint32 trigger_value_max;
    uint32 lvl_min;
    uint32 lvl_max;
    std::string item[15];
    uint32 mail_title;
    uint32 mail_body;
};

#pragma pack(pop)