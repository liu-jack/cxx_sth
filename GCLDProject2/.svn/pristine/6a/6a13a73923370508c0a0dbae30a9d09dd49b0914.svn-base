#include "PalaceDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "Palace.pb.h"
#include "../Palace/PalaceMgr.h"
#include "../object/Player.h"
#include "../Palace/PalaceAchievementLog.h"

void PalaceDB::RecvAllInfoFromDB(const pb::GS2C_Palace_All_Info& msg)
{
    if (msg.office_lst_size() > 0)
    {
        for (int i = 1; i <= MAX_PLAYER_COUNTRY_ID; ++i)
        {
            sPalaceMgr(i)->LoadFrom(i, msg);
        }
    }
    else
    {
        SendAllInfoToDb(); //开服，数据中没有，发过去
    }
}
void PalaceDB::SendAllInfoToDb()
{
#ifdef _MMO_SERVER_
    pb::GS2C_Palace_All_Info msg;
    for (int i = 1; i <= MAX_PLAYER_COUNTRY_ID; ++i)
    {
        sPalaceMgr(i)->SaveTo(i, msg);
    }
    sDbProxyClient.Send(pb::SG2D_PALACE_ALL_INFO, msg);
#endif
}
void PalaceDB::UpdateOfficeInfoToDb(int countryId, int officeId)
{
#ifdef _MMO_SERVER_
    pb::GS2C_Palace_All_Info msg;
    sPalaceMgr(countryId)->SaveTo(countryId, officeId, msg);
    sDbProxyClient.Send(pb::SG2D_PALACE_ALL_INFO, msg);
#endif
}

void PalaceDB::SendAchievementInfoToDb(Player& player,int office_id)
{
#ifdef _MMO_SERVER_
	pb::PalaceAchievementInfo msg;
	player.m_PalaceAchievementLog->SaveTo(msg,office_id);
	sDbProxyClient.SendPlayerPack(player,pb::SG2D_PALACE_ACHIEVEMENT_INFO,msg);
#endif
}
