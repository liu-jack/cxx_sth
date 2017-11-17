
#include "ContinueOccupyCityTable.h"
#include "Fuben.pb.h"
#include "OS.h"

void ContinueOccupyCityTable::SaveTo(pb::BaseContinueMsg& msg)
{
	if(sOS.IsToday(static_cast<time_t>(active_time)))
	{
		msg.set_finish_time(finish_time);
		msg.set_active_time(active_time);
		msg.set_con_occupy_citys(serial_city_num);
	}
}

void ContinueOccupyCityTable::LoadFrom(const pb::BaseContinueMsg& msg)
{
	active_time = msg.active_time();
	finish_time = msg.finish_time();
	serial_city_num = msg.con_occupy_citys();
}
