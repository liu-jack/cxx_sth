#pragma once
#include <string>
#include "struct_base.h"
#pragma pack(push,1)

struct DB_ExploitRank{
public:
	//DB_ExploitRank();

	INIT_GAMEDATA(DB_ExploitRank)

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
		return "exploit_rank";
	}
	static const char* GetFormat() {
		return 
			"us";
	}

	uint32	id;
	std::string rank_area;
};


struct DB_ExploitBoxProto{
public:
	//DB_ExploitBoxProto();
	INIT_GAMEDATA(DB_ExploitBoxProto)

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
		return "exploit_box_proto";
	}
	static const char* GetFormat() {
		return 
			"uuuuuuu";
	}

	uint32 id;
	uint32 exploit_level;//军功等级
	uint32 need_exploit;//所需军功
	uint32 reward_box_num;//奖励宝箱数量
	uint32 reward_exbox_num;
	uint32 kill_get_exploit;//杀掉这个数量的人获得一点功勋值
	uint32 box_type;
};


struct DB_ChallengeRank{
public:
	//DB_ChallengeRank();
	INIT_GAMEDATA(DB_ChallengeRank)

		
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
		return "challenge_rank";
	}
	static const char* GetFormat() {
		return 
			"uuuuuus";
	}

	uint32 id;
	uint32 victory_times;
	uint32 reward_item_id;
	uint32 reward_item_num;
	uint32 title_id;
	uint32 title_quilty;
	std::string comment;
};


struct DB_AttackCityRank
{
public:
	//DB_AttackCityRank();
	INIT_GAMEDATA(DB_AttackCityRank)

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
		return "attack_city_rank";
	}
	static const char* GetFormat() {
		return 
			"uuuuuuus";
	}

	uint32 id;
	uint32 level;
	uint32 occupy_num;
	uint32 reward_item_id;
	uint32 reward_item_num;
	uint32 title_id;
	uint32 title_quilty;
	std::string comment;
};


struct DB_FightRankReward
{
public:
	INIT_GAMEDATA(DB_FightRankReward)

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
		return "fight_rank_reward";
	}
	static const char* GetFormat() {
		return 
			"uss";
	}

	uint32 id;
	string rank_area;
	string rank_list;
};

#pragma pack(pop)
