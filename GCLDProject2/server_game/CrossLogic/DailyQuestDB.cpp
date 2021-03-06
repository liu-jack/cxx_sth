/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "DailyQuestDB.h"
#include "object/Player.h"
#include "../server_client/DbProxyServerClient.h"
#include "DailyQuest/DailyQuest.h"
#include "Quest.pb.h"
#include "ServerOpcode.pb.h"
#include "DailyQuest/Table/DailyQuestTableMgr.h"
using namespace pb;


void DailyQuestDb::SendUpdateInfoToDb( Player& player, const uint32 quest_id ,const  DailyQuestStruct &ref)
{
#ifdef _MMO_SERVER_
	pb::GxDB_Daily_Quest_Info msg;
	msg.set_quest_id(quest_id);
	msg.set_finished_times(ref.finish_times);
	msg.set_is_finished(ref.is_finished);
	sDbProxyClient.SendPlayerPack(player, SG2D_DAILY_QUEST_UPDATE, msg);
#endif
}
//重置每日任务的时候用
void DailyQuestDb::SendUpdateInfoToDb( Player& player,const uint32 quest_id )
{
#ifdef _MMO_SERVER_
	pb::GxDB_Daily_Quest_Info msg;
	msg.set_quest_id(quest_id);
	msg.set_finished_times(0);
	msg.set_is_finished(0);
	sDbProxyClient.SendPlayerPack(player, SG2D_DAILY_QUEST_UPDATE, msg);
#endif
}