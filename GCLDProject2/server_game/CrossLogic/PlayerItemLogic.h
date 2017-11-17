#pragma once

#include "def/TypeDef.h"


class Player;
class LootList;

class LogicItemProto;

class ItemArray;
class Item;
class ItemEquip;
struct ItemSuitComplex;
namespace pb
{
    class GS2C_LootList;
    class GS2C_ItemUse;
    class C2GS_ItemUp;
    class C2GS_UseItemsToTarget;
}


class PlayerItemLogic
{
public:
    typedef uint32 ErrCode ;
public:
    //
    static ErrCode UseItem( Player* player, int32 slot, pb::GS2C_ItemUse& msg, int32 count = 1); 

    static void SendBagModify( Player* player);
	static void UseItem(Player* player,IntPairVec& vec);
    //
    static bool PlayerLootImmBox( Player* player, const LogicItemProto& proto);


	static ErrCode ComposeGem(Player* player, uint32 itemSlotId);
	static ErrCode DecomposeGem(Player* player, uint32 itemSlotId);

    static bool TryGetItemEquip( ItemArray* bag, uint32 itemId, ItemEquip*& outItemEquip);

	
	static ErrCode SuitComplex(Player* player, uint32 suitProtoId);

	static ErrCode BuyBagCapacity(Player* player);

private:
    static ErrCode UseItem( Player* player, const LogicItemProto& proto, pb::GS2C_ItemUse& msg, uint32 count);
    static bool FillLootList( Player* player, const LogicItemProto& proto, LootList& lootList);


	static ErrCode SuitComplexPri( Player* player, const ItemSuitComplex* suitComp, std::vector<int>& vecSolt );
	static ErrCode SuitComplexMid( Player* player, const ItemSuitComplex* suitComp );

    
};