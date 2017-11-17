#include "PlayerMapLogicDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "../Map/PlayerMapLogic.h"
#include "MapLogic.pb.h"
#include "InterServer.pb.h"
#include "object/Player.h"

using namespace pb;

void PlayerMapLogicDB::SendBaseToDb(Player& player, PlayerMapLogic& log)
{
#ifdef _MMO_SERVER_
	GS2C_MapLogic_Init msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, SG2D_MAP_LOGIC_BASE, msg);
#endif
}
void PlayerMapLogicDB::UpdateCityEventToDb(Player& player, uint32 eventId, const CityEventInfo& info)
{
#ifdef _MMO_SERVER_
    CityEvent msg;
    PlayerMapLogic::SaveTo(msg, eventId, info);
    sDbProxyClient.SendPlayerPack(player, SG2D_MAP_LOGIC_UPDATE_CITY_EVENT, msg);
#endif
}

void PlayerMapLogicDB::UpdateFogRec(Player& player, PlayerMapLogic& log)
{
#ifdef _MMO_SERVER_
	GS2C_FOG_INFO_Lst msg;
	log.SaveTo(msg);
	sDbProxyClient.SendPlayerPack(player, SG2D_MAP_FOG_REC, msg);
#endif
}

void PlayerMapLogicDB::UpdatePersonalECToDb(Player& player, PersonalEventChain& personalEC)
{
#ifdef _MMO_SERVER_
	SG2D_PersonalEC msg;
	personalEC.SaveTo(msg,player.GetGuid());
	sDbProxyClient.SendPlayerPack(player,SG2D_UpdatePersonalEC,msg);
#endif
}