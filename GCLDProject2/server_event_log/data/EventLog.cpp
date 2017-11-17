
#include <sstream>

#include "EventLog.h"
#include "Logger.h"
#include "utility/Utility.h"
#include "utility/TimeParser.h"
#include "def/MmoAssert.h"
#include "InterServer.pb.h"

std::string EventLog::s_table_name ;

void EventLog::ReadFrom( const pb::Server_Event &info, uint32 reg_id, uint32 ser_id )
{
	event_id    =info.event_id() ;
	region_id   =reg_id ;
	server_id   =ser_id ;
	account_id  =info.account_id() ;
	player_id   =info.player_id() ;
	time        =info.time() ;

	for( int i =0; i < info.params_size() && i < EVENT_LOG_PARAM_COUNT; ++i )
	{
		const pb::EventParm &paramInfo =info.params( i ) ;
		switch( paramInfo.parm_type() )
		{
		case pb::EventParm::INT32:
			{
				Utility::FormatString( params[ i ], "%d", paramInfo.value_int32() ) ;
			} break ;

		case pb::EventParm::INT64:
			{
				Utility::FormatString( params[ i ], "%lld", paramInfo.value_int64() ) ;
			} break ;

		case pb::EventParm::UINT32:
			{
				Utility::FormatString( params[ i ], "%u", paramInfo.value_uint32() ) ;
			} break ;

		case pb::EventParm::UINT64:
			{
				Utility::FormatString( params[ i ], "%llu", paramInfo.value_uint64() ) ;
			} break ;
		case pb::EventParm::STRING:
			{
				params[ i ] =paramInfo.value_str() ;
			} break ;
		case pb::EventParm::FLOAT:
			{
				Utility::FormatString( params[ i ], "%0.2f", paramInfo.value_float() ) ;
			} break ;
		default:
			ELOG( "EventLog::ReadFrom( const pb::Server_Event &info ) recv unkonw type( %u )!!", paramInfo.parm_type() ) ;
		}
	}
}


void EventLog::GetCreateTableSql( std::string &sql )
{
	std::stringstream ss ;

	string format( GetFormat() ) ;

	ss << "CREATE TABLE "<< GetTableName() << " ("
		<< "id         bigint(33) unsigned NOT NULL auto_increment,"
		<< "event_id   int(11)    unsigned NOT NULL default 0,"
		<< "region_id  int(11)    unsigned NOT NULL default 0,"
		<< "server_id  int(11)    unsigned NOT NULL default 0,"
		<< "account_id bigint(33) unsigned NOT NULL default 0,"
		<< "player_id  bigint(33) unsigned NOT NULL default 0,"
		<< "time       datetime                     default '0000-00-00 00:00:00'," ;

	std::string::size_type begPos =format.find( 's' ) ;
	if( begPos != string::npos )
	{
		for( size_t i =begPos; i < format.size(); ++i )
		{
			switch( format[ i ] )
			{
			case 's':
				{
					ss << "param" << i - begPos << " varchar(125) default NULL," ;
				} break ;

			default:
				{
					ELOG( "EventLog::GetCreateTableSql( std::string &sql ) had unknow create type!" ) ;
				}
			}
		}
	}


	ss << "PRIMARY KEY (id)"
		<<") ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;" ;

	sql =ss.str() ;
}

std::string EventLog::GetInsertSql()
{
	std::stringstream ss;
	ss<<"REPLACE INTO ";
	ss<< GetTableName();
	ss<<"(event_id,region_id,server_id,account_id,player_id,time";

	string format( GetFormat() ) ;
	std::string::size_type begPos =format.find( 's' ) ;
	if( begPos != string::npos )
	{
		for( size_t i =begPos; i < format.size(); ++i )
		{
			switch( format[ i ] )
			{
			case 's':
				{
					ss << ",param" << i - begPos;
				} break ;

			default:
				{
					ELOG( "EventLog::GetInsertSqlhad unknow create type!" ) ;
				}
			}
		}
	}
	ss<<")";
	ss<<" value (";
	const char * p = GetFormat();
	char * structpointer = (char*)this;
	uint32 offset = 0;
	for(; *p != 0; ++p)
	{
		switch(*p)
		{
		case 'k':
		case 'u':	// Unsigned integer
			ss <<  *(uint32*)&structpointer[offset];
			offset += sizeof(uint32);
			break;

		case 'i':	// Signed integer
			ss << *(int32*)&structpointer[offset];
			offset += sizeof(int32);
			break;

		case 'l':	// Signed integer 64
			ss << *(int64*)&structpointer[offset];
			offset += sizeof(int64);
			break;

		case 'b':	// Unsigned integer 64
			ss << *(uint64*)&structpointer[offset];
			offset += sizeof(uint64);
			break;

		case 's':	// Null-terminated string
			ss << "\"" << (*(std::string*)&structpointer[offset])/*.c_str()*/ << "\"";
			offset += sizeof(std::string);
			break;
		case 'x':	// Skip
			//MASSERT(false,"No Skip In Develop Step");
			break;

		case 'f':	// Float
			ss << *(float*)&structpointer[offset];
			offset += sizeof(float);
			break;

		case 'c':	// Char
			ss << (uint32)(*(uint8*)&structpointer[offset]);
			offset += sizeof(uint8);
			break;

		case 'h':	// Short
			ss << (uint32)(*(uint16*)&structpointer[offset]);
			offset += sizeof(uint16);
			break;

		case 't':	// time 
			{
				string strtime( "1970-01-01 08:00:00" );
				TimeT2String(*(int64*)&structpointer[offset], strtime);
				ss << "\"" << strtime.c_str() << "\"";
				offset += sizeof(int64);
			}
			break;

		default:	// unknown
			printf("%s:unknown field type in string: %d\n", GetTableName(), *p);
			break;
		}
		if ( *(p+1) != 0 )
		{
			ss << ",";
		}else{
			ss << ")";
		}
	}
	return ss.str();
}
