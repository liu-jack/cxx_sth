
#ifndef DB_PROXY_SERVER_CLIENT_H__
#define DB_PROXY_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "Singleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"


class Player ;
class NetPack ;
#ifdef _MMO_SERVER_
class PlayerSession ;
#endif

typedef boost::shared_ptr<Player> PlayerPtr ;

class DbProxyServerClient : public Singleton<DbProxyServerClient>, public ServerClient
{
private:
	friend class Singleton<DbProxyServerClient>;

public:
	typedef void (DbProxyServerClient::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );

private:
	static HandlerArray s_handlers ;

private:
	DbProxyServerClient ( void ) ;

public:
	virtual ~DbProxyServerClient( void ) ;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// virtual 
private:
	virtual void ClientConnect( void ) ;
	virtual void ClientDisconnect( void ) ;
	virtual void ClientUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual bool HandlePacket( NetPack &packet ) ;

public:
	void AddLostdataPlayer( const PlayerPtr &ptr ) {    m_lostDataPlayers.push_back( ptr ) ;    }
	void SendPlayerOffLine( uint64 playerId ) ;
	void SendCreatePlayerData( Player &player ) ;

private:
	typedef std::vector<PlayerPtr> LostDataPlayersTable ;
	typedef LostDataPlayersTable::iterator LostDataPlayersTableItr ;

	LostDataPlayersTable m_lostDataPlayers ;

public:
	virtual void SendPing( void ) ;
	void SendRegisterInfo( void ) ;
    bool SendPlayerPack( Player &player, const int opCode, const ::google::protobuf::Message &msg );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
private:
	void DirectSendToPlayerBySessionId( uint64 sessionId, uint16 newOpcoe, NetPack &packet ) ;
#ifdef _MMO_SERVER_
	PlayerSession* GetPlayerTakerSession( uint64 sessionId ) ;
public:
	virtual void HandlePong( NetPack &packet ) ;
	void HandleRegisterRet( NetPack &packet ) ;
	void HandleTakerPlayerList( NetPack &packet ) ;
	void HandleTakerCreatePlayer( NetPack &packet ) ;
	void HandleTakerSelectPlayer( NetPack &packet ) ;
	void HandleMailSendCheck( NetPack &packet );
	void HandleMailUpdate( NetPack &packet );
	void HandleSocialLoadFriendInfo( NetPack &packet );
	void HandleFriendOperateRes( NetPack &packet );
	void HandleBroadCastOfflineRespon( NetPack &packet );
	void HandleFriendSearchRespon( NetPack &packet );
	void HandleGuildCreateRespon( NetPack &packet );
	void HandleReceivePlayerAppendData( NetPack &packet );
    void HandleReceiveCountryALLData(NetPack &packet);
    void HandleReceiveCountryLevelUpTaskRankData(NetPack &packet);
    void HandleReceivePalaceALLData(NetPack &packet);
	void HandlePlayerCacheDataSync(NetPack &packet);
	void HandlePlayerCacheRemoveSync(NetPack &packet);
	void HandleGuildPlayerSearchRespon(NetPack &packet);
	void HandleWebGetPlayerListRet( NetPack &packet ) ;
	void HandleGetPlayerInfoResPon( NetPack &packet );
	void HandleGuilds( NetPack &packet );
	void HandlePvPCreateVirtualPlayer( NetPack &packet ) ;
	void HandlePvPEntryArena( NetPack &packet ) ;
	void HandlePvPGetPlayerInfo( NetPack &packet ) ;
	void HandlePvPFightResultInfo( NetPack &packet ) ;
	void HandlePvPFightRecordInfo( NetPack &packet ) ;
	void HandleServerGobalValues( NetPack &packet ) ;
	void HandlePvPGetRewardRankListRet( NetPack &packet ) ;
	void HandlePvPGetRankingList( NetPack &packet ) ;
	void HandlePvPMarkFightRet( NetPack &packet ) ;
    void HandlePlayerChangeName( NetPack &packet ) ;
    void HandleWebPayRet( NetPack &packet ) ;
	void HandlePlayerForbidenRet( NetPack &packet ) ;
	///add zhoulunhao
 	void HandlePlayerSetNameAndHeadId(NetPack& packet);
	void HandlePlayerUpdateActivityInfo(NetPack& packet);

	void HandlePlayerLoadActivityInfo(NetPack& packet);
	void HandlePlayerSelectCountry(NetPack& packet);
	void HandlePlayerAllSyncFromDB(NetPack& packet);
	void HandlePlayerActivityOccupyCity(NetPack& packet);
#endif
};

#define sDbProxyClient (DbProxyServerClient::Instance())

#endif
