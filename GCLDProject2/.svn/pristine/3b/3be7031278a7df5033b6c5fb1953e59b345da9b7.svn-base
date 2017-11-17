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
class CrossServerSession;
class EventLogServer : public Singleton<EventLogServer>, public Server
{
private:
	friend class Singleton<EventLogServer>;

private:
	EventLogServer( void );

public:
	virtual ~EventLogServer( void );

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
	int GetRegionId( void ) {   return m_regionId ;   }
	void SetRegionId( int id ) {   m_regionId =id ;   }

	bool InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;

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

	PairIdSessionMap& GetGameSessionMap( void ) {   return m_gameSessions ;   }

	GameServerSession* GetGameServerSessoin( const SessionPairId &id )
	{
		PairIdSessionMapItr itr = m_gameSessions.find( id ) ;
		return ( itr != m_gameSessions.end() ? reinterpret_cast<GameServerSession*>( itr->second ) : NULL ) ;
	}

	void CrossServerUnRegister( Session *pSession ) ;
	ServerRegisterResult CrossServerRegister( Session *pSession ) ;

	PairIdSessionMap& GetCrossSessionMap( void ) {   return m_crossSessions ;   }

	CrossServerSession* GetCrossServerSessoin( const SessionPairId &id )
	{
		PairIdSessionMapItr itr = m_crossSessions.find( id ) ;
		return ( itr != m_crossSessions.end() ? reinterpret_cast<CrossServerSession*>( itr->second ) : NULL ) ;
	}

	void LobbyServerUnRegister( Session *pSession ) ;
	ServerRegisterResult LobbyServerRegister( Session *pSession ) ;
private:
	PairIdSessionMap  m_gameSessions;
	SessionAddressSet m_gameSessionAddresses ;

	PairIdSessionMap  m_crossSessions;
	SessionAddressSet m_crossSessionAddresses ;

	PairIdSessionMap  m_lobbySessions;
	SessionAddressSet m_lobbySessionAddresses ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Loign server relative
public:
	void LoginServerUnRegister( Session *pSession ) ;
	ServerRegisterResult LoginServerRegister( Session *pSession ) ;

	SessionAddressSet& GetLoginAddressTable( void ) {   return m_loginSessionAddresses ;   }

	void BroadcastToLoginServer( NetPack &packet )
	{
		BroadcastBySessionMap( m_loginSessions, packet ) ;
	}

	void CheckNoticeNewLoginServerToGame( void ) ;

private:
	SessionMap        m_loginSessions ;
	SessionVec        m_newLoginSessions ;
	SessionAddressSet m_loginSessionAddresses ;



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:

};

#define sEventLogSvr (EventLogServer::Instance())
