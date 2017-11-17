#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "LotteryLog.h"
#include "Map/PlayerMapLogic.h"
#include "Map/country/country.h"

#include "Item.pb.h"
#include "Opcode.pb.h"
#include "Activity.pb.h"
using namespace pb;




OpHandler( CMSG_ALL_LOTTERY_FREE_REQ)
{
   /* if ( !m_lotteryLogger.get())
        return;

    GS2C_AllLotteryInfo msg;
    m_lotteryLogger->SaveTo(msg);
    
    Send( SMSG_ALL_LOTTERY_FREE_RESP, msg);*/
}

OpHandler( CMSG_USE_FREE_LOTTERY)
{
    uint32 lotteryType = 0;
    pack >> lotteryType;
    GS2C_LootResult msg;
    uint32 err_code = LotteryLogic::PlayerTryUseFreeLottery( this, lotteryType, msg);
    msg.set_err_code( err_code);

    Send( SMSG_USE_FREE_LOTTERY_RESP, msg); 
}

OpHandler( CMSG_LOTTERY_ONE)
{
    uint32 lotteryType = 0;
    pack >> lotteryType;
    GS2C_LootResult msg;
    uint32 err_code = LotteryLogic::PlayerTryLotteryOne( this, lotteryType, msg);
    msg.set_err_code( err_code);

    Send( SMSG_LOTTERY_ONE_RESP, msg);
}

OpHandler( CMSG_LOTTERY_TEN)
{
    uint32 lotteryType = 0;
    pack >> lotteryType;
    GS2C_LootResult msg;
    uint32 err_code = LotteryLogic::PlayerTryLotteryTen( this, lotteryType, msg);
    msg.set_err_code( err_code);

    Send( SMSG_LOTTERY_TEN_RESP, msg);
}

OpHandler( CMSG_LOOT_EQUIP_SHOP)
{
	GS2C_LootShopEquipResult msg;
	if (TestFlag(pb::PLAYER_FLAG_FIRST_REFRESH_EQUIP_SHOP))
	{
		msg.set_err_code(ErrCommonSuccess);
		int itemIdArry[6] = {1000001,1020001,1040001,1010001,1030001,1050001};
		for (int i=0; i<6; i++)
		{
			pb::GS2C_SlotItem* soltItem = msg.mutable_loot_list()->add_items();
			soltItem->set_is_deleted(false);
			soltItem->mutable_item()->set_proto_id(itemIdArry[i]);
			soltItem->mutable_item()->set_count(1);
			soltItem->mutable_item()->set_is_new(true);
			soltItem->mutable_item()->set_is_sold(false);
			soltItem->mutable_equip()->set_equip_to_charid(0);
		}
		UpdateEquipShop(msg);
		time_t colTime =  GetEquipShopBuyCoolDownTime();
		if (colTime < sOS.GetRealTime())
		{
			SetEquipShopBuyCoolDownTime(sOS.GetRealTime());
			colTime =  GetEquipShopBuyCoolDownTime();
		}
		int cd = 0;
		if (m_worldMapLogic->GetWorldMapCountry()->HaveSpecialCity(MapCity::Equipment))
			cd = SHOP_EQUIP_BUY_ONCE_COOLDOWN_SEC_TIME/2;
		else
			cd = SHOP_EQUIP_BUY_ONCE_COOLDOWN_SEC_TIME;

		SetEquipShopBuyCoolDownTime(colTime + cd);
		RemoveFlag(pb::PLAYER_FLAG_FIRST_REFRESH_EQUIP_SHOP);
	}
	else
	{
		uint32 err_code = LotteryLogic::PlayerTryLootShopEquip( this, msg);
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
	Send( SMSG_LOOT_EQUIP_SHOP_RESP, msg);
}

OpHandler( CMSG_RESET_BUY_ITEM_CD)
{
	if(!TestFlag(pb::PLAYER_NO_SHOP_COOL_DOWN_TIME))
	{
		SendErrorCode(pb::ErrNeedVipPrivilege);
	}
	uint32 nNeedGood = GetDbBaseDefineValueUint(pb::BD_CLEAR_EQUIPSHOP_COST_GOLD);
	if (EnoughGold(nNeedGood))
	{
		TryDeductCurrency(pb::IR_EESET_TIME_COST,eSysGold,nNeedGood);
		SetEquipShopBuyCoolDownTime(0);
	}
	else
	{
		SendErrorCode(ErrNotEnoughGold);
	}
}


OpHandler( CMSG_EQUIP_PURIFY)
{
	uint32 purifyType = 0;
	uint32 itemBagSolt = 0;
	pack >> itemBagSolt >> purifyType;
	uint32 err_code = LotteryLogic::PlayerTryLootEquipSkillAttr( this, purifyType,itemBagSolt);
	if (ErrCommonSuccess != err_code)
	{
		NetPack packet( SMSG_EQUIP_PURIFY_RESP, sizeof(uint32)  + 2);
		packet << err_code;
		Send(packet);

	}
	UpdateAllDailyQuestInfo(XILIAN,1,0);//每日任务  洗练
	AddXiLianValue(1);
}

OpHandler( CMSG_EQUIP_PURIFY_RETRIEVAL)
{
	uint32 itemBagSolt = 0;
	pack >> itemBagSolt;
	uint32 err_code = LotteryLogic::PlayerTryRetrievalPurify( this, itemBagSolt);
	if (ErrCommonSuccess != err_code)
	{
		NetPack packet( SMSG_EQUIP_PURIFY_RETRIEVAL_RESP, sizeof(uint32)  + 2);
		packet << err_code;
		Send(packet);
	}
}

OpHandler(CMSG_EQUIP_PURIFY_ITEM)
{
	pb::C2GS_Purify_Item msg;
	pack >> msg;
	uint32 err_code = LotteryLogic::PlayerTryLootEquipSkillAttrByXilianItem(this,msg.type(),msg.solt_id(),msg.attr_id());
	pb::GS2C_Purify_Item_Rsp sendmsg;
	sendmsg.set_err_code(err_code);
	Send(pb::SMSG_EQUIP_PURIFY_ITEM_RSP,sendmsg);
}