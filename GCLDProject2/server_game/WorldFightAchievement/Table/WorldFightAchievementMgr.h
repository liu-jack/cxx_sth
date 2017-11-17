/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __WORLD_FIGHT_ACHIEVEMENT_MGR_H
#define __WORLD_FIGHT_ACHIEVEMENT_MGR_H

#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"
#include "game_data/datastruct/struct_world_fight_achievement.h"

class WorldFightAchievement
{
private:
	const DB_WorldFightAchievement& ment_;
	IntPair attack_reward_,occupy_reward_;
public:
	explicit WorldFightAchievement(const DB_WorldFightAchievement& event);
	uint32 Id() const;
	uint32 CityId() const;
	const IntPair& getAttackReward() const;
	const IntPair& getOccupyReward() const;
};
class WorldFightAchievementBox
{
private:
	const DB_WorldFightAchievementBox& box_;
	IntPair attack_city_reward_,occupy_city_reward_;
public:
	explicit WorldFightAchievementBox(const DB_WorldFightAchievementBox& box);
	uint32 Id() const;
	uint32 AttackCityNeedNum() const;
	uint32 OccupyCityNeedNum() const;
	const IntPair& getAttackCityReward() const;
	const IntPair& getOccupyCityReward() const;
};

typedef PtrMap<uint32,WorldFightAchievement> WorldFightAchievementMap;
typedef PtrMap<uint32,WorldFightAchievementBox> WorldFightAchievementBoxMap;


class WorldFightAchievementMgr:public SimpleSingleton<WorldFightAchievementMgr>
{
private:
	WorldFightAchievementMap achievement_map_;
	WorldFightAchievementBoxMap achievement_box_map_;
public:
	void Init();
	const WorldFightAchievementMap& getWorldFightAchievementMap() const;
	const WorldFightAchievementBoxMap& getWorldFightAchievementBoxMap() const;
	const WorldFightAchievement* getWorldFightAchievement(const uint32 city_id) const;
	const WorldFightAchievementBox* getWorldFightAchievementBox(const uint32 id) const;
	bool IsInvalidCityId(const uint32 city_id) const;
};

#define sWorldFightAchievementMgr WorldFightAchievementMgr::Instance()

#endif