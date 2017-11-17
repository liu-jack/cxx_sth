#pragma once

class Player;
struct LootParam;
namespace pb
{
    class GS2C_LootList;
}
#include "Enum.pb.h"

class PlayerLootLogic
{
public:
    static bool PlayerLoot( 
        Player& player, pb::GS2C_LootList& lootListMsg,
        LootParam& lootparam, pb::ITEM_REASON);
private:

};