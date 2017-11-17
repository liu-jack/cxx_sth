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
class CrossServerSession;


class LobbyServer : public Singleton<LobbyServer>, public Server
{
private:
	friend class Singleton<LobbyServer>;

private:
	LobbyServer( void );

public:
	virtual ~LobbyServer( void );

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
	uint64 GetServerIndex( const SessionPairId &pairId );
	SessionPairId GetServerPairId( uint64 index );

	void GameServerUnRegister( Session *pSession ) ;
	ServerRegisterResult GameServerRegister( Session *pSession ) ;

	PairIdSessionMap& GetGameSessionMap( void ) {   return m_gameSessions ;   }

	GameServerSession* GetGameServerSessoin( const SessionPairId &id )
	{
		PairIdSessionMapItr itr = m_gameSessions.find( id ) ;
		return ( itr != m_gameSessions.end() ? reinterpret_cast<GameServerSession*>( itr->second ) : NULL ) ;
	}
	
	uint32 GetGameServerSessionSize() { return m_gameSessions.size(); }


	void CrossServerUnRegister( Session *pSession ) ;
	ServerRegisterResult CrossServerRegister( Session *pSession ) ;

	PairIdSessionMap& GetCrossSessionMap( void ) {   return m_crossSessions ;   }

	CrossServerSession* GetCrossServerSessoin( const SessionPairId &id )
	{
		PairIdSessionMapItr itr = m_crossSessions.find( id ) ;
		return ( itr != m_crossSessions.end() ? reinterpret_cast<CrossServerSession*>( itr->second ) : NULL ) ;
	}

public:
	bool AddOneEventLogServer( const std::string &ip, const std::string &port ) ;


private:
	PairIdSessionMap   m_gameSessions;
	SessionAddressSet  m_gameSessionAddresses ;

	PairIdSessionMap   m_crossSessions;
	SessionAddressSet  m_crossSessionAddresses ;

public:

};

#define sLobbySvr (LobbyServer::Instance())
