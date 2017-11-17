#pragma once
#include "def/TypeDef.h"

class Player;
struct TechnologyStudyStruct;
class PlayerTechnology;
class PlayerTechnologyDB {
public:
    //static void SendAllInfoToDb(Player& player, PlayerTechnology& log);
    static void UpdateStudyInfoToDb(Player& player, uint32 id, const ::TechnologyStudyStruct& data);
};