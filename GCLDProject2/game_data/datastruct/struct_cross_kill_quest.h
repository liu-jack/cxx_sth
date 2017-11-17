#pragma once
#include "struct_base.h"

#pragma pack(push,1)

struct DB_CrossKillQuest
{
	INIT_GAMEDATA(DB_CrossKillQuest)
		static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "cross_kill_quest"; }
	static const char* GetFormat() {
		return "usuuusssss";
	}

	uint32 id;				//ÈÎÎñid
	string level_area;		
	uint32 questid_1;		
	uint32 questid_2;		
	uint32 questid_3;		
	string questid_4_list;
	string defense_reward;
	string one_star_reward;
	string two_star_reward;
	string three_star_reward;
};

#pragma pack(pop)