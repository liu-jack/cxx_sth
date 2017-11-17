#pragma once

#include "mysql_common/structDef.h"

namespace pb
{
	class SC2G_activity_plan;
}
#pragma pack(push,1)

struct ActivityPlanTable
{
public:
	INIT_GAMEDATA( ActivityPlanTable ) ;
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
		return "activity_plan";
	}

	static const char* GetFormat()
	{

		return "buuuuutttsuu";
	}

	void SaveTo(pb::SC2G_activity_plan& info);
public:
	uint64			id;
	uint32			activity_id;
	uint32			open_way;
	uint32			start_time;
	uint32			last_time;
	uint32			show_time;
	time_t			start_date;
	time_t			end_date;
	time_t			end_show_date;
	std::string		stage_area;
	uint32			need_greater_than_open;
	uint32			need_less_than_open;
} ;

#pragma pack(pop)
