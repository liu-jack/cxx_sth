
#ifndef GAME_SERVER_H__
#define GAME_SERVER_H__

#include <map>
#include <boost/smart_ptr.hpp>
#include "ConfigMgr.h"

#include "Singleton.h"
#include "Thread.h"

#include "net_framework/Server.h"
#include "lock_free/LockFreeQueue.h"
#include "net_framework/NetPackDispatcher.h"
//#include "def/TypeDef.h"
#include "def/ObjectDefines.h"

#include "GlobalKeyValue.h"
#include "pay/PayItemsInfo.h"
#include "CURLRequests.h"


class NetPack ;
class Session ;
class PlayerSession ;
class ServerClient ;
class Player;
class GameServerLogicalTaker;

typedef boost::shared_ptr<Player> PlayerPtr ;
typedef boost::shared_ptr<ServerClient>  ServerClientPtr ;
//会话关键信息
struct SessionKeyInfo
{
public:
	SessionKeyInfo( void )
	{
		m_canUse            =true ;
		m_sessionId         =0 ;
		m_accountId         =0 ;
		m_usefullLife       =0 ;
		m_preQuickLoginTime =0 ;
	}

public:
	bool        m_canUse ;
	SessionId   m_sessionId ;
	uint64      m_accountId ;
	uint64      m_usefullLife ;
	uint64      m_preQuickLoginTime ;
	std::string m_key ;
} ;

struct ServerValue
{
public:
	ServerValue( void )
	{
		::memset( this, 0, sizeof( ServerValue ) ) ;
	}

public:
	uint32 index ;
	uint32 type ;
	int64  i64Value ;
} ;








class GameServer : public Singleton<GameServer>, public Server
{
private:
	friend class Singleton<GameServer>;

private:
	GameServer( void );

public:
	virtual ~GameServer( void );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Session
public:
	void OnBeforeStart( void );
	void OnAfterStop( void );
	virtual Session* CreateSesion( int type ) ;
	PlayerSession* GetPlayerSession( SessionId id ) ;

	void OnSessionClose( PlayerSession &session ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Config
public:
	int GetServerId( void ){   return m_serverId;   }

	int GetRegionId( void ) {   return m_regionId ;   }
	void SetRegionId( int id ) {   m_regionId =id ; }

	bool InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;
	uint64 GetGuidById(OBJECT_TYPE type,uint32 id);

private:
	int m_serverId ;
	int m_regionId ;
	int m_keyUsefullLifeTime ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Other
public:
    typedef boost::shared_ptr<GameServerLogicalTaker> GSLogicTakerPtr;
    GSLogicTakerPtr& GetGSLogicalTaker( void ) {   return m_pGSLogicalTaker ;   }

private:
    GSLogicTakerPtr m_pGSLogicalTaker ;


public:
	bool IsPlayerOnline(uint64 playerId);
	int GetOnlinePlayerCount();
	Player* GetPlayerPtr(uint64 playerId);
	Player* GetPlayerPtrByName( const std::string &name ) ;
	bool GetLogicTheardUpdateTimeInfo(std::string& info);



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Login Clients
public:
	bool AddOneLoginServer( const std::string &ip, const std::string &port ) ;
	bool AddOneCrossServer( const std::string &ip, const std::string &port );
	bool AddOneEventLogServer( const std::string &ip, const std::string &port ) ;
	ServerClientPtr GetCrossServerClient(const std::string &ip, const std::string &port);
	void CloseAllCrossServerClient();
	void BroadcastPacketToLoginServer(NetPack& refPacket);
public:
	typedef std::map< SessionAddress, ServerClientPtr > ServerClientMap ;

private:
	ServerClientMap m_loginClients ;
	ServerClientMap m_crossClients ;



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Client Key
public:
	typedef uint64                                AccountId ;
	typedef std::map< AccountId, SessionKeyInfo > KeyInfoTable ;
	typedef KeyInfoTable::iterator                KeyInfoTableItr ;

public:
	bool CheckForLogin( SessionKeyInfo *pSKeyInfo, PlayerSession &curSession ) ;

	SessionKeyInfo* GetSessionKeyInfo( AccountId id ) ;
	SessionKeyInfo* AddSessionKeyInfo( AccountId aId, SessionId sId, const std::string &key ) ;

private:
	KeyInfoTable m_keyTables ;



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Broadcas
public:
	void BroadcastMsgToPlayer(uint16 opcode, const ::google::protobuf::Message& msg);
	void BroadcastMsgToCountryPlayer(uint32 countryId, uint16 opcode, const ::google::protobuf::Message& msg);
	bool SendMsgToPlayer(uint64 playerId, NetPack &packet);
	bool SendMsgToPlayer(uint64 playerId, uint16 opcode, const ::google::protobuf::Message& msg);
	void PostPlayerMsg(uint64 playerId, uint16 opcode, const ::google::protobuf::Message& msg);
	void PostPlayerNetPack(uint64 playerId, NetPack *packet);
	void GetPlayerInfo(uint64 playerId,uint64 youId);
	void BroadcastServerMsg(const int opCode,const ::google::protobuf::Message &msg );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Gobal values
public:
	typedef std::map< uint32, ServerValue > ServerValueMap ;
	typedef ServerValueMap::iterator        ServerValueMapItr ;

public:
	void InitServerGobalValues( NetPack &packet ) ;

	bool NewServerValue( const ServerValue &value ) ;
	void SaveServerValue( ServerValue &value ) const ;
	ServerValue* GetServerValue( uint32 index ) ;
	ServerValue& GetValueCreatIfNot( uint32 index ) ;


private:
	ServerValueMap m_gobalValues ;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
public:
    PayItemsInfo        m_payItemsInfo;
    CURLRequests        m_curlRequest;
    GlobalKeyValue      m_globalKeyValue;

    
public:
	void CheckCurlResult( void ) ;


};

#define sGameSvr (GameServer::Instance())

#endif
