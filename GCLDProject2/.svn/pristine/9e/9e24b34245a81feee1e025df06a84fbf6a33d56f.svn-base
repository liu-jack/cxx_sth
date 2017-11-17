#pragma once

#include <boost/smart_ptr.hpp>

#include "ConfigMgr.h"

#include "Singleton.h"
#include "Thread.h"

#include "net_framework/Server.h"
#include "lock_free/LockFreeQueue.h"
#include "net_framework/NetPackDispatcher.h"
//#include "session/GameSessionVersionInfo.h"

class NetPack ;
class Session ;
class GameServerSession ;



class CrossServer : public Singleton<CrossServer>, public Server
{
private:
	friend class Singleton<CrossServer>;

private:
	CrossServer( void );

public:
	virtual ~CrossServer( void );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Session
public:
	virtual Session* CreateSesion( int type ) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void OnBeforeStart( void ) ;
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Config
public:
	bool InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;

	// typedef
public:
	enum ServerRegisterResult
	{
		SERVER_REGISTER_OK           =0,
		SERVER_REGISTER_FAIL_ID      =1,
		SERVER_REGISTER_FAIL_ADDRESS =2,
	} ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	int GetServerId( void ){   return m_serverId;   }
	int GetRegionId( void ) {   return m_regionId ;   }
	void SetRegionId( int id ) {   m_regionId =id ;   }
private:
	int m_serverId ;
	int m_regionId ;
public:
	void GameServerUnRegister( Session *pSession ) ;
	ServerRegisterResult GameServerRegister( Session *pSession ) ;

	PairIdSessionMap& GetGameSessionMap( void ) {   return m_gameSessions ;   }

	GameServerSession* GetGameServerSessoin( const SessionPairId &id )
	{
		PairIdSessionMapItr itr = m_gameSessions.find( id ) ;
		return ( itr != m_gameSessions.end() ? reinterpret_cast<GameServerSession*>( itr->second ) : NULL ) ;
	}

	SessionPairId GetGamePairId( uint64 index ) ;
	uint64 GetGameIndex( const SessionPairId &pairId ) ;
	bool AddOneEventLogServer( const std::string &ip, const std::string &port );
private:
	PairIdSessionMap   m_gameSessions;
	SessionAddressSet  m_gameSessionAddresses ;

public:

};

#define sCrossSvr (CrossServer::Instance())
