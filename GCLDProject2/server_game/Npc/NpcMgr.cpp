#include "NpcMgr.h"
#include "Npc.h"
#include "Table/NpcTableMgr.h"
#include "def/MmoAssert.h"
#include "../Map/city/city.h"

NpcMgr::NpcMgr()
{

}
Npc* NpcMgr::CreateNpc(uint32 tableId, uint32 level, uint32 countryId /* = 0 */, City* pCity /* = NULL */)
{
    const NpcProto* proto = sNpcTableMgr.GetProto(tableId);
    MMO_ASSERT(proto);
    if (proto == NULL) return NULL;

    Npc* pNpc = new Npc(*proto);
	if(countryId > 0)
		pNpc->SetCountryId(countryId);
	else
		pNpc->SetCountryId(proto->faction_option);
    pNpc->SetCurCity(pCity ? pCity->id : 0);
    pNpc->SetMap(pCity ? &pCity->m_map : NULL);
    pNpc->SetLevel(level);
	pNpc->InitCombatData();
	pNpc->RecoverFullSoldier();
	pNpc->SetObjType(Combat::Obj_Npc);
    if (pCity) pCity->NpcEnter(*pNpc);

    return pNpc;
}