/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "PlayerWorldFightAchievementDB.h"
#include "WorldFightAchievement/WorldFightAchievement.h"
#include "../object/Player.h"
#include "../server_client/DbProxyServerClient.h"
#include "World.pb.h"
#include "ServerOpcode.pb.h"

using namespace pb;
void PlayerWorldFightAchievementDB::UpdateAchievementInfoToDb(Player& player,const uint32 city_id, const AchievementInfo* info)
{
#ifdef _MMO_SERVER_
	pb::GxDB_World_Achievement_Info msg;
	msg.set_state(info->state);
	msg.set_city_id(city_id);
	msg.set_type(info->type);
	sDbProxyClient.SendPlayerPack(player, SG2D_WORLD_FIGHT_ACHIEVEMENT_UPDATE, msg);
#endif
}

void PlayerWorldFightAchievementDB::UpdateAchievementInfoToDb(Player& player,const uint32 city_id, const uint32 type,const uint32 state)
{
#ifdef _MMO_SERVER_
	pb::GxDB_World_Achievement_Info msg;
	msg.set_state(state);
	msg.set_city_id(city_id);
	msg.set_type(type);
	sDbProxyClient.SendPlayerPack(player, SG2D_WORLD_FIGHT_ACHIEVEMENT_UPDATE, msg);
#endif
}
