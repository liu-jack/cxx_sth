#include "PlayerFuBenDB.h"
#include "Fuben.pb.h"
#include "../fuben/FubenLog.h"
#include "../server_client/DbProxyServerClient.h"
#include "InterServer.pb.h"
#include "ServerOpcode.pb.h"
#include "Combat.pb.h"
#include "object/Player.h"
#include "Opcode.pb.h"


void PlayerFuBenDB::SendFuBenBaseToDb(Player& player, FubenLog& log)
{
    pb::SG2D_UpdataFuBenBase msg;
    msg.set_cur_fuben_id(log.m_curFubenID);
    msg.set_cur_stage_id(log.m_curStageID);
	for(size_t i = 0;i < ARRAY_SIZE(log.NR_stars_);++i)
	{
		msg.add_nr_stars(log.NR_stars_[i]);
	}

    msg.set_op_type(pb::DATA_UPDATE);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_UPDATA_FUBEN_BASE, msg);
}
void PlayerFuBenDB::SendHideFubenToDb(Player& player, uint32 stageId, HideFubenBaseInfo& p)
{
	pb::HideFubenInfo msg;
	msg.set_free_times(p.free_time_);
	msg.set_hide_stage_id(stageId);
	msg.set_count_times(p.today_fight_times_);
	msg.set_kill_monster(p.kill_monsters_);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_HIDE_FUBEN_UPDATE, msg);
}
void PlayerFuBenDB::SendPassStageToDb(Player& player, uint32 stagetype,stage_info*p)
{
    pb::Stage_Info msg;
    msg.set_stage_type(stagetype);
	msg.set_pass_count(p->pass_count_);
	msg.set_is_take_reward(p->is_take_rewrd_);
    sDbProxyClient.SendPlayerPack(player, pb::SG2D_UPDATA_FUBEN_PASSSTAGE, msg);
}

void PlayerFuBenDB::SendCrusadeToDb(Player& player, FubenLog& log)
{
	pb::GS2C_Combat_Crusade msg;
	log.SaveTo(msg);
	sDbProxyClient.SendPlayerPack(player, pb::SG2D_CRUSADE_UPDATE, msg);
}

void PlayerFuBenDB::SendBranchFubenToDb(Player& player, const uint32 stageId,const BranchBaseInfo& info,FubenLog& log)
{
	pb::BranchFubenInfo msg;
	log.SaveTo(stageId,info,msg);
	sDbProxyClient.SendPlayerPack(player, pb::SG2D_BRANCH_FUBEN_UPDATE, msg);
	player.Send(pb::SMSG_UPDATE_BRANCH_FUBEN,msg);
}
