#pragma once

class Player;
class LootList;

struct ILootPackage
{
	virtual ~ILootPackage(){}

	virtual bool IsLockedBy(const Player* player) const { return false; }
	virtual bool HasLoot() const { return false; }
	virtual bool HasLootFor(const Player* player) const { return false; }

	virtual void AssignLootList(LootList* lootList, Player* player) {}
	//virtual void StartLoot(Player* player, ObjGUID giverGuid) {}
	//virtual void PickMoneyBy(Player* player) {}
	virtual void PickItemBy(Player* player, uint32 itemId) {}
	virtual void QuitLoot(Player* player) {}
};