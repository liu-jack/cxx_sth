#include "LootPackage.h"
#include "LootList.h"
#include "datastruct/struct_loot.h"
#include "../object/Player.h"
#include "def/MmoAssert.h"
#include "Logger.h"
#include "Opcode.pb.h"
#include "Enum.pb.h"

LootPackage::LootPackage()
{
}

LootPackage::~LootPackage()
{
}

void LootPackage::AssignLootList( LootList* lootList, Player* player )
{
	m_lootList.reset(lootList);
}

//void LootPackage::StartLoot( Player* player, ObjGUID giverGuid )
//{
//	MMO_ASSERT(player);
//	MMO_ASSERT(HasLootFor(player));
//	MMO_ASSERT(!IsLockedBy(player));
//
//	m_player = player;
//
//	pb::GS2C_LootList msg;
//	m_lootList->SaveTo(msg);
//	msg.set_guid(giverGuid);
//	
//	player->Send(pb::SMSG_PRIZES, msg);
//}

//void LootPackage::PickMoneyBy( Player* player )
//{
//	//MMO_ASSERT(player);
//	//MMO_ASSERT(IsLockedBy(player));
//	//MMO_ASSERT(HasLootFor(player));
//
//	//uint32 money = m_lootList->PickMoney();
//	//if (money)
//	//{
//	//	player->AddMoney(money);
//	//	CheckEmptyState( player );
//	//}
//}

void LootPackage::PickItemBy( Player* player, uint32 itemId )
{
	MMO_ASSERT(player);
	MMO_ASSERT(IsLockedBy(player));
	MMO_ASSERT(HasLootFor(player));

	uint32 count = m_lootList->CountItem(itemId);
	if (count == 0)
	{
		return;
	}

    if ( player->CreateItem( pb::IR_OPEN_BOX_GET, itemId, count) != pb::ErrCommonSuccess) 
	{	
		return;
	}

	m_lootList->PickItem(itemId);
	CheckEmptyState( player );
}

void LootPackage::QuitLoot( Player* player )
{
	MMO_ASSERT(player);
	MMO_ASSERT(IsLockedBy(player));

	m_player = NULL;
}

void LootPackage::CheckEmptyState( Player* player )
{
	MMO_ASSERT(player);
	MMO_ASSERT(IsLockedBy(player));
	MMO_ASSERT(m_lootList.get()); 

	if (!m_lootList->IsEmpty())
	{
		return;
	}

	QuitLoot(player);
	m_lootList.reset();
}

bool LootPackage::HasLoot() const
{
	return m_lootList.get() && (!m_lootList->IsEmpty());
}

bool LootPackage::IsLockedBy(const Player* player ) const
{
	if (!m_player.IsValid())
	{
		return false;
	}

	return m_player.Get() == player;
}

bool LootPackage::HasLootFor( const Player* player ) const
{
	return HasLoot();
}
