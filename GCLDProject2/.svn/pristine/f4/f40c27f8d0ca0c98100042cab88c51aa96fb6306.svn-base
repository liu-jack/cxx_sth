
#include "activity_plan.h"
#include "InterServer.pb.h"


// void ActivityPlanTable::LoadFrom( const pb::SC2G_activity_plan& info )
// {
// 	activity_id = info.activity_id();
// 	open_way = info.open_way();
// 	start_time = info.start_time();
// 	last_time = info.last_time();
// 	show_time = info.show_time();
// 	start_date = info.start_date();
// 	end_date = info.end_date();
// 	end_show_date = info.end_show_date();
// 	stage_area = info.stage_area();
// 	need_greater_than_open = info.need_greater_than_open();
// 	need_less_than_open = info.need_less_than_open();
// }

void ActivityPlanTable::SaveTo( pb::SC2G_activity_plan& info )
{
	info.set_table_id(id);
	info.set_activity_id(activity_id);
	info.set_open_way(open_way);
	info.set_start_time(start_time);
	info.set_last_time(last_time);
	info.set_show_time(show_time);
	info.set_start_date(start_date);
	info.set_end_date(end_date);
	info.set_end_show_date(end_show_date);
	info.set_stage_area(stage_area);
	info.set_need_greater_than_open(need_greater_than_open);
	info.set_need_less_than_open(need_less_than_open);
}