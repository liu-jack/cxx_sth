#ifndef CENTER_SERVER_H__
#define CENTER_SERVER_H__

#include <set>

#include <boost/smart_ptr.hpp>

#include "ConfigMgr.h"

#include "Singleton.h"
#include "Thread.h"

#include "net_framework/Server.h"
#include "lock_free/LockFreeQueue.h"
#include "net_framework/NetPackDispatcher.h"


class NetPack ;
class Session ;
class WebServerSession ;

class CenterServer : public Singleton<CenterServer>, public Server
{
private:
	friend class Singleton<CenterServer>;

private:
	CenterServer( void );

public:
	virtual ~CenterServer( void );

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

private:
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
		BroadcastByServerIdSessionMap( m_gameSessions, packet ) ;
	}

	void BroadcastMsgToGameServer( const int opCode, std::vector<int>& retVec, const ::google::protobuf::Message &msg, SessionPairIdSet& sendResult);

	SessionByServerIdMap& GetGameSessions( void ) {   return m_gameSessions ;   }

private:
	SessionByServerIdMap m_gameSessions ;
	SessionPairIdSet     m_gameSessionIds;
	SessionAddressSet    m_gameSessionAddresses ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Loign server relative
public:
	void LoginServerUnRegister( Session *pSession ) ;
	void CrossServerUnRegister(Session *pSession);
	void LobbyServerUnRegister(Session *pSession);
	ServerRegisterResult LoginServerRegister( Session *pSession ) ;
	ServerRegisterResult CrossServerRegister( Session *pSession );
	ServerRegisterResult LobbyServerRegister( Session *pSession );
	SessionAddressSet& GetLoginAddressTable( void ) {   return m_loginSessionAddresses ;   }
	SessionAddressSet& GetCrossAddressTable( void ) {   return m_crossSessionAddresses ;   }

	void BroadcastToLoginServer( NetPack &packet )
	{
		BroadcastBySessionMap( m_loginSessions, packet ) ;
	}

	void BroadcastToLoginServer(const int opCode, const ::google::protobuf::Message &msg)
	{
		BroadcastMsgBySessionMap( m_loginSessions, opCode,msg) ;
	}
	void CheckNoticeNewLoginServerToGame( void ) ;
	void CheckNoticeNewCrossServerToGame( void ) ;

private:
	SessionMap        m_loginSessions ;
	SessionVec        m_newLoginSessions ;
	SessionAddressSet m_loginSessionAddresses ;
	SessionMap        m_crossSessions ;
	SessionVec        m_newCrossSessions ;
	SessionAddressSet m_crossSessionAddresses ;
	SessionMap        m_lobbySessions ;
	SessionAddressSet m_lobbySessionAddresses ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Web server relative
public:
	WebServerSession* GetWebServerSession( SessionId sesionId ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Event log server relative
public:
	void EventLogServerUnRegister( Session *pSession ) ;
	ServerRegisterResult EventLogServerRegister( Session *pSession ) ;

	void CheckNoticeNewEventLogServerToGameAndLogin( void ) ;

	Session* GetEventLogSession( void ) {   return m_pEventLogSession  ;   }

private:
	Session          *m_pEventLogSession ;
	Session          *m_pNewEventLogSession ;

};


#define sCenterSvr (CenterServer::Instance())




#endif