#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Market.pb.h"
#include "PlayerMarketLog.h"
#include "ActivityControl/ActivityMgr.h"
using namespace pb;

OpHandler(CMSG_MARKET_ALL_INFO)
{
    m_marketLog->UpdateBuyTimes(this);

    GS2C_Market_All_Info msg;
    m_marketLog->SaveTo(msg);
    Send(SMSG_MARKET_ALL_INFO, msg);
}
OpHandler(CMSG_MARKET_COLLECT)
{
    C2GS_Market_Collect req; pack >> req;
	uint32 table_id = req.table_id();
	uint32 coll_time = req.coll_times();
	GS2C_Market_Collect msg;
    if(m_marketLog->Collect(this, table_id,coll_time,msg))
	{
		UpdateAllDailyQuestInfo(COLLECT,coll_time,0);//每日任务
	}
	Send(pb::SMSG_MARKET_COLLECT,msg);
}
OpHandler(CMSG_MARKET_BUY)
{
    C2GS_Market_Buy req; pack >> req;

    if(m_marketLog->Buy(*this, req.idx()))
	{
		LLOG("MARKET BUY ONCE");
		GS2C_Market_Buy msg;
		m_marketLog->SaveTo(msg);
		Send(SMSG_MARKET_BUY, msg);
		UpdateAllDailyQuestInfo(MARKET_BUY,1,0);//每日任务
	}
}
OpHandler(CMSG_MARKET_BLACK)
{
    C2GS_Market_Black req; pack >> req;

    m_marketLog->BlackMarket(*this, req.table_id(), req.idx());
}
OpHandler(CMSG_MARKET_TICKET_CHANGE)
{
    C2GS_Market_Ticket_Change req; pack >> req;

    m_marketLog->TicketChange(*this, req.table_id());
}
OpHandler(CMSG_MARKET_MYSTERY_INVEST)
{
    C2GS_Market_Mystery_Shop_Invest req; pack >> req;

    m_marketLog->MysteryShopInvest(*this, req.table_id());
}
OpHandler(CMSG_MARKET_MYSTERY_TAKE_ITEM)
{
    C2GS_Market_Mystery_Shop_Take_Item req; pack >> req;

    m_marketLog->MysteryShopTakeItem(*this, req.table_id());
}

OpHandler(CMSG_ACTIVITY_IRON_COLLECT_REWARD)
{
	sActivityMgr.MarkTakeInActivity(pb::ACTIVITY_IRON_COLLECT,GetGuid());
	C2GS_Activity_Iron_Collect_Reward req;pack >> req;
	m_marketLog->TakeActivityIronCollectReward(this,req.id());
}

