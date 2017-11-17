
#include "SeigeForceTable.h"
#include "Fuben.pb.h"

void SeigeForceTable::SaveTo(pb::Seige_Force_info& msg)
{
	msg.set_army_id(army_id);
	msg.set_level(level);
	msg.set_cur_all_times(cur_all_times);
	msg.set_cur_exp(cur_exp);
	msg.set_left_times(left_times);
	msg.set_skill_id(skill_id);
	for(int i = 0;i < ARRAY_SIZE(specific_ids);++i)
	{
		if(specific_ids[i] != 0)
		{
			msg.add_specific_ids(specific_ids[i]);
		}
	}
	for(int i = 0;i < ARRAY_SIZE(terrains);++i)
	{
		if(terrains[i] != 0)
		{
			msg.add_terrain(terrains[i]);
		}
	}
}

void SeigeForceTable::LoadFrom(const pb::Seige_Force_info& msg)
{
	army_id = msg.army_id();
	left_times = msg.left_times();
	level = msg.level();
	cur_all_times = msg.cur_all_times();
	cur_exp = msg.cur_exp();
	skill_id = msg.skill_id();
	for(int i = 0;i < msg.specific_ids_size();++i)
	{
		specific_ids[i] = msg.specific_ids(i);
	}
	for(int i = 0;i < msg.terrain_size();++i)
	{
		terrains[i] = msg.terrain(i);
	}
}
