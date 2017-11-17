/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#ifndef __ACTIVITY_HERO_REWARD_LOG_H
#define __ACTIVITY_HERO_REWARD_LOG_H

#include "def/ConstDef.h"
#include "def/TypeDef.h"
#include "character/Character.h"
#include "character/CharacterStorage.h"
////êûÉÍÎä½«///////////////////////////////////////////////
namespace pb
{
	class GxDB_Hero_Reward;
	class GS2C_Reward_Hero_Info;
}
class Player;

typedef std::map<uint32,bool> B_MAP;
class ActivityHeroRewardLog
{
public:
	ActivityHeroRewardLog();
	void SaveTo(Player* player,pb::GS2C_Reward_Hero_Info& msg);
	void LoadFrom(Player* player,const pb::GxDB_Hero_Reward& msg);
	void UpdateCharInfo(const uint32 char_Id);
	void Clear();
	bool getIsFirstReward(const uint32 char_id);
	void setHasFirstReward(const uint32 char_id,bool value);
	void SyncCharId();
private:
	void FillCharId(Player* player);
private:
	B_MAP troops_has_first_reward_log_;
	std::deque<uint32> char_queue_;
};


#endif