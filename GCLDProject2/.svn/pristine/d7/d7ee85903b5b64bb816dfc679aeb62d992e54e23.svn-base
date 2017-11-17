// 市集建筑功能：征收、集市、黑市、点劵
#pragma once
#include "def/TypeDef.h"


class Player;
namespace pb
{
    class GS2C_Market_All_Info;
    class GS2C_Market_Collect;
    class GS2C_Market_Buy;
    class GS2C_Market_Black;
    class GS2C_Market_Mystery_Shop;
}
struct DB_MarketBuy;

struct iron_collect
{
	time_t use_time;
	uint32 left_times;
	iron_collect(time_t use_time_,uint32 left_time_)
		:use_time(use_time_),left_times(left_time_)
	{
	}
	iron_collect()
		:use_time(0),left_times(0)
	{}
};
class PlayerMarketLog {
    enum {
        MysteryShop_GoodsCnt = 5,   //神秘商人
        SuitsCnt_Max = 4,           //套装残页最大数目
		COLLECT_TYPE_COUNT=5,		//征收活动种类
		IRON_COLLECT_TABLE_ID = 4,
    };
    typedef std::vector<const DB_MarketBuy*> MarketBuyVec;
public:
    uint64 m_BlackTime; //过点能交易的时刻
    uint64 m_LeftBuyNumAddTime;
    uint64 m_BuyContentRefreshTime;
	uint64  Market_Buy_Left_Times_CD;
    uint32 m_BuyContent[3][5]; //当前集市资源，[costTyp, costVal, rewardTyp, rewardVal, quality] 

    //神秘商人：每种商品的投资进度、投资时刻
    uint8   m_MysteryShopProgress[MysteryShop_GoodsCnt];
    uint64  m_MysteryShopInvestTime[MysteryShop_GoodsCnt];
	std::map<uint32, uint8>  m_SuitsPartCnt; //玩家获得过的套装残页数目
	uint32 collect_times_[COLLECT_TYPE_COUNT];//五种征收方式
	std::map<uint32,iron_collect>  iron_ticket_;
public:
    PlayerMarketLog();
    void SaveTo(pb::GS2C_Market_All_Info& msg);
    void SaveTo(pb::GS2C_Market_Collect& msg);
    void SaveTo(pb::GS2C_Market_Buy& msg);
    void SaveTo(pb::GS2C_Market_Black& msg);
    void SaveTo(pb::GS2C_Market_Mystery_Shop& msg);
    void LoadFrom(const pb::GS2C_Market_All_Info& msg);

    //集市
	bool InitBuyContent(Player& player);
    uint64 UpdateBuyTimes(Player* player);
    bool Buy(Player& player, uint32 idx);
    //征收
	bool Collect(Player* player, int tableId,uint32 coll_times,pb::GS2C_Market_Collect& msg);
    //黑市
    bool BlackMarket(Player& player, int tableId, int idx);
    //点券
    bool TicketChange(Player& player, int tableId); //sell_program_items表
    //神秘商人
    bool MysteryShopInvest(Player& player, int tableId);    //投资
    bool MysteryShopTakeItem(Player& player, int tableId);  //领取物品
    void _AddSuitsPart(int itemId);
    int _RandSuitsPartItemId();
	//铁矿征收活动领奖
	void TakeActivityIronCollectReward(Player* player,uint32 id);


	void UpdateMatkets(Player* player);
	void ResetCollectTimes(Player* player);

	void CalcMulti(int &multi);
	void GetMultiByItemId(ItemId item_id,std::map<uint32,iron_collect>::iterator & iter,int &multi);
private:
    void RefreshBuyContent(Player& player);
    void DoubleBuyContent();

    void BuildBuyWeightTable(Player& player, MarketBuyVec& vec);

};
