#pragma once

#include "def/TypeDef.h"

class Player;
class PlayerAltarLog;

class PlayerAltarLogDB {
public:
    static void SendAllInfoToDb(Player& player, PlayerAltarLog& log);
    static void SendSubAltarToDb(Player& player, PlayerAltarLog& log);
    static void SendLevelDataToDb(Player& player, PlayerAltarLog& log);
};