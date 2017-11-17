/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once
#include "def/TypeDef.h"
namespace pb
{
	class GxDB_PalaceAchievement;
	class GS2C_Palace_Take_Reward_Rsp;
	class PalaceAchievementInfo;
	class GS2C_Palace_Level_Info;
}
enum PALACE_STATE
{
	NOT_ACHIEVE = 0,
	HAS_ACHIEVE = 1,
	HAS_TAKE_REWARD = 2,
};
class Player;
class PalaceAchievementLog
{
private:
	std::map<int,int> m_achievement;//��ְ�ɾ�
public:
	PalaceAchievementLog();
	void LoadFrom(const pb::GxDB_PalaceAchievement& msg);
	void SaveTo(pb::PalaceAchievementInfo& msg,int office_id);
	void SaveTo(Player* player,pb::GS2C_Palace_Level_Info& msg);
	void SetPalaceLevel(Player& player,const int office_id,int is_take);
	int GetOfficeState(const int office_id);
	void TakePalaceAchievement(Player& player,int office_id,pb::GS2C_Palace_Take_Reward_Rsp& msg);
};