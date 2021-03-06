#include <boost/typeof/typeof.hpp>

#include "ItemArray.h"

#include "def/MmoAssert.h"
#include "Logger.h"

#include "ItemManager.h"
#include "ItemEquip.h"
#include "LogicItemProto.h"
#include "OS.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "InterServer.pb.h"  //
#include "utility/Utility.h"
//////////////////////////////////////////////////////////////////////////

const static uint32 QUALITY_SUIT = 6;


ItemArray::ItemArray( int capacity)
: m_capacity( capacity)
, m_maxSolts(capacity)
, m_needCheckExpire(false)
{

}


ItemArray::~ItemArray()
{
    for( MultiKeyItemTableItr itr =m_itemTable.begin(); itr != m_itemTable.end(); ++itr )
    {
		if(*itr)
			delete *itr ;
    }

    m_itemTable.clear() ;
}


const Item* ItemArray::GetItem( int slot ) const
{
    return MutableItem( slot);
}

bool ItemArray::IsEquipForBagSolt( int slot ) const
{
	const Item* soltItem = GetItem(slot);
	if (soltItem == NULL)
	{
		return false;
	}
	return soltItem->IsEquip();
}

ItemEquip* ItemArray::MutableItemEquip( int slot ) const
{
	MMO_ASSERT (slot >=0 && slot < m_capacity);

	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr itr = bySlot.find( slot ) ;
	if( itr != bySlot.end() )
	{
		Item * item = *itr;
		ItemEquip* item_equip = dynamic_cast<ItemEquip*>( item);
		return item_equip;
	}
	return NULL;
}

ItemEquip* ItemArray::MutableItemEquipByProId( int entry ) const
{
	ItemEquip* item_equip = NULL;
	EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
	for( TableByProtoIdItr itr =pItr.first; itr != pItr.second; ++itr )
	{
		Item *pItem = *itr;
		item_equip = dynamic_cast<ItemEquip*>( pItem);
		if (item_equip)
		{
			if(item_equip->CanBeDestroyed())
				break;
		}
	}
	return item_equip;
}

bool ItemArray::HasSameMaxLvAttrEquip( int entry, int attrid, int& out_item_solt) const
{
	EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
	for( TableByProtoIdItr itr =pItr.first; itr != pItr.second; ++itr )
	{
		Item *pItem = *itr;
		ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
		if (item_equip)
		{
			out_item_solt = item_equip->BagSlot();
		    if(item_equip->IsSameMaxLevelAttr(attrid))
			{
				return true;
			}
		}
	}
	return false;
}

Item* ItemArray::MutableItem( int slot ) const
{
	MMO_ASSERT (slot >=0 && slot < m_capacity);

    const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
    TableByItemSlotItr itr = bySlot.find( slot ) ;
    if( itr != bySlot.end() )
    {
        return *itr ;
    }
    return NULL;
}

Item* ItemArray::MutableOneItemByProId( int entry, bool isSold ) const
{
	EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
	for( TableByProtoIdItr itr =pItr.first; itr != pItr.second; ++itr )
	{
		Item *pItem = *itr;
		if (pItem->IsSold() == isSold)
		{
			 return *itr;
		}
	}
	return NULL;
}

int ItemArray::GetCountItem( int entry ) const
{
	return CountItem(entry);
}

bool ItemArray::CreateItem( int entry, uint32 bagCapcity,int count /*= 1*/, ItemPtrList* outList /*= NULL*/,ItemPtrList* sell_list /*=NULL*/)
{
    MMO_ASSERT(entry > 0);
    MMO_ASSERT(count > 0);
    MMO_ASSERT(m_capacity > 0);

    const LogicItemProto* proto = sItemMgr.Find(entry);
    if (proto == NULL)
    {
        ELOG ("No item with entry %d", entry);
        return false;
    }

    return CreateItem( proto, count,bagCapcity, outList,sell_list);
}

uint32 ItemArray::CurBagItemCount()
{
	uint32 nBagitem = 0;
	TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item *pItem = *itr;
		if (pItem->IsBagItemType() && !pItem->IsSold() && !pItem->IsGem())
		{
			if (pItem->IsEquip())
			{
				ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
				if (item_equip)
				{
					if (item_equip->IsEquiped())
					{
						continue;
					}
				}
			}
			nBagitem++;
		}
	}
	return nBagitem;
}
//获得相关品质且星星数的数量
uint32 ItemArray::GetQualityAndStarCount(uint32 quality,uint32 star){
	uint32 nBagitem = 0;
	TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item *pItem = *itr;
		if (pItem->IsEquip())
		{
			if(pItem->Proto()->Quality() >=quality)
			{
				ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
				int starcount = item_equip->GetAttrCount();
				if(starcount >=(int)star)
					nBagitem++;
			}
		}
	}
	return nBagitem;
}

//拥有相应的装备
uint32 ItemArray::HaveEquipClass(uint type){//type 40506  三种类型  4,5,6
	std::vector<int> vec;
	Utility::GetValue(type,vec,2);
	uint32 n_count = 0;
	TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item* pItem = *itr;
		for(std::vector<int>::iterator Iter = vec.begin();Iter != vec.end();++Iter)
		{
			if(pItem->Proto()->Class2() == *Iter) n_count++;
		}
	}
	return n_count >= vec.size() ? 1 : 0;
}

uint32 ItemArray::CountEquipClass(uint type){//type 40506  三种类型  4,5,6
	std::vector<int> vec;
	Utility::GetValue(type,vec,2);
	uint32 n_count = 0;
	TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item* pItem = *itr;
		if( pItem->IsEquip() )
		{
			for(std::vector<int>::iterator Iter = vec.begin();Iter != vec.end();++Iter)
			{
				if(pItem->Proto()->Class2() == *Iter) n_count++;
			}
		}
	}
	return n_count >= vec.size() ? 1 : 0;
}
bool ItemArray::CreateItem( const LogicItemProto* proto ,uint32 bagCapcity,int count /*= 1*/, ItemPtrList* outList /*= NULL*/,ItemPtrList* sell_list /*=NULL*/)
{
    if ( !proto->CanBeCreateToBag())
    {
        MMO_ASSERT( false && "Can't add to Bag, Please Use Function In Player : CreateItem");
        return false;
    }

    if ( proto->CanStack())
    {
        Item* item = MutableOneItemByProId(proto->id,false) ; 
		if (item)
		{
			item->DoStack( count);
			SetModifiedID(item->BagSlot());
			if ( outList)
			{
				outList->push_back( item);
			}
			return true;
		}
    }
    return CreateItemOnFreeSlots(proto, count,bagCapcity, outList,sell_list);
}

bool ItemArray::DestroyItem( int entry, int count )
{
    MMO_ASSERT(entry > 0);
    MMO_ASSERT(count > 0);
    MMO_ASSERT(m_capacity > 0);

    if ( CountItem(entry , PredCanDestroy() ) < count )
    {
        return false;
    }

    int32 remain = count;

    EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
    for(TableByProtoIdItr itr = pItr.first ; itr != pItr.second && remain >0; )
    {
        Item* item = *itr ;

        MMO_ASSERT (item->Entry() == static_cast<uint32>( entry ))  ;   
        SetModifiedID( item->BagSlot());

        if (remain >= item->StackCount())
        {
            remain -= item->StackCount();     
            TableByProtoIdItr tmpItr =itr++ ;
            EraseItem( tmpItr ) ;
        }
        else
        {
            ++itr;
            item->TakeOut( remain);
            remain = 0 ;
        }
    }

    MMO_ASSERT(remain == 0);   
    return true;
}

bool ItemArray::DestroyItem( const ItemID2Count& itemid2count )
{
    if ( EnoughItem(itemid2count))
    {
        for ( ItemID2Count::const_iterator it = itemid2count.begin(); it != itemid2count.end(); ++ it)
        {
            DestroyItem(it->first, it->second);
        }
        return true;
    }
    return false;
}

void ItemArray::DestroyAll()
{
	TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
    for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); )
    {
		TableByItemSlotItr tmpItr =itr++ ;
		EraseItem(tmpItr);
    }
}

bool ItemArray::RemoveExpireItem(std::vector<int>& itemIdVec)
{
	bool remainHas = false;
	time_t curTime = sOS.GetRealTime();
	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr it =bySlot.begin() ;
	for ( ; it != bySlot.end();)
	{
		Item *pItem = *it;
		if (pItem->IsSold() && pItem->CanBeSell())
		{
			if (pItem->GetRepoEndTime() < curTime)
			{
				itemIdVec.push_back(pItem->ProtoId());
				SetModifiedID( pItem->BagSlot());
				TableByItemSlotItr tmpItr = it++ ;
				EraseItem( tmpItr ) ;
			}
			else
			{
				remainHas = true;
				++it;
			}
		}
		else
		{
			++it;
		}
	}
	m_needCheckExpire = remainHas;

	return (int)itemIdVec.size() > 0;
}

bool ItemArray::CheckExpireItem(std::vector<int>& itemIdVec)
{
	if(m_needCheckExpire)
	{
	  return RemoveExpireItem(itemIdVec);
	}
	return false;
}

bool ItemArray::TakeOutSlotItem( int slot, int count , int& out_item_proto_id)
{
    const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
    TableByItemSlotItr it = bySlot.find(slot);
    if (it == bySlot.end())
    {
        return false;
    }

    Item* item = *it;
	out_item_proto_id = item->Entry();
    if ( item->StackCount() > count)
    {
        item->TakeOut( count);
    }else if ( item->StackCount() == count)
    {
        EraseItem(it);
    }else
    {
        return false;
    }

    SetModifiedID(slot);
    return true;
}

bool ItemArray::SellSlotItem( int slot, int& out_item_proto_id, int& out_count, bool forceSell/* = false*/)
{
	MMO_ASSERT (IsValidSlot(slot));   

	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr it = bySlot.find(slot);
	if (it == bySlot.end())
	{
		return false;
	}

	Item* item = *it;
	if ( !item->CanBeDestroyed() )
	{
		return false;
	} 
	if ( !item->CanBeSell() && !forceSell)
	{
		return false;
	}
	out_item_proto_id = item->Entry();
	out_count = item->StackCount();
	if (item->CanStack())
	{
		Item* curItem = MutableOneItemByProId(item->Entry(),true);
		if (curItem)
		{
			curItem->DoStack( out_count);
			curItem->SetRepoEndTime(sOS.GetRealTime() + GET_BASE_DEF_UINT(pb::BD_SOLDITEMBUYBACKEXPIREDTIME));
			EraseItem(it);
			SetModifiedID(slot);
			SetModifiedID(curItem->BagSlot());
			return true;
		}
	}
	item->SetSold(true);
	item->SetRepoEndTime(sOS.GetRealTime() + GET_BASE_DEF_UINT(pb::BD_SOLDITEMBUYBACKEXPIREDTIME));
	SetModifiedID(item->BagSlot());
	m_needCheckExpire = true;
	return true;
}


bool ItemArray::BuyBackSlotItem( int slot, int& out_item_proto_id, int& out_count )
{
	MMO_ASSERT (IsValidSlot(slot));   

	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr it = bySlot.find(slot);
	if (it == bySlot.end())
	{
		return false;
	}

	Item* item = *it;
	if ( !item->CanBeDestroyed() )
	{
		return false;
	} 
	if ( !item->IsSold())
	{
		return false;
	}
	out_item_proto_id = item->Entry();
	out_count = item->StackCount();
	if (item->CanStack())
	{
		Item* curItem = MutableOneItemByProId(item->Entry(),false);
		if (curItem)
		{
			curItem->DoStack( out_count);
			EraseItem(it);
			SetModifiedID(slot);
			SetModifiedID(curItem->BagSlot());
			return true;
		}
	}
	item->SetSold(false);
	item->SetRepoEndTime(sOS.GetRealTime());
	SetModifiedID(item->BagSlot());
	m_needCheckExpire = true;
	return true;
}

bool ItemArray::DestroySlotItem( int slot, int& out_item_proto_id, int& out_count)
{
    MMO_ASSERT (IsValidSlot(slot));   

    const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
    TableByItemSlotItr it = bySlot.find(slot);
    if (it == bySlot.end())
    {
        return false;
    }

    Item* item = *it;
    if ( !item->CanBeDestroyed() )
    {
        return false;
    } 
	out_item_proto_id = item->Entry();
	out_count = item->StackCount();
    SetModifiedID(item->BagSlot());
    EraseItem(it);
    return true;
}

bool ItemArray::CreateItemOnFreeSlots( const LogicItemProto* proto, int count ,uint32 bagCapcity, ItemPtrList* outList/* = NULL*/,ItemPtrList* sell_list /*= NULL*/)
{
    int time = 0, count_per_time = 0;
    if ( proto->CanStack())
    {
        time = 1;
        count_per_time = count;
    }
    else
    {
        time = count;
        count_per_time = 1;
    }
    bool success = true;
	uint32 cur_bag_item_count = CurBagItemCount();
    for ( int i = 0; i < time ; ++i,++cur_bag_item_count)
    {
        uint32 freeSlotId = GetOneFreeSlotId();
        Item* item = sItemMgr.CreateItemInst(proto, count_per_time);
        if ( item)
        {
            Add(freeSlotId, item, true);
            if ( outList)
            {
                outList->push_back( item);
            }
			if(cur_bag_item_count >= bagCapcity)
			{
				if(sell_list)
				{
					sell_list->push_back(item);
				}
			}
        }
        else
        {
            success = false;
        }
    }
    return success;
}

uint32 ItemArray::GetOneFreeSlotId() const
{
    uint32 min = MIN_SLOT;

    const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
    TableByItemSlotItr it =bySlot.begin() ;

    for ( ; it != bySlot.end(); ++it, ++min)
    {
        if ( (*it)->BagSlot() != min)
        {
            break;
        }
    }
    return min;
}

void ItemArray::EraseItem( TableByProtoIdItr &itr )
{
    Item *pItem = *itr ;
    delete pItem;
    m_itemTable.get<ByProtoId>().erase( itr ) ;
}

void ItemArray::EraseItem( TableByItemSlotItr &itr )
{
    Item *pItem = *itr ;
    delete pItem;
    m_itemTable.get<ByItemSlot>().erase( itr ) ;
}

void ItemArray::ItemChecked( int slot )
{
    if (!IsValidSlot(slot))
    {
        return;
    }
    Item* item = MutableItem( slot);
    if (item == NULL)
    {
        return;
    }
    item->SetOld();
}


bool ItemArray::IsValidSlot( int slot ) const
{
	return slot >=0 && slot < m_capacity;
}

bool ItemArray::SetMaxSolt( int nMax )
{
	if (nMax <= m_capacity)
	{
		m_maxSolts = nMax;
		return true;
	}
	return false;
}

int ItemArray::GetMaxSolt() const
{
	return m_maxSolts;
}

bool ItemArray::HasFreeSolt() const
{
	return ItemCount() < GetMaxSolt();
}

void ItemArray::WriteAllItemTo( pb::GS2C_Bag& bagMsg) const
{
	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr it =bySlot.begin() ;
	for (; it != bySlot.end(); ++it)
	{
		const Item* item = *it;
		if (item)
		{
			pb::GS2C_SlotItem* itemMsg =  bagMsg.add_items();
			itemMsg->set_slot(item->BagSlot());
			item->SaveTo(*itemMsg);
			itemMsg->set_is_deleted( false);
		}
	}
}

template<>
void ItemArray::SaveTo( pb::GS2C_Bag& bagMsg) const
{
    BOOST_AUTO( it , ModIdBegin());
    for ( ; it!= ModIdEnd(); ++it)
    {
        int slot = *it;
        pb::GS2C_SlotItem* itemMsg =  bagMsg.add_items();
        itemMsg->set_slot(slot);

        const Item* item = MutableItem(slot);
        if (item)
        {
            item->SaveTo(*itemMsg);
            itemMsg->set_is_deleted( false);
        }
        else
        {
            itemMsg->set_is_deleted( true);
        }
    }
}



template<>
void ItemArray::LoadFrom(const pb::GS2C_Bag& bagMsg)
{
    for ( int i = 0; i < bagMsg.items_size(); ++i)
    {
        const pb::GS2C_SlotItem& slotItemMsg = bagMsg.items(i);
        std::auto_ptr<Item> item ( sItemMgr.CreateItemInst( slotItemMsg));
        if (item.get())
        {
            Add( slotItemMsg.slot(), item.release(), false);
        }
    }
}

template<>
void ItemArray::WriteModifiedTo( pb::GS2C_Bag& msg) const
{
    BOOST_AUTO( it, ModIdBegin());
    BOOST_AUTO( itEnd, ModIdEnd());
    for ( ; it!= itEnd; ++it)
    {
        int slot = *it;

        pb::GS2C_SlotItem* itemMsg =  msg.add_items();
        itemMsg->set_slot(slot);

        const Item* item = MutableItem(slot);
        if (item)
        {
            item->SaveTo(*itemMsg);
            itemMsg->set_is_deleted( false);
        }
        else
        {
            itemMsg->set_is_deleted( true);
        }
    }
}

// 机器人 客户端用
template<>
void ItemArray::LoadModifiedFrom( const pb::GS2C_Bag& msg )
{
    for ( int i = 0; i < msg.items_size(); ++i)
    {
        const pb::GS2C_SlotItem& slotItemMsg = msg.items(i);
        if ( slotItemMsg.is_deleted())
        {
            int32 proto = 0, count = 0; 
            DestroySlotItem( slotItemMsg.slot(), proto, count);     
        }
        else
        {
            if ( Item* item = MutableItem( slotItemMsg.slot()))
            {
                item->LoadFrom( slotItemMsg);
            }
            else
            {
                std::auto_ptr<Item> new_item( sItemMgr.CreateItemInst( slotItemMsg));
                if (new_item.get())
                {
                    Add( slotItemMsg.slot(), new_item.release(), false);
                }
            }
        }
    }
}

void ItemArray::GetItems( int entry, ConstItemPtrList& itemList ) const
{
	MMO_ASSERT(entry > 0);
	MMO_ASSERT(m_capacity > 0);          
	EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
	for( TableByProtoIdItr itr =pItr.first; itr != pItr.second; ++itr )
	{
		const Item *pItem = *itr;
		MMO_ASSERT( pItem->Entry() == static_cast<uint32>( entry ) ) ;
		itemList.push_back(pItem);
	}
}

int ItemArray::CountItem( int entry ) const
{
    MMO_ASSERT(entry > 0);
    MMO_ASSERT(m_capacity > 0);

    int32 total = 0;            
    EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
    for( TableByProtoIdItr itr =pItr.first; itr != pItr.second; ++itr )
    {
        const Item *pItem = *itr;
        MMO_ASSERT( pItem->Entry() == static_cast<uint32>( entry ) ) ;
        total += pItem->StackCount();
    }
    return total;
}


int ItemArray::CountItemBySolt( int slot ) const
{
	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr it = bySlot.find(slot);
	if (it == bySlot.end())
	{
		return 0;
	}
	const Item* item = *it;
	return item->StackCount();
}

int ItemArray::CountGem() const
{
	int count  = 0;
	const TableByItemSlot &bySlot = m_itemTable.get<ByItemSlot>() ;
	TableByItemSlotItr it =bySlot.begin() ;
	for ( ; it != bySlot.end(); ++it)
	{
		if ( (*it)->IsGem())
		{
			count++;
		}
	}
	return count;
}

bool ItemArray::CanAddItemToBag( int entry, uint32 bagCapcity )
{
	const LogicItemProto* proto = sItemMgr.Find(entry);
	if (proto == NULL)
	{
		ELOG ("No item with entry %d", entry);
		return false;
	}
	if (!proto->CanBeCreateToBag())
	{
		return false;
	}
	uint32 curCount = CurBagItemCount();
	if (curCount < bagCapcity)
	{
		return true;
	}
	else
	{
		if (proto->IsGem())
		{
			if(CountGem() >= 16 )
				return false;
			else
				return true;
		}
		else if (proto->CanStack() && MutableOneItemByProId(entry,false))
		{
			return true;
		}
	}
	return false;
}

bool ItemArray::HasItem( int entry ) const
{
    MMO_ASSERT(m_capacity > 0);

    const TableByProtoId &byId =m_itemTable.get<ByProtoId>() ;
    return byId.find( entry ) != byId.end() ;
}


void ItemArray::Add( int slot, Item* item, bool isNewCreated )
{
    MMO_ASSERT (slot >=0 && slot < m_capacity);
    MMO_ASSERT (item != NULL);
    MMO_ASSERT (GetItem(slot) == NULL); 

    // 键值要先设定
    item->SetSlot( slot ) ; 

    bool bInsert = m_itemTable.insert( item ).second ;
    MMO_ASSERT( bInsert ) ;

    SetModifiedID(slot);
    //AfterItemAdded(slot, item, isNewCreated);
}

void ItemArray::AfterItemAdded( int slot, Item* item, bool isNewCreated )
{

}

EqualRangeByProtoId ItemArray::GetEqualRangeByProtoId( int entry ) const
{
    const TableByProtoId &byId =m_itemTable.get<ByProtoId>() ;
    return byId.equal_range( entry );
}

bool ItemArray::EnoughItem( int entry, int count ) const
{
    return CountItem(entry, PredCanDestroy()) >= count;
}

bool ItemArray::EnoughItem( const ItemID2Count& itemid2count ) const
{
    for ( ItemID2Count::const_iterator it = itemid2count.begin(); it != itemid2count.end(); ++it)
    {
        if ( !EnoughItem( it->first, it->second))
            return false;
    }
    return true;

}

int ItemArray::ItemCount() const
{
	int nCount = 0;
	const TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		nCount++;
	}
	return nCount;
}

void ItemArray::CollectFreeEquipAndSort( std::vector<ItemEquip*>& equip_group )
{
    const TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
    for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
    {
        Item * item = *itr;
        ItemEquip* item_equip = dynamic_cast<ItemEquip*>( item);
        if ( item_equip && !item_equip->IsEquiped())
        {
            equip_group.push_back( item_equip);
        }
    }
    
    std::sort(equip_group.begin(), equip_group.end(), CompareItemEquip);
}

uint32 ItemArray::GetQualityEquipOfSuitCount(const uint32 quality )
{
	uint32 vec_equip[QUALITY_SUIT] = {0};
	//size_t vec_size = sizeof(vec_equip) / sizeof(*vec_equip);
	const TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( CTableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item *pItem = *itr;
		if (pItem->IsEquip() && !pItem->IsSold())
		{
			if(pItem->Proto()->Quality() == quality)
			{
				if(pItem->Proto()->IsBook() || pItem->Proto()->IsFlag() ||
					pItem->Proto()->IsWeapon() || pItem->Proto()->IsHorse() || pItem->Proto()->IsChest() || pItem->Proto()->IsJade())
				{
					vec_equip[pItem->Proto()->Class2()]++;
				}
			}
		}
	}
	std::sort(vec_equip,vec_equip + QUALITY_SUIT);
	return vec_equip[0];
}

uint32 ItemArray::GetGreaterQualityEquipOfSuitCount( const uint32 quality )
{
	uint32 vec_equip[QUALITY_SUIT] = {0};
	//size_t vec_size = sizeof(vec_equip) / sizeof(*vec_equip);
	const TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( CTableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item *pItem = *itr;
		if (pItem->IsEquip() && !pItem->IsSold())
		{
			if(pItem->Proto()->Quality() >= quality)
			{
				if(pItem->Proto()->IsBook() || pItem->Proto()->IsFlag() ||
					pItem->Proto()->IsWeapon() || pItem->Proto()->IsHorse() || pItem->Proto()->IsChest() || pItem->Proto()->IsJade())
				{
					vec_equip[pItem->Proto()->Class2()]++;
				}
			}
		}
	}
	std::sort(vec_equip,vec_equip + QUALITY_SUIT);
	return vec_equip[0];
}


uint32 ItemArray::GetHasMijiEquipCount(const uint32 quality ) const
{
	uint32 count = 0;
	const TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( CTableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item *pItem = *itr;
		ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
		if(pItem->Quality() >= quality && item_equip &&  item_equip->IsHasMiji())
		{
			count++;
		}
	}
	return count;
}

uint32 ItemArray::GetQualityAndAnyStar( uint32 quality,uint32 star )
{
	uint32 nBagitem = 0;
	TableByItemSlot& bySlot = m_itemTable.get<ByItemSlot>() ;
	for ( TableByItemSlotItr itr = bySlot.begin(); itr != bySlot.end(); ++itr)
	{
		Item *pItem = *itr;
		if (pItem->IsEquip())
		{
			if(pItem->Proto()->Quality() ==quality)
			{
				ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
				int starcount = item_equip->GetAttrCount();
				if(starcount >=(int)star)
					nBagitem++;
			}
		}
	}
	return nBagitem;
}
