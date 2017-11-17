

#ifndef __STRUCT_MONTH_CARD__H
#define __STRUCT_MONTH_CARD__H

#include "struct_base.h"

#pragma pack(push,1)

struct DB_MonthCard
{
	INIT_GAMEDATA(DB_MonthCard)
	static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "month_card"; }
	static const char* GetFormat()
	{
		return 	"usus";
	}

	uint32          id;
	string			goods_id;
	uint32			time;
	string			award;
};

#pragma pack(pop)
#endif