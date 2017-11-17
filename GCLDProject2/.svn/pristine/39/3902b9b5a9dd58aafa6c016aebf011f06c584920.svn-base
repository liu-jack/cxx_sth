#include "ReinforceMgr.h"
#include "Reinforce.h"
#include "ReinforceTableMgr.h"
#include "Map/city/city.h"
#include "seige_force/SeigeForceLog.h"
#include "object/Player.h"

ReinforceMgr::ReinforceMgr()
{

}

Reinforce* ReinforceMgr::CreateReinforceMgr(Player* player,const uint32 armyId)
{
	if(!player) return NULL;
 	const CharSiegeArmy* proto = sSeigeArmyForceMgr.getCharSiegeArmy(armyId);
	if (proto == NULL) return NULL;

	LogOfSeigeArmy* log_of_seige_army = player->m_SeigeForceLog->GetLogOfSeigeArmyStruct(armyId);
	if(!log_of_seige_army) return NULL;
	Reinforce* pReinforce = new Reinforce(*proto,player,log_of_seige_army);
	pReinforce->InitCombatData();
	pReinforce->RecoverFullSoldier();
	return pReinforce;

}