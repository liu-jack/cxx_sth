
#pragma once
#include <string>
#include "struct_base.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
index string		'k'
*/


//set byte alignment 
#pragma pack(push,1)

#define  MAX_TREE_NUM (5)
#define  MAX_TYPE_NUM (10)
#define  MAX_REWARD_NUM (3)

enum PLAYER_ACHIEVEMENT_TYPE
{
	PAT_NO_LIMIT = 0,//do not use this!
	PAT_KILL_CREATURE,
	PAT_KILL_PLAYER,
	PAT_DUNGEON_FINISHED,
	PAT_EXPLORE_ZONE,
	PAT_EXPLORE_MAP,
	PAT_ACHIEVE_ACHIEVEMENT,
	PAT_USE_ITEM,
	PAT_CREATE_ITEM,
	PAT_LOOT_ITEM,
	PAT_EQUIP_ITEM,
	PAT_OWN_ITEM,
	PAT_LOOT_MONEY,
	PAT_AUCTION_MONEY,
	PAT_TOTAL_MONEY,
	PAT_PAY_MONEY,
	PAT_IAP_PAY_STONE,
	PAT_IAP_GET_STONE,
	PAT_IAP_GET_MONEY,
	PAT_IAP_GET_TIME,
	PAT_PLAYED,
	PAT_KILLED,
	PAT_WOUNDED,
	PAT_MAKE_DAMAGE,
	PAT_HEALED,
	PAT_HEAL,
	PAT_SKILL,
	PAT_ACHIEVEMENT_BONUS,
	PAT_QUEST,
	PAT_QUEST_SPECIFIC,
	PAT_PLAYER_LEVEL,
	PAT_NUM,
};

enum ACHIEVEMENT_REWARD_TYPE
{
	ART_NONE = 0,
	ART_ITEM,
	ART_MONEY,
	ART_SYMBOL,
	ART_NUM
};
class Player;
struct Achievement
{
public:
	Achievement();
	DECLARE_EXTRA_SIZE_ZERO
	//~Achievement();
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
		return "Id";
	}
	static const char* GetTableName()
	{
		return "achievement";
	}
	static const char* GetFormat()
	{
		return "uiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiuuuuuuuuuuuiiuuuuuuuuuus";
	}
public:
	bool Completed();
	bool UpdateProcess(int type, int targetType, int addValue, bool add);
	uint32 Id;
	int m_trees[MAX_TREE_NUM];
	int m_typeNum;
	int m_type[MAX_TYPE_NUM];
	int m_targetType[MAX_TYPE_NUM];
	int m_targetValue[MAX_TYPE_NUM];
	uint32 m_str[MAX_TYPE_NUM+1];
	int m_bonus;
	int m_level;
	uint32 m_rewardType[MAX_REWARD_NUM];
	uint32 m_rewardValue[MAX_REWARD_NUM];
	uint32 m_rewardNum[MAX_REWARD_NUM];
	uint32 m_totalDescStr;
	std::string icon;

	int m_curValue[MAX_TYPE_NUM];
	bool m_completed;

	void	LoadFromAchievement(const Achievement* other);
};

struct Achievement_Type
{
	Achievement_Type();
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
		return "Id";
	}
	static const char* GetTableName()
	{
		return "achievement_type";
	}
	static const char* GetFormat()
	{
		return "uss";
	}
	uint32 Id;
	std::string name;
	std::string description;
};
#pragma pack(pop)