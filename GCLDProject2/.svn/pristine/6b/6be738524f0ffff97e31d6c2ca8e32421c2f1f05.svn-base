
#ifndef CENTER_SERVER_SESSION_H__
#define CENTER_SERVER_SESSION_H__

#include <boost/smart_ptr.hpp>

#include "../../net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"


class Player ;
class GameServerLogicalTaker ;

typedef boost::shared_ptr<Player> PlayerPtr ;

class PlayerSession : public Session
{
	DECLARE_OBJ_POOL(PlayerSession)

public:
	typedef void (PlayerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	PlayerSession( void ) ;
	virtual ~PlayerSession( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS );
	virtual bool HandleNetPack( NetPack &pack ) ;
	bool IsOpenChatChannel(uint32 chatType);
	bool CheckHasName(const std::string& strName);

	Player* GetPlayerPtr( void ) {   return m_pPlayer.get() ; }
	PlayerPtr& GetPlayer( void ) {   return m_pPlayer ;   }
private:
	PlayerPtr m_pPlayer ;

public:
	uint64 GetSessionId( void ) const {   return Session::GetSessionId() ;   } // ¸ø player session multi_index Ê¹ÓÃ

	uint64 GetPlayerId( void ) const ;
	uint32 GetCountryId( void ) const;
	const std::string& GetPlayerName( void ) const ;
	uint64 GetAccountId( void ) {   return m_accountId ;   }
	void SetAccountId( uint64 accountId ) {   m_accountId =accountId ;   }
	const std::string& GetAccount( void ) {   return m_account ;   }
	void SetAccount( string account ) {   m_account = account ;   }

private:
	uint64 m_accountId ;
	string m_account;


public:
	boost::shared_ptr<GameServerLogicalTaker>& GetGameLogicalTaker( void ) {   return m_pLogicalTaker ;   }
	void SetGameLogicalTaker( const boost::shared_ptr<GameServerLogicalTaker> &spLogicalTaker )
	{
		m_pLogicalTaker =spLogicalTaker ;
	}

private:
	boost::shared_ptr<GameServerLogicalTaker> m_pLogicalTaker ;


public:
	virtual void HandlePing( NetPack &packet ) ;

	void HandleGetPlayerList( NetPack &packet ) ;
	void TakerGetPlayerList( NetPack &packet ) ;

	void HandleCreatePlayer( NetPack &packet ) ;
	void TakerCreatePlayer( NetPack &packet ) ;

	void HandleSelectPlayer( NetPack &packet ) ;
	void TakerSelectPlayer( NetPack &packet ) ;

	void HandleQuickLogin( NetPack &packet ) ;

	void HandleGetServerTime( NetPack &packet ) ;

	void HandlePvPEntryArena( NetPack &packet ) ;
	void LoadPlayerAppendDatas( NetPack &packet );
	void TakerPvPEntryArena( NetPack &packet ) ;

	void HandlePvPGetPlayerInfo( NetPack &packet ) ;
	void TakerPvPGetPlayerInfo( NetPack &packet ) ;

	void HandlePvPFightResultInfo( NetPack &packet ) ;
	void TakerPvPFightResultInfo( NetPack &packet ) ;

	void HandlePvPFightRecordInfo( NetPack &packet ) ;
	void TakerPvPFightRecordInfo( NetPack &packet ) ;

	void HandlePvPGetRankingList( NetPack &packet ) ;
	void TakerPvPGetRankingList( NetPack &packet ) ;

	void HandlePvPMarkFight( NetPack &packet ) ;
	void TakerPvPMarkFightRet( NetPack &packet ) ;

	void HandleTryResetValueTimer( NetPack &packet );

    void TakerPlayerChangeName( NetPack &packet );

	void HandleEnterCrossWar( NetPack &packet );
	void HandleRegisterCrossPlayerRet( NetPack &packet );
	void TakerPlayerPayRecordRet( NetPack &packet );
	void TakerPlayerSetNameAndHeadId(NetPack &packet);
	void HandleSelectCountryForAward(NetPack& packet);
	void TakerSelectCountryRet(NetPack& packet);
// 	void HandleCongraulateAccept(NetPack& packet);
// 	void HandleCongraulateCurrent(NetPack& pack);
} ;



#endif


