#include "PlayerMarketLogDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "../Market/PlayerMarketLog.h"
#include "Market.pb.h"
#include "../object/Player.h"

using namespace pb;

void PlayerMarketLogDB::SendCollectToDb(Player& player, PlayerMarketLog& log)
{
    GS2C_Market_Collect msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_MARKET_COLLECT, msg);
}
void PlayerMarketLogDB::SendBlackToDb(Player& player, PlayerMarketLog& log)
{
    GS2C_Market_Black msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_MARKET_BLACK, msg);
	//player.Send(pb::SG2D_MARKET_BLACK, msg);
}    

void PlayerMarketLogDB::SendBuyToDb(Player& player, PlayerMarketLog& log)
{
    GS2C_Market_Buy msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_MARKET_BUY, msg);
}
void PlayerMarketLogDB::SendMysteryToDb(Player& player, PlayerMarketLog& log)
{
    GS2C_Market_Mystery_Shop msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_MARKET_MYSTERY, msg);
}
