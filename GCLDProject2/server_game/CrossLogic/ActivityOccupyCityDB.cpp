

/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "ActivityOccupyCityDB.h"
#include "object/Player.h"
#include "../server_client/DbProxyServerClient.h"
#include "InterServer.pb.h"
#include "ServerOpcode.pb.h"
#include "GameServer.h"
void ActivityOccupyCityDB::SendUpdateInfoToDb(const uint32 country_id,uint32 city_num)
{
#ifdef _MMO_SERVER_
	pb::Activity_Occupy_City_Num msg;
	msg.set_country_id(country_id);
	msg.set_city_num(city_num);
	msg.set_server_id(sGameSvr.GetServerId());
	msg.set_region_id(sGameSvr.GetRegionId());
	sDbProxyClient.Send(pb::SG2D_ACTIVITY_COUNTRY_OCCUPY_CITY_UPDATE, msg);
#endif
}

