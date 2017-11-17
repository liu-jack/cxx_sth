

#include "WorldFightTable.h"
#include "World.pb.h"
#define E_CITY_OCCUPY_CITY 2
void WorldFightAchievementTable::SaveTo(pb::GxDB_World_Achievement_Info& msg)
{
	msg.set_type(type);
	msg.set_state(state);
	msg.set_city_id(city_id);
}

void WorldFightAchievementTable::LoadFrom(const pb::GxDB_World_Achievement_Info& msg)
{
	if(msg.type() == E_CITY_OCCUPY_CITY)
	{
		type |= (msg.type() << SHIFT_4);
		state &= 0x0f;
		state |= (msg.state() << SHIFT_4);
	}
	else
	{
		type |= msg.type();
		state &= (~0xf);
		state |= msg.state();
	}
	city_id = msg.city_id();
}
