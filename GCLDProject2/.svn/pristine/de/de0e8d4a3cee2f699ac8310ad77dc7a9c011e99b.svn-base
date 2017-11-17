#include "MarketTableMgr.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "System.h"
//#include "AutomaticUpdate/AutomaticUpdate.h"
#include "../../session/PlayerPool.h"
#include "../PlayerMarketLog.h"
static const int Ticket_Sell_ProtoId    = 1;

void MarketTableMgr::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_MarketBuy)
    {
        m_BuyTable[ptr1->id] = ptr1;
    }
    FOREACH_DB_ITEM(ptr2, DB_MarketBlack)
    {
        m_BlackTable[ptr2->id] = ptr2;
    }
    FOREACH_DB_ITEM(ptr3, DB_MarketCollect)
    {
        m_CollectTable[ptr3->id] = ptr3;
    }
    MMO_ASSERT(m_CollectTable.size() <= 5);

    FOREACH_DB_ITEM(ptr4, DB_SellProgItems)
    {
        if (ptr4->programID == Ticket_Sell_ProtoId)
        {
            m_TicketTable[ptr4->id] = ptr4;
        }
    }
    FOREACH_DB_ITEM(ptr5, DB_MysteryShop)
    {
        m_MysteryTable[ptr5->id] = ptr5;
        MMO_ASSERT(ptr5->id >= 1 && ptr5->id <= 5); //商品数目，见PlayerMarketLog::MysteryShop_GoodsCnt
    }
    FOREACH_DB_ITEM(ptr6, DB_ItemProtoType)
    {
        if (ptr6->IsSuitScrap())
        {
            m_SuitsPartIds.push_back(ptr6->id);
        }
    }
	FOREACH_DB_ITEM(ptr7, DB_ActivityObsidianCollect)
	{
		m_ActivityObsidianCollectMapTable[ptr7->id] = ptr7;
	}

	//sAutoUpdate.Add(boost::bind(&MarketTableMgr::UpdateMatkets,this),detail_enum::TIME_ZERO,detail_enum::PER_HOUR);
}
const DB_MarketBuy* MarketTableMgr::GetBuy(int id)
{
    return m_BuyTable.pfind(id);
}
const DB_MarketBlack* MarketTableMgr::GetBlack(int id)
{
    return m_BlackTable.pfind(id);
}
const DB_MarketCollect* MarketTableMgr::GetCollect(int id)
{
    return m_CollectTable.pfind(id);
}
const DB_SellProgItems* MarketTableMgr::GetTicket(int id)
{
    return m_TicketTable.pfind(id);
}
const DB_MysteryShop* MarketTableMgr::GetMysteryGoods(int id)
{
    return m_MysteryTable.pfind(id);
}

const DB_ActivityObsidianCollect* MarketTableMgr::GetActivityObsidianCollect(int id)
{
	return m_ActivityObsidianCollectMapTable.pfind(id);
}

const ActivityObsidianCollectMap& MarketTableMgr::GetActivityObsidianCollectMap() const
{
	return m_ActivityObsidianCollectMapTable;
}

int MarketTableMgr::RandCollectMulti(int id)
{
    if (const DB_MarketCollect* table = GetCollect(id))
    {
        IntPairVec vecWeight;
        Utility::SplitStr2(table->collect_critical, vecWeight);
        return System::RandWeight(vecWeight);
    }
    return 1;
}

//void MarketTableMgr::UpdateMatkets()
//{
//	NLOG("MarketTableMgr::UpdateMatkets");
//	FOREACH_PLAYERPTR(PlayerId,playerPtr)
//	{
//		if(!playerPtr->GetDataIsLost())
//		{
//			NLOG("MarketTableMgr::UpdateMatkets one");
//			playerPtr->m_marketLog->UpdateMatkets(playerPtr.get());
//		}
//	}
//}