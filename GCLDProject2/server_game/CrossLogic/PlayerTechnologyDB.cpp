#include "PlayerTechnologyDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "Technology.pb.h"
#include "../Technology/PlayerTechnology.h"

using namespace pb;

// void PlayerTechnologyDB::SendAllInfoToDb(Player& player, PlayerTechnology& log)
// {
//     C2GS_Technology_All_Info msg;
//     log.SaveTo(msg);
//     sDbProxyClient.SendPlayerPack(player, SG2D_UPDATA_TECHNOLOGY, msg);
// }
void PlayerTechnologyDB::UpdateStudyInfoToDb(Player& player, uint32 id, const ::TechnologyStudyStruct& data)
{
#ifdef _MMO_SERVER_
    pb::TechnologyStudy msg;
    msg.set_tec_id(id);
    msg.set_star_cnt(data.starNum);
    msg.set_end_time(data.endTime);
	msg.set_is_finish_study(data.is_finished);
    sDbProxyClient.SendPlayerPack(player, SG2D_UPDATA_TECHNOLOGY_STUDY, msg);
#endif
}
