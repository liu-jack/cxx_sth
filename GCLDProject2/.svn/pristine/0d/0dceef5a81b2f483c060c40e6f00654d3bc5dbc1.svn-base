/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#pragma once

#include "BaseDefineMgr.h"
#include "../object/Player.h"
namespace pb
{
	class GxDB_GovAffairsInfo;
	class GS2C_Show_Dialog;
}

struct struct_gov_affair
{
	uint32 left_id;
	uint32 right_id;
	uint32 title_id;
	uint32 left_reward_type;
	uint32 left_reward_count;
	uint32 right_reward_type;
	uint32 right_reward_count;
	uint32 cost_diamond;
	string image_name;
	struct_gov_affair()
		:left_id(0),right_id(0),title_id(0),left_reward_type(0),left_reward_count(0),
		right_reward_count(0),right_reward_type(0),cost_diamond(0),image_name(string())
	{
	}
};
class GovAffairsLog
{
	uint64 m_left_update_time;
	uint64 m_LeftAddTimes;
	struct_gov_affair struct_gov_affair_;
public:
	GovAffairsLog();
	uint64  UpdateZhengWuTimes(Player* player);

	void loadFrom(const pb::GxDB_GovAffairsInfo& msg);
	void saveTo(pb::GxDB_GovAffairsInfo& msg);
	void saveTo(pb::GS2C_Show_Dialog& msg);
	void loadFrom(const pb::GS2C_Show_Dialog& msg);
	void reset();
	void SendZhengWuInfo(Player* player);
};