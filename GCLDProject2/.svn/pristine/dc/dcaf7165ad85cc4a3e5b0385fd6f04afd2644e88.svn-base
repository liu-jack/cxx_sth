#pragma once

#include "mysql_common/structDef.h"

namespace pb
{
	class SC2G_server_open_time;
}
#pragma pack(push,1)

struct ServerOpenTimeTable
{
public:
	INIT_GAMEDATA( ServerOpenTimeTable ) ;
	DECLARE_INCREASEID_FUCTION(uint64,uint64,id,"id");
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey( void ) const
	{
		return id ;
	}

	static const char* GetLogicalKeyName( void )
	{
		return "id" ;
	}

	static const char* GetTableName()
	{
		return "server_open_time";
	}

	static const char* GetFormat()
	{

		return "but";
	}

	//void LoadFrom(const pb::SC2G_server_open_time& info);
	void SaveTo(pb::SC2G_server_open_time& info);
public:
	uint64			id;
	uint32			server_id;
	time_t			open_time;
} ;

#pragma pack(pop)
