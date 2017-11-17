#pragma once

#include "mysql_common/structDef.h"

namespace pb
{
	class SC2G_activity_reward;
}
#pragma pack(push,1)

struct ActivityRewardTable
{
public:
	INIT_GAMEDATA( ActivityRewardTable ) ;
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
		return "activity_reward";
	}

	static const char* GetFormat()
	{

		return "buuuuus";
	}

	//	void LoadFrom(const pb::SC2G_activity_proto& info);
	void SaveTo(pb::SC2G_activity_reward& info);
public:
	uint64			id;
	uint32			activity_id;
	uint32			cost_price;
	uint32			is_has_discount;
	uint32			cost_discount;
	uint32			condition;
	string			reward_list;
} ;

#pragma pack(pop)
