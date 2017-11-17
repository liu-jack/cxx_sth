/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __WORLD_FIGHT_ACHIEVEMENT_H
#define __WORLD_FIGHT_ACHIEVEMENT_H
#include "def/TypeDef.h"

enum TYPE_CITY
{
	OTHER = 0,
	CITY_ATTACK_CITY = 1,
	CITY_OCCUPY_CITY,
};

enum CITY_STATE
{
	STATE_NOT_ACHIEVE= 0,
	STATE_CAN_TAKE,
	STATE_HAS_TAKE,
};

enum TAKE_REWARD_TYPE
{
	CITY_REWARD = 1,
	BOX_REWARD,
};



struct AchievementInfo
{
	uint32 type;
	uint32 state;
	AchievementInfo()
		:type(OTHER)
		,state(STATE_NOT_ACHIEVE){}
	explicit AchievementInfo(uint32 type_)
		:type(type_)
		,state(STATE_NOT_ACHIEVE)
	{
	}
	void set_state(const uint32 state_)
	{
		if(state_ > state)
			state = state_;
	}
	void set_type(const uint32 type_)
	{
		if(type_ > type)
			type = type_;
	}
};
namespace pb
{
	class GS2C_World_Achievement_All_Info;
	class GxDB_World_Achievement;
}
typedef std::map<uint32,AchievementInfo> AchievementInfoMap;
typedef AchievementInfoMap::iterator Iter;
class Player;
class WorldFightAchievementLog
{
private:
	AchievementInfoMap achievement_attack_map_,achievement_occupy_map_;
public:
	WorldFightAchievementLog();
	void SaveTo(pb::GS2C_World_Achievement_All_Info& msg);
	void LoadFrom(const pb::GxDB_World_Achievement& msg);
	//void SaveTo(pb::GxDB_World_Achievement_Info& msg);
	AchievementInfo* FindInfoInMap(const uint32 id,const uint32 type);
	void UpdateCityState(Player* player,const uint32 city_id,const uint32 type,const uint32 state);
	size_t GetCanTakeRewardCount(const uint32 type);
};


#endif