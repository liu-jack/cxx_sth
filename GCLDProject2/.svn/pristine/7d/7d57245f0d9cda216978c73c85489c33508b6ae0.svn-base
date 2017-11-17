

#include "DailyQuestTable.h"
#include "Quest.pb.h"

void DailyQuestTable::SaveTo(pb::GxDB_Daily_Quest_Info& msg)
{
	msg.set_quest_id(questid);
	msg.set_finished_times(finish_times);
	msg.set_is_finished(is_finished);
}


void DailyQuestTable::LoadFrom(const pb::GxDB_Daily_Quest_Info& msg)
{
	questid = msg.quest_id();
	finish_times = msg.finished_times();
	is_finished = msg.is_finished();
}