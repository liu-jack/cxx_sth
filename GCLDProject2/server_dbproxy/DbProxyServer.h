#pragma once

#include <boost/smart_ptr.hpp>

#include "ConfigMgr.h"

#include "Singleton.h"
#include "Thread.h"

#include "net_framework/Server.h"
#include "lock_free/LockFreeQueue.h"
#include "net_framework/NetPackDispatcher.h"

class NetPack ;
class Session ;
class GameServerSession ;


class DbProxyServer : public Singleton<DbProxyServer>, public Server
{
private:
	friend class Singleton<DbProxyServer>;

private:
	DbProxyServer( void );

public:
	virtual ~DbProxyServer( void );

	virtual void OnBeforeStart( void ) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Session
public:
	virtual Session* CreateSesion( int type ) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Config
public:
	int GetRegionId( void ) {   return m_regionId ;   }
	void SetRegionId( int id ) {   m_regionId =id ;   }


	static uint32 GetMaxCachePlayerBaseDataCount( void ) {   return s_maxCachePlayerBaseDataCount ;   }
	static void SetMaxCachePlayerBaseDataCount( uint32 maxCount ) {   s_maxCachePlayerBaseDataCount =maxCount ;   }

	static uint32 GetMaxCachePlayerDays( void ) {   return s_maxCachePlayerDays ;   }
	static void SetMaxCachePlayerDays( uint32 maxDays ) {   s_maxCachePlayerDays =maxDays ;   }

	static uint32 GetMaxCachePlayerCount( void ) {   return s_maxCachePlayerCount ;   }
	static void SetMaxCachePlayerCount( uint32 maxCount ) {   s_maxCachePlayerCount =maxCount ;   }

	static time_t GetMaxCacheOfflinePlayerSeconds( void ) {   return s_maxCacheOfflinePlayerSeconds ;   }
	static void SetMaxCacheOfflinePlayerSeconds( time_t maxSeconds ) {   s_maxCacheOfflinePlayerSeconds =maxSeconds ;   }

	static time_t GetPlayerDataSaveDiffMS( void ) {   return s_playerDataSaveDiffMS ;   }
	static void SetPlayerDataSaveDiffMS( time_t ms ) {   s_playerDataSaveDiffMS =ms ;   }


	bool InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;
private:
	static uint32       s_maxCachePlayerBaseDataCount ;
	static uint32       s_maxCachePlayerDays ;
	static uint32       s_maxCachePlayerCount ;
	static time_t       s_maxCacheOfflinePlayerSeconds ;
	static time_t       s_playerDataSaveDiffMS ;

public:
	int m_regionId ;

	// typedef
public:
	enum ServerRegisterResult
	{
		SERVER_REGISTER_OK           =0,
		SERVER_REGISTER_FAIL_ID      =1,
		SERVER_REGISTER_FAIL_ADDRESS =2,
	} ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Game server relative
public:
	void GameServerUnRegister( Session *pSession ) ;
	ServerRegisterResult GameServerRegister( Session *pSession ) ;

	void BroadcastToGameServer( NetPack &packet )
	{
		BroadcastBySessionMap( m_gameSessions, packet ) ;
	}

private:
	SessionMap        m_gameSessions ;
	SessionPairIdSet  m_gameSessionIds;
	//SessionAddressSet m_gameSessionAddresses ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:

};

#define sDBServer (DbProxyServer::Instance())
