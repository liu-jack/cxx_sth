#ifndef LOBBYDATA_MGR_H__
#define LOBBYDATA_MGR_H__
#include "Singleton.h"
#include "table/CountryInfoTable.h"
#include "table/PlayerInfoTable.h"
#include "ptr_container/TableCachePtrMap.h"
union CombinId;
typedef CombinId CountryId;
typedef TableCachePtrMap<PlayerInfoTable> PlayerInfoCacheMap;
typedef TableCachePtrMap<CountryWarTable> CountryWarInfoCacheMap;
typedef TableCachePtrMap<CountryAnabasisTable> CountryAnabasisInfoCacheMap;
typedef TableCachePtrMap<CountryWarMatchTable> WarMatchCacheMap;
typedef TableCachePtrMap<CountryAnabasisMatchTable> AnabasisMatchCacheMap;
class LobbyDataMgr: public Singleton<LobbyDataMgr>
{
private:
	friend class Singleton<LobbyDataMgr> ;
private:
	LobbyDataMgr( void ) ;

public:
	~LobbyDataMgr( void ) ;

public:
	bool InitData( uint32 regionId  ) ;
	bool ReleasePlayerInfoTable( uint64 playerId) ;
	bool ReleasePlayerInfoTable( PlayerInfoTable *pPlayerInfo ) ;
	PlayerInfoTable* AddNewPlayerInfoTable( PlayerInfoTable &newPlayer ) ;
	PlayerInfoTable* GetPlayerInfoTable( uint64 playerId );
	PlayerInfoCacheMap& GetPlayerInfoCacheMap( void ) {   return m_playerInfoCacheMap ;}

	bool ReleaseAllCountryWarInfoTable() ;
	bool ReleaseCountryWarInfoTable( CountryWarTable *pCountryInfo ) ;
	CountryWarTable* AddNewCountryWarInfoTable( CountryWarTable &newCountry ) ;
	CountryWarTable* GetCountryWarInfoTable(const CountryId& countryId );
	CountryWarInfoCacheMap& GetCountryWarInfoCacheMap( void ) {   return m_countryWarInfoCacheMap ;}

	bool ReleaseAllCountryAnabaInfoTable() ;
	bool ReleaseCountryAnabaInfoTable( CountryAnabasisTable *pCountryInfo ) ;
	CountryAnabasisTable* AddNewCountryAnabaInfoTable( CountryAnabasisTable &newCountry ) ;
	CountryAnabasisTable* GetCountryAnabaInfoTable( const CountryId& countryId );
	CountryAnabasisInfoCacheMap& GetCountryAnabaInfoCacheMap( void ) {   return m_countryAnabasisInfoCacheMap ;}

private:
	PlayerInfoCacheMap m_playerInfoCacheMap;
	CountryWarInfoCacheMap m_countryWarInfoCacheMap;
	CountryAnabasisInfoCacheMap m_countryAnabasisInfoCacheMap;
	WarMatchCacheMap m_warMatchCacheMap;
	AnabasisMatchCacheMap m_anabasisMatchCacheMap;
};


#define sLobbyDataMgr (LobbyDataMgr::Instance())


#endif
