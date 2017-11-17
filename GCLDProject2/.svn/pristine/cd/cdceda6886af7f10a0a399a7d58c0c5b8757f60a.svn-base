/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "PlayerPassStageReward.h"
#include "PassStageActivity/ActivtyStage.h"
#include "../object/Player.h"
#include "Quest.pb.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"

using namespace pb;
void PlayerPassStageRewardDB::SendInfoToDB( Player& player,const uint32 id,PassStageRewardStruct& ref,uint32 operate_type)
{
	pb::pass_stage_reward_info reward_info;
	reward_info.set_id(id);
	reward_info.set_is_take_reward(ref.is_take_reward);
	reward_info.set_stage_is_passed(ref.stage_is_passed);
	reward_info.set_stage_level(ref.stage_level);
	reward_info.set_operate_type(operate_type);
	sDbProxyClient.SendPlayerPack(player, SG2D_ACTIVITY_PASS_STAGE_REWARD_DIAMOND_UPDATE, reward_info);
}

void PlayerPassStageRewardDB::SendInfoToDB( Player& player,const uint32 table_id,const uint32 is_take_reward,const uint32 stage_is_passed ,const uint32 stage_level ,uint32 operate_type)
{
	pb::pass_stage_reward_info reward_info;
	reward_info.set_id(table_id);
	reward_info.set_is_take_reward(is_take_reward);
	reward_info.set_stage_is_passed(stage_is_passed);
	reward_info.set_stage_level(stage_level);
	reward_info.set_operate_type(operate_type);
	sDbProxyClient.SendPlayerPack(player, SG2D_ACTIVITY_PASS_STAGE_REWARD_DIAMOND_UPDATE, reward_info);
}

void PlayerPassStageRewardDB::SendDeleteInfoToDb(Player& player,uint32 operate_type)
{
	pb::GxDB_Pass_Stage_Reward_Delete msg;
	msg.set_operate_type(operate_type);
	sDbProxyClient.SendPlayerPack(player, SG2D_PASS_STAGE_REWARD_DIAMOND_DELETE, msg);
}
