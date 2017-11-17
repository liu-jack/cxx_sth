#pragma once

#include "def/TypeDef.h"

class Player;
class PlayerBaoQiLog;

class PlayerBaoQiLogDB {
public:
    static void SendAllInfoToDb(Player& player, PlayerBaoQiLog& log);
	static void SendAllInfoToDb(Player& player);
};