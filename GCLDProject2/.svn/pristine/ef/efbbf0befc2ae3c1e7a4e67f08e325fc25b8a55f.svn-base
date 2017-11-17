
#include "ActivityControlTable.h"
#include "InterServer.pb.h"


void ActivityProtoTable::LoadFrom( const pb::SC2G_activity_proto& msg )
{
	id = msg.table_id();
	act_enum_type = msg.act_type_id();
	reward_type = msg.reward_type();
}

void ActivityProtoTable::SaveTo( pb::SC2G_activity_proto& msg )
{
	msg.set_table_id(id);
	msg.set_reward_type(reward_type);
	msg.set_act_type_id(act_enum_type);
}

bool ActivityProtoTable::IsModified( const pb::SC2G_activity_proto& msg )
{
	return id == msg.table_id() && (act_enum_type != msg.act_type_id() || reward_type != msg.reward_type());
}
void ActivityPlanTable::LoadFrom( const pb::SC2G_activity_plan& msg )
{
	id = msg.table_id();
	activity_id = msg.activity_id();
	open_way = msg.open_way();
	start_time = msg.start_time();
	last_time = msg.last_time();
	show_time = msg.show_time();
	start_date = msg.start_date();
	end_date = msg.end_date();
	end_show_date = msg.end_show_date();
	stage_area = msg.stage_area();
	need_greater_than_open = msg.need_greater_than_open();
	need_less_than_open = msg.need_less_than_open();
}

void ActivityPlanTable::SaveTo(  pb::SC2G_activity_plan& msg )
{
	msg.set_table_id(id);
	msg.set_open_way(open_way);
	msg.set_show_time(show_time);
	msg.set_activity_id(activity_id);
	msg.set_end_date(end_date);
	msg.set_end_show_date(end_show_date);
	msg.set_need_greater_than_open(need_greater_than_open);
	msg.set_need_less_than_open(need_less_than_open);
	msg.set_start_date(start_date);
	msg.set_start_time(start_time);
	msg.set_last_time(last_time);
	msg.set_stage_area(stage_area);
}

bool ActivityPlanTable::IsModified( const pb::SC2G_activity_plan& msg )
{
	return id == msg.table_id() &&  (activity_id != msg.activity_id() || end_date != msg.end_date()
		|| end_show_date != msg.end_show_date() || last_time != msg.last_time()
		|| need_greater_than_open != msg.need_greater_than_open() || need_less_than_open != msg.need_less_than_open()
		|| open_way != msg.open_way() || show_time != msg.show_time()
		|| stage_area != msg.stage_area() || start_date != msg.start_date()
		|| start_time != msg.start_time());
}
void ServerOpenTimeTable::LoadFrom( const pb::SC2G_server_open_time& msg )
{
	id = msg.table_id();
	server_id = msg.server_id();
	open_time = msg.open_time();
}

void ServerOpenTimeTable::SaveTo( pb::SC2G_server_open_time& msg )
{
	msg.set_table_id(id);
	msg.set_server_id(server_id);
	msg.set_open_time(open_time);
}

bool ServerOpenTimeTable::IsModified( const pb::SC2G_server_open_time& msg )
{
	return id == msg.table_id() && (server_id != msg.server_id() || open_time != msg.open_time());
}

void ActivityRewardTable::LoadFrom(const pb::SC2G_activity_reward& msg)
{
	id = msg.table_id();
	activity_id = msg.activity_id();
	cost_price = msg.cost_price();
	is_has_discount = msg.is_has_discount();
	cost_discount = msg.cost_discount();
	condition = msg.conditon();
	reward_list = msg.reward_list();
}

void ActivityRewardTable::SaveTo(pb::SC2G_activity_reward& msg)
{
	msg.set_activity_id(activity_id);
	msg.set_conditon(condition);
	msg.set_cost_discount(cost_discount);
	msg.set_table_id(id);
	msg.set_is_has_discount(is_has_discount);
	msg.set_reward_list(reward_list);
	msg.set_cost_price(cost_price);
}

bool ActivityRewardTable::IsModified(const pb::SC2G_activity_reward& msg)
{
	return id == msg.table_id() && (activity_id != msg.activity_id() || condition != msg.conditon() 
		|| cost_discount != msg.cost_discount() || is_has_discount != msg.is_has_discount() || reward_list != msg.reward_list()
		|| cost_price != msg.cost_price());
}
