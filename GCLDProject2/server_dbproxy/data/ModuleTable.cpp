#include "ModuleTable.h"
//#include "boost/dynamic_bitset/dynamic_bitset.hpp"
//#include "utility/Utility.h"
#include "Module.pb.h"
//#include "Logger.h"


void ModuleTable::SaveTo( pb::GxDB_Module& msg )
{
    msg.set_module_id( module_id);
    msg.set_gain_mode(gain_mod);
	msg.set_expire_time(expire_time);
	msg.set_is_rebuild(is_rebuild);
}

void ModuleTable::LoadFrom( const pb::GxDB_Module& msg )
{
    module_id = msg.module_id();
    gain_mod = msg.gain_mode();
	expire_time = msg.expire_time();
	is_rebuild = msg.is_rebuild();
}


void BuildingTable::SaveTo( pb::GxDB_Building& msg )
{
   msg.set_building_id(building_id);
   msg.set_building_lv(cur_lv);
   msg.set_is_rebuild(is_rebuild);
   msg.set_lvup_start_time(lvup_start_time);
}

void BuildingTable::LoadFrom( const pb::GxDB_Building& msg )
{
   building_id = msg.building_id();
   cur_lv = msg.building_lv();
   is_rebuild = msg.is_rebuild();
   lvup_start_time = msg.lvup_start_time();
}