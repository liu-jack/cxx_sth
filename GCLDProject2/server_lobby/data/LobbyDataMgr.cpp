
#include <stdlib.h>
#include <algorithm>
#include "LobbyDataMgr.h"
#include "OS.h"


LobbyDataMgr::LobbyDataMgr( void )
{

}


LobbyDataMgr::~LobbyDataMgr( void )
{

}



bool LobbyDataMgr::InitData( uint32 regionId  )
{
	m_playerInfoCacheMap.InitDb(regionId);
	m_countryWarInfoCacheMap.InitDb(regionId);
	m_countryAnabasisInfoCacheMap.InitDb(regionId);
	m_warMatchCacheMap.InitDb(regionId);
	m_anabasisMatchCacheMap.InitDb(regionId);
	m_playerInfoCacheMap.LoadDatas(NULL,FlagCleanupReload);
	m_countryWarInfoCacheMap.LoadDatas(NULL,FlagCleanupReload);
	m_countryAnabasisInfoCacheMap.LoadDatas(NULL,FlagCleanupReload);
	m_warMatchCacheMap.LoadDatas(NULL,FlagCleanupReload);
	m_anabasisMatchCacheMap.LoadDatas(NULL,FlagCleanupReload);
	return true ;
}

bool LobbyDataMgr::ReleasePlayerInfoTable( PlayerInfoTable *pPlayerInfo )
{
	return m_playerInfoCacheMap.DeleteEntry(pPlayerInfo);
}

bool LobbyDataMgr::ReleasePlayerInfoTable( uint64 playerId )
{
	return  m_playerInfoCacheMap.DeleteEntry(playerId);
}

PlayerInfoTable* LobbyDataMgr::AddNewPlayerInfoTable( PlayerInfoTable &newPlayer )
{
	return m_playerInfoCacheMap.AddAndAddToCache(&newPlayer);
}

PlayerInfoTable* LobbyDataMgr::GetPlayerInfoTable( uint64 playerId )
{
	return m_playerInfoCacheMap.GetElement(playerId);
}

bool LobbyDataMgr::ReleaseAllCountryWarInfoTable()
{
	return m_countryWarInfoCacheMap.RemoveAllEntryAndCache();
}

bool LobbyDataMgr::ReleaseCountryWarInfoTable( CountryWarTable *pCountryInfo )
{
	return m_countryWarInfoCacheMap.DeleteEntry(pCountryInfo);
}

CountryWarTable* LobbyDataMgr::AddNewCountryWarInfoTable( CountryWarTable &newCountry )
{
	return m_countryWarInfoCacheMap.AddAndAddToCache(&newCountry);
}

CountryWarTable* LobbyDataMgr::GetCountryWarInfoTable( const CountryId& countryId )
{
	return m_countryWarInfoCacheMap.GetElement(countryId);
}

bool LobbyDataMgr::ReleaseAllCountryAnabaInfoTable()
{
  return m_countryAnabasisInfoCacheMap.RemoveAllEntryAndCache();
}

bool LobbyDataMgr::ReleaseCountryAnabaInfoTable( CountryAnabasisTable *pCountryInfo )
{
  return m_countryAnabasisInfoCacheMap.DeleteEntry(pCountryInfo);
}

CountryAnabasisTable* LobbyDataMgr::AddNewCountryAnabaInfoTable( CountryAnabasisTable &newCountry )
{
	return m_countryAnabasisInfoCacheMap.AddAndAddToCache(&newCountry);
}

CountryAnabasisTable* LobbyDataMgr::GetCountryAnabaInfoTable( const CountryId& countryId )
{
	return m_countryAnabasisInfoCacheMap.GetElement(countryId);
}


