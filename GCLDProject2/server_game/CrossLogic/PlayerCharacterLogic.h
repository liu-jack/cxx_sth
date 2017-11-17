#pragma once

#include "def/TypeDef.h"

class Player;
class Character;
class LootList;  

namespace pb
{
    class C2GS_UseItemsToTarget;
    class C2GS_UpgradeQuality;
	class GS2C_PlayerStringGroup;
}

class PlayerCharacterLogic
{
public:
    typedef uint32 ErrCode ;
public:
    static void SendCharacterStorageModify( Player* player);

    static ErrCode UseUpgradeLvItemToCharacter(Player* player, pb::C2GS_UseItemsToTarget& msg);

    //static ErrCode UpgradeCharacterQuality(Player* player, pb::C2GS_UpgradeQuality& msg);

    static ErrCode InlayRuneToCharacter(Player* player, uint32 charId , uint32 slot);

    static ErrCode UpgradeCharacterReinforce(Player* player, uint32 charId );

    static ErrCode UpgradeCharacterSkill(Player* player, uint32 charId, uint32 skillIndex);

    static ErrCode ConfirmTrainingCharacter(Player* player, uint32 charId );

    static ErrCode RebirthCharacter( Player *player, uint32 charId, LootList& outLootList); 

    static ErrCode DecomposeCharacter( Player *player, uint32 charId, LootList& outLootList);

    static Character* CreateCharacterForPlayer( Player* player, uint32 protoId);

    static bool DestroyCharacterForPlayer( Player* player, uint32 charId);

	static ErrCode BuyPlayerHeadIcon( Player* player, uint32 iconId, pb::GS2C_PlayerStringGroup& msg);

	static void CheckPlayerNewFreeUnlockInfo( Player* player);
	static ErrCode SwitchPlayerHeadIcon( Player* player, uint32 iconId);
	static ErrCode OnKeyRecoverHp( Player* player , uint32 charId);

	static void InitCreatPlayerData(Player* player);
	
	static void updateCharacterInfoEx(Player* player,uint32 charId);
	static void GenBattleArrayProtoStr( Player& player, std::string& out_str);
	static int32 GenBattleArrayConbatPower( Player& player);

	static bool TryToRecoverHp(Player* player, Character* character, uint32 hpSpeed, bool isUseCoin);	//尝试最大可能的恢复兵力  返回是否满血

private:
    static ErrCode RebirthCharacter( Player *player, Character* character, LootList& outLootList);
};