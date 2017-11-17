#ifndef __NpcMgr_h__
#define __NpcMgr_h__
#include "def/TypeDef.h"

class Npc;
class City;
class NpcMgr {
public:
    NpcMgr();
    static NpcMgr& Instance(){ static NpcMgr T; return T; }

    Npc* CreateNpc(uint32 tableId, uint32 level, uint32 countryId = 0, City* pCity = NULL);
};
#define sNpcMgr (NpcMgr::Instance())

#endif