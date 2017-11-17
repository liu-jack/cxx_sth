#include "PlayerBaoQiLogDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "../BaoQi/PlayerBaoQiLog.h"
#include "BaoQi.pb.h"
#include "../object/Player.h"
using namespace pb;

void PlayerBaoQiLogDB::SendAllInfoToDb(Player& player, PlayerBaoQiLog& log)
{
#ifdef _MMO_SERVER_
    GS2C_BaoQi_All_Info msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_BAO_QI_ALL_INFO, msg);
#endif
}

void PlayerBaoQiLogDB::SendAllInfoToDb(Player& player)
{
#ifdef _MMO_SERVER_
	GS2C_BaoQi_All_Info msg;
	player.m_BaoQiLog->SaveTo(msg);
	sDbProxyClient.SendPlayerPack(player, pb::SG2D_BAO_QI_ALL_INFO, msg);
#endif
}
