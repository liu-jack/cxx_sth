
#ifndef GAME_SERVER_SESSION_H__
#define GAME_SERVER_SESSION_H__

#include "../data/DbproxyDataDefine.h"
#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"

//////////////////////////////////////////////////////////////////////////

class GameServerSession : public Session
{
	DECLARE_OBJ_POOL(GameServerSession)

public:
	typedef void (GameServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;
private:
	static HandlerArray s_serverHandlers ;

public:
	static HandlerArray RegisteServerHandlers( void );

public:
	GameServerSession( void ) ;
	virtual ~GameServerSession( void ) ;
	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;
	int GetRegionId( void );
	int GetServerId( void );
private:
	SessionDataPtr m_sessionDataPtr;
	GloabalDataPtr m_gloabalDataPtr;
	bool m_isRegistOk;
	bool m_isNotNotifiedCache;




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PvP
//public:
//	typedef CachePtrMap< uint32, PvPRankTable > PvPRankMap ;
//	typedef PvPRankMap::Iter                    PvPRankMapItr ;
//
//public:
//	void InitPvPRankList( void ) ;
//	int AddPvPRank( uint64 playerId, uint64 accountId, uint32 rank ) ;
//
//private:
//	PvPRankMap m_pvpRank ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual bool HandleNetPack( NetPack &pack );
	virtual void HandlePing( NetPack &packet ) ;

#undef Msg_Declare
#define Msg_Declare(e, MsgType) void Handle_##e(NetPack &packet);
    #include "../msg/GlobalDataMsg.h"
    #include "../msg/PlayerDataMsg.h"


	void HandleRegister( NetPack &packet ) ;
	void HandleGetPlayerList( NetPack &packet ) ;
	void HandleCreatePlayer( NetPack &packet ) ;
	void HandleSelectPlayer( NetPack &packet ) ;
	void HandlePlayerOffline( NetPack &packet ) ;
	void HandleUpdateStageLevel( NetPack &packet );
    void HandleUpdateStageLevelEndless( NetPack &packet );

    void HandleUpdateBag( NetPack &packet );
    
    void HandleCreatePlayerData( NetPack &packet );
    void HandleUpdatePlayerValue( NetPack &packet );

    void HandlePlayerChangeName( NetPack &packet );


    void HandleUpdateQuestCounter( NetPack &packet );
    void HandleUpdateQuestComplete( NetPack &packet );
    void HandleUpdateQuestLog( NetPack &packet );    
    
    // character 
    void HandleUpdateBattleChars( NetPack &packet );
    void HandleDestoryCharacters( NetPack &packet );
    void HandleCreateCharacter( NetPack &packet );
    void HandleUpdateCharacter( NetPack &packet );
    // sell
    void HandleUpdateSellRandomProgram( NetPack &packet );
    void HandleUpdateSellProgramInfo( NetPack &packet );

    void HandleUpdateLootRoundRecord( NetPack &packet );
    void HandleUpdateLotteryFreeRecord( NetPack &packet );

    void HandleUpdatePlayerString( NetPack& packet );


	void HandleCheckMailReciVersExist( NetPack &packet );
	void HandleOffLineMsg( NetPack &packet );
	void HandleReceveMail( NetPack &packet );
	void HandleUpdateMail( NetPack &packet );
	void HandleFriendOperate( NetPack &packet );
	void HandleFriendSearch( NetPack &packet );
	void HandleCreateGuildInfo( NetPack &packet );
	void HandleUpdateGuildMemberInfo( NetPack &packet );
	void HandleUpdateGuildInfo( NetPack &packet );
	void HandleGuildSearchPlayerInfo( NetPack &packet );
	void HandleFriendUpdateData( NetPack &packet );
	void HandleWebGetPlayerList( NetPack &packet );
	void HandlerGetPlayerInfo( NetPack &packet );

	void HandlePvPCreateVirtualPlayer( NetPack &packet );
	void HandlePvPEntryArena( NetPack &packet ) ;
	void HandlePvPGetPlayerInfo( NetPack &packet ) ;
	void HandlePvPFightResultInfo( NetPack &packet ) ;
	void HandlePvPFightRecordInfo( NetPack &packet ) ;
	void HandleServerGobalValue( NetPack &packet ) ;
	void HandleGetPvPRewardRankList( NetPack &packet ) ;
	void HandlePvPGetRankingList( NetPack &packet ) ;
	void HandlePvPMarkFight( NetPack &packet ) ;
	void HandleWebPay( NetPack &packet ) ;
	void HandleGetPlayerPayRecord( NetPack &packet ) ;
	void HandleUpdatePlayerModule( NetPack &packet ) ;
	void HandleUpdatePlayerBuilding( NetPack &packet ) ;
	void HandleGetPlayerAppendData( NetPack &packet );
	void HandleForbidenPlayer( NetPack &packet );
	void HandleSavePlayerCache( NetPack &packet );
	void HandleSaveAllPlayerCache(NetPack& packet);

	void HandleSetNameAndHeadId(NetPack& packet);
	void HandleUpdateActivityData(NetPack& packet);
	void HandleDeleteActivityStarBoxTable(NetPack& packet);
	void HandleDeleteGeneralRewardTable(NetPack& packet);
	void HandleSelectCountry(NetPack& packet);
	void HandleSaveDbproxy(NetPack& packet);
} ;



#endif


