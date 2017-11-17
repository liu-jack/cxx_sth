/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "def/TypeDef.h"
class Player;
class SeigeForceLog;
struct LogOfSeigeArmy;
class PlayerSeigeForceArmyDB
{
public:
	static void SendInfoToDB(Player& player,const uint32 armyId,SeigeForceLog* log,const LogOfSeigeArmy& ref);
};