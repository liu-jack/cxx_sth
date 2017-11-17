/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "PlayerGeneralRewardDB.h"
#include "Activity.pb.h"
#include "../server_client/DbProxyServerClient.h"
#include "../object/Player.h"
#include "ServerOpcode.pb.h"
#include "WannaBeStronger/WannaBeStrongerLog.h"
#include "Opcode.pb.h"

void GeneralRewardDB::SendUpdateInfoToDb(Player& player,const uint32 charId,bool has_first_reward,uint32 operate_type)
{
#ifdef _MMO_SERVER_
	pb::GxDB_Hero_Reward_info msg;
	msg.set_charid(charId);
	msg.set_has_first_reward(has_first_reward);
	msg.set_operate_type(operate_type);
	sDbProxyClient.SendPlayerPack(player, pb::SG2D_ACTIVITY_GENERAL_REWARD_UPDATE, msg);
#endif
}

void GeneralRewardDB::SendDeleteInfoToDb(uint32 operate_type)
{
#ifdef _MMO_SERVER_
	pb::GxDB_Hero_Reward_delete msg;
	msg.set_operate_type(operate_type);
	sDbProxyClient.Send(pb::SG2D_ACTIVITY_GENERAL_REWARD_DELETE, msg);
#endif
}

void WannaBeStrongerDB::SendUpdateInfoToDb(Player& player,pb::WannaBeStrongerInfo& msg)
{
	sDbProxyClient.SendPlayerPack(player,pb::SG2D_WANNA_BE_STRONGER_UPDATE, msg);
}

void WannaBeStrongerDB::SendUpdateInfoToDb(Player& player,const uint32 groupId,const BeStrongerInfo& ref)
{
	pb::WannaBeStrongerInfo msg;
	player.m_WannaBeStrongerLog->SaveTo(groupId,ref,msg);
	sDbProxyClient.SendPlayerPack(player,pb::SG2D_WANNA_BE_STRONGER_UPDATE, msg);
}
