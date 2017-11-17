#include <assert.h>

#include <iostream>

#include "Logger.h"

#include "MySqlSingleton.h"

#include "DbProxyServer.h"

#include "data/DbProxyDataMgr.h"

#include "memory_buffer/NetPack.h"
#include "net_framework/IoServicePool.h"


int run_server(int argc, char *argv[])
{
	DbProxyServer::NewInstance() ;

	// Init config
	if( !sDBServer.InitGameConf( "etc/", "server_dbproxy.conf", "default/server_dbproxy_default.conf" ) )
	{
		printf( "config loading fail.....................!" ) ;
		assert( false ) ;
		return 0 ;
	}

	IoServicePool::NewInstance() ;
	DbProxyDataMgr::NewInstance() ;


	bool isRunning =true ;
	const ConfigMgr &config =sDBServer.Config() ;

	sLogger.AddFileLog( config.GetString("Log.dir", "logs/server_dbproxy_" ).c_str() ) ;

	// 数据库服务器线程初始化
	int port             = config.GetInt("Database.port", 3306) ;
	int connect_count    = config.GetInt("Database.connect_count", 3) ;
	int max_connect_count= config.GetInt("Database.max_connect_count", 5) ;
	std::string username = config.GetString("Database.user_name", "root" ) ;
	std::string password = config.GetString("Database.password", "123456" ) ;
	std::string hostname = config.GetString("Database.ip","127.0.0.1" ) ;
	std::string db_name  = config.GetString("Database.db_name", "db_char" ) ;
	int curdbproxyid     = config.GetInt( "Self.dbpro_id", 1);

	isRunning &= sDatabase.Connect( hostname.c_str(), (unsigned int)port, username.c_str(), password.c_str(), db_name.c_str(), connect_count, max_connect_count ) ;
	
    if ( isRunning)
    {
		NLOG( "Starting database therad...." ) ;
        sDatabase.Start() ;

        // Init data
        sDataMgr.InitData(curdbproxyid) ;

		NLOG( "Start Io service pool........." ) ;
		int io_count =config.GetInt( "Self.io_service_count", 0 ) ;
		sIoPool.Run( io_count <= 0 ? boost::thread::hardware_concurrency() : io_count ) ; 

        NLOG( "Starting network service ...." ) ;
        sDBServer.SetServerType( PACKET_TYPE_SERVER_DBPROXY ) ;
        sDBServer.SetPackPrintDiffCount( config.GetInt( "Self.packet_status_print_diff_count", 20000 ) ) ;

		std::string listenPort =sDBServer.Config().GetString( "Self.port", "55600" ) ;
        NLOG( "Starting network service listen port: %s ....", listenPort.c_str() ) ;
        sDBServer.StartListenPort( listenPort ) ;

        NLOG( "Starting dbproxy server ...." ) ;
        sDBServer.Start() ;

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

	NLOG( "Stopping dbproxy server...." ) ;
	sDBServer.Stop() ;

	NLOG( "Delete DbProxyServer....." ) ;
	DbProxyServer::DeleteInstance() ;

	NLOG( "Delete DbProxyDataMgr....." ) ;
	DbProxyDataMgr::DeleteInstance() ;

	NLOG( "Stopping Io service pool...." ) ;
	sIoPool.Stop() ;

	NLOG( "Delete IoServicePool....." ) ;
	IoServicePool::DeleteInstance() ;
	return 0;
}
