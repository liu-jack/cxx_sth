#include <assert.h>

#include <iostream>

#include "CenterServer.h"
#include "CenterServerHttpClients.h"

#include "Logger.h"

#include "memory_buffer/NetPack.h"
#include "net_framework/IoServicePool.h"
#include "mysql_common/MySqlSingleton.h"
#include "CenterDataMgr.h"

int run_server(int argc, char *argv[])
{
	CenterServer::NewInstance() ;

	// Init config
	if( !sCenterSvr.InitGameConf( "etc/", "server_center.conf", "default/server_center_default.conf" ) )
	{
		printf( "config loading fail.....................!" ) ;
		assert( false ) ;
		return 0 ;
	}

	IoServicePool::NewInstance() ;
	CenterServerHttpClients::NewInstance() ;
	CenterDataMgr::NewInstance();



	const ConfigMgr &config =sCenterSvr.Config() ;

	sLogger.AddFileLog( sCenterSvr.Config().GetString("Log.dir", "logs/server_center_" ).c_str() ) ;

	bool isRunning =true ;

	// 数据库线程初始化
	int port             = config.GetInt("Database.port", 3306) ;
	int connect_count    = config.GetInt("Database.connect_count", 3) ;
	int max_connect_count= config.GetInt("Database.max_connect_count", 5) ;
	std::string username = config.GetString("Database.user_name", "root" ) ;
	std::string password = config.GetString("Database.password", "123456" ) ;
	std::string hostname = config.GetString("Database.ip","127.0.0.1" ) ;
	std::string db_name  = config.GetString("Database.db_name", "slg_backstage" ) ;

	isRunning &= sDatabase.Connect( hostname.c_str(), (unsigned int)port, username.c_str(), password.c_str(), db_name.c_str(), connect_count, max_connect_count ) ;

	if (isRunning)
	{
		sDatabase.Start() ;

		// Init data
		if( !sCenterDataMgr.InitData(sCenterSvr.GetRegionId()) )
		{
			isRunning =false ;
			ELOG( "Init data fail will close !" ) ;
		}

		NLOG( "Start Io service pool........." ) ;
		int io_count =config.GetInt( "Self.io_service_count", 0 ) ;
		sIoPool.Run( io_count <= 0 ? boost::thread::hardware_concurrency() : io_count ) ; 

		NLOG( "Starting network service ...." ) ;
		sCenterSvr.SetServerType( PACKET_TYPE_SERVER_CENTER ) ;
		sCenterSvr.SetPackPrintDiffCount( config.GetInt( "Self.packet_status_print_diff_count", 20000 ) ) ;

		std::string listenPort =sCenterSvr.Config().GetString( "Self.port", "55800" ) ;
		NLOG( "Starting network service listen port: %s ....", listenPort.c_str() ) ;
		sCenterSvr.StartListenPort( listenPort ) ;

		NLOG( "Starting center server ...." ) ;
		sCenterSvr.Start() ;
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

	NLOG( "Stopping center server...." ) ;
	sCenterSvr.Stop() ;

	NLOG( "Delete CenterServerHttpClients....." ) ;
	CenterServerHttpClients::DeleteInstance() ;

	NLOG( "Delete center server...." ) ;
	CenterServer::DeleteInstance() ;

	NLOG( "Stopping Io service pool...." ) ;
	sIoPool.Stop() ;

	NLOG( "Delete IoServicePool....." ) ;
	IoServicePool::DeleteInstance() ;

	return 0;
}
