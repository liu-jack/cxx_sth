
#include <assert.h>

#include <iostream>

#include "Logger.h"

#include "EventLogServer.h"
#include "MySqlSingleton.h"

#include "memory_buffer/NetPack.h"

#include "server_client/CenterServerClient.h"
#include "data/EventLogDataMgr.h"

#include "net_framework/IoServicePool.h"

#include "IntegerKeyGenerate.h"



int run_server(int argc, char *argv[])
{
	EventLogServer::NewInstance() ;

	// Init config
	if( !sEventLogSvr.InitGameConf( "etc/", "server_event_log.conf", "default/server_event_log_default.conf" ) )
	{
		printf( "config loading fail.....................!" ) ;
		assert( false ) ;
		return 0 ;
	}

	IoServicePool::NewInstance() ;
	CenterServerClient::NewInstance() ;


	bool isRunning =true ;

	const ConfigMgr &config =sEventLogSvr.Config() ;

	sLogger.AddFileLog( config.GetString("Log.dir", "logs/server_event_log_" ).c_str() ) ;


	// 数据库线程初始化
	int port             = config.GetInt("Database.port", 3306) ;
	int connect_count    = config.GetInt("Database.connect_count", 3) ;
	int max_connect_count= config.GetInt("Database.max_connect_count", 5) ;
	std::string username = config.GetString("Database.user_name", "root" ) ;
	std::string password = config.GetString("Database.password", "123456" ) ;
	std::string hostname = config.GetString("Database.ip","127.0.0.1" ) ;
	std::string db_name  = config.GetString("Database.db_name", "mb_event_log" ) ;

	isRunning &= sDatabase.Connect( hostname.c_str(), (unsigned int)port, username.c_str(), password.c_str(), db_name.c_str(), connect_count, max_connect_count ) ;

	if( isRunning )
	{
		NLOG( "Starting database therad...." ) ;
		sDatabase.Start() ;

		NLOG( "Start Io service pool........." ) ;
		int io_count =config.GetInt( "Self.io_service_count", 0 ) ;
		sIoPool.Run( io_count <= 0 ? boost::thread::hardware_concurrency() : io_count ) ; 

		// Set center address
		std::string centerIp   =config.GetString( "CenterServer.ip", "127.0.0.1" ) ;
		std::string centerPort =config.GetString( "CenterServer.port", "55800" ) ;
		NLOG( "Setting center server client %s(%s) .", centerIp.c_str(), centerPort.c_str() ) ;
		sCenterClient.SetDstInfo( centerIp, centerPort ) ;

		NLOG( "Starting network service ...." ) ;
		sEventLogSvr.SetServerType( PACKET_TYPE_SERVER_EVENT ) ;
		sEventLogSvr.SetPackPrintDiffCount( config.GetInt( "Self.packet_status_print_diff_count", 20000 ) ) ;

		std::string listenPort =config.GetString( "Self.port", "55700" ) ;
		NLOG( "Starting network service listen port:%s ....", listenPort.c_str() ) ;
		sEventLogSvr.StartListenPort( listenPort ) ;
	
		NLOG( "Starting event server ...." ) ;
		sEventLogSvr.Start() ;

		// 初始化数据
		EventLogDataMgr::NewInstance().InitData() ;
	}
	else
	{
		MASSERT( false, " Connect database fail" ) ;
	}

#ifdef _WIN32
	char input[512];
#endif
	while( isRunning )
	{
#ifdef _WIN32
		std::cin.getline( input, 512, '\n' ) ;

		char *pInput =input ;
		char cIntput =*pInput++ ;
		switch( cIntput )
		{
		case 'q':
			{
				isRunning =false;
			} break ;

		default:
			{
			}
		}
#endif
		boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
	}

	NLOG( "Stopping Event server...." ) ;
	sEventLogSvr.Stop() ;

	NLOG( "Delete Event Server....." ) ;
	EventLogServer::DeleteInstance() ;

	NLOG( "Delete single client...." ) ;
	CenterServerClient::NewInstance() ;

	NLOG( "Stopping Io service pool...." ) ;
	sIoPool.Stop() ;

	NLOG( "Delete IoServicePool....." ) ;
	IoServicePool::DeleteInstance() ;

	return 0;
}
