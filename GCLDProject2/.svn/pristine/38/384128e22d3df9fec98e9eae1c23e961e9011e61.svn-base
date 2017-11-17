#pragma once
#include "def/TypeDef.h"

//Notice：须同道具表一致(item_proto)
#undef Declare
#undef Reward_Enum
#define Declare(typ, n) typ = n,
#define Reward_Enum\
    Declare(PlayerExp       , 5001900)\
    Declare(Coin_Silver     , 5000000) /*银币*/\
    Declare(Coin_Gold       , 5000100) /*系统赠送金币*/\
    Declare(RMB_Gold        , 5001800) /*rmb金币*/\
    Declare(Iron            , 5000200) /*铁矿*/\
    Declare(Food            , 5000300) /*粮食*/\
    Declare(Wood            , 5000400) /*木材*/\
    Declare(Honour          , 5000500) /*荣誉点*/\
    Declare(Build_Honour    , 5000600) /*建筑荣誉点*/\
    Declare(TrainToken_Normal , 5000700) /*普通练兵令*/\
    Declare(TrainToken_Fight  , 5000701) /*战力练兵令*/\
    Declare(TrainToken_Double , 5000702) /*翻倍练兵令*/\
    Declare(TrainToken_Reduce , 5000703) /*减耗练兵令*/\
	Declare(PeopleLoyal,    5002500)/*民忠*/\
	Declare(ZhengWu,        5002600)/*政务*/\
	Declare(Renown,        5002700)/*声望*/\
	Declare(StarCrystal,    5002800)/*星耀晶*/\
	Declare(Board,			 5002900)/*集成板*/\
	Declare(GeneCode,        5003000)/*基因码*/\
	Declare(Beef,        5003100)/*牛肉*/\
	Declare(Wine,        5003200)/*美酒*/\
    Declare(AddSoldierTimes , 5000800) /*免费增兵次数*/\
    Declare(FreeCollect     , 5000900) /*免费征收次数*/\
    Declare(SuperCollect    , 5001000) /*超级征收次数*/\
    Declare(TroopsOder      , 5001100) /*征兵令*/\
    Declare(GoldHammer      , 5001200) /*黄金锤*/\
    Declare(Coupon          , 5001300) /*点券*/\
    Declare(FreePurify      , 5001500) /*免费洗练*/\
    Declare(ExtrmePurify    , 5001600) /*至尊洗练*/\
    Declare(GodStonePurify  , 5001700) /*神石洗练*/\
    Declare(VipExp          , 5002000) /*vip经验*/\
    Declare(DailyScore      , 5002100) /*每日任务积分*/\


class Player;
class Reward {
   DISALLOW_IMPLICIT_CONSTRUCTORS(Reward);
public:
    typedef bool(Reward::*RewardFunc)(Player&, int);
    enum Type {
        Reward_Enum
    };
    static Reward& Instance(){ static Reward T; return T; }

	bool Enough(Player& player, uint32 resourceId, int diff);
	bool Enough(Player& player, const IntPairVec& resource);
	bool Enough(Player& player,const IntPair& pair_);
    bool Change(Player& player, uint32 resourceId, int diff,bool is_auto_give_reward = false);
    bool Change(Player& player, const IntPairVec& resource,bool is_auto_give_reward = false);
	bool Change(Player& player, const IntPair& resource,bool is_auto_give_reward = false);
    bool _Change(Player& player, uint32 resourceId, int diff,bool is_auto_give_reward = false);
	void Append(IntPairVec& reward,const IntPairVec& resource);
	void Append(IntPairVec& reward,const uint32 resourceId, const int diff);
    //各类资源变更函数
private:
    bool ChangeItem(Player& player, int itemId, int diff,bool is_auto_give_reward = false);

	bool EnoughItem(Player& player, int itemId, int diff);
#undef Declare
#define Declare(typ, n) bool Change_##typ(Player& player, int diff);
    Reward_Enum
#undef Declare
#define Declare(typ, n) bool Enough_##typ(Player& player, int diff);
	Reward_Enum
};
#define sReward Reward::Instance()

#define PackRewardMsg(reward, msg) for(IntPairVec::iterator it = reward.begin(); it != reward.end(); ++it){ \
    pb::StReward* p = msg.add_rewards(); \
    p->set_type(it->first); \
    p->set_value(it->second); \
}
