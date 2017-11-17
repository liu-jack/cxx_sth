#ifndef PlayerOperationLogic_h__
#define PlayerOperationLogic_h__

#include "def/TypeDef.h"

class Player;
class IAPProgramRecord;

namespace pb{
    class GXDB_PayOrder;
}


class PlayerOperationLogic
{

public:
	static uint32 HandlePlayerStartAutoIncreaseSolders( Player& player);
	static void UpdateBuildingInfo(Player &player, uint32 buildId);
	static void UpdateBuildingInfoToDB(Player &player, uint32 buildId);
	static void UpdateBuildingInfoToClient(Player &player, uint32 buildId);
	static void UpdateModuleInfo(Player &player, uint32 moduleId);
	static void UpdateModuleInfoToDB(Player &player, uint32 moduleId);
	static void UpdateModuleInfoToClient(Player &player, uint32 moduleId);
	static void AddIncomeTimer(Player *player);


};


#endif // PlayerOperationLogic_h__
