/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/


#include "DailyLottery/Table/DailyLotteryTableMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"
#include "System.h"
#include "reward/reward.h"
DailyLottery::DailyLottery(const DB_LotteryDaily& db)
:db_(db)
{
	Utility::SplitStr(db_.first_reward,first_reward_,'|');
	Utility::SplitStr(db_.second_reward,second_reward_,'|');
	Utility::SplitStr(db_.third_reward,third_reward_,'|');
}

uint32 DailyLottery::Id() const
{
	return db_.id;
}

const IntPair& DailyLottery::GetFirstReward() const
{
	return first_reward_;
}

const IntPair& DailyLottery::GetSecondReward() const
{
	return second_reward_;
}

const IntPair& DailyLottery::GetThirdReward() const
{
	return third_reward_;
}

DailyLotteryTableMgr::DailyLotteryTableMgr()
{
	FOREACH_DB_ITEM(daily_lottery,DB_LotteryDaily)
	{
		DailyLottery * lottery = new DailyLottery(*daily_lottery);
		daily_lottery_map_[daily_lottery->id] = lottery;
		first_weight_.push_back(std::make_pair(daily_lottery->id,daily_lottery->first_weight));
		second_weight_.push_back(std::make_pair(daily_lottery->id,daily_lottery->second_weight));
		third_weight_.push_back(std::make_pair(daily_lottery->id,daily_lottery->third_weight));
	}
}

uint32 DailyLotteryTableMgr::GetTableId(const uint32 Type)
{
	uint32 table_id = 0;
	if(Type == TYPE_FIRST)
	{
		table_id =  System::RandWeight(first_weight_);
	}
	else if(Type == TYPE_SECOND)
	{
		table_id =  System::RandWeight(second_weight_);
	}
	else if(Type == TYPE_THIRD)
	{
		table_id = System::RandWeight(third_weight_);
	}
	return table_id;
}


void DailyLotteryTableMgr::GiveReward(Player& player,const uint32 first_table_id,const uint32 second_table_id,const uint32 third_table_id)
{
	DailyLottery * third_lottery = daily_lottery_map_.pfind(third_table_id);
	if(!third_lottery) return;
	DailyLottery * first_lottery = daily_lottery_map_.pfind(first_table_id);
	if(first_lottery)
	{
		const IntPair& first_reward = first_lottery->GetFirstReward();
		uint32 count = first_reward.second * third_lottery->GetThirdReward().second;
		sReward.Change(player,first_reward.first,count);
	}

	DailyLottery * second_lottery = daily_lottery_map_.pfind(second_table_id);
	if(second_lottery)
	{
		const IntPair& second_reward = second_lottery->GetSecondReward();
		uint32 count = second_reward.second * third_lottery->GetThirdReward().second;
		sReward.Change(player,second_reward.first,count);
	}
}
