#pragma once

#include "mysql_common/structDef.h"

namespace pb
{
	class SC2G_activity_proto;
}
#pragma pack(push,1)

struct ActivityProtoTable
{
public:
	INIT_GAMEDATA( ActivityProtoTable ) ;
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
		return "activity_proto";
	}

	static const char* GetFormat()
	{

		return "buu";
	}

//	void LoadFrom(const pb::SC2G_activity_proto& info);
	void SaveTo(pb::SC2G_activity_proto& info);
public:
	uint64			id;
	uint32			act_enum_id;
	uint32			reward_type;
} ;

#pragma pack(pop)
