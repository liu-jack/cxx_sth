
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#ifndef INCLUDE_DAILY_LOTTERY_TABLE_MGR_H
#define INCLUDE_DAILY_LOTTERY_TABLE_MGR_H
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
#include "SimpleSingleton.h"
#include "game_data/datastruct/struct_lottery_daily.h"
class Player;
class DailyLottery
{
public:
	DailyLottery(const DB_LotteryDaily& db);
	uint32 Id() const;
	const IntPair& GetFirstReward() const;
	const IntPair& GetSecondReward() const;
	const IntPair& GetThirdReward() const;
private:
	const DB_LotteryDaily& db_;
	IntPair first_reward_;
	IntPair second_reward_;
	IntPair third_reward_;
};
enum DAILY_LOTTERY_TYPE{
	TYPE_FIRST,
	TYPE_SECOND,
	TYPE_THIRD,
};

typedef PtrMap<uint32,DailyLottery> DailyLotteryMap;
class DailyLotteryTableMgr:public SimpleSingleton<DailyLotteryTableMgr>
{
	DailyLotteryMap daily_lottery_map_;
	IntPairVec first_weight_,second_weight_,third_weight_;
public:
	DailyLotteryTableMgr();
	uint32 GetTableId(const uint32 Type);
	void GiveReward(Player& player,const uint32 first_table_id,const uint32 second_table_id,const uint32 third_table_id);
};



#define sDailyLotteryTableMgr (DailyLotteryTableMgr::Instance())

#endif///INCLUDE_DAILY_LOTTERY_TABLE_MGR_H