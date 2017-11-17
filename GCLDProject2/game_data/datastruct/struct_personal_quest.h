#pragma once
#include "struct_base.h"

#pragma pack(push,1)

struct DB_PersonalQuest
{
	INIT_GAMEDATA(DB_PersonalQuest)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "personal_quest"; }
	static const char* GetFormat() {
		return "uuuussuu";
	}

	uint32 id;				//任务id
	uint32 description_id;	//描述
	uint32 trigger_id;		//触发条件
	uint32 type_id;			//类型id
	string type_value;		//完成目标
	string rewards;			//奖励
	uint32 name_id;
	uint32 quest_target;
};

#pragma pack(pop)