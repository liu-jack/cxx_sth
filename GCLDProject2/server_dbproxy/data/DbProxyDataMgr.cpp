
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include <stdio.h>

#include "DbProxyDataMgr.h"
#include "MySqlSingleton.h"
#include "InterServer.pb.h"
#include "OS.h"


#include "../PlayerData.h"
#include "../DbProxyServer.h"
#include "GlobalData.h"
#include "SessionData.h"
#include "utility/STLUtility.h"
#include "Social.pb.h"

DbProxyDataMgr::DbProxyDataMgr( void )
:m_dbproxyId(1)
{
}


DbProxyDataMgr::~DbProxyDataMgr( void )
{
	SessionDataMapItr iterSDataPtr = m_sessionDataMap.begin();
	for(; iterSDataPtr != m_sessionDataMap.end(); ++iterSDataPtr)
	{
		SessionDataPtr sdPtr = iterSDataPtr->second;
		sdPtr->UnLoadSessionData();
		sdPtr.reset();
	}
	m_sessionDataMap.clear();

	m_gloabalDataPtr->UnloadGlobalData();
	m_gloabalDataPtr.reset();
}

#undef LOAD_TAB
#define LOAD_TAB(tab_name) \
	template <>\
	boost::shared_ptr<CMysqlTableCache<tab_name>> DbProxyDataMgr::GetTable<tab_name>()\
    {\
	    return m_##tab_name;\
    }

LOAD_ALL_TAB



void DbProxyDataMgr::InitData( int dbproxId )
{
	m_dbproxyId = dbproxId;
#define INIT_TABLE( TABLE_STRUCT) \
	TABLE_STRUCT##CacheType::SetDB( &sDatabase ) ; \
	m_##TABLE_STRUCT.reset(new TABLE_STRUCT##CacheType);\
	m_##TABLE_STRUCT->InitMaxId(dbproxId) ;\
	//INIT_TABLE(EventRecodeTable);
	//INIT_TABLE(GuildInfoTable);
	//INIT_TABLE(GuildMemberInfoTable);
    INIT_TABLE(CountryBaseTable);
    INIT_TABLE(CountryCityTable);
    INIT_TABLE(CountryLevelUpTaskPlayerTable);
    INIT_TABLE(PalaceOfficeTable);
	INIT_TABLE(ActivityProtoTable);
	INIT_TABLE(ActivityPlanTable);
	INIT_TABLE(ServerOpenTimeTable);
	INIT_TABLE(ActivityRewardTable);
	INIT_TABLE(ActivityCountryOccupyCityTable);

#undef LOAD_TAB
#define LOAD_TAB(tab_name) \
	tab_name##CacheType::SetDB( &sDatabase ) ; \
	m_##tab_name.reset(new tab_name##CacheType);\
	m_##tab_name->InitMaxId(dbproxId) ;\

	LOAD_ALL_TAB


	LoadGlobalData();
}



bool DbProxyDataMgr::GetHadThisPlayerInDB( const std::string &name )
{
	return PlayerTableCacheType::QueryIsTure( "SELECT 1 FROM %s WHERE name = '%s' LIMIT 1; ", PlayerTable::GetTableName(), name.c_str() ) ;
}

bool DbProxyDataMgr::GetHadThisPlayerInDB( const uint64 playerId )
{
	return PlayerTableCacheType::QueryIsTure( "SELECT 1 FROM %s WHERE playerId =%llu LIMIT 1;", PlayerTable::GetTableName(), playerId ) ;
}

bool DbProxyDataMgr::GetHadThisPlayerInDBLocked( const uint64 playerId )
{
	return PlayerTableCacheType::QueryIsTure( "SELECT 1 FROM %s WHERE  lockReason = 2 AND playerId =%llu LIMIT 1;", PlayerTable::GetTableName(), playerId ) ;
}

bool DbProxyDataMgr::GetHadThisSdkAccountPlayer( const uint64 playerId, const std::string &sdkAccount )
{
	return PlayerTableCacheType::QueryIsTure( "SELECT 1 FROM %s WHERE playerId =%llu AND sdkUserId = '%s' LIMIT 1;", PlayerTable::GetTableName(), playerId, sdkAccount.c_str() ) ;
}

void  DbProxyDataMgr::GetSameCountryNum(IntPairVec & vec)
{
	CMySqlConnection* pConn = sDatabase.GetConnection() ;
	if( pConn == NULL )
	{
		ASSERT( false ) ;
		return;
	}
	CMysqlRstPtr pRst;

	pRst = pConn->QueryFormat("SELECT c.country_id,COUNT(c.country_id) from (SELECT a.playerId,b.country_id FROM %s a,%s b WHERE a.playerId = b.player_id ORDER BY a.LEVEL desc LIMIT 200) as c GROUP BY c.country_id;", PlayerTable::GetTableName(),MapLogicTable::GetTableName());
	if( pRst && pRst->GetRowCount() > 0 )
	{
		Field* pField;
		do 
		{
			pField = pRst->Fetch();
			if (pField)
			{
				Field* f = pField;
				int countryId = f->GetUInt8();
				++f;
				int playerNum = f->GetInt32();
				vec.push_back(std::make_pair(countryId,playerNum));
			}
		}while (pRst->NextRow());
	}
	sDatabase.PutConnection( pConn ) ;
}

bool DbProxyDataMgr::GetPlayerIds( std::set< uint64>& idSet,const char* sqlSrc/* = NULL*/)
{
	uint64 playerId =0 ;
	bool bResult =false ;
	CMySqlConnection* pConn = sDatabase.GetConnection() ;
	if( pConn == NULL )
	{
		ASSERT( false ) ;
		return bResult ;
	}
	CMysqlRstPtr pRst;
	if ( sqlSrc != NULL)
	{
		pRst = pConn->Query( sqlSrc, strlen( sqlSrc ) );
	}
	else
	{
		pRst = pConn->QueryFormat("SELECT playerId FROM %s;", PlayerTable::GetTableName());
	}
	if( pRst && pRst->GetRowCount() > 0 )
	{
		bResult = true;
		Field* pField;
		do 
		{
			pField = pRst->Fetch();
			if (pField)
			{
				playerId = pField->GetUInt64();
				idSet.insert(playerId);
			}
		}while (pRst->NextRow());
	}
	sDatabase.PutConnection( pConn ) ;
	return bResult ;
}


uint64 DbProxyDataMgr::GetPlayerIdByName( const std::string &name )
{
	uint64 playerId =0 ;
	CMySqlConnection* pConn = sDatabase.GetConnection() ;
	if( pConn == NULL )
	{
		ASSERT( false ) ;
		return playerId ;
	}

	std::string sql ;
	Utility::FormatString( sql, "SELECT playerId FROM %s WHERE name = '%s' LIMIT 1; ", PlayerTable::GetTableName(), name.c_str() ) ;
	CMysqlRstPtr pResult =pConn->QueryFormat( sql.c_str()) ;
	if( pResult )
	{
		Field* pField = pResult->Fetch();
		playerId = pField[0].GetUInt64();
	}

	sDatabase.PutConnection( pConn ) ;
	return playerId ;
}


bool DbProxyDataMgr::AddNewPlayerToTable( PlayerTable &newPlayer )
{
	// 玩家创建使用同步流程
	newPlayer.id =m_PlayerTable->IncreaseMaxId() ;

	uint64 insertId =m_PlayerTable->WriteEntry( &newPlayer, true ) ;
	ASSERT( newPlayer.id == insertId ) ;

	return insertId > 0 ;
}

void DbProxyDataMgr::GetMapLogicRecord( MapLogicTableCacheType& tmpTable, uint64 playerId)
{
	std::string sql ;
	Utility::FormatString( sql, "SELECT * FROM %s WHERE player_id = %llu LIMIT 1; ", MapLogicTable::GetTableName(), playerId) ;
	tmpTable.SqlLoadAndAddData( sql.c_str() ) ;
}

void DbProxyDataMgr::GetPlayerList( PlayerTableCacheType& tmpTable, uint64 accountId, uint32 regionId, uint32 serverId )
{
	std::string sql ;
    Utility::FormatString( sql, "SELECT * FROM %s WHERE accountId = %llu AND regionId = %u AND serverId =%u ; ", PlayerTable::GetTableName(), accountId, regionId, serverId ) ;
	tmpTable.SqlLoadAndAddData( sql.c_str() ) ;
}


void DbProxyDataMgr::UnLoadPlayerData( PlayerData *pPlayerData )
{
	ASSERT( pPlayerData != NULL ) ;
	if( pPlayerData == NULL )
	{
		return ;
	}

	pPlayerData->m_unloadCount++;

	DLOG( "DbProxyDataMgr::UnLoadPlayerData( player id: %llu )", pPlayerData->GetPlayerId() ) ;

	// 缓存基础数据
	//AddPlayerBaseData( pPlayerData ) ;

#undef LOAD_TAB
#define LOAD_TAB(tab_name) \
    pPlayerData->m_##tab_name.RemoveContentFromCache();

    LOAD_ALL_PLAYER_KEY_TAB


#undef LOAD_TAB
#define LOAD_TAB(tab_name) \
	pPlayerData->m_##tab_name.RemoveAllEntry();

	LOAD_ALL_PLAYER_MAP_TAB         

}




namespace{
	void GetTableSql( string& outSql,const char* tableName, uint64 playerId)
	{
		char buffer[128];
		sprintf( buffer, "SELECT * FROM %s WHERE player_id =%llu ", tableName, playerId);
		outSql.assign(buffer);
	}

	void AppendWhere( std::string& SQLstr, const char* columnName, const std::set<uint32>& inIds)
	{
		if ( inIds.empty())
			return;
		std::set<uint32>::const_iterator it = inIds.begin();
		std::stringstream ss;
		ss << "where " << columnName << " in (" << *it++;
		while( it != inIds.end() )
		{
			ss << ", "<< *it++;
		}
		ss << ")";
	}

    template< class TablePtrVec, class PlayerMapTable >
    void CopyVecToPlayerMapTable( TablePtrVec& vec, PlayerMapTable& maptable)
    {
        for ( BOOST_AUTO( it , vec.begin()); it != vec.end(); ++it )
        {
            maptable.AddToPtrMap( *it);
        }
    }
}

PlayerData* DbProxyDataMgr::LoadPlayerData( uint64 playerId)
{
	PlayerTable *pPlayerTable = m_PlayerTable->GetEntryByKey(playerId);
	if( pPlayerTable != NULL )
	{
		PlayerData * pPData = LoadOnePlayerData(pPlayerTable);
		if (pPData)
		{
			return pPData;
		}
	}

	std::string sql ;
	Utility::FormatString( sql, "SELECT * FROM %s WHERE lockReason != 2  AND playerId =%llu LIMIT 1; ", PlayerTable::GetTableName(), playerId ) ;


	std::vector<PlayerData*> vecPlayerData ;
	LoadPlayerData( sql, vecPlayerData ) ;
	ASSERT( vecPlayerData.size() <= 1 ) ;

	return !vecPlayerData.empty() ?  vecPlayerData[0] : NULL ;
}


PlayerData* DbProxyDataMgr::LoadPlayerData( uint64 accountId, uint64 playerId)
{
	PlayerTable *pPlayerTable = m_PlayerTable->GetEntryByKey(playerId);
	if( pPlayerTable != NULL )
	{
		PlayerData * pPData = LoadOnePlayerData(pPlayerTable);
		if (pPData)
		{
			return pPData;
		}
	}

	std::string sql ;
	Utility::FormatString( sql, "SELECT * FROM %s WHERE lockReason != 2  AND playerId =%llu AND accountId =%llu LIMIT 1; ", PlayerTable::GetTableName(), playerId, accountId ) ;


	std::vector<PlayerData*> vecPlayerData ;
	LoadPlayerData( sql,vecPlayerData ) ;
	ASSERT( vecPlayerData.size() <= 1 ) ;

	return !vecPlayerData.empty() ?  vecPlayerData[0] : NULL ;
}

bool DbProxyDataMgr::LoadPlayerTable(const std::string &playerTableSql,std::vector<PlayerTable*>& vecPlayerTable)
{
	if( !m_PlayerTable->SqlLoadAndAddData( playerTableSql.c_str(), &vecPlayerTable, FlagReload ) )
	{
		return false;
	}
	return true;
}

PlayerData * DbProxyDataMgr::LoadOnePlayerData(PlayerTable *pPlayerTable)
{
	if (pPlayerTable == NULL)
	{
		return NULL;
	}
	uint64 playerId =pPlayerTable->playerId ;
	PlayerData *pPlayerData =new PlayerData( playerId) ;
	pPlayerData->m_loadCount++;
	// 移除玩家基本缓存数据
	//RemovePlayerBaseData( playerId ) ;
	pPlayerData->m_PlayerTable.SetContent( pPlayerTable ) ;
	std::string sql ;

#undef LOAD_TAB
#define LOAD_TAB(tab_name) \
	GetTableSql(sql, tab_name::GetTableName(), playerId);\
	vector< tab_name*> vec##tab_name;\
	if (m_##tab_name->SqlLoadAndAddData(sql.c_str(), &vec##tab_name, FlagReload)){\
	pPlayerData->m_##tab_name.SetContent(vec##tab_name[0]);\
	}
	LOAD_ALL_PLAYER_KEY_TAB2

#undef LOAD_TAB
#define LOAD_TAB(tab_name) \
	GetTableSql(sql, tab_name::GetTableName(), playerId);\
	vector< tab_name*> vec##tab_name;\
	m_##tab_name->SqlLoadAndAddData(sql.c_str(), &vec##tab_name, FlagReload);\
	CopyVecToPlayerMapTable( vec##tab_name, pPlayerData->m_##tab_name);
	LOAD_ALL_PLAYER_MAP_TAB

	return pPlayerData;
}

void DbProxyDataMgr::LoadPlayerData( const std::string &playerTableSql,std::vector<PlayerData*> &vecPlayerData )
{
	
	std::vector<PlayerTable*> vecPlayerTable ;
	if (!LoadPlayerTable(playerTableSql,vecPlayerTable))
	{
		return;
	}
	vecPlayerData.reserve( vecPlayerData.size() + vecPlayerTable.size() ) ;
	BOOST_AUTO( itrBeg, vecPlayerTable.begin() );
	BOOST_AUTO( itrEnd, vecPlayerTable.end() );
	for( ; itrBeg != itrEnd; ++itrBeg )
	{
		PlayerTable *pPlayerTable =*itrBeg ;
		PlayerData* tempData = LoadOnePlayerData(pPlayerTable);
		if (tempData)
		{
			vecPlayerData.push_back( tempData ) ;
		}
	}
}

PlayerTable * DbProxyDataMgr::GetPlayerTable(PlayerTableCacheType& tableCache,uint64 playerId)
{
	std::string playerTableSql ;
	Utility::FormatString( playerTableSql, "SELECT * FROM %s WHERE  playerId =%llu LIMIT 1; ", PlayerTable::GetTableName(), playerId ) ;
	std::vector<PlayerTable*> vecPlayerTable ;
	if( !tableCache.SqlLoadAndAddData( playerTableSql.c_str(), &vecPlayerTable, FlagReload ) )
	{
		return NULL;
	}
	ASSERT( vecPlayerTable.size() == 1 ) ;
	PlayerTable *pPlayerTable = vecPlayerTable.front() ;
	ASSERT( playerId == pPlayerTable->playerId ) ;
	return pPlayerTable;
}

bool DbProxyDataMgr::LoadPlayerBaseData( uint64 playerId, pb::MsgPlayerBaseData& baseData)
{
    return false;
}


void DbProxyDataMgr::LoadSessionGlobalData( SessionDataPtr& sesPtr)
{
	std::vector< PlatfromMailTable* > vec ;
	sDataMgr.LoadTableByServerLimit<PlatfromMailTable>(sesPtr->GetRegionId(), sesPtr->GetServerId(), vec ) ;
	std::string sql ;
#define LOAD_SESSION_GLOBAL_TABLE(tabName)\
	Utility::FormatString(sql, "SELECT * FROM %s WHERE regionId =%u AND serverId =%u;",\
	tabName::GetTableName(), sesPtr->GetRegionId(), sesPtr->GetServerId());\
	std::vector<tabName*> vec##tabName##Records;\
	m_##tabName->SqlLoadAndAddData(sql.c_str(), &vec##tabName##Records, FlagReload);\
	for (BOOST_AUTO(it, vec##tabName##Records.begin()); it != vec##tabName##Records.end(); ++it){\
	sesPtr->m_##tabName.AddToPtrMap(*it);\
	}

	LOAD_SESSION_GLOBAL_TABLE(CountryBaseTable);
	
	//LOAD_SESSION_GLOBAL_TABLE(ServerGobalValueTable);

#undef LOAD_SESSION_GLOBAL_TABLE
#define LOAD_SESSION_GLOBAL_TABLE(tabName)\
	Utility::FormatString(sql, "SELECT * FROM %s WHERE regionId =%u AND serverId =%u;",\
	tabName::GetTableName(), sesPtr->GetRegionId(), sesPtr->GetServerId());\
	std::vector<tabName*> vec##tabName##Records;\
	m_##tabName->SqlLoadAndAddData(sql.c_str(), &vec##tabName##Records, FlagReload);\
	for (BOOST_AUTO(it, vec##tabName##Records.begin()); it != vec##tabName##Records.end(); ++it){\
	sesPtr->m_##tabName[(*it)->country_id - 1].AddToPtrMap(*it);\
	}


	LOAD_SESSION_GLOBAL_TABLE(CountryCityTable);
	LOAD_SESSION_GLOBAL_TABLE(CountryLevelUpTaskPlayerTable); // 国家升级任务 蛮族试练 排行榜
	LOAD_SESSION_GLOBAL_TABLE(PalaceOfficeTable);


	CheckAndRemoveExpiredPlatformMail();
}

SessionDataPtr DbProxyDataMgr::GetOrLoadSessionData( const SessionPairId &pairId )
{
	SessionDataMapItr iterSDataPtr = m_sessionDataMap.find(pairId);
	if (iterSDataPtr != m_sessionDataMap.end())
	{
		return iterSDataPtr->second;
	}

	SessionDataPtr tempSessionDataPtr(new SessionData(pairId));
	LoadSessionGlobalData(tempSessionDataPtr);
	tempSessionDataPtr->InitPlayerDataCache();
	std::pair< SessionDataMapItr, bool> retPair = m_sessionDataMap.insert( std::make_pair(pairId,tempSessionDataPtr) ) ;
	if (retPair.second)
	{
		return retPair.first->second ;
	}
	else
	{
		return SessionDataPtr();
	}
}

void DbProxyDataMgr::LoadGlobalData()
{
	m_gloabalDataPtr.reset(new GlobalData());
	std::string sql ;
#define LOAD_GLOBAL_TABLE(tabName)\
	Utility::FormatString(sql, "SELECT * FROM %s;",tabName::GetTableName());\
	std::vector<tabName*> vec##tabName##Records;\
	m_##tabName->SqlLoadAndAddData(sql.c_str(), &vec##tabName##Records, FlagReload);\
	for (BOOST_AUTO(it, vec##tabName##Records.begin()); it != vec##tabName##Records.end(); ++it){\
	m_gloabalDataPtr->m_##tabName.AddToPtrMap(*it);\
	}

	LOAD_GLOBAL_TABLE(ActivityProtoTable);
	LOAD_GLOBAL_TABLE(ActivityPlanTable);
	LOAD_GLOBAL_TABLE(ActivityRewardTable);
	LOAD_GLOBAL_TABLE(ServerOpenTimeTable);
	LOAD_GLOBAL_TABLE(ActivityCountryOccupyCityTable);
}

GloabalDataPtr DbProxyDataMgr::GetOrLoadGlobalData()
{
	if (m_gloabalDataPtr.get())
	{
		return m_gloabalDataPtr;
	}
	LoadGlobalData();
	if (m_gloabalDataPtr.get())
	{
		return m_gloabalDataPtr;
	}
	else
	{
		return GloabalDataPtr();
	}
}


void DbProxyDataMgr::AddPlayerBaseData( PlayerData *pPlayerData )
{
	CheckPlayerBaseDataCacheCount() ;
	BOOST_AUTO( iterTeamInfo , m_PlayerBaseInfoMap.find(pPlayerData->GetPlayerId()));
	if (iterTeamInfo != m_PlayerBaseInfoMap.end())
	{
		m_PlayerBaseInfoMap.erase(iterTeamInfo);
	}
	pb::MsgPlayerBaseData* teamInfo = new pb::MsgPlayerBaseData();
	pPlayerData->WritePlayerInfo(*teamInfo);
	m_PlayerBaseInfoMap[pPlayerData->GetPlayerId()] = teamInfo;
}

void DbProxyDataMgr::RemovePlayerBaseData( uint64 playerId )
{
	m_PlayerBaseInfoMap.erase(playerId);
}

void DbProxyDataMgr::CheckPlayerBaseDataCacheCount( void )
{
	if( m_PlayerBaseInfoMap.size() >= DbProxyServer::GetMaxCachePlayerBaseDataCount() )
	{
		m_PlayerBaseInfoMap.erase(m_PlayerBaseInfoMap.begin());
	}
}

pb::MsgPlayerBaseData* DbProxyDataMgr::GetPlayerBaseData(uint64 playerId, uint32 regionId, uint32 serverId )
{
	pb::MsgPlayerBaseData* curPlayerTeamInfo = NULL;
	BOOST_AUTO( iterPlayerInfo , m_PlayerBaseInfoMap.find(playerId));
	if (iterPlayerInfo != m_PlayerBaseInfoMap.end())
	{
		curPlayerTeamInfo = iterPlayerInfo->second;
	}
	else
	{
		CheckPlayerBaseDataCacheCount();
		pb::MsgPlayerBaseData* teamInfo = new pb::MsgPlayerBaseData();
		if (teamInfo)
		{
			if(LoadPlayerBaseData(playerId,*teamInfo))
			{
				m_PlayerBaseInfoMap[playerId] = teamInfo;
				curPlayerTeamInfo = teamInfo;
			}
			else
			{
				delete teamInfo;
				teamInfo = NULL;
			}
		}
	}
	return curPlayerTeamInfo;
}

void DbProxyDataMgr::CheckAndRemoveExpiredPlatformMail()
{
	PlatfromMailTableCacheType::MapItr iterMail = m_PlatfromMailTable->MapBegin();
	std::vector<PlatfromMailTable*> toDelMails;
	for (; iterMail != m_PlatfromMailTable->MapEnd(); ++iterMail)
	{
		PlatfromMailTable* curPMail = &iterMail->second;
		if (curPMail->create_time + MAIL_NOLOGIN_CANT_RECEIVE_TIME < ::time( NULL ))
		{
			toDelMails.push_back(curPMail);
		}
	}
	int nTodelMail = toDelMails.size();
	for (int i = 0; i<nTodelMail; i++)
	{
		PlatfromMailTable* curMail = toDelMails[i];
		m_PlatfromMailTable->RemoveKeyEntry( curMail);
	}
	toDelMails.clear();
}

template< class T >
void DbProxyDataMgr::LoadTableByServerLimit( int regionId, int serverId, std::vector< T* > &vec )
{
	std::string sql ;

	Utility::FormatString( sql, "SELECT * FROM %s WHERE regionId =%u AND serverId =%u;"
		, T::GetTableName(), regionId, serverId ) ;

	GetTable<T>()->SqlLoadAndAddData( sql.c_str(), &vec, FlagReload ) ;
}

//PvPRankTable* DbProxyDataMgr::GetPlayerPvPRank( uint64 playerId )
//{
//	PvPRankTable *pRank =m_PvPRankTable->GetEntryByKey( playerId ) ;
//	return pRank ;
//}

//
//void DbProxyDataMgr::LoadPvPRank( int regionId, int serverId, std::vector< PvPRankTable* > &vec )
//{
//	std::string sql ;
//
//	PvPRankTableCache::GetFormat( sql, "SELECT * FROM %s WHERE regionId =%u AND serverId =%u;"
//		, PvPRankTable::GetTableName(), regionId, serverId ) ;
//
//	m_PvPRankTable.SqlLoadAndAddData( sql.c_str(), &vec, FlagReload ) ;
//}

