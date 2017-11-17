/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "../object/Player.h"
#include "GrowUpQuest/GrowUpQuest.h"
#include "PlayerGrowUpQuestDB.h"
#include "ServerOpcode.pb.h"
#include "../server_client/DbProxyServerClient.h"
#include "Quest.pb.h"
using namespace pb;

void PlayerGrowUpQuestDb::SendUpdateInfoToDb( Player& player,const uint32 id,const GrowUpQuestStruct &ref ,uint32 operate_type)
{
#ifdef _MMO_SERVER_
	pb::GxDB_Grow_Up_Quest_Info msg;
	msg.set_operate_type(operate_type);
	msg.set_quest_type(ref.quest_type);
	msg.set_quest_id(id);
	msg.set_is_finished(ref.is_finished);
	msg.set_cur_value(ref.cur_value);
	msg.set_group_id(ref.group_id);
	msg.set_is_take_reward(ref.is_take_reward);
	sDbProxyClient.SendPlayerPack(player, SG2D_GROW_UP_QUEST_UPDATE, msg);
#endif
}	

void PlayerGrowUpQuestDb::SendUpdateGroupInfoToDb( Player& player,const uint32 group_id,const GrowUpQuestGroupStruct& ref,uint32 operate_type)
{
#ifdef _MMO_SERVER_
	pb::GxDB_Grow_Up_Quest_Group_Info msg;
	msg.set_operate_type(operate_type);
	msg.set_group_id(group_id);
	msg.set_group_is_finished(ref.is_finished);
	msg.set_is_take_group_reward(ref.is_take_group_reward);
	if(ref.is_finished)
	{
		sDbProxyClient.SendPlayerPack(player,SG2D_GROW_UP_QUEST_GROUP_UPDATE,msg);
	}
#endif
}

void PlayerGrowUpQuestDb::SendDeleteAllInfo(Player& player,uint32 operate_type)
{
#ifdef _MMO_SERVER_
	pb::GxDB_Grow_Up_Quest_Delete msg;
	msg.set_operate_type(operate_type);
	sDbProxyClient.SendPlayerPack(player,SG2D_GROW_UP_QUEST_DELETE_INFO,msg);
#endif
}
