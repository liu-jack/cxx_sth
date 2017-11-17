#include "reward.h"
#include "def/TypeDef.h"
#include "../object/Player.h"
#include "Enum.pb.h"
#include "item/ItemArray.h"


static std::map<int, Reward::RewardFunc> g_handler;

static std::map<int, Reward::RewardFunc> g_enoughHandler;

Reward::Reward()
{
#undef Declare
#define Declare(typ, n) g_handler[typ] = &Reward::Change_##typ;

    Reward_Enum
#undef Declare
#define Declare(typ, n) g_enoughHandler[typ] = &Reward::Enough_##typ;
		Reward_Enum
}

bool Reward::Enough(Player& player, uint32 resourceId, int diff)
{
	Reward::RewardFunc func = g_enoughHandler[resourceId];
	if (func == NULL){
		return EnoughItem(player, resourceId, diff);
	} else {
		return (this->*func)(player, diff);
	}
}

bool Reward::Enough( Player& player, const IntPairVec& resource )
{
	for (size_t i = 0; i < resource.size(); ++i)
	{
		if(resource[i].second < 0 && !Enough(player, resource[i].first, -resource[i].second)) return false;
	}
	return true;
}

bool Reward::Enough( Player& player,const IntPair& pair_ )
{
	if(pair_.second < 0)
		return Enough(player,pair_.first,-pair_.second);
	else
		return true;
}
bool Reward::Change(Player& player, uint32 resourceId, int diff,bool is_auto_give_reward /*= false*/)
{
	if(diff<0 && !Enough(player,resourceId,-diff))  return false;	//扣除才检测是否足够

    return _Change(player, resourceId, diff,is_auto_give_reward);
}
bool Reward::Change(Player& player, const IntPairVec& resource,bool is_auto_give_reward /*= false*/)
{
	if(!Enough(player,resource))  return false;
	std::vector<bool> flag;
	bool ret = true;
    for (int i = 0; i < (int)resource.size(); ++i)
    {
		flag.push_back(_Change(player, resource[i].first, resource[i].second,is_auto_give_reward));
    }
	for(std::vector<bool>::iterator iter = flag.begin();iter != flag.end();++iter)
	{
		ret = ret && (*iter);
	}
    return ret;
}

bool Reward::Change( Player& player, const IntPair& resource,bool is_auto_give_reward /*= false*/ )
{
	if(!Enough(player,resource)) return false;
	return _Change(player,resource.first,resource.second,is_auto_give_reward);
}
bool Reward::_Change(Player& player, uint32 resourceId, int diff,bool is_auto_give_reward /*= false*/)
{
    Reward::RewardFunc func = g_handler[resourceId];
    if (func == NULL){
        return ChangeItem(player, resourceId, diff,is_auto_give_reward);
    } else {
        return (this->*func)(player, diff);
    }
}

void Reward::Append(IntPairVec& reward,const IntPairVec& resource)
{
	reward.insert(reward.end(),resource.begin(),resource.end());
}

void Reward::Append(IntPairVec& reward,const uint32 resourceId, const int diff)
{
	reward.push_back(std::make_pair(resourceId,diff));
}

bool Reward::ChangeItem(Player& player, int itemId, int diff,bool is_auto_give_reward /*= false*/)
{
	if (diff > 0) 
	{
		return pb::ErrCommonSuccess == player.CreateItem(pb::IR_OPERATION_REWARD_GET, itemId, diff,NULL,is_auto_give_reward);
	}
	else 
	{
		return	player.DestroyItem(pb::IR_LOTTO_COST, itemId, -diff);
	}
}

bool Reward::EnoughItem(Player& player, int itemId, int diff)
{
	return player.m_bag->CountItem(itemId) >= diff;
}

//----------------------------------------------------------
//各类资源变更函数
#undef Realize
#define Realize(typ) bool Reward::Change_##typ(Player& player, int diff)

Realize(PlayerExp)
{
	player.AddXP(diff);
    return true;
}
Realize(Coin_Gold)
{
    if (diff > 0) 
	{
        player.AddCurrency(pb::IR_OPERATION_REWARD_GET, eSysGold, diff);
		return true;
    } 
	else 
	{
         return   player.TryDeductCurrency(pb::IR_LOTTO_COST, eSysGold, -diff);
    }
    
}

#undef Realize
#define Realize(typ) bool Reward::Enough_##typ(Player& player, int diff)
Realize(PlayerExp)
{
	return player.GetXP() >= diff;
}
Realize(Coin_Gold)
{
	return player.EnoughGold(diff);
}

#define RealizeCurrency(typ, currencyType)												\
	bool Reward::Change_##typ(Player& player, int diff)									\
	{																					\
		if (player.GetCurrency(currencyType) + diff < 0)                                \
			return false;                                                               \
		if (diff > 0)																	\
		{																				\
			player.AddCurrency(pb::IR_OPERATION_REWARD_GET, currencyType, diff);		\
			return true;																\
		}                                                                               \
		else																			\
		{																				\
			return player.TryDeductCurrency(pb::IR_LOTTO_COST, currencyType, -diff);    \
		}                                                                               \
	}																					\
	bool Reward::Enough_##typ(Player& player, int diff)									\
	{																					\
			return player.EnoughCurrency(currencyType,diff);							\
	}
RealizeCurrency(RMB_Gold, eCashGold)
RealizeCurrency(Coin_Silver, eCoin)
RealizeCurrency(Iron, eIron)
RealizeCurrency(Food, eFood)
RealizeCurrency(Wood, eWood)
RealizeCurrency(Honour, eHornor)
RealizeCurrency(Build_Honour, eBuildHornor)
RealizeCurrency(TrainToken_Normal, eTraining)
RealizeCurrency(TrainToken_Fight, eTrainingFight)
RealizeCurrency(TrainToken_Double, eTrainingDouble)
RealizeCurrency(TrainToken_Reduce, eTrainingReduce)
RealizeCurrency(AddSoldierTimes, eTroopsNum)
RealizeCurrency(FreeCollect, eFreeCollectNum)
RealizeCurrency(SuperCollect, eSuperCollectNum)
RealizeCurrency(TroopsOder, eTroopsOder)
RealizeCurrency(GoldHammer, eGoldHammer)
RealizeCurrency(Coupon, eCoupon)
RealizeCurrency(FreePurify, eFreePurify)
RealizeCurrency(ExtrmePurify, eExtrmePurify)
RealizeCurrency(GodStonePurify, eGodStonePurify)
RealizeCurrency(VipExp, eVipExp)
RealizeCurrency(DailyScore, eDailyScore)
RealizeCurrency(PeopleLoyal,ePeopleLoyal)
RealizeCurrency(ZhengWu,eZhengWu)
RealizeCurrency(Renown,eRenown)
RealizeCurrency(StarCrystal,eStarCrystal)
RealizeCurrency(Board,eBoard)
RealizeCurrency(GeneCode,eGeneCode)
RealizeCurrency(Beef,eBeef)
RealizeCurrency(Wine,eWine)