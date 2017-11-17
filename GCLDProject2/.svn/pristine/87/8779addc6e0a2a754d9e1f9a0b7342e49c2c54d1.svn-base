


#include "CongratulateTable.h"
#include "Rank.pb.h"


void CongratulateTable::SaveTo(pb::GxDB_CongratulateInfo& msg)
{
	msg.set_table_id(table_id);
	msg.set_is_achieve(is_achieve);
	//msg.set_is_reward(is_reward);
	msg.set_been_congratulated_count(been_congratulated_count);
	msg.set_active_time(active_time);
}

void CongratulateTable::LoadFrom(const pb::GxDB_CongratulateInfo& msg)
{
	table_id = msg.table_id();
	//is_reward = msg.is_reward();
	is_achieve = msg.is_achieve();
	been_congratulated_count = msg.been_congratulated_count();
	active_time = msg.active_time();
}

void CongratulateBeenCongratulatedTable::SaveTo(pb::C2GS_Congratulate_Accept& msg)
{
	msg.set_table_id(table_id);
	msg.set_suit_id(suit_id);
	msg.set_rank(rank);
	msg.set_to_player_id(to_player_id);
	msg.set_congratulate_type(congratulate_tpe);
	msg.set_active_time(active_time);
}

void CongratulateBeenCongratulatedTable::LoadFrom(const pb::C2GS_Congratulate_Accept& msg)
{
	table_id = msg.table_id();
	suit_id = msg.suit_id();
	rank = msg.rank();
	congratulate_tpe = msg.congratulate_type();
	to_player_id = msg.to_player_id();
	active_time = msg.active_time();
}
