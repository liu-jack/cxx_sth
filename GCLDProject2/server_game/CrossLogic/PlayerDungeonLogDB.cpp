#include "PlayerDungeonLogDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "Dungeon.pb.h"

using namespace pb;

void PlayerDungeonLogDB::SendBaseInfoToDb(Player& player, PlayerDungeonLog& log)
{
    GS2C_Dungeon_Base msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_DUNGEON_BASE_INFO, msg);
}
void PlayerDungeonLogDB::SendAddHeroToDb(Player& player, PlayerDungeonLog::CatchHero& hero)
{
    GS2C_Dungeon_Add_Hero msg;
    msg.mutable_hero()->set_player_id(hero.playerId);
    msg.mutable_hero()->set_hero_tid(hero.heroTid);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_DUNGEON_ADD_HERO, msg);
}
void PlayerDungeonLogDB::SendDelHeroToDb(Player& player, PlayerDungeonLog::CatchHero& hero)
{
    GS2C_Dungeon_Del_Hero msg;
    msg.mutable_hero()->set_player_id(hero.playerId);
    msg.mutable_hero()->set_hero_tid(hero.heroTid);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_DUNGEON_DEL_HERO, msg);
}