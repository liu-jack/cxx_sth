
#ifndef SESSIONDATA_H__
#define SESSIONDATA_H__
#include "../session/PlayerDataCacheContainer.h"
#include "DbproxyDataDefine.h"

#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"
#include "../player_data/CachePtrMap.h"

/////////////////////////////////////////////////////////////////////////
using namespace pb;

namespace pb
{
	class MsgPlayerBaseData;
	class MsgPlayerBaseInfo;
	class Friend_list_Record;
	class SG2D_Forbiden_Player;
	class SD2G_Check_Mail_Recivers_Exist_Rspone;
	class SG2G_Mail_SendMail;
}

struct PlayerTable;
//struct PvPRankTable ;
class SessionData
{

public:
	SessionData( const SessionPairId& pairId ) ;
	virtual ~SessionData( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;


public:
	void OnPlayerOffLine( uint64 playerId ) ;
	void OnPlayerOnline( PlayerDataPtr &ptr ) ;
	void SetSession(Session *psession);
	void UnLoadSessionData();
	const SessionPairId& GetPairId();
	int GetRegionId( void );
	int GetServerId( void );

private:
	void AddToOfflineTable( PlayerDataPtr &ptr ) ;
	void Send( const int opCode, const ::google::protobuf::Message &msg ) const ;
	void Send( const int opCode, const uint64 sessionId, const ::google::protobuf::Message &msg ) const ;

public:
	PlayerDataPtr GetPlayerData( uint64 playerId ) ;
	PlayerDataPtr GetPlayerData( const string &name ) ;
	PlayerDataPtr GetOrLoadPlayerData( uint64 playerId ) ;
	bool IsInPlayerCache( uint64 playerId );
	void SavePlayerCache(uint64 playerId );
	void SaveAllPlayerCache();
	void SaveAndRemoveAllPlayerCache();
	bool AddPlayerDataToMap( const PlayerDataPtr &ptr ) ;
	void RemovePlayerDataFromMap( uint64 playerId ) ;
public:
	uint32 GetNextPlayerGuid();
	void InitPlayerDataCache( void ) ;
	uint64 CachePlayerData(PlayerTable *pPlayerTable);
	void SyncPlayerDataToGameServer(PlayerDataPtr& pPlayerPtr);

	inline time_t GetLastSaveTime( void ){    return m_lastSaveTime;    }
	inline void SetLastSaveTime(time_t time){    m_lastSaveTime = time;    }

public:
	bool AddOfflineMail(uint64 playerId,const pb::SG2G_Mail_SendMail& sendMail);
	PlatfromMailTable* AddNewPlatfromMailToTable( PlatfromMailTable &newMail );
	void ReceveMails(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg);
	bool UpdateForbidenInfo(PlayerTable *pPlayerTable, const pb::SG2D_Forbiden_Player& info);
	void OnGameServerRegist();
	void ClearActivityStartBoxTable();
	void ClearGeneralRewardTable();
private:
	void SyncCountryInfo();
	void SyncPalaceInfo();
	void SyncCountryLevelUpTaskRankInfo();
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

private:
	void BrodCastCacheReceveMail(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg,std::set< uint64>& recIds);
	void BrodCastReceveOfflineMail(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg,const std::set< uint64>& notIds);
	void BrodCastPlayersReceveMail(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg,std::set< uint64>& recIds,bool jsOnline);

public:
	CachePtrMap< uint32, CountryBaseTable>          m_CountryBaseTable;
	CachePtrMap< uint32, CountryCityTable>          m_CountryCityTable[MONSTER_COUNTRY_ID];//m_CountryCityTable[MAX_PLAYER_COUNTRY_ID];
	//CachePtrMap< int, ServerGobalValueTable >			m_serverGobalValueMap;
	CachePtrMap< uint64, CountryLevelUpTaskPlayerTable> m_CountryLevelUpTaskPlayerTable[MAX_PLAYER_COUNTRY_ID];
	CachePtrMap< int, PalaceOfficeTable>         m_PalaceOfficeTable[MAX_PLAYER_COUNTRY_ID];

private:
	std::list< PlayerTable* > m_playerTableList;
	std::list<uint64>        m_syncToGamePIdList;
	bool m_isCachePlayerFinish;
	bool m_isSyncPTGFinish;
	uint32 m_curPlayerIdSeed;
	SessionPairId  m_curSessionPairId;
	Session	 *m_curSession;
	PlayerDataMap          m_playerMap;
	MKPlayerOfflineTable   m_offlinePlayers ;
	time_t m_lastSaveTime;

} ;



#endif //SESSIONDATA_H__


