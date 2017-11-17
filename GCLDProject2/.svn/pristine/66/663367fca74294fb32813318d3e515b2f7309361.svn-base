/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "PlayerContinueOccupyCityDB.h"
#include "Fuben.pb.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "../object/Player.h"
#include "../ContinueOccupyCity/ContinueOccupyCityLog.h"


void ContinueOccupyCityDB::SendInfoToDb(Player& player, ContinueOccupyCityLog& log)
{
#ifdef _MMO_SERVER_
	pb::BaseContinueMsg msg;
	log.SaveTo(msg);
	sDbProxyClient.SendPlayerPack(player, pb::SG2D_CONTINUE_OCCUPY_CITY_UPDATE, msg);
#endif
}

