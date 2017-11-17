#pragma once

#include "def/TypeDef.h"
#include "../Dungeon/PlayerDungeonLog.h"

class Player;
class PlayerDungeonLogDB {
public:
    static void SendBaseInfoToDb(Player& player, PlayerDungeonLog& log);
	static void SendAddHeroToDb(Player& player, PlayerDungeonLog::CatchHero& hero);
	static void SendDelHeroToDb(Player& player, PlayerDungeonLog::CatchHero& hero);
};
