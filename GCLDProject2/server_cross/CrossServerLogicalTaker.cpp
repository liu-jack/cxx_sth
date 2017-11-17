

#include "CrossServerLogicalTaker.h"

#include "Logger.h"
#include "OS.h"
#include "session/GameServerSession.h"

#include "def/MmoAssert.h"
#include "utility/STLUtility.h"

#include "Opcode.pb.h"

#include "AutomaticUpdate/AutoUpdateCross.h"
#include "Service/ServiceMgr.h"
#include "Npc/Npc.h"
#include "character/Character.h"
#include "session/PlayerPool.h"

CrossServerLogicalTaker::CrossServerLogicalTaker( void )
{
}


CrossServerLogicalTaker::~CrossServerLogicalTaker( void )
{
}


void CrossServerLogicalTaker::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("[1002] CrossServerLogicalTaker BeforeSeesionUpdate");
	//sDbProxyClient.Update( nowTimeMS,diffMS ) ;
	//sSysEventMgr.Update( nowTimeMS,diffMS ) ;
	sAutoUpdateCross.Update(nowTimeMS,diffMS);
	sPlayerPool.Update(nowTimeMS,diffMS);
	ServiceMgr::RunAllService((uint32)diffMS, (uint32)nowTimeMS);
	Npc::_DeleteNpc();
	Character::_DeleteShadow();
	//sActMgr._Update();
	//sPlayerPool.UpdateOffLinePlayer(nowTimeMS,diffMS);
	//sLobbyClient.Update(nowTimeMS,diffMS);
}

void CrossServerLogicalTaker::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}

