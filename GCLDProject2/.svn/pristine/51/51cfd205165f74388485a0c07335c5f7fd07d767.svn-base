#pragma once

#include "def/TypeDef.h"

class Player;
class PlayerMapLogic;
class PersonalEventChain;
struct CityEventInfo;

class PlayerMapLogicDB {
public:
    static void SendBaseToDb(Player& player, PlayerMapLogic& log);
    static void UpdateCityEventToDb(Player& player, uint32 eventId, const CityEventInfo& info);
	static void UpdateFogRec(Player& player, PlayerMapLogic& log);

	static void UpdatePersonalECToDb(Player& player, PersonalEventChain& personalEC);
};