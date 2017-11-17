#pragma once

#include <memory>
#include "def/TypeDef.h"
#include "ptr_container/ObjectPtr.h"
#include "ILootPackage.h"

class LootList;
class Player;

class LootPackage : public ILootPackage
{
public:
	LootPackage();
	~LootPackage();

	// ILootPackage
	virtual bool IsLockedBy(const Player* player) const;
	virtual bool HasLoot() const;
	virtual bool HasLootFor(const Player* player) const;

	virtual void AssignLootList(LootList* lootList, Player* player);
	//virtual void StartLoot(Player* player, ObjGUID giverGuid);
	//virtual void PickMoneyBy(Player* player);
	virtual void PickItemBy(Player* player, uint32 itemId);
	virtual void QuitLoot(Player* player);

private:
	void CheckEmptyState(Player* player);

private:
	std::auto_ptr<LootList>		m_lootList;
	ObjectPtr<Player>			m_player;
};