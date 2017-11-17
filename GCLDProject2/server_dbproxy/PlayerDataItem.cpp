#include "PlayerData.h"

// PlayerData.cpp 部分实现

//////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include "data/DbProxyDataMgr.h"

#include "Item.pb.h"



void PlayerData::UpdateBag(const pb::GS2C_Bag& bagUpdate )
{
    for (int i = 0 ; i < bagUpdate.items_size(); ++i)
    {
        UpdateBagItem( bagUpdate.items(i));
    }
}

void PlayerData::UpdateBagItem(const pb::GS2C_SlotItem& itemUpdate )
{
    ItemBaseTable* itemBase = m_ItemBaseTable.GetElement( itemUpdate.slot());
    if (itemBase == NULL)
    {
        AddItem(itemUpdate);
    }
    else
    {
        ItemEquipTable* itemEquip = m_ItemEquipTable.GetElement( itemUpdate.slot()) ;
        if ( itemUpdate.is_deleted())
        {
            m_ItemBaseTable.DeleteEntry( itemBase);
            if ( itemEquip)
            {
                m_ItemEquipTable.DeleteEntry( itemEquip); 
            }
        }
        else
        {
            // 由于 slot 的复用， 会出现下面的情况
            // 1.装备被删除后，普通物品 复用了原有slot
            // 2.装备被删除后，装备 复用了原有slot
            // 3.普通物品被删除后， 装备 复用了原有slot
            // 4.普通物品被删除后， 普通物品 复用原有slot

            SetIsDirty(DIRTY_ITEM);
            itemBase->LoadFrom( itemUpdate.item());
            m_ItemBaseTable.SetModifiedID( itemUpdate.slot());
            if ( itemEquip)
            {
                if ( itemUpdate.has_equip())
                {
                    itemEquip->LoadFrom( itemUpdate.equip());
                    m_ItemEquipTable.SetModifiedID( itemUpdate.slot());
                }
                else 
                {
                    m_ItemEquipTable.DeleteEntry(itemEquip);
                }
            }
            else{
                if ( itemUpdate.has_equip())
                {
                    AddItemEquip(itemUpdate);

                }
                else
                {
                    // ...
                }
            }
        }
    }   
}

void PlayerData::AddItemEquip( const pb::GS2C_SlotItem &itemUpdate )
{
    ItemEquipTable itemEquipInfo;
    itemEquipInfo.player_id = GetPlayerId();
    itemEquipInfo.slot_id = itemUpdate.slot();
    itemEquipInfo.LoadFrom( itemUpdate.equip());
    m_ItemEquipTable.AddAndAddToCache( &itemEquipInfo);
}


void PlayerData::AddItem( const pb::GS2C_SlotItem &itemUpdate )
{
    ItemBaseTable itemBaseInfo;
    itemBaseInfo.player_id = GetPlayerId();
    itemBaseInfo.slot_id = itemUpdate.slot();
    itemBaseInfo.LoadFrom( itemUpdate.item());

    if ( m_ItemBaseTable.AddAndAddToCache( &itemBaseInfo))
    {
        if (itemUpdate.has_equip())
        {
            AddItemEquip(itemUpdate);
        }
    }
}

void PlayerData::WritePlayerBagInfo( pb::GS2C_Bag& bagInfo )
{
    for ( BOOST_AUTO( it , m_ItemBaseTable.Begin());
        it != m_ItemBaseTable.End(); ++it)
    {
        _WritePlayerBagInfo(bagInfo, it->second);
    }
}

void PlayerData::_WritePlayerBagInfo( pb::GS2C_Bag& bagInfo, ItemBaseTable* item_base )
{
    if ( item_base )
    {
        pb::GS2C_SlotItem* slotItem = bagInfo.add_items();
        if ( slotItem)
        {
            slotItem->set_slot( item_base->slot_id);
            pb::GxDB_ItemInfo* item_info = slotItem->mutable_item();
            if (item_info)
            {
                item_base->SaveTo( *item_info);
            }

            pb::GxDB_EquipInfo* equip_info = slotItem->mutable_equip();
            ItemEquipTable* item_equip = m_ItemEquipTable.GetElement( item_base->slot_id);
            if ( item_equip && equip_info)
            {
                item_equip->SaveTo( *equip_info);    
            }
        }
    }
}


bool PlayerData::save_mode_item()
{
    m_ItemBaseTable.SaveMod();
    m_ItemEquipTable.SaveMod();
    return true ;
}
