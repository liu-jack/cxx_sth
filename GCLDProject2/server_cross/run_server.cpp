
#include <assert.h>

#include <iostream>

#include "DatabaseMgr.h"
#include "Logger.h"

#include "CrossServer.h"

#include "memory_buffer/NetPack.h"

#include "net_framework/IoServicePool.h"

#include "server_client/CenterServerClient.h"
#include "server_client/EventLogServerClient.h"
#include "server_client/LobbyServerClient.h"

#include "character/CharacterManager.h"
//#include "stage/StageManager.h"
//#include "Loot/LootManager.h"
#include "item/ItemManager.h"
#include "quest/QuestManager.h"
#include "Exploit/ExploitMgr.h"
#include "utility/TimePeriod.h"
//#include "sell/SellManager.h"
//#include "SysEvent/SysEventMgr.h"
//#include "operation/OperationMgr.h"
//#include "Guild/GuildHandler.h"
#include "BaseDefineMgr.h"
#include "object/PlayerDataManager.h"
#include "Loot/FixFakeRandom/FixFakeRandomBitSet.h"
#include "Building/BuildingManager.h"
//#include "fuben/FubenDataMgr.h"
#include "Map/Table/MapTableMgr.h"
#include "Trigger/trigger.h"
#include "Npc/Table/NpcTableMgr.h"
#include "datastruct/struct_soldier_food.h"
#include "datastruct/struct_panzer.h"
#include "Map/Table/MapCountry.h"
#include "Palace/Table/PalaceTableMgr.h"
//#include "Market/Table/MarketTableMgr.h"
//#include "Altar/Table/AltarTableMgr.h"
//#include "Dungeon/Table/DungeonTableMgr.h"
#include "BaoQi/Table/BaoQiTableMgr.h"
#include "Activity/Table/ActTableMgr.h"
//#include "Ironsmith/Table/IronsmithTableMgr.h"
#include "datastruct/struct_monster_city.h"
#include "Map/WorldMap.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "Exploit/ExploitMgr.h"
//#include "AutomaticUpdate/AutomaticUpdate.h"
#include "DailyQuest/Table/DailyQuestTableMgr.h"
#include "GrowUpQuest/Table/GrowUpQuestTableMgr.h"
#include "Activity/Personal/PersonalQuestMgr.h"
#include "SignUp/Table/SignUpTableMgr.h"
#include "PassStageActivity/Table/ActivtyStageRewardTableMgr.h"
#include "ActivityStarBox/Table/ActivityStarBoxTableMgr.h"
#include "cross_wall/CrossActivity/CrossActMgr.h"
#include "cross_wall/table/CrossTableMgr.h"
#include "server_game/AutomaticUpdate/AutoUpdateCross.h"

void LoadDBTables()
{
	//游戏基础数据数据库连接信息
	int port                  = sCrossSvr.Config().GetInt("Database.database_port", 3306 ) ;					
	std::string sIP           = sCrossSvr.Config().GetString("Database.database_ip", "127.0.0.1" ) ;			
	std::string sUser         = sCrossSvr.Config().GetString("Database.database_user", "root" ) ;
	std::string sPassword     = sCrossSvr.Config().GetString("Database.database_password", "123456" ) ;
	std::string sDatabaseName = sCrossSvr.Config().GetString("Database.database_name", "slg_db" ) ;

	//初始化数据库连接
	sDBMgr.InitWorldDB( sIP.c_str(), port, sUser.c_str(), sPassword.c_str(), sDatabaseName.c_str() ) ;
	sDBMgr.LoadServerTables(FlagCleanupReload) ;	//加载数据表
	sBaseDefineMgr.Init();


	sPeriodSystem.InitBeginTime("2012-1-1 04:00:00" ) ; // 公共系统重置时间
	sPeriodEvent.InitBeginTime("2012-1-1 00:00:00" ) ;  // 公共活动重置时间
	sPeriodSettle.InitBeginTime("2012-1-1 22:00:00" ) ; // 公共结算时间
	sPeriodSimple.InitBeginTime("2012-1-1 23:10:00" ) ; // 少数系统重置时间时间
	sPeriodSimpleTwentyThree.InitBeginTime("2012-1-1 23:00:00"); //
	sCharacterMgr.Init();				//角色管理系统
	//sGuildHandler.InitGuildProto();

	//sFubenDataMgr.Init();				//副本管理系统
	sMapTableMgr.Init();				//地图管理系统
	sNpcTableMgr.Init();				//npc管理系统
	sCountryTableMgr.Init();			//城市管理系统
	//sAltarTableMgr.Init();				//祭坛管理系统
	sBaoQiTableMgr.Init();				//宝器管理系统
	//sDungeonTableMgr.Init();			//地牢管理系统
	//sMarketTableMgr.Init();				//商店管理系统
	sPalaceTableMgr.Init();				//宫殿管理系统
	sQuestMgr.Init();					//探索管理系统
	sItemMgr.Init() ;					//道具管理系统
	//sLootMgr.Init() ;					//战利品管理系统
	//sSellMgr.Init();					//售卖管理系统
	sPlayerDataMgr.Init();				//玩家数据管理系统
	//sSysEventMgr.Init();				//系统事件管理系统
	//sOperationMgr.Init();				//操作管理系统
	sBuildingManager.Init();			//建筑管理系统
	sTrigger.Init();					//触发器
	sTechnologyTableMgr.Init();			//科技系统
	sActTableMgr.Init();				//行动管理系统
	//sSmithyTableMgr.Init();				//铁匠铺管理系统
	SoldierFoodTable::Init();			//招募系统
	PanzerTable::Init();				//装甲车系统
	PlayerBuffTable::Init();			//玩家增益状态系统
	Build_buffTable::Init();			//建筑物增益状态系统
	Table_MonsterCity::Init();			//蛮族
	//sWorldMap.Init();					//世界地图
	sFixFakeRD;							//修理命中

	//sPvP.Init() ;						//pvp系统
	sExploitMgr.GetInit();					//功勋榜系统
	//sAutoUpdate.Init();				//自动更新系统
	sDailyQuestTableMgr.Init();   //每日任务
	sGrowUpQuestTableMgr.Init();  //成长任务
	sSignUpTableMgr.Init();			//每日签到
	sActivityStageRewardTableMgr.Init();////通关送金
	//sActivityStarBoxMgr.Init();				//异星宝箱
	sPersonalQuestMgr.Init();			//世界任务-个人任务

	sAutoUpdateCross.Init();

	sCrossActMgr.Init();
	sCrossTableMgr.Init();
}

int run_server(int argc, char *argv[])
{
	CrossServer::NewInstance();

	// Init config
	if( !sCrossSvr.InitGameConf( "etc/", "server_cross.conf", "default/server_cross_default.conf" ) )
	{
		printf( "config loading fail.....................!" ) ;
		assert( false ) ;
		return 0 ;
	}

	IoServicePool::NewInstance() ;
	CenterServerClient::NewInstance() ;
	EventLogServerClient::NewInstance();
	LobbyServerClient::NewInstance();



	bool isRunning =true ;

	const ConfigMgr &config =sCrossSvr.Config() ;

	sLogger.AddFileLog( config.GetString("Log.dir", "logs/server_cross_" ).c_str() ) ;


	LoadDBTables() ;
	if( isRunning )
	{

		NLOG( "Start Io service pool........." ) ;
		int io_count =config.GetInt( "Self.io_service_count", 0 ) ;
		sIoPool.Run( io_count <= 0 ? boost::thread::hardware_concurrency() : io_count ) ; 

		// Set center address
		std::string centerIp   =config.GetString( "CenterServer.ip", "127.0.0.1" ) ;
		std::string centerPort =config.GetString( "CenterServer.port", "55800" ) ;
		NLOG( "Setting center server client %s(%s) .", centerIp.c_str(), centerPort.c_str() ) ;
		sCenterClient.SetDstInfo( centerIp, centerPort ) ;


		NLOG( "Starting network service ...." ) ;
		sCrossSvr.SetServerType( PACKET_TYPE_SERVER_CROSS ) ;
		sCrossSvr.SetPackPrintDiffCount( config.GetInt( "Self.packet_status_print_diff_count", 20000 ) ) ;

		std::string listenPort =config.GetString( "Self.port", "55400" ) ;
		NLOG( "Starting network service listen port:%s ....", listenPort.c_str() ) ;
		sCrossSvr.StartListenPort( listenPort ) ;
	
		NLOG( "Starting cross server ...." ) ;
		sCrossSvr.Start() ;
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

	NLOG( "Stopping Lobby server...." ) ;
	sCrossSvr.Stop() ;

	NLOG( "Delete Login Server....." ) ;
	CrossServer::DeleteInstance() ;

//	NLOG( "Delete data manager....." ) ;
//	LobbyDataMgr::DeleteInstance() ;

	NLOG( "Delete single client...." ) ;
	CenterServerClient::DeleteInstance() ;
	EventLogServerClient::DeleteInstance();
	LobbyServerClient::DeleteInstance();

	NLOG( "Stopping Io service pool...." ) ;
	sIoPool.Stop() ;

	NLOG( "Delete IoServicePool....." ) ;
	IoServicePool::DeleteInstance() ;
	
	return 0;
}
