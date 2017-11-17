#ifndef ACCOUNT_H__
#define ACCOUNT_H__

#include "structDef.h"

namespace pb
{
	class Server_Event ;
}

#define EVENT_LOG_PARAM_COUNT 10

//set byte alignment 
#pragma pack(push,1)


struct EventLog : public BaseStruct
{
public:
	typedef uint64 Key ;

public:
	INIT_GAMEDATA( EventLog ) ;

public:
	static const char* GetTableName()
	{
		return s_table_name.c_str() ;
	}

	static void SetTableName( const std::string &str )
	{
		s_table_name =str ;
	}

	static const char* GetFormat()
	{
		return "uuubbt""ssssssssss";
	}

	void ReadFrom( const pb::Server_Event &info, uint32 reg_id, uint32 ser_id) ;
	static void GetCreateTableSql( std::string &sql ) ;
	std::string GetInsertSql();
public:
	static std::string s_table_name ;

public:
	uint32      event_id ;
	uint32      region_id ;
	uint32      server_id ;
	uint64      account_id ;
	uint64      player_id ;
	uint64      time ;
	std::string params[ EVENT_LOG_PARAM_COUNT ] ;
} ;



#pragma pack(pop)

#endif

