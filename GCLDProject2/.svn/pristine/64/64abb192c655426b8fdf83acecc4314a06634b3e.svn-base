#include "PlayerAltarLogDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "../Altar/PlayerAltarLog.h"
#include "Altar.pb.h"

using namespace pb;

void PlayerAltarLogDB::SendAllInfoToDb(Player& player, PlayerAltarLog& log)
{
    GS2C_Altar_All_Info msg;
    log.SaveTo(msg);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_ALTAR_ALL_INFO, msg);
}
void PlayerAltarLogDB::SendSubAltarToDb(Player& player, PlayerAltarLog& log)
{
    GS2C_Altar_All_Info msg;
    for (std::vector<PlayerAltarLog::SubAltar>::iterator it = log.m_subAltarLst.begin();
        it != log.m_subAltarLst.end(); ++it)
    {
        pb::SubAltar* data = msg.add_sub_altar();
        data->set_is_level_up(it->isBuildingUp);
        data->set_have_reward(it->haveReward);
        data->set_study_times(it->studyTimes);
        data->set_last_study_time(it->studyEndTime);
	}
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_ALTAR_ALL_INFO, msg);
}
void PlayerAltarLogDB::SendLevelDataToDb(Player& player, PlayerAltarLog& log)
{
    GS2C_Altar_All_Info msg;
    msg.set_levle(log.m_level);
    msg.set_silver_coin(log.m_SilverCoinForBuild);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_ALTAR_ALL_INFO, msg);
}

