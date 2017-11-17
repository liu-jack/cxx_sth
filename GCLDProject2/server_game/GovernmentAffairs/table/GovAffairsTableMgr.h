/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#pragma once

#include "game_data/datastruct/struct_zhenwu.h"
#include "def/TypeDef.h"
#include "SimpleSingleton.h"

namespace pb
{
	class GS2C_Show_Dialog;
	class C2GS_Take_Reward;
	class GS2C_Take_MinZhong_Reward_Rsp;
}

class Player;

typedef std::map<uint32,DB_ArmyGovText> textMap;
typedef std::map<uint32,DB_ArmyGovRewardType> typeMap;
typedef std::map<uint32,DB_ArmyGovRewardNum> numMap;
typedef std::map<uint32,DB_ArmyGovRewardCritical> criticalMap;
typedef std::map<uint32,DB_ArmyGovLoyalReward> loyalRewardMap;

enum RewardType
{
	REWARD_TYPE_OTHER = 1,
	REWARD_TYPE_MINZHONG = 2
};

enum WHICH_SIDE
{
	BOTH_SIDE = 0,
	ONLY_RIGHT_SIDE = 1,
};
class GovAffairsTableMgr:public SimpleSingleton<GovAffairsTableMgr>
{
private:
	textMap text_map_;
	typeMap type_map_;
	numMap num_map_;
	criticalMap critical_map_;
	loyalRewardMap loyal_reward_map_;
	IntPairVec text_int_vec_,type_int_vec_,critical_int_vec_;
	IntPairVec type_int_left_vec_;
public:
	void Init();
	void GetRandomTextId(Player* player,pb::GS2C_Show_Dialog& msg);//随机选择一个text Id;

	void GetRewardNum(const int32 player_level,const uint32 reward_type,IntPairVec& vec);
	void TakeReward(Player* player,const pb::C2GS_Take_Reward& msg);
	void TakeMinZhongReward(Player* player,pb::GS2C_Take_MinZhong_Reward_Rsp& msg);
	//void GetLeftRewardTypeVec(const IntPairVec& vec,IntPairVec& out_left);
};

#define sGovAffairsTableMgr (GovAffairsTableMgr::Instance())