/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "DailyQuest.h"
#include "Table/DailyQuestTableMgr.h"
#include "Quest.pb.h"
#include "../Trigger/trigger.h"
#include "object/Player.h"
#include "daily_quest_def.h"
#include "../CrossLogic/DailyQuestDB.h"
#include "Opcode.pb.h"

DailyQuestLog::DailyQuestLog()
{
	Init();
}

void DailyQuestLog::SaveTo( pb::GxDB_Daily_Quest& msg )
{
	for(DailyQuestStructMap::iterator iter = daily_quest_struct_map_.begin();iter != daily_quest_struct_map_.end();++iter)
	{
		pb::GxDB_Daily_Quest_Info* info = msg.add_quest_info();
		info->set_quest_id(iter->first);
		info->set_finished_times(iter->second.finish_times);
		info->set_is_finished(iter->second.is_finished);
	}
}

void DailyQuestLog::SaveTo( Player& player,pb::GS2C_Daily_Quest_All_Info& msg )
{
	for (DailyQuestStructMap::iterator it = daily_quest_struct_map_.begin(); it != daily_quest_struct_map_.end(); ++it)
	{
		const DailyQuest* quest = sDailyQuestTableMgr.GetDailyQuest(it->first);
		if(quest)
		{
			if(sTrigger.Check(&player, quest->TriggerId()))
			{
				pb::Daily_Quest_Info* info = msg.add_quest_info();
				info->set_quest_id(it->first);
				info->set_finish_times(it->second.finish_times);
				info->set_need_finish_times(quest->Value1());
				info->set_score(quest->QuestRewardScore());
			}
		}
	}
}

void DailyQuestLog::LoadFrom( const pb::GxDB_Daily_Quest& msg )
{
	for(int i = 0;i < msg.quest_info_size();++i)
	{
		const pb::GxDB_Daily_Quest_Info& Info = msg.quest_info(i);
		DailyQuestStruct& ref = daily_quest_struct_map_[Info.quest_id()];
		ref.finish_times = Info.finished_times();
		ref.is_finished = Info.is_finished();
	}
}

void DailyQuestLog::Init()
{
	daily_quest_struct_map_.clear();
	const DailyQuestMap& questmap = sDailyQuestTableMgr.GetDailyQuestMap();
	for(DailyQuestMap::const_iterator iter = questmap.begin();iter != questmap.end();++iter)
	{
		DailyQuestStruct& ref = daily_quest_struct_map_[iter->first];
		::memset(&ref,0,sizeof(DailyQuestStruct));
	}
}

DailyQuestLog::~DailyQuestLog()
{

}



bool DailyQuestLog::CheckDailyQuestFinished( const uint32 quest_id ) const
{
	DailyQuestStructMap::const_iterator Iter = daily_quest_struct_map_.find(quest_id);
	if(Iter != daily_quest_struct_map_.end())
	{
		if(Iter->second.is_finished)
		{
			return true;
		}
	}
	return false;
}

void DailyQuestLog::UpdateDailyQuestData( Player& player, const uint32 quest_id,const uint32 times )//to Mysql
{
	DailyQuestStruct& ref = daily_quest_struct_map_[quest_id];
	ref.finish_times += times;

	if(LIKELY(ref.finish_times >= sDailyQuestTableMgr.GetDailyQuest(quest_id)->Value1()))
	{
		if(ref.is_finished == 0)
		{
			ref.is_finished = 1;
			player.AddDailyQuestScore(GetScoreFromType(quest_id));
		}
	}
	else
	{
		ref.is_finished = 0;
	}

}

DailyQuestStruct& DailyQuestLog::GetDailyQuestStruct(const uint32 quest_id)
{
	return daily_quest_struct_map_[quest_id];
}

const uint32 DailyQuestLog::GetScoreFromType( const uint32 quest_id) const
{
	const DailyQuest* daily_quest = sDailyQuestTableMgr.GetDailyQuest(quest_id);
	if(daily_quest)
	{
		return daily_quest->QuestRewardScore();
	}
	return 0;
}

void DailyQuestLog::SendUpdateInfoToClient(Player& player,const uint32 quest_id )//to client
{
	pb::GS2C_Daily_Quest_Update msg;
	DailyQuestStruct& ref = daily_quest_struct_map_[quest_id];
	const DailyQuest* quest = sDailyQuestTableMgr.GetDailyQuest(quest_id);
	if(!quest) return;
	pb::Daily_Quest_Info *info = msg.mutable_quest_info();
	info->set_quest_id(quest_id);
	info->set_finish_times(ref.finish_times);
	info->set_need_finish_times(quest->Value1());
	info->set_score(quest->QuestRewardScore());
	player.Send(pb::SMSG_DAILY_QUEST_UPDATE,msg);
}

void DailyQuestLog::GM_finish( Player& player,uint32 quest_id )
{
	if(quest_id == 0) return;
	if(!CheckDailyQuestFinished(quest_id))
	{
		const DailyQuest* quest = sDailyQuestTableMgr.GetDailyQuest(quest_id);
		if(!quest) return;
		DailyQuestStruct& ref = daily_quest_struct_map_[quest_id];
		ref.finish_times = quest->Value1();
		ref.is_finished = 1;
		player.AddDailyQuestScore(GetScoreFromQuestId(quest_id));

		SendUpdateInfoToClient(player,quest_id);
		DailyQuestDb::SendUpdateInfoToDb(player,quest_id,ref);
	}
}

const uint32 DailyQuestLog::GetScoreFromQuestId( const uint32 quest_id ) const
{
	const DailyQuest* daily_quest = sDailyQuestTableMgr.GetDailyQuest(quest_id);
	if(daily_quest)
	{
		return daily_quest->QuestRewardScore();
	}
	return 0;
}

DailyQuestStructMap& DailyQuestLog::GetMutableDailyQuestStructMap() 
{
	return daily_quest_struct_map_;
}