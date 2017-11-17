#include "Item.h"
#include "ItemManager.h"
#include "LogicItemProto.h"

#include "def/MmoAssert.h"

//#include "Enum.pb.h"
#include "Item.pb.h"



static const int SlotInvalid = -1;


Item::Item( int itemId, int count )
: m_count ( count)
, m_isNew ( true)
, m_isSold (false)
, m_isBoughtInMarket(false)
, m_repo_end_time(0)
{
    m_proto = sItemMgr.Find(itemId);
    SetSlot( static_cast<uint32>( SlotInvalid ) ) ;
}

Item::Item( const LogicItemProto* proto, int count )
: m_proto ( proto)
, m_count ( count)
, m_isNew ( true)
, m_isSold (false)
, m_isBoughtInMarket(false)
, m_repo_end_time(0)
{
	SetSlot( static_cast<uint32>( SlotInvalid ) ) ;
}

int Item::StackCount() const
{
    return m_count;
}

bool Item::CanStack() const
{
	 return m_proto->CanStack();
}

uint32 Item::Entry() const
{
    return m_proto->Id();
}

int Item::DoStack( int count )
{
	MMO_ASSERT( Proto()->CanStack());
    m_count += count;
    return count;
}

void Item::TakeOut( int count )
{
    MMO_ASSERT(count >0);
    MMO_ASSERT(count < m_count);

    m_count -= count;
}

void Item::SetOld()
{
    m_isNew = false;
}

bool Item::IsNew() const
{
    return m_isNew;
}

bool Item::IsSold() const
{
	return m_isSold;
}

time_t Item::RepoEndTime() const
{
	return m_repo_end_time;
}

bool Item::IsJewel() const
{
	if (m_proto)
	{
		return m_proto->IsJewel();
	} 
	 return false;
}

bool Item::IsGem() const
{
	if (m_proto)
	{
		return m_proto->IsGem();
	} 
	return false;
}

bool Item::IsEquip() const
{
  if (m_proto)
  {
	  return m_proto->IsEquip();
  }
  return false;
}

uint32 Item::SellPrice() const
{
	if (m_proto)
	{
		return m_proto->sell_price;
	}
	return 0;
}

bool Item::IsBagItemType() const
{
	if (m_proto)
	{
		return m_proto->CanBeCreateToBag();
	}
	return false;
}

uint32 Item::BagSlot() const
{
    return m_slot ;
}

void Item::SetSlot( uint32 slot )
{
    m_slot = slot;
}

void Item::SetIsBoughtInMarket(bool judge)
{
	m_isBoughtInMarket = judge;
}

bool Item::IsBoughtInMarket()
{
	return m_isBoughtInMarket;
}

void Item::SetSold(bool isSold)
{
	m_isSold = isSold;
}

time_t Item::GetRepoEndTime() const
{
  return m_repo_end_time;
}

void Item::SetRepoEndTime(time_t curT)
{
	m_repo_end_time = curT;
}

bool Item::CanBeSell() const
{
   if(m_proto)
   {
	   return m_proto->is_sell > 0;
   }
   return false;
}

void Item::SaveTo( pb::GS2C_SlotItem& msg ) const
{

    pb::GxDB_ItemInfo* itemInfo = msg.mutable_item();
    if ( itemInfo)
    {
        itemInfo->set_count( StackCount());
        itemInfo->set_is_new( IsNew());
        itemInfo->set_proto_id( Entry());
		itemInfo->set_is_sold(IsSold());
		itemInfo->set_repo_end_time(RepoEndTime());
    }
}

void Item::LoadFrom( const pb::GS2C_SlotItem& msg )
{
    const pb::GxDB_ItemInfo& itemInfo = msg.item();
    m_count = itemInfo.count();
    m_isNew = itemInfo.is_new();
    m_slot = msg.slot();
	m_isSold = itemInfo.is_sold();
	m_repo_end_time = itemInfo.repo_end_time();
}


