#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"

struct DB_MarketBuy;
struct DB_MarketBlack;
struct DB_MarketCollect;
struct DB_SellProgItems;
struct DB_MysteryShop;
struct DB_ActivityObsidianCollect;

typedef PtrMap<int, const DB_MarketBuy> BuyMap;
typedef PtrMap<int, const DB_MarketBlack> BlackMap;
typedef PtrMap<int, const DB_MarketCollect> CollectMap;
typedef PtrMap<int, const DB_SellProgItems> TicketMap;
typedef PtrMap<int, const DB_MysteryShop>   MysteryMap;
typedef PtrMap<int,	const DB_ActivityObsidianCollect> ActivityObsidianCollectMap;
class MarketTableMgr {
public:
    BuyMap      m_BuyTable;
    BlackMap    m_BlackTable;
    CollectMap  m_CollectTable;
    TicketMap   m_TicketTable;
    MysteryMap  m_MysteryTable;
	ActivityObsidianCollectMap m_ActivityObsidianCollectMapTable;
	std::vector<uint32> m_SuitsPartIds; //Ì××°²ÐÒ³itemId
public:
    static MarketTableMgr& Instance(){ static MarketTableMgr T; return T; }

    void Init();

    const DB_MarketBuy* GetBuy(int id);
    const DB_MarketBlack* GetBlack(int id);
    const DB_MarketCollect* GetCollect(int id);
    const DB_SellProgItems* GetTicket(int id);
    const DB_MysteryShop* GetMysteryGoods(int id);
	const DB_ActivityObsidianCollect* GetActivityObsidianCollect(int id);
	const ActivityObsidianCollectMap& GetActivityObsidianCollectMap() const;

    int RandCollectMulti(int id);
	//void UpdateMatkets();
};
#define sMarketTableMgr MarketTableMgr::Instance()