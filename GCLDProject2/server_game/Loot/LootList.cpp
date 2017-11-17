#include "LootList.h"

#include "item/Item.h"
#include "item/ItemManager.h"
#include "item/LogicItemProto.h"

#include "datastruct/struct_item.h"
#include "def/MmoAssert.h"
#include "Logger.h"
#include "Enum.pb.h"

LootList::LootList()
: m_items()
, m_moneys()
, m_playerExp(0)
, m_charExp(0)
{
	//NLOG( "LootList()" ) ;
}

LootList::~LootList()
{
}

int LootList::Size() const
{
	return m_items.size();
}

bool LootList::HasItem( uint32 itemId ) const
{
	return m_items.find(itemId) != m_items.end();
}

uint32 LootList::CountItem( uint32 itemId ) const
{
	ItemMap::const_iterator it = m_items.find(itemId);
	if (it == m_items.end())
	{
		return 0;
	}

	return it->second;
}

void LootList::Clear()
{
	m_items.clear();
	m_moneys.clear();
	m_playerExp = 0;
	m_charExp = 0;
}

MoneyMap& LootList::MutableMoneys(){
	return m_moneys;
}

ItemMap& LootList::MutableItem()
{
	return m_items;
}

const MoneyMap& LootList::Moneys() const
{
	return m_moneys;
}
const ItemMap& LootList::Items() const
{
	return m_items;
}

void LootList::AddMoney( uint32 moneyType, uint32 money )
{
    if ( money != 0)
    {
        m_moneys[moneyType] += money;
    }
}

void LootList::AddMoney( const CurrType2Count& count )
{
    for ( CurrType2Count::const_iterator it = count.begin(); it != count.end(); ++it)
    {
        AddMoney( it->first, it->second);
    }
}

void LootList::AddItem(  uint32 itemId, uint32 count )
{
    if ( count != 0)
    {
        m_items[itemId] += count;
    }
}

void LootList::AddItem( const ItemID2Count& count )
{
    for ( CurrType2Count::const_iterator it = count.begin(); it != count.end(); ++it)
    {
        AddItem( it->first, it->second);
    }
}

void LootList::SetItem( uint32 itemId, uint32 count )
{
	NLOG("LootList::SetItem id=%d,count=%d",itemId,count);
	m_items[itemId] = count;
}

void LootList::AddPlayerExp(uint32 exp)
{
	m_playerExp += exp;
}
void LootList::AddCharExp(uint32 exp)
{
	m_charExp += exp;
}

uint32 LootList::GetPlayerExp()
{
	return m_playerExp;
}

void LootList::SetPlayerExp(const uint32 player_exp)
{
	m_playerExp = player_exp;
}

uint32 LootList::GetCharExp()
{
	return m_charExp;
}
bool LootList::IsEmpty() const
{
	return m_moneys.empty() && m_items.empty() && m_playerExp == 0 && m_charExp == 0;
}

//uint32 LootList::PickMoney(uint32 type)
//{
//	uint32 money = m_money;
//	m_money = 0;
//
//	return money;
//}

void LootList::PickItem( uint32 itemId )
{
	MMO_ASSERT(HasItem(itemId));

	m_items.erase(itemId);
}

void LootList::SaveTo( pb::GS2C_LootList& msg ) const
{
	for (ItemMap::const_iterator it = m_items.begin(); it != m_items.end(); ++it)
	{
		pb::PairValue* itemMsg = msg.add_item_id_count();
		itemMsg->set_first(it->first);
		itemMsg->set_second(it->second);
	}
	for (ItemMap::const_iterator it = m_moneys.begin(); it != m_moneys.end(); ++it)
	{
		pb::PairValue* itemMsg = msg.add_curency_type_count();
		itemMsg->set_first(it->first);
		itemMsg->set_second(it->second);
	}
	msg.set_player_exp(m_playerExp);
	msg.set_char_exp(m_charExp);
}

void LootList::SaveTo( pb::GS2C_ItemLootList& msg ) const
{
	for (ItemMap::const_iterator it = m_items.begin(); it != m_items.end(); ++it)
	{
		pb::PairValue* itemMsg = msg.add_item_id_value();
		itemMsg->set_first(it->first);
		itemMsg->set_second(it->second);
	}
}

void LootList::SaveTo( pb::GS2C_Bag& msg ) const
{
	for (ItemMap::const_iterator it = m_items.begin(); it != m_items.end(); ++it)
	{
		pb::GS2C_SlotItem* itemMsg = msg.add_items();
		pb::GxDB_ItemInfo* soltItem = itemMsg->mutable_item();
		if (soltItem)
		{
			soltItem->set_proto_id(it->first);
		}
	}
}

void LootList::SaveTo( pb::GxDB_EquipInfo& msg ) const
{
	for (ItemMap::const_iterator it = m_items.begin(); it != m_items.end(); ++it)
	{
		pb::PairValue* itemMsg = msg.add_skill_attrs();
		itemMsg->set_first(it->first);
		itemMsg->set_second(it->second);
	}
}

void LootList::LoadFrom( const pb::GS2C_LootList& msg )
{
	m_items.clear();
	for (int i=0; i<msg.item_id_count_size(); ++i)
	{
		const pb::PairValue& msgItem = msg.item_id_count(i);
		m_items[msgItem.first()] += msgItem.second();
	}

	m_moneys.clear();
	for (int i=0; i<msg.curency_type_count_size(); ++i)
	{
		const pb::PairValue& msgItem = msg.curency_type_count(i);
		m_moneys[msgItem.first()] += msgItem.second();
	}

	m_playerExp = msg.player_exp();
	m_charExp = msg.char_exp();
}

//bool LootList::IsFull() const
//{
//	if (m_maxSize <= 0)
//	{
//		return false;
//	}
//
//	return Size() >= m_maxSize;
//}

void LootList::Append( const LootList& other )
{
	for (ItemMap::const_iterator it = other.m_items.begin(); it != other.m_items.end(); ++it)
	{
		AddItem(it->first, it->second);
	}

	for (MoneyMap::const_iterator it = other.m_moneys.begin(); it != other.m_moneys.end(); ++it)
	{
		AddMoney(it->first, it->second);
	}

	m_playerExp += other.m_playerExp;
	m_charExp += other.m_charExp;
}


bool LootList::PickOneItem( int& itemId, int& count )
{
	if (m_items.empty())
	{
		itemId = 0;
		count =0;

		return false;
	}

	itemId = m_items.begin()->first;
	count = m_items.begin()->second;

	m_items.erase(m_items.begin());

	return true;
}



