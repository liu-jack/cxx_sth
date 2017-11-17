#include "PlayerItemLogic.h"

#include <boost/typeof/typeof.hpp>

#include "server_client/DbProxyServerClient.h"

#include "item/LogicItemProto.h"
#include "item/ItemArray.h"
#include "item/Item.h"
#include "item/ItemEquip.h"
#include "item/ItemManager.h"

#include "object/Player.h"
#include "object/PlayerDataManager.h"

#include "character/CharacterStorage.h"

#include "quest/behavior/PlayerBehaviorCounter.h"

#include "Loot/LootList.h"
#include "Loot/LootManager.h"

#include "CrossLogic/ItemCharacterLogic.h"
#include "CrossLogic/PlayerCharacterLogic.h"
#include "../reward/reward.h"
#include "Enum.pb.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "BaseDefineMgr.h"
#include "Technology/PlayerTechnology.h"
#include "utility/Utility.h"
#include "../CrossLogic/PlayerBaoQiLogDB.h"
#include "Market/PlayerMarketLog.h"
#include "BaoQi/PlayerBaoQiLog.h"
using namespace pb;

namespace 
{
    bool TryGetQuestCat1ByItemType( const LogicItemProto& proto , uint32 questCat1)
    {
        if ( proto.IsEquip())
        {
            questCat1 = QC1_EQUIP;
            return true;

        }
        return false;
    }

    uint32 GetRuneQualityFactor( uint32 quality)
    {
        static const uint32 factor[] = {
            GET_BASE_DEF_UINT( pb::BD_DECOMPOSE_RUNE_QUALITY0),
            GET_BASE_DEF_UINT( pb::BD_DECOMPOSE_RUNE_QUALITY1),
            GET_BASE_DEF_UINT( pb::BD_DECOMPOSE_RUNE_QUALITY2),
            GET_BASE_DEF_UINT( pb::BD_DECOMPOSE_RUNE_QUALITY3),
            GET_BASE_DEF_UINT( pb::BD_DECOMPOSE_RUNE_QUALITY4),
            GET_BASE_DEF_UINT( pb::BD_DECOMPOSE_RUNE_QUALITY5)
        };
        ASSERT(quality < sizeof( factor ) / sizeof(uint32));
        return factor[quality];
    }

    float GetEquipQualityFactor( uint32 quality)
    {
        static const float factor[] = {
            GET_BASE_DEF_FLOAT( pb::BD_DECOMPOSE_EQUIP_QUALITY0),
            GET_BASE_DEF_FLOAT( pb::BD_DECOMPOSE_EQUIP_QUALITY1),
            GET_BASE_DEF_FLOAT( pb::BD_DECOMPOSE_EQUIP_QUALITY2),
            GET_BASE_DEF_FLOAT( pb::BD_DECOMPOSE_EQUIP_QUALITY3),
            GET_BASE_DEF_FLOAT( pb::BD_DECOMPOSE_EQUIP_QUALITY4),
            GET_BASE_DEF_FLOAT( pb::BD_DECOMPOSE_EQUIP_QUALITY5)
        };
        ASSERT(quality < sizeof( factor )/sizeof(float));
        return factor[quality];
    }
}

PlayerItemLogic::ErrCode PlayerItemLogic::UseItem( Player* player, const LogicItemProto& proto, pb::GS2C_ItemUse& msg, uint32 count )
{
    ErrCode err_code = pb::ErrCommonSuccess;

    if ( proto.IsBoxMed())
    {
        LootList loot_list;
        for( uint32 i = 0; i< count; ++i)
        {
            FillLootList( player, proto, loot_list);
        }
        loot_list.PickAllMoneyAndItems( player, *msg.mutable_loot_list(), pb::IR_OPEN_BOX_GET);
    }
    else if ( proto.IsPuzzle())
    {
        if ( proto.IsPuzzleCard())
        {
            PlayerCharacterLogic::CreateCharacterForPlayer( player, proto.ObjectValue());
            msg.set_is_card(true);
        }
        else
        {
            player->CreateItem( pb::IR_ITEM_COMPOUND_GET, proto.ObjectValue() );
        }

        uint32 cat1 = 0;
        if ( TryGetQuestCat1ByItemType(proto, cat1))
        {
            if ( PlayerBehaviorCounter * counter = player->m_behaviorCounter.get())
            {
                counter->TriggerBehavior(player, cat1, QC2_SPECIFIC_ITEM, QC3_COMBINED, proto.ObjectValue() );
                counter->TriggerBehavior(player, cat1, QC2_ANY_ITEM,  QC3_COMBINED);
            }
        }  

        msg.set_item_id( proto.ObjectValue());
    }
	else if ( proto.IsVoucher())
	{
		if(proto.IsIronTenPunk())
		{
#ifdef _MMO_SERVER_
			std::map<uint32,iron_collect>::iterator iter = player->m_marketLog->iron_ticket_.find(proto.Id());
			if(iter != player->m_marketLog->iron_ticket_.end())
			{
				iter->second.left_times += count;
			}
			else
				player->m_marketLog->iron_ticket_[proto.Id()] = iron_collect(sOS.TimeSeconds(),count);
#endif
		}
		if(proto.IsShenQiPunk())
		{
			player->m_BaoQiLog->ItemUsed_[sOS.TimeSeconds()] = item_use(proto.Id(),count);
		}
		PlayerBaoQiLogDB::SendAllInfoToDb(*player);
	}
    else if ( proto.IsCurrency())
    {
        pb::GS2C_LootList msg;
        if ( count != 0)
        {
            player->AddCurrency(
                pb::IR_ITEM_USE_GET,
				proto.category_3, 
                count
                );

            pb::PairValue* itemMsg = msg.add_curency_type_count();
            itemMsg->set_first( proto.category_3);
            itemMsg->set_second( count);
        }
        player->Send( pb::SMSG_ITEM_GET_LIKE_LOOT, msg);
    }
    else
    {
        err_code = pb::ErrCommonFail;
    }

    return err_code;
}


PlayerItemLogic::ErrCode PlayerItemLogic::UseItem( Player* player, int32 slot, pb::GS2C_ItemUse& msg, int32 count /*= 1*/ )
{
    ItemArray* bag = player->m_bag.get();
    if ( bag == NULL)
        return pb::ErrCommonFail;

    if ( !bag->IsValidSlot(slot))
    {
        return pb::ErrInvalidItemId;
    }

    Item* item = bag->MutableItem(slot);
    if (item == NULL)
    {
        return pb::ErrInvalidItemId;
    }
    const LogicItemProto* item_proto = item->Proto();


    if ( item_proto->IsPuzzle())
    {
        if ( !item_proto->TryGetPuzzleComposeCount( count))
        {
            return pb::ErrCommonFail;            
        }        
    }
    if (item->StackCount() < count)
    {
        return pb::ErrNotEnoughItem;
    }

    ErrorCode err_code = PlayerItemLogic::UseItem( player, *item_proto, msg, count);

    if ( err_code == pb::ErrCommonSuccess)
    {
        player->TakeOutSlotItem( pb::IR_ITEM_USED_COST, slot, count);
    }
    return err_code; 
}

void PlayerItemLogic::UseItem(Player* player,IntPairVec& vec)
{
	for(IntPairVec::iterator it = vec.begin();it != vec.end();++it)
	{
		const LogicItemProto* proto = sItemMgr.Find(it->first);
		if(proto)
		{
			if(proto->IsVoucher())
			{
				if(proto->IsIronTenPunk())
				{
#ifdef _MMO_SERVER_
				std::map<uint32,iron_collect>::iterator iter = player->m_marketLog->iron_ticket_.find(proto->Id());
				if(iter != player->m_marketLog->iron_ticket_.end())
				{
					iter->second.left_times += it->second;
				}
				else
					player->m_marketLog->iron_ticket_[proto->Id()] = iron_collect(sOS.TimeSeconds(),it->second);
#endif
				}
				if(proto->IsShenQiPunk())
				{
					player->m_BaoQiLog->ItemUsed_[sOS.TimeSeconds()] = item_use(proto->Id(),it->second);
				}
				PlayerBaoQiLogDB::SendAllInfoToDb(*player);
			}
		}
	}
}

bool PlayerItemLogic::FillLootList( Player* player, const LogicItemProto& proto, LootList& lootList )
{
#ifdef _MMO_SERVER_
    LootParam param;
    param.provider = NULL;//player->m_lootRecord.get();
    param.victim_type = LOOTTYPE_ITEM;
    param.victim_id = proto.Id();
    param.victim_lv = proto.ObjectValue();
    param.player_vip_lv = player->GetVipLevel();

    return sLootMgr.FillLootList(player,param,lootList);
#endif
	return false;
}

bool PlayerItemLogic::PlayerLootImmBox( Player* player, const LogicItemProto& proto )
{
#ifdef _MMO_SERVER_
    if ( player == NULL)
        return false;

    if ( !proto.IsBoxImm())
        return false;

    LootList lootList;
    FillLootList( player, proto, lootList);
    pb::GS2C_ItemUse msg;
    bool result = lootList.PickAllMoneyAndItems(player, *msg.mutable_loot_list(), pb::IR_OPEN_BOX_GET);
    if ( result )
    {
        msg.set_err_code( pb::ErrCommonSuccess);
        player->Send( pb::SMSG_ITEM_USE_RESP, msg);
    }
    return result;
#endif
	return false;
}

bool PlayerItemLogic::TryGetItemEquip( ItemArray* bag, uint32 itemId, ItemEquip*& outItemEquip )
{
    Item* the_item = bag->MutableItem(itemId);
    if ( the_item == NULL)
    {
        return false;
    } 
    outItemEquip = dynamic_cast<ItemEquip*>( the_item);
    if ( outItemEquip == NULL)
    {
        return false;
    }
    return true;
}



PlayerItemLogic::ErrCode PlayerItemLogic::SuitComplex( Player* player, uint32 suitProtoId )
{
	if (player)
	{
		if(player->HaveTechnology(SUIT))
		{
			const ItemSuitComplex* suitComp = sItemMgr.FindSuitComplex(suitProtoId);
			if (suitComp)
			{
				const LogicItemProto* logicPro = sItemMgr.Find(suitProtoId);
				if (logicPro)
				{
					if (logicPro->Isprisuit())
					{
						std::vector<int> reqItemSoltVec;
						for (int i = 0; i< 6; i++)
						{
							uint32 reqId = suitComp->req_equip[i];
							uint32 reqAttr = suitComp->req_attr[i];
							int ItemSolt = -1;
							if (!player->m_bag->HasSameMaxLvAttrEquip(reqId,reqAttr,ItemSolt))
							{
								return pb::ErrNotEnoughItem;
							}
							if (ItemSolt < 0)
							{
								return pb::ErrNotEnoughItem;
							}
							reqItemSoltVec.push_back(ItemSolt);
						}

						return SuitComplexPri(player,suitComp,reqItemSoltVec);
					}
					else if (logicPro->Ismidsuit())
					{
						for (int i = 0; i< 2; i++)
						{
							uint32 reqId = suitComp->req_equip[i];
							if (!player->m_bag->HasItem(reqId))
							{
								return pb::ErrNotEnoughItem;
							}
						}

						return SuitComplexMid(player,suitComp);
					}
				}
				else
				{
					return pb::ErrInvalidProto;
				}
			}
			else
			{
				return pb::ErrInvalidProto;
			}
		}
		else
			return pb::ErrNoTechnology;
	}

	return pb::ErrCommonFail;
}

PlayerItemLogic::ErrCode PlayerItemLogic::BuyBagCapacity( Player* player )
{
	if (player)
	{
		uint32 curBagCapacity = player->GetBagCapacity();
		if (curBagCapacity < GET_BASE_DEF_UINT(BD_PLAYER_BAG_MAX_CAPACITY))
		{
			const ItemBagSoltSell* bagSoltSellInfo = sItemMgr.FindBagsoltSellInfo(curBagCapacity+1);
			if (bagSoltSellInfo)
			{
				const LogicItemProto* curItem = sItemMgr.Find(bagSoltSellInfo->need_item);
				if (curItem)
				{
					if (curItem->IsCurrency())
					{
						if(!player->EnoughCurrency(curItem->category_3,bagSoltSellInfo->need_num))
						{
							return pb::ErrNotEnoughCurr;
						}
						if(player->TryDeductCurrency(IR_PLAYER_BUY_ITEM_COST,curItem->category_3,bagSoltSellInfo->need_num))
						{
							player->SetBagCapacity(curBagCapacity +1);
							return pb::ErrCommonSuccess;
						}
						else
						{
							return pb::ErrNotEnoughCurr;
						}

					}
				}
			}
		}	
	}
	return pb::ErrCommonFail;
}

PlayerItemLogic::ErrCode PlayerItemLogic::SuitComplexPri( Player* player, const ItemSuitComplex* suitComp, std::vector<int>& vecSolt )
{

	if ((int32)suitComp->creat_req_level > player->GetLevel())
	{
		return pb::ErrPlayerLevelNotEnough;
	}

	if (player->m_bag->CountItem(suitComp->need_paper_id) < (int)suitComp->need_paper_num)
	{
		return pb::ErrNotEnoughItem;
	}

	ASSERT((int)vecSolt.size() == 6);
	for (int i = 0; i < 6; i++)
	{
		int curSolt = vecSolt[i];
		player->TakeOutSlotItem(pb::IR_ITEM_COMPOUND_COST,curSolt,1);
	}

	player->CreateItem(pb::IR_ITEM_COMPOUND_GET,suitComp->item_id,1);
	
	IntPairVec vec;
	vec.push_back(std::make_pair(suitComp->need_paper_id, -(int)suitComp->need_paper_num));
	sReward.Change(*player, vec);

	return pb::ErrCommonSuccess;
}

PlayerItemLogic::ErrCode PlayerItemLogic::SuitComplexMid( Player* player, const ItemSuitComplex* suitComp )
{
	if ((int32)suitComp->creat_req_level > player->GetLevel())
	{
		return pb::ErrPlayerLevelNotEnough;
	}

	if (!player->m_bag->EnoughItem(suitComp->need_paper_id,suitComp->need_paper_num))
	{
		return pb::ErrNotEnoughItem;
	}

	ItemEquip * equip[2];
	equip[0] = NULL;
	equip[1] = NULL;
	equip[0] = player->m_bag->MutableItemEquipByProId(suitComp->req_equip[0]);
	equip[1] = player->m_bag->MutableItemEquipByProId(suitComp->req_equip[1]);
	if (equip[0] == NULL ||  equip[1] == NULL)
	{
		return pb::ErrNotEnoughItem;
	}

	for (int i=0; i< 2; i++)
	{
		player->TakeOutSlotItem(pb::IR_ITEM_COMPOUND_COST,equip[i]->BagSlot(),1);
	}

	player->TakeOutItem(pb::IR_ITEM_COMPOUND_COST,suitComp->need_paper_id,suitComp->need_paper_num);
	return pb::ErrCommonSuccess;
}



PlayerItemLogic::ErrCode PlayerItemLogic::ComposeGem( Player* player, uint32 itemSlotId )
{
	ItemArray* bag = player->m_bag.get();
	if ( bag == NULL)                                                                
		return ErrCommonFail;

	Item* pSelectItem = bag->MutableItem(itemSlotId);
	if (pSelectItem && pSelectItem->IsJewel())
	{
		uint32 maxGemLevel = player->GetMaxGemLevel();
		if(pSelectItem->Proto()->category_3 + 1 >= maxGemLevel)		//等级到最大了
		{
			return ErrRechGemMaxLevel;
		}

		int nItemCount = pSelectItem->StackCount();	//拥有量
		if (nItemCount < (int)pSelectItem->Proto()->value03)	//需要数量
		{
			return ErrNotEnoughItem;
		}
		int nNewItem = nItemCount / pSelectItem->Proto()->value03;	//合成后新宝石的数量
		int nToDel = nNewItem * pSelectItem->Proto()->value03;	//待删除的宝石颗数
		if (pSelectItem->Proto()->value01 == 0)	//不能够再合成
		{
			//TODO,先判断已有多少颗晶石，一共不能超过6颗，再合成。
		}
		else
		{
			ItemID2Count id2count;
			id2count[pSelectItem->ProtoId()] = nToDel;
			ItemID2Count id2countNew;
			id2countNew[pSelectItem->Proto()->value01] = nNewItem;
			if ( !player->DestroyItem( pb::IR_ITEM_COMPOUND_COST, id2count))
				return ErrCommonFail;
			player->CreateItem( pb::IR_ITEM_COMPOUND_GET, id2countNew);        
		}
	}
	else
	{
		return ErrInvalidItemId;
	}
	return ErrCommonSuccess;
}

PlayerItemLogic::ErrCode PlayerItemLogic::DecomposeGem( Player* player, uint32 itemSlotId )
{
	ItemArray* bag = player->m_bag.get();
	if ( bag == NULL)                                                                
		return ErrCommonFail;

	Item* pSelectItem = bag->MutableItem(itemSlotId);
	if (pSelectItem && pSelectItem->IsJewel())
	{
		if (pSelectItem->Proto()->value02 == 0)
		{
			return ErrItemLevelLimited;
		}
		else
		{
			int nItemCount = pSelectItem->StackCount();
			if (nItemCount < 1)
			{
				return ErrNotEnoughItem;
			}
			int nNewItem =  pSelectItem->Proto()->value03;
			ItemID2Count id2count;
			id2count[pSelectItem->ProtoId()] = 1;
			ItemID2Count id2countNew;
			id2countNew[pSelectItem->Proto()->value02] = nNewItem;
			if ( !player->DestroyItem( pb::IR_RUN_DECOMPOSE_COST, id2count))
				return ErrCommonFail;
			player->CreateItem( pb::IR_RUN_DECOMPOSE_GET, id2countNew);        
		}
	}
	else
	{
		return ErrInvalidItemId;
	}
	return ErrCommonSuccess;
}

void PlayerItemLogic::SendBagModify( Player* player )
{
    if ( player == NULL)
        return;

    ItemArray* bag = player->m_bag.get();
    if ( bag == NULL)                                                                
        return;

    if ( bag->HasModified())
    {
        pb::GS2C_Bag msg;
        bag->WriteModifiedTo( msg );
        player->Send( pb::SMSG_BAG_UPDATE, msg);
#ifdef _MMO_SERVER_
        sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_BAG, msg);
#endif
        bag->SetUnmodified();
    }
	std::vector<int> todelIds;
	if(bag->CheckExpireItem(todelIds))
	{
		for (std::vector<int>::iterator itemIter = todelIds.begin(); itemIter != todelIds.end(); ++itemIter)
		{
			player->OnDestoryItemLog(IR_ITEM_EXPIRY_COST,(int)(*itemIter),1);
		}
	}
}
