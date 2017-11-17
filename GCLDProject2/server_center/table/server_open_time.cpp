
#include "server_open_time.h"
#include "InterServer.pb.h"

// 
// void ServerOpenTimeTable::LoadFrom( const pb::SC2G_server_open_time& info )
// {
// 	server_id = info.server_id();
// 	open_time = info.open_time();
// }

void ServerOpenTimeTable::SaveTo( pb::SC2G_server_open_time& info )
{
	info.set_table_id(id);
	info.set_server_id(server_id);
	info.set_open_time(open_time);
}