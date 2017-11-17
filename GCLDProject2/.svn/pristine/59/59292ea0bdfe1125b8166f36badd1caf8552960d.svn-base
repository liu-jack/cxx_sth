#pragma once

#include "def/TypeDef.h"
#include "character/CharacterStorage.h"

class Player;
class ItemEquip;
class Character;

class ItemCharacterLogic
{
public:
    typedef uint32 ErrCode ;
public:
    static bool SetBattleArray( Player* player, const CharacterIds& battleArray);

    static void ReCalculateBattleAttr( Player* player,const uint32 siege_army_id = 0);

    static void AutomaticEquipToCharacters(Player* player, std::vector< uint32>& charIdsToEquip);

    static ErrCode ClearEquipFromCharacter(Player* player, int charId);

    // 处理网络请求用， 所以带了equipSlot
    static ErrCode EquipItemToCharacter(Player* player, int itemId, int charId, uint32 equipSlot);
    // 服务器内部逻辑用
    static ErrCode EquipItemToCharacter(Player* player, ItemEquip* equip, Character* character);

	static ErrCode _RemoveEquipFromCharacter(Character* character, ItemArray* bag, ItemEquip* equip,  uint32 equipSlot);

	static ErrCode QuikExchangeCharacterEquips(Player* player, uint32 charId ,uint32 toCharId);
	static ErrCode QuikChangeEquipsOwnerToNewCharacter(Player* player, uint32 charId ,uint32 toCharId);
private:
    static void AutomaticEquipToCharacter(Player* player, Character* character, std::list<ItemEquip*>& equip_group);

    static ErrCode _EquipItemToCharacter(Player* player, ItemEquip* equip, Character* character, uint32 equipSlot, bool replaceRaw = true);

    

};