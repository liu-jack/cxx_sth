#pragma once

#include "struct_base.h"
#pragma pack(push,1)

struct DB_ActivityRechargeRebate{
public:

	INIT_GAMEDATA(DB_ActivityRechargeRebate)

		static const bool HasIndex() {
			return true;
	}
	const uint32 GetKey() const {
		return id;
	}
	static const char* GetKeyName() {
		return "id";
	}
	static const char* GetTableName() {
		return "activity_recharge_rebate";
	}
	static const char* GetFormat() {
		return 
			"uuu";
	}

	uint32	id;
	uint32 need_charge;
	uint32 reward;
};


#pragma pack(pop)