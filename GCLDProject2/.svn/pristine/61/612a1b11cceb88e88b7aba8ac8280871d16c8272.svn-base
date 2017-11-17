


#include "ActivityCountryOccupyCityTable.h"
#include "InterServer.pb.h"
void ActivityCountryOccupyCityTable::LoadFrom(const pb::Activity_Occupy_City_Num& msg)
{
	country_id = msg.country_id();
	city_num = msg.city_num();
	region_id = msg.region_id();
	server_id = msg.server_id();
}

void ActivityCountryOccupyCityTable::SaveTo(pb::Activity_Occupy_City_Num& msg)
{
	msg.set_city_num(city_num);
	msg.set_country_id(country_id);
	msg.set_region_id(region_id);
	msg.set_server_id(server_id);
}
