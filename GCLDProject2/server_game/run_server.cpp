#include <assert.h>

#include <iostream>

#include "DatabaseMgr.h"
#include "GameServer.h"
#include "Logger.h"
#include "memory_buffer/NetPack.h"
#include "server_client/CenterServerClient.h"
#include "server_client/DbProxyServerClient.h"
#include "server_client/EventLogServerClient.h"
#include "server_client/LobbyServerClient.h"

#include "net_framework/IoServicePool.h"

#include "GameServerHttpClients.h"

#include "character/CharacterManager.h"
#include "Loot/LootManager.h"
#include "item/ItemManager.h"
#include "quest/QuestManager.h"
#include "utility/TimePeriod.h"
#include "sell/SellManager.h"
#include "SysEvent/SysEventMgr.h"
#include "BaseDefineMgr.h"
#include "object/PlayerDataManager.h"
#include "Loot/FixFakeRandom/FixFakeRandomBitSet.h"
#include "Building/BuildingManager.h"
#include "fuben/FubenDataMgr.h"
#include "Map/Table/MapTableMgr.h"
#include "Trigger/trigger.h"
#include "Npc/Table/NpcTableMgr.h"
#include "datastruct/struct_soldier_food.h"
#include "datastruct/struct_panzer.h"
#include "Map/Table/MapCountry.h"
#include "Palace/Table/PalaceTableMgr.h"
#include "Market/Table/MarketTableMgr.h"
#include "Altar/Table/AltarTableMgr.h"
#include "Dungeon/Table/DungeonTableMgr.h"
#include "BaoQi/Table/BaoQiTableMgr.h"
#include "Activity/Table/ActTableMgr.h"
#include "Ironsmith/Table/IronsmithTableMgr.h"
#include "datastruct/struct_monster_city.h"
#include "Map/WorldMap.h"
#include "Technology/Table/TechnologyTableMgr.h"
//#include "AutomaticUpdate/AutomaticUpdate.h"
#include "AutomaticUpdate/AutoUpdateGame.h"
#include "Combat/SkillDataMgr.h"
#include "Exploit/ExploitMgr.h"
#include "Activity/ActMgr.h"
#include "DailyQuest/Table/DailyQuestTableMgr.h"
#include "GrowUpQuest/Table/GrowUpQuestTableMgr.h"
#include "Activity/Personal/PersonalQuestMgr.h"
#include "SignUp/Table/SignUpTableMgr.h"
#include "PassStageActivity/Table/ActivtyStageRewardTableMgr.h"
#include "ActivityStarBox/Table/ActivityStarBoxTableMgr.h"
#include "ActivityXiLian/Table/ActivityXiLianTableMgr.h"
#include "ActivityControl/ActivityMgr.h"
#include "GovernmentAffairs/table/GovAffairsTableMgr.h"
#include "ActivityCelebrity/Table/ActivityCelebrityTableMgr.h"
#include "ActivityHeroReward/Table/ActivityHeroRewardTableMgr.h"
#include "ActivityMoneyToGold/Table/ActivityMoneyToGoldTableMgr.h"
#include "WorldFightAchievement/Table/WorldFightAchievementMgr.h"
#include "Reinforcement/ReinforceTableMgr.h"
#include "TeachMap/TeachMap.h"
#include "ContinueOccupyCity/Table/ContinueOccupyCityMgr.h"
#include "ActivityRank/table/ActivityRankTableMgr.h"

void LoadDBTables()
{
	//游戏基础数据数据库连接信息
	int port                  = sGameSvr.Config().GetInt("Database.database_port", 3306 ) ;					
	std::string sIP           = sGameSvr.Config().GetString("Database.database_ip", "127.0.0.1" ) ;			
	std::string sUser         = sGameSvr.Config().GetString("Database.database_user", "root" ) ;
	std::string sPassword     = sGameSvr.Config().GetString("Database.database_password", "123456" ) ;
	std::string sDatabaseName = sGameSvr.Config().GetString("Database.database_name", "par_db_new" ) ;

	//初始化数据库连接
	sDBMgr.InitWorldDB( sIP.c_str(), port, sUser.c_str(), sPassword.c_str(), sDatabaseName.c_str() ) ;
	sDBMgr.LoadServerTables(FlagCleanupReload) ;	//加载数据表
    sBaseDefineMgr.Init();
    


    sPeriodSystem.InitBeginTime("2012-1-1 04:00:00" ) ; // 公共系统重置时间
    sPeriodEvent.InitBeginTime("2012-1-1 00:00:00" ) ;  // 公共活动重置时间
	sPeriodSettle.InitBeginTime("2012-1-1 22:00:00" ) ; // 公共结算时间
	sPeriodSimple.InitBeginTime("2012-1-1 23:10:00" ) ; // 少数系统重置时间时间
	sPeriodSimpleTwentyThree.InitBeginTime("2012-1-1 23:00:00");

	sCharacterMgr.Init();				//角色管理系统
	//sGuildHandler.InitGuildProto();

    sFubenDataMgr.Init();				//副本管理系统
    sMapTableMgr.Init();				//地图管理系统
    sNpcTableMgr.Init();				//npc管理系统
    sCountryTableMgr.Init();			//城市管理系统
    sAltarTableMgr.Init();				//祭坛管理系统
    sBaoQiTableMgr.Init();				//宝器管理系统
    sDungeonTableMgr.Init();			//地牢管理系统
    sMarketTableMgr.Init();				//商店管理系统
    sPalaceTableMgr.Init();				//宫殿管理系统
    sQuestMgr.Init();					//探索管理系统
    sItemMgr.Init() ;					//道具管理系统
	sLootMgr.Init() ;					//战利品管理系统
    sSellMgr.Init();					//售卖管理系统
    sPlayerDataMgr.Init();				//玩家数据管理系统
	sSysEventMgr.Init();				//系统事件管理系统
	sBuildingManager.Init();			//建筑管理系统
    sTrigger.Init();					//触发器
	sFunctionOpen.Init();				//功能开启
    sTechnologyTableMgr.Init();			//科技系统
    sActTableMgr.Init();				//行动管理系统
	sSmithyTableMgr.Init();				//铁匠铺管理系统
    SoldierFoodTable::Init();			//招募系统
	PanzerTable::Init();				//装甲车系统
	PlayerBuffTable::Init();			//玩家增益状态系统
	Build_buffTable::Init();			//建筑物增益状态系统
    Table_MonsterCity::Init();			//蛮族
	sWorldMap.Init();					//世界地图
    sFixFakeRD;							//修理命中
	

	sSkillDataMgr.Init();				//技能系统
	sActMgr.Init();
	sExploitMgr.GetInit();					//排行榜系统
	sDailyQuestTableMgr.Init();				//每日任务
	sGrowUpQuestTableMgr.Init();			//成长任务
	sSignUpTableMgr.Init();					//每日签到
	sActivityStageRewardTableMgr.Init();	//通关送金
	sActivityStarBoxMgr.Init();				//异星宝箱
	sActivityXilianMgr.Init();
	sPersonalQuestMgr.Init();
	//sAutoUpdate.Init();				//自动更新系统
	sActivityMgr.Init();
	sGovAffairsTableMgr.Init();			///政务
	sActivityCelebrityMgr.Init();		//结交名仕
	sActivityGeneralRewardMgr.Init();  //武将犒赏
	sActivityRechargeRebateMgr.Init();  //冲值送金
	sWorldFightAchievementMgr.Init();  //国战成就
	sTeachMapTableMgr.Init();			//世界教学地图
	sSeigeArmyForceMgr.Init();			//攻城部队
	sContinueOccupyCityTableMgr.Init();	//连城 
	sActivityRankTableMgr.Init();//战场活动
	sAutoUpdateGame.Init();
}

int run_server(int argc, char *argv[])
{
	sOS.SRand() ;
	GameServer::NewInstance() ;				//游戏服务器

	// Init config
	if( !sGameSvr.InitGameConf( "etc/", "server_game.conf", "default/server_game_default.conf" ) )
	{
		printf( "config loading fail.....................!" ) ;
		assert( false ) ;
		return 0 ;
	}

	//启动各个单例
	IoServicePool::NewInstance() ;			//线程池
	GameServerHttpClients::NewInstance() ;	
	CenterServerClient::NewInstance() ;
	DbProxyServerClient::NewInstance() ;
	EventLogServerClient::NewInstance() ;
	LobbyServerClient::NewInstance();

	const ConfigMgr &config =sGameSvr.Config() ;

	//建立日志文件
	sLogger.AddFileLog( config.GetString("Log.dir", "logs/server_game_" ).c_str() ) ;
	
	//加载数据表
	LoadDBTables() ;

	NLOG( "Start Io service pool........." ) ;
	int io_count =config.GetInt( "Self.io_service_count", 0 ) ;
	sIoPool.Run( io_count <= 0 ? boost::thread::hardware_concurrency() : io_count ) ; //硬件线程上下文的数量

	// Set center address
	//中心服务器端的配置
	std::string centerIp   =config.GetString( "CenterServer.ip", "127.0.0.1" ) ;
	std::string centerPort =config.GetString( "CenterServer.port", "55800" ) ;
	NLOG( "Setting center server client %s(%s) .", centerIp.c_str(), centerPort.c_str() ) ;
	sCenterClient.SetDstInfo( centerIp, centerPort ) ;


	NLOG( "Starting network service ...." ) ;
	sGameSvr.SetServerType( PACKET_TYPE_SERVER_GAME ) ;		//设置服务器类型为游戏服务器
	sGameSvr.SetPackPrintDiffCount( config.GetInt( "Self.packet_status_print_diff_count", 20000 ) ) ;	//数据包统计间隔数量

	//设置监听端口
	std::string listenPort =config.GetString( "Self.port", "55500" ) ;
	NLOG( "Starting network service listen port:%s ....", listenPort.c_str() ) ;
	sGameSvr.StartListenPort( listenPort ) ;

	NLOG( "Starting game server ...." ) ;
	sGameSvr.Start() ;		//开启线程

#ifdef _WIN32
	char input[512];
#endif
	bool isRunning =true ;
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

	NLOG( "Stopping game server...." ) ;
	sGameSvr.Stop() ;

	NLOG( "Delete GameServerHttpClients....." ) ;
	GameServerHttpClients::DeleteInstance() ;

	NLOG( "Delete sGameSvr...." ) ;
	GameServer::DeleteInstance() ;

	NLOG( "Delete single client...." ) ;
	CenterServerClient::DeleteInstance() ;
	DbProxyServerClient::DeleteInstance() ;
	EventLogServerClient::DeleteInstance() ;
	LobbyServerClient::DeleteInstance();

	NLOG( "Stopping Io service pool...." ) ;
	sIoPool.Stop() ;

	NLOG( "Delete IoServicePool....." ) ;
	IoServicePool::DeleteInstance() ;

	return 0;
}
