#include "object/Player.h"

#include "SellLog.h"
#include "memory_buffer/NetPack.h"


#include "CrossLogic/PlayerSellLogic.h"
#include "Opcode.pb.h"
#include "item/ItemArray.h"
#include "Lottery/LotteryLog.h"
using namespace pb;

namespace PlayerHandle
{
    extern void SendOpcodeWithErrCode( Player* player, int opcode, pb::CxGS_ERROR_CODE err_code);

}

OpHandler( CMSG_ALL_SELL_LOG)
{
  /*  if ( !m_sellLog.get())
        return;
    GS2C_AllSellData sellData;
    m_sellLog->SaveTo(sellData);
    Send( SMSG_ALL_SELL_LOG, sellData);*/
}

OpHandler( CMSG_REQUEST_RANDOM_SELL_PROGRAM)
{
    //if ( !m_sellLog.get())
    //    return ;

    //VictimType victim_type = 0;
    //pack >> victim_type;

    //CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)PlayerSellLogic::HandleRequestSellProgram( *this, victim_type);
    //PlayerHandle::SendOpcodeWithErrCode( this, SMSG_REQUEST_RANDOM_SELL_PROGRAM_RESP, code); 

}

OpHandler( CMSG_BUY_SELL_ITEM)
{
    SellId sell_id = 0;
    uint32 count = 0;
    pack >> sell_id >> count;

    uint32 item_proto_id, item_extra_count, critical_count;

    CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)PlayerSellLogic::HandleBuySellItem(
        *this, sell_id, count, item_proto_id, item_extra_count, critical_count);
    NetPack packet( SMSG_BUY_SELL_ITEM_RESP, sizeof(uint32) * 4  + 2);
    packet << code;
    if ( code == ErrCommonSuccess)
    {
        packet << item_proto_id << item_extra_count << critical_count;
    }
    Send(packet);
}

OpHandler( CMSG_GET_EQUIP_SHOP_ITEMS)
{
	GS2C_LootShopEquipResult msg;
	if (m_equipShop->ItemCount() > 0)
	{
		m_equipShop->SaveTo(*msg.mutable_loot_list());
		msg.set_err_code( ErrCommonSuccess);
	}
	else
	{
		uint32 err_code = LotteryLogic::PlayerTryLootShopEquip( this, msg,true);
		int equipCout = msg.loot_list().items_size();
		if (equipCout == 6)
		{
			msg.set_err_code( err_code);
			UpdateEquipShop(msg);
		}
		else
		{
			msg.set_err_code( ErrCommonFail);
		}
	}
	Send( SMSG_GET_EQUIP_SHOP_ITEMS_RESP, msg);
}

OpHandler( CMSG_BUY_EQUIP_SHOP_SELL_ITEM)
{
	uint32 solt_id = 0;
	uint32 count = 0;
	pack >> solt_id >> count;
	CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)PlayerSellLogic::HandleBuyEquipShopSellItem(
		*this, solt_id, count);
	NetPack packet( SMSG_BUY_EQUIP_SHOP_SELL_ITEM_RESP, sizeof(uint32) * 2  + 2);
	packet << code;
	if ( code == ErrCommonSuccess)
	{
		packet << solt_id ;
	}
	else
	{
		packet << 0 ;
	}
	Send(packet);
}

OpHandler( CMSG_SELL_BAG_ITEM)
{
	uint32 solt_id = 0;
	pack >> solt_id ;
	CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)PlayerSellLogic::HandleSellBagItem(*this, solt_id);
	NetPack packet( SMSG_SELL_BAG_ITEM_RSP, sizeof(uint32) * 2  + 2);
	packet << code;
	if ( code == ErrCommonSuccess)
	{
		packet << solt_id ;
	}
	Send(packet);
}

OpHandler( CMSG_BUYBACK_BAG_ITEM)
{
	uint32 solt_id = 0;
	pack >> solt_id ;
	CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)PlayerSellLogic::HandleBuyBackItem(*this, solt_id);
	NetPack packet( SMSG_BUYBACK_BAG_ITEM_RSP, sizeof(uint32) * 2  + 2);
	packet << code;
	if ( code == ErrCommonSuccess)
	{
		packet << solt_id ;
	}
	Send(packet);
}

OpHandler( CMSG_MANUAL_REFRESH)
{
   /* if ( !m_sellLog.get())
        return ;

    if ( !m_bag.get())
        return;

    VictimType victim_type = 0;
    pack >> victim_type;

    CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)PlayerSellLogic::HandleManualRefresh(*this, victim_type);
    PlayerHandle::SendOpcodeWithErrCode( this, SMSG_MANUAL_REFRESH_RESP, code); */
}

//OpHandler( CMSG_REQUEST_GLOBAL_SHOP_PROGRAM)
//{
//    NetPack packet( SMSG_REQUEST_GLOBAL_SHOP_PROGRAM_RESP, 10);
//    packet << PlayerSellLogic::GetGlobalProgram( SELL_VICTIM_PROMO) 
//        << PlayerSellLogic::GetGlobalProgram( SELL_VICTIM_VIP_GIFT_CYCLE);
//    Send(packet);
//}