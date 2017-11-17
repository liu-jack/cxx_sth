#include "PlayerSellLogic.h"

#include "server_client/DbProxyServerClient.h"

//#include "GameServer.h"
#include "GlobalKeyValue.h"

#include "object/Player.h"
#include "object/PlayerDataManager.h"

#include "item/ItemArray.h"

#include "quest/behavior/PlayerBehaviorCounter.h"
#include "PlayerItemLogic.h"
#include "sell/SellLog.h"
#include "sell/SellManager.h"

#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "item/ItemEquip.h"
#include "Enum.pb.h"

using namespace pb;

namespace
{
    inline uint32 VictimType2QuestCat2( VictimType victim)
    {
        return victim + 1;
    }

    bool TryGetNeedCrystal(VictimType victim_type, uint32& need_crystal )
    {
        switch( victim_type)
        {
        case SELL_VICTIM_HERO:
            need_crystal = GET_BASE_DEF_UINT( BD_SELL_VICTIM_HERO_MANUAL_REFRESH_GOLD_COUNT);
            return true;
        case SELL_VICTIM_AWAKE:
            need_crystal = GET_BASE_DEF_UINT( BD_SELL_VICTIM_AWAKE_MANUAL_REFRESH_GOLD_COUNT);
            return true;
        default:
            return false;
        }
    }

    inline void TriggerQuestBuyItem( Player &player, const LogicSellItem* logicSellItem, uint32 count )
    {
        if ( PlayerBehaviorCounter * counter = player.m_behaviorCounter.get())
        {
            counter->TriggerBehavior(&player, QC1_SHOP, QC2_BUY_SPECIFIC_ITEM, QC3_BUY, logicSellItem->ItemProtoId(), count);
            counter->TriggerBehavior(&player, QC1_SHOP, VictimType2QuestCat2(logicSellItem->GetVictimType()),  QC3_BUY , 0 , count);
        }
    }

    bool IsMatchBuyRequire( Player& player, const DB_SellProgItems& sell_item)
    {
        switch( sell_item.require_trigger)
        {
        case DB_SellProgItems::REQUIRE_TYPE_VIP:
            if ( sell_item.require_value > (uint32)player.GetVipLevel() )
            {
                return false;
            }
            break;

        case DB_SellProgItems::REQUIRE_TYPE_PLAYER_LV:
            if ( sell_item.require_value > (uint32)player.GetLevel() )
            {
                return false;
            }
            break;

        case DB_SellProgItems::REQUIRE_TYPE_NONE:
            {
                return true;
            }
            break;

        default:
            MASSERT( false, "Undefine require");
            break;
        }

        return true;
    }
}


PlayerSellLogic::ErrCode PlayerSellLogic::HandleBuyEquipShopSellItem(
	Player& player, uint32 solt_id, uint32 count)
{
	if ( !player.m_bag->HasFreeSolt())
		return ErrCommonFail;

	Item* curItem = player.m_equipShop->MutableItem(solt_id);
	if ( curItem == NULL)
		return ErrCommonFail;

	if(curItem->IsBoughtInMarket())
	{
		NLOG("The Item Is Already Bought");
		return ErrCommonFail;
	}

	ItemEquip* item_equip = dynamic_cast<ItemEquip*>( curItem);
	if ( item_equip == NULL || item_equip->IsEquiped())
	{
		return ErrCommonFail;
	}
	if (item_equip->IsSold())
	{
		return ErrNotBuyTimes;
	}

	if ( !player.EnoughCurrency(eCoin, curItem->Proto()->sell_price*count* 2))
		return ErrNotEnoughCurr;

	ErrCode code = ErrCommonSuccess;
	std::list< Item *> outList;
	code = player.CreateItem( pb::IR_VENDOR_GET, curItem->Proto()->id,  count,&outList);
	if (code != pb::ErrCommonSuccess )
	{
		return code;
	}
	MMO_ASSERT(outList.size() == 1);
	bool result = player.TryDeductCurrency( pb::IR_VENDOR_COST, eCoin, curItem->Proto()->sell_price*count* 2);
	MMO_ASSERT( result == true);
	Item* curEquipItem = outList.front();
	ItemEquip* cur_item_equip = dynamic_cast<ItemEquip*>( curEquipItem);
	if ( cur_item_equip)
	{
		cur_item_equip->LoadFrom(item_equip);
		player.m_equipShop->ItemChecked(solt_id);
		item_equip->SetSold(true);
	}

	curItem->SetIsBoughtInMarket(true);

	return code; 
}



PlayerSellLogic::ErrCode PlayerSellLogic::HandleBuySellItem(
    Player& player, SellId sell_id, uint32 count,
    ItemId& item_id, uint32& item_extra_count, uint32& hitCritical)
{
    /*SellLog* sellLog = player.m_sellLog.get();
    if ( !sellLog)*/
        return ErrCommonFail;

    //const LogicSellItem* logicSellItem = sSellMgr.GetLogicSellItem( sell_id);
    //if ( logicSellItem == NULL)
    //    return ErrCommonFail;

    //if( !IsMatchBuyRequire( player, logicSellItem->Proto()))
    //    return ErrCommonFail;

    //if ( !logicSellItem->IsProgramActiveWhen( sOS.GetRealTime()))
    //    return ErrSellProgramNotActive;
    //
    //uint32 sellcount = sellLog->GetTodaySellCount(*logicSellItem);
    //CurrType2Count curr2count;
    //float vipPriceScale =1;// sPlayerDataMgr.GetVipPlayerBuyScale(player.GetVipLevel());
    //logicSellItem->GetCurr2Count(sellcount, count, vipPriceScale, curr2count);

    //if ( !player.EnoughCurrency(curr2count))
    //    return ErrNotEnoughCurr;


    //ErrCode code = TryAddSellRecord( player, *logicSellItem, count);
    //if ( code == ErrCommonSuccess)
    //{
    //    bool result = player.TryDeductCurrency( pb::IR_VENDOR_COST, curr2count);
    //    MMO_ASSERT( result == true);


    //   // float vipCritical = sPlayerDataMgr.GetVipPlayerBuyCritical(player.GetVipLevel());
    //    //hitCritical = 1;//logicSellItem->VipCritial(count, vipCritical);
    //    item_extra_count = logicSellItem->BulkCount();// * hitCritical;
    //    item_id = logicSellItem->ItemProtoId();
    //    player.CreateItem( pb::IR_VENDOR_GET, logicSellItem->ItemProtoId(),  logicSellItem->BulkCount() * count);

    //    TriggerQuestBuyItem(player, logicSellItem, count);
    //}
    //return code; 
}

PlayerSellLogic::ErrCode PlayerSellLogic::HandleSellBagItem( Player& player, uint32 solt_id )
{

	if (player.SellSlotItem(pb::IR_ITEM_SELL_GET,solt_id))
	{
		return ErrCommonSuccess;
	}
	else
	{
		return ErrCommonFail;
	}
}

PlayerSellLogic::ErrCode PlayerSellLogic::HandleBuyBackItem( Player& player, uint32 solt_id )
{
	ItemArray* bag = player.m_bag.get();
	if ( !bag)
		return ErrCommonFail;
	Item* curItem = bag->MutableItem(solt_id);
	if (!curItem)
	{
		return ErrCommonFail;
	}
	if (player.IsRechBagCapcity())
	{
		if (curItem->CanStack())
		{
			if(NULL == bag->MutableOneItemByProId(curItem->Entry(),false))
			{
				player.SendErrorCode(ErrRechBagCapacity);
				return ErrRechBagCapacity;
			}
		}
		else
		{
			player.SendErrorCode(ErrRechBagCapacity);
			return ErrRechBagCapacity;
		}
	}
	return player.BuyBackSlotItem(pb::IR_PLAYER_BUY_BACK_ITEM_COST,solt_id);
}

PlayerSellLogic::ErrCode PlayerSellLogic::HandleManualRefresh( Player& player, VictimType victim_type )
{
    //SellLog *sellLog = player.m_sellLog.get();
    //ItemArray* bag = player.m_bag.get();

    //if ( !sellLog)
    //    return ErrCommonFail;

    //if ( !bag)
    //    return ErrCommonFail;

    //uint32 need_crystal = 0 ;
    //if ( !TryGetNeedCrystal(victim_type, need_crystal))
    //    return ErrCommonFail;

    //RandomProgram* rd_program = sellLog->GetRandomProgram(victim_type);
    //if ( rd_program == NULL)
    //    return ErrCommonFail;

    ////CxGS_ERROR_CODE code = ErrCommonFail;

    //bool haveItem = bag->EnoughItem( GET_BASE_DEF_UINT( BD_SELL_VICTIM_REFRESH_ITEM_ID), 1);
    //bool haveCashGold = player.EnoughGold(need_crystal); 

    //if ( haveItem || haveCashGold)
    //{
    //    if ( rd_program->TryManualRefresh( player.GetVipLevel()))
    //    {
    //        SendVictimRDProgramUpdate(player, victim_type);
    //        ResetSellProgram( player, rd_program->GetProgramId());

    //        if ( haveItem)
    //        {
    //            player.DestroyItem( pb::IR_VENDOR_REFRESH_COST, GET_BASE_DEF_UINT(BD_SELL_VICTIM_REFRESH_ITEM_ID), 1);
    //        }
    //        else
    //        {
    //            player.TryDeductCurrency( 
    //                pb::IR_VENDOR_REFRESH_COST,
    //                eSysGold, need_crystal);
    //        }
    //        return ErrCommonSuccess;
    //    }
    //    else
    //    {
    //        return ErrCommonFail;
    //    }
    //}

    return ErrNotEnoughCurr;
}

PlayerSellLogic::ErrCode PlayerSellLogic::HandleRequestSellProgram( Player& player, VictimType victim_type )
{
    /*RandomProgram* rd_program =  player.m_sellLog->GetRandomProgram(victim_type);

    if ( rd_program && rd_program->LazyUpdate())
    {
        SendVictimRDProgramUpdate(player, victim_type);
        ResetSellProgram(player, rd_program->GetProgramId());

        return ErrCommonSuccess;
    }
    else*/
        return ErrCommonFail;
}

void PlayerSellLogic::InitVictimRDProgram( Player& player )
{
   /* for ( int i = 0; i < SELL_VICTIM_COUNT; ++i)
    {
        if ( sSellMgr.IsVictimRandomProgram(i))
        {
            if ( player.m_sellLog->InitVictimRDProgram(i))
            {
                SendVictimRDProgramUpdate( player, i, false);
            }
        }
    } */
}

void PlayerSellLogic::SendSellProgramInfoUpdate( Player& player, ProgramID programid, SellId sell_id /*= 0*/ )
{
   /* pb::GS2C_SellProgramInfo msg;
    if (const SellProgramRecord* record = player.m_sellLog->GetSellProgramRecord( programid))
    {
        record->SaveTo(msg, sell_id, true);
        player.Send(pb::SMSG_UPDATE_SELL_PROGRAM_INFO, msg);
        
        msg.Clear();
        record->SaveTo(msg, sell_id, false);
        sDbProxyClient.SendPlayerPack( player,pb::SG2D_UPDATE_SELL_PROGRAM_INFO, msg);
    }
    else
        return;*/


}

void PlayerSellLogic::SendVictimRDProgramUpdate( Player& player, VictimType victim,  bool sendToClient /*= true*/)
{
   /* pb::GS2C_SellRandomProgram msg;
    if ( RandomProgram* program = player.m_sellLog->GetRandomProgram( victim))
    {
        program->SaveTo( msg);
        if ( sendToClient)
        {
            player.Send(pb::SMSG_UPDATE_SELL_RANDOM_PROGRAME, msg);
        }
        sDbProxyClient.SendPlayerPack( player,pb::SG2D_UPDATE_SELL_RANDOM_PROGRAME, msg); 
    }
    else
        return;*/

}

PlayerSellLogic::ErrCode PlayerSellLogic::TryAddSellRecord( Player& player, const LogicSellItem& logicSellItem, uint32 count )
{
    //VictimType victim = logicSellItem.GetVictimType();

    //SellLog * sell_log = player.m_sellLog.get();

    //RandomProgram* randomProgram = sell_log->GetRandomProgram( victim);
    //if ( randomProgram != NULL)
    //{
    //    if ( randomProgram->LazyUpdate())
    //        SendVictimRDProgramUpdate( player, victim);

    //    if ( randomProgram->GetProgramId() != logicSellItem.ProgramId())
    //        return ErrSellProgramNotActive;

    //}

    ////if ( IsGlobalConfiguredProgram( victim))
    ////{
    ////    if ( static_cast<uint32>( GetGlobalProgram(victim) ) != logicSellItem.ProgramId())
    ////        return ErrSellProgramNotActive;
    ////}

    //if ( sell_log->IsItemReachCountLimit(logicSellItem, count, player.GetVipLevel()))
    //    return ErrCommonFail;


    //SellProgramRecord* programRecord = sell_log->AddOrGetSellProgramRecord(logicSellItem.ProgramId());

    //if (programRecord)
    //{
    //    SellProgramRecord::OP_RESULT result = programRecord->TryAddSellItemCount( logicSellItem , count , player.GetVipLevel());
    //    switch ( result)
    //    {
    //    case SellProgramRecord::OP_FALSE:
    //        return false;
    //    case SellProgramRecord::OP_AFFECT_ONE:
    //        SendSellProgramInfoUpdate( player, logicSellItem.ProgramId(), logicSellItem.SellId());
    //        return ErrCommonSuccess;
    //    case SellProgramRecord::OP_AFFECT_ALL:
    //        SendSellProgramInfoUpdate( player, logicSellItem.ProgramId());
    //        return ErrCommonSuccess;
    //    default:
    //        break;
    //    }
    //}

    return ErrCommonFail;
}

void PlayerSellLogic::ResetSellProgram( Player &player, ProgramID programId )
{
    /*if ( SellProgramRecord* record = player.m_sellLog->MutableSellProgramRecord( programId))
    {
        record->ResetSellRecord();
        record->UpdateRecordValidTime();
        SendSellProgramInfoUpdate(player, programId);
    }*/
}
/*
bool PlayerSellLogic::IsGlobalConfiguredProgram( VictimType victim )
{
    return victim == SELL_VICTIM_PROMO 
        || victim == SELL_VICTIM_VIP_GIFT_CYCLE;
}

int PlayerSellLogic::GetGlobalProgram( VictimType victim )
{
    ASSERT( IsGlobalConfiguredProgram( victim));
    int programId = 0;
    if ( victim == SELL_VICTIM_PROMO)
    {
        sGameSvr.m_globalKeyValue.TryGetValueInt("SELL_PROMO_PROGRAM", programId);
    }
    else if ( victim == SELL_VICTIM_VIP_GIFT_CYCLE)
    {
        sGameSvr.m_globalKeyValue.TryGetValueInt("SELL_VIP_GIFT_CYCLE_PROGRAM", programId); 
    }
    else
    {
        ASSERT( false);
    }

    return programId;
}

*/








