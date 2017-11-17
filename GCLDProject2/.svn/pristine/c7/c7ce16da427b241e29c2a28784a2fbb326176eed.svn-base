#pragma once
#include "def/TypeDef.h"

class Reinforce;
class City;
class Player;
class ReinforceMgr{
public:
	ReinforceMgr();
	static ReinforceMgr& Instance(){ static ReinforceMgr T; return T; }

	Reinforce* CreateReinforceMgr(Player* player,const uint32 army_id);
};

#define sReinforceMgr (ReinforceMgr::Instance())
