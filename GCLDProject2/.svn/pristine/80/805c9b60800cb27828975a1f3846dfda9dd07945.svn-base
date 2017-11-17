#pragma once

#include <boost/smart_ptr.hpp>

#include "ConfigMgr.h"

#include "Singleton.h"
#include "Thread.h"

#include "net_framework/Server.h"
#include "lock_free/LockFreeQueue.h"
#include "net_framework/NetPackDispatcher.h"
#include "session/GameSessionVersionInfo.h"

class NetPack ;
class Session ;
class GameServerSession ;

struct ConfigServerInfo
{
public:
	ConfigServerInfo( void );

	void UpdateState( void ) ;

	bool IsMaintainState();
	bool IsCloseChannel(int channel);

public:
	uint32             m_region_id ;
	uint32             m_group_id ;
	uint32             m_server_id ;
	uint32			   m_to_server_id ;
	uint32             m_player_limit ;
	uint32             m_delay_limit ;
	uint32             m_state ;
	uint32             m_is_new ;
	uint32             m_is_default ;
	uint32             m_test_state ;
	std::set< int>     m_set_close_channel;
	GameServerSession *m_pGameSession ;
} ;

class LoginServer : public Singleton<LoginServer>, public Server
{
private:
	friend class Singleton<LoginServer>;

private:
	LoginServer( void );

public:
	virtual ~LoginServer( void );

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

	bool IsOpenSdkCheck( void ) const {   return m_isOpenSdkCheck ;   }


public:
	bool        m_isOpenSdkCheck ;
	int         m_regionId ;

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
	typedef std::map< SessionPairId, ConfigServerInfo > ConfigServerMap ;

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

	void GetConfigServerList( void ) ;
	void GetChannelVersionInfo( void );
	void GetVersionChannel( void );
	void GetTesterAccountIds( void );
	void InitConfiServerList( const std::string &data ) ;

	void InitVersionChannel( const std::string &data );
	void InitTesterAccountIds( const std::string &data );
	SessionPairId&   GetCurDefauteServer(void) { return m_defautServer; }
	ConfigServerMap& GetConfigServerTable( void ) {   return m_configServerMap ;   }

	ConfigServerInfo& GetOrCreateConfigServerInfo( const SessionPairId &id ) {   return m_configServerMap[id] ;   }
	bool IsInComfigMap(const SessionPairId &id);
	bool IsCloseChannel(const SessionPairId &id, int channelId);
	bool IsTesterAccountId(uint64 accountId);
	bool IsTesterServer(const SessionPairId &id);
	bool IfIsCombinServerGetOther(SessionPairId &id);

	SessionPairIdSet& GetCombinServerSet( const SessionPairId &id ) {   return m_combinServerMap[id] ; }

	const ClientVersion* GetVersions( uint32 channelId, uint32 platformId ) ;

public:
	const VersionInfoTable& GetVersonInfoTable( void ) {   return m_versonTale ;   }

	bool CanConnectByThisVersion( uint32 channelId, uint32 platformId, uint32 versionId) ;

private:
	VersionInfoTable m_versonTale ;

private:
	PairIdSessionMap   m_gameSessions;
	SessionAddressSet  m_gameSessionAddresses ;
	ConfigServerMap    m_configServerMap ;
	std::set<uint64>   m_testerAccountIds;
	std::map<SessionPairId,SessionPairIdSet> m_combinServerMap;
	SessionPairId m_defautServer;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Event log server relative
public:
	bool AddOneEventLogServer( const std::string &ip, const std::string &port ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:

};

#define sLoginSvr (LoginServer::Instance())
