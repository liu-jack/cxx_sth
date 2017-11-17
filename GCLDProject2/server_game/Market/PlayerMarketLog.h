// �м��������ܣ����ա����С����С��ㄻ
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
        MysteryShop_GoodsCnt = 5,   //��������
        SuitsCnt_Max = 4,           //��װ��ҳ�����Ŀ
		COLLECT_TYPE_COUNT=5,		//���ջ����
		IRON_COLLECT_TABLE_ID = 4,
    };
    typedef std::vector<const DB_MarketBuy*> MarketBuyVec;
public:
    uint64 m_BlackTime; //�����ܽ��׵�ʱ��
    uint64 m_LeftBuyNumAddTime;
    uint64 m_BuyContentRefreshTime;
	uint64  Market_Buy_Left_Times_CD;
    uint32 m_BuyContent[3][5]; //��ǰ������Դ��[costTyp, costVal, rewardTyp, rewardVal, quality] 

    //�������ˣ�ÿ����Ʒ��Ͷ�ʽ��ȡ�Ͷ��ʱ��
    uint8   m_MysteryShopProgress[MysteryShop_GoodsCnt];
    uint64  m_MysteryShopInvestTime[MysteryShop_GoodsCnt];
	std::map<uint32, uint8>  m_SuitsPartCnt; //��һ�ù�����װ��ҳ��Ŀ
	uint32 collect_times_[COLLECT_TYPE_COUNT];//�������շ�ʽ
	std::map<uint32,iron_collect>  iron_ticket_;
public:
    PlayerMarketLog();
    void SaveTo(pb::GS2C_Market_All_Info& msg);
    void SaveTo(pb::GS2C_Market_Collect& msg);
    void SaveTo(pb::GS2C_Market_Buy& msg);
    void SaveTo(pb::GS2C_Market_Black& msg);
    void SaveTo(pb::GS2C_Market_Mystery_Shop& msg);
    void LoadFrom(const pb::GS2C_Market_All_Info& msg);

    //����
	bool InitBuyContent(Player& player);
    uint64 UpdateBuyTimes(Player* player);
    bool Buy(Player& player, uint32 idx);
    //����
	bool Collect(Player* player, int tableId,uint32 coll_times,pb::GS2C_Market_Collect& msg);
    //����
    bool BlackMarket(Player& player, int tableId, int idx);
    //��ȯ
    bool TicketChange(Player& player, int tableId); //sell_program_items��
    //��������
    bool MysteryShopInvest(Player& player, int tableId);    //Ͷ��
    bool MysteryShopTakeItem(Player& player, int tableId);  //��ȡ��Ʒ
    void _AddSuitsPart(int itemId);
    int _RandSuitsPartItemId();
	//�������ջ�콱
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
