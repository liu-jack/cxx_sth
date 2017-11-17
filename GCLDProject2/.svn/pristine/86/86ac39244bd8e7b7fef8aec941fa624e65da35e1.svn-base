
#include <sstream>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>

#include "SessionData.h"

#include "Logger.h"
#include "OS.h"

#include "../PlayerData.h"
#include "../DbProxyServer.h"
#include "DbProxyDataMgr.h"
//#include "GlobalData.h"
//#include "data/PayItemTable.h"

#include "Base.h"
#include "def/MmoAssert.h"
#include "def/ObjectDefines.h"
#include "utility/Utility.h"
#include "utility/TimeParser.h"
#include "utility/TimePeriod.h"
#include "memory_buffer/NetPack.h"

#include "Enum.pb.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "Login.pb.h"
#include "Character.pb.h"
#include "InterServer.pb.h"
#include "Quest.pb.h"
//#include "Social.pb.h"
#include "Web.pb.h"
//#include "PvP.pb.h"
#include "GameServer.pb.h"
#include "Module.pb.h"
#include "Palace.pb.h"
//#include "Activity.pb.h"

using namespace pb;


SessionData::SessionData( const SessionPairId& pairId )
:m_isCachePlayerFinish(true)
,m_isSyncPTGFinish(true)
,m_curPlayerIdSeed(0)
,m_curSessionPairId(pairId)
{
	SetLastSaveTime(sOS.TimeMS());
}

SessionData::~SessionData( void )
{
}


void SessionData::Update( uint64 nowTimeMS, int64 diffMS )
{
	PlayerDataMapItr endItr =m_playerMap.end() ;
	for( PlayerDataMapItr itr =m_playerMap.begin(); itr != endItr; ++itr )
	{
		PlayerDataPtr &pData =itr->second ;
		if( pData->GetLastSaveTime() + DbProxyServer::GetPlayerDataSaveDiffMS() < static_cast<time_t>( nowTimeMS ) )
		{
			pData->SaveDirtyPart( nowTimeMS ) ;
		}
	}
	
	if (!m_isCachePlayerFinish)
	{
		if(m_playerTableList.empty())
		{
			m_isCachePlayerFinish = true;
		}
		else
		{
			PlayerTable *pTable = m_playerTableList.front();
			uint64 pId = CachePlayerData(pTable);
			if (pId > 0)
			{
				m_syncToGamePIdList.push_back(pTable->playerId);
			}
			m_playerTableList.pop_front();
		}
	}

	if (!m_isSyncPTGFinish)
	{
		if (m_syncToGamePIdList.empty())
		{
			m_isSyncPTGFinish = true;
			pb::SD2G_All_player_has_sync msg;
			msg.set_result(true);
			Send(pb::SD2G_PLAYER_HAS_ALL_SYNC,msg);
		}
		else
		{
			uint64 curId = m_syncToGamePIdList.front();
			PlayerDataMapItr itr =m_playerMap.find( curId ) ;
			if( itr != m_playerMap.end() )
			{
				SyncPlayerDataToGameServer(itr->second);
			}
			m_syncToGamePIdList.pop_front();
		}
	}
}

void SessionData::OnPlayerOnline( PlayerDataPtr &ptr )
{
	if( ptr->GetIsOffline() )
	{
		TableByPlayerId &byId =m_offlinePlayers.get<ById>() ;
		size_t eraseCount =byId.erase( ptr->GetPlayerId() ) ;
		if( eraseCount <=0 )
		{
			WLOG( "SessionData::OnPlayerOnline() be invoke twice time ??? " ) ;
		}
	}

	ptr->SetIsOffline( false ) ;
	//OnPlayerOnline(ptr->GetPlayerId(),true);
	//uint32 curPlayers = (uint32)m_playerMap.size();
	//if( !m_offlinePlayers.empty() && curPlayers > GetMaxCachePlayerCount() )
	//{
	//	uint32 needRemove = curPlayers - GetMaxCachePlayerCount();
	//	TableByPlayerTime &byTime =m_offlinePlayers.get<ByTime>() ;
	//	time_t limitTime = sOS.TimeSeconds() - DbProxyServer::GetMaxCacheOfflinePlayerSeconds() ;
	//	TableByPlayerTimeItr endItr =byTime.upper_bound( limitTime ) ;
	//	TableByPlayerTimeItr itr =byTime.begin();
	//	for( ; itr != endItr && needRemove > 0; ++itr,--needRemove )
	//	{
	//		ASSERT( itr->offlineTime <= limitTime ) ;
	//		RemovePlayerDataFromMap( itr->playerId ) ;
	//		NLOG( "player id: %llu remove from m_playerMap, no cache the player .", itr->playerId ) ;
	//	}

	//	byTime.erase( byTime.begin(), itr ) ;
	//}
}

void SessionData::ClearActivityStartBoxTable()
{
	sDataMgr.GetTable<ActivityStarBoxTable>()->RemoveAndDeleteAllEntry();
	for( PlayerDataMapItr itr =m_playerMap.begin(); itr != m_playerMap.end(); ++itr )
	{
		itr->second->m_ActivityStarBoxTable.JustRemoveAllFromContianer();
	}
}

void SessionData::ClearGeneralRewardTable()
{
	sDataMgr.GetTable<GeneralRewardTable>()->RemoveAndDeleteAllEntry();
	for( PlayerDataMapItr itr =m_playerMap.begin(); itr != m_playerMap.end(); ++itr )
	{
		itr->second->m_GeneralRewardTable.JustRemoveAllFromContianer();
	}
}

void SessionData::OnPlayerOffLine( uint64 playerId )
{
	PlayerDataMapItr itr =m_playerMap.find( playerId ) ;
	if( itr == m_playerMap.end() )
	{
		ELOG( "SessionData::OnPlayerOffLine() can not find player %llu", playerId ) ;
		return ;
	}

	time_t curTimeMS =sOS.TimeMS() ;

	// 保存
	itr->second->SaveDirtyPart( curTimeMS ) ;

	//OnPlayerOnline(playerId,false);

	AddToOfflineTable( itr->second ) ;

	// 竞技场状态修改, 当在挑战中突然离开游戏, 要取消自己与挑战对象的战斗记录
	//PvPRankTable *pSelfRank   =sDataMgr.GetTable<PvPRankTable>()->GetEntryByKey( playerId ) ;
	//if( pSelfRank != NULL && pSelfRank->extra.targetPlayerId != 0 )
	//{
	//	ASSERT( pSelfRank->extra.isFighting ) ;

	//	pSelfRank->extra.isFighting     =false ;

	//	PvPRankTable *pTargetRank   =sDataMgr.GetTable<PvPRankTable>()->GetEntryByKey( pSelfRank->extra.targetPlayerId ) ;
	//	if( pTargetRank != NULL )
	//	{
	//		ASSERT( pTargetRank->extra.isFighting ) ;

	//		// 被打的人，不应该可以挑战
	//		ASSERT( pTargetRank->extra.targetPlayerId == 0 ) ;

	//		pTargetRank->extra.isFighting     =false ;
	//		pTargetRank->extra.targetPlayerId =0 ;
	//	}

	//	pSelfRank->extra.targetPlayerId =0 ;
	//}

	NLOG( "player id: %llu is remian in m_playerMap, record in m_offlinePlayers use to memory cache !", playerId ) ;
}


void SessionData::AddToOfflineTable( PlayerDataPtr &ptr )
{
	ptr->SetIsOffline( true ) ;

	TableByPlayerId &byId =m_offlinePlayers.get<ById>() ;

	bool isInsert = byId.insert( OfflineData( ptr->m_PlayerTable->playerId, ::time( NULL ) ) ).second ;
	if( !isInsert )
	{
		ELOG( "SessionData::AddToOfflineTable() find %llu is existed in offline record!", ptr->m_PlayerTable->playerId ) ;
		return ;
	}

}

PlayerDataPtr SessionData::GetOrLoadPlayerData( uint64 playerId )
{
	PlayerDataMapItr itr =m_playerMap.find( playerId ) ;
	if( itr != m_playerMap.end() )
	{
		return itr->second ;
	}

	ASSERT( itr == m_playerMap.end() ) ;

	PlayerDataPtr dataPtr( sDataMgr.LoadPlayerData( playerId ) );
	if( !dataPtr )
	{
		ELOG( "SessionData::GetPlayerData( playerId: %llu ) can not find player ~ .", playerId ) ;
		return dataPtr ;
	}

	// 设置离线
	dataPtr->SetIsOffline( true ) ;

	if( m_playerMap.size() < DbProxyServer::GetMaxCachePlayerCount() )
	{
		bool isInsert =AddPlayerDataToMap( dataPtr ) ;
		ASSERT( isInsert ) ;
		AddToOfflineTable( dataPtr ) ;
	}

	return dataPtr ;
}

bool SessionData::IsInPlayerCache( uint64 playerId )
{
	PlayerDataMapItr itr =m_playerMap.find( playerId ) ;
	if( itr != m_playerMap.end() )
	{
		return true;
	}
	return false;
}

PlayerDataPtr SessionData::GetPlayerData( uint64 playerId )
{
	PlayerDataMapItr itr =m_playerMap.find( playerId ) ;
	if( itr != m_playerMap.end() )
	{
		return itr->second ;
	}

	return PlayerDataPtr() ;
}


bool SessionData::AddPlayerDataToMap( const PlayerDataPtr &ptr )
{
	DLOG( "SessionData::AddPlayerDataToMap( player id: %llu )", ptr->GetPlayerId() ) ;
	bool isInsert =m_playerMap.insert( std::make_pair( ptr->GetPlayerId(), ptr ) ).second ;
	return isInsert ;
}

void SessionData::RemovePlayerDataFromMap( uint64 playerId )
{
	DLOG( "SessionData::RemovePlayerDataFromMap( player id: %llu )", playerId ) ;
	PlayerDataMapItr itr =m_playerMap.find( playerId ) ;
	if( itr != m_playerMap.end() )
	{
		PlayerDataPtr &ptr =itr->second ;
		//ASSERT( ptr->GetIsOffline() ) ;

		time_t curTimeMS =sOS.TimeMS() ;
		ptr->SaveDirtyPart( curTimeMS ) ;

		sDataMgr.UnLoadPlayerData( ptr.get() ) ;

		ptr.reset() ;

		m_playerMap.erase( itr ) ;

	/*	pb::SG2D_Player_Id info ;
		info.set_player_id(playerId);
		Send(pb::SD2G_PLAYER_CACHE_REMOVE_SYNC,info);
		DLOG( "SessionData::RemovePlayerDataFromMap( player id: %llu ) success!!", playerId ) ;*/
	}
}


static uint32 GetGuidSeed( const int32 region_id, const int32 server_id, const std::string &table_name, const string &guid_name, OBJECT_TYPE obj_type)
{
	Guid min_guid(obj_type, region_id, server_id, 0 ) ;
	Guid max_guid(obj_type, region_id, server_id, -1) ;

	MMO_ASSERT( min_guid < max_guid ) ;

	CMySqlConnection* pConn = sDatabase.GetConnection() ;
	if( pConn == NULL )
	{
		ASSERT( false ) ;
		return 1 ;
	}

	uint64 cur_guid = 0;
	CMysqlRstPtr pResult =pConn->QueryFormat( "SELECT MAX( %s ) FROM %s WHERE %s > %llu AND %s < %llu"
		, guid_name.c_str(), table_name.c_str(), guid_name.c_str(), static_cast<uint64>(min_guid), guid_name.c_str(), static_cast<uint64>(max_guid) ) ;

	if( pResult )
	{
		Field* pField = pResult->Fetch();
		cur_guid = pField[0].GetUInt64();
	}
	else
	{
		cur_guid =min_guid ;
	}

	if( cur_guid < (uint64)min_guid )
	{
		cur_guid =min_guid ;
	}

	sDatabase.PutConnection( pConn ) ;

	return  Guid::LowGuid( cur_guid ) ;
}

uint32 SessionData::GetNextPlayerGuid()
{
	if (m_curPlayerIdSeed == 0)
	{
		m_curPlayerIdSeed = GetGuidSeed( m_curSessionPairId.second, m_curSessionPairId.first, "player", "playerId", TYPE_PLAYER);
	}
	return ++m_curPlayerIdSeed;
}

void SessionData::InitPlayerDataCache( void )
{
	// '12/01/2014'  请问是几月？同理，'2014-12-01'又是几月？ MYSQL是凭什么知道的呢？ 而STR_TO_DATE('2014-12-01 12:59:59','%Y-%m-%d %H:%i:%s'); 则非常明确。
	NLOG( "Loading playerdata cache.............................." ) ;

	int y =0, m =0, d =0 ;
	uint32 limitDays = DbProxyServer::GetMaxCachePlayerDays() < 1 ? 1 : DbProxyServer::GetMaxCachePlayerDays() ;

	time_t loadLimit =::time( NULL ) - limitDays * 24 * 60 * 60 ;
	if( TimeT2Date( loadLimit, y, m, d ) )
	{
		size_t limitCount = static_cast<size_t>( DbProxyServer::GetMaxCachePlayerCount()) ;
		if( limitCount < 1 )
		{
			limitCount =1 ;
		}
		std::string sql ;
		Utility::FormatString( sql, "SELECT * FROM %s WHERE accountId != 0 AND lockReason != 2  AND regionId =%u AND serverId =%u AND lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') ORDER BY lastLogin DESC LIMIT %u ; "
			, PlayerTable::GetTableName(), m_curSessionPairId.second, m_curSessionPairId.first, y, m, d, limitCount ) ;
		//std::vector<PlayerData*> vecPlayerData ;
		//sDataMgr.LoadPlayerData( sql.c_str(),vecPlayerData ) ;
		uint32 playerCacheCount = 0;
		std::vector<PlayerTable*> vecPlayerTable ;
		if (!sDataMgr.LoadPlayerTable(sql.c_str(),vecPlayerTable))
		{
			return;
		}
		BOOST_AUTO( itrBeg, vecPlayerTable.begin() );
		BOOST_AUTO( itrEnd, vecPlayerTable.end() );
		for( ; itrBeg != itrEnd; ++itrBeg )
		{
			PlayerTable *pPlayerTable =*itrBeg ;
			m_playerTableList.push_back(pPlayerTable ) ;
			playerCacheCount++;
		}
		NLOG( "total %u player add to cache .", playerCacheCount );
	}
	else
	{
		ASSERT( false ) ;
		ELOG( " SessionData::InitPlayerDataCache( void ) fail~~~~!!!!!" ) ;
	}
	m_isCachePlayerFinish = false;
}

uint64 SessionData::CachePlayerData(PlayerTable *pPlayerTable)
{
	PlayerDataMapItr itr =m_playerMap.find( pPlayerTable->playerId ) ;
	if( itr == m_playerMap.end() )
	{
		PlayerData* tempData = sDataMgr.LoadOnePlayerData(pPlayerTable);
		if (tempData)
		{
			PlayerDataPtr dataPtr(tempData) ;
			bool isInsert =AddPlayerDataToMap( dataPtr ) ;
			AddToOfflineTable( dataPtr ) ;
			if (isInsert)
			{
				return tempData->GetPlayerId();
			}
			else
			{
				ELOG( "error SessionData::InitPlayerDataCache() AddPlayerDataToMap() fail palyer datat not cache ....................." ) ;
			}
		}
	}
	return 0;
}


void SessionData::SyncPlayerDataToGameServer( PlayerDataPtr& pPlayerPtr )
{
	pb::SyncPlayerCacheData  synData;
	synData.set_account_id(pPlayerPtr->GetPlayerAccountId());
	pPlayerPtr->WriteModuleInfo(*synData.mutable_append()->mutable_module());
	pPlayerPtr->WriteBuildInfo(*synData.mutable_append()->mutable_building());
	pPlayerPtr->WriteAllPlayerData(synData.mutable_all_data());
	Send(SD2G_PLAYER_CACHE_DATA_SYNC,synData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PvP
//void SessionData::InitPvPRankList( void )
//{
//	NLOG( "Init pvp rank........" ) ;
//
//	std::vector<  PvPRankTable* > vec ;
//
//	sDataMgr.LoadTableByServerLimit( m_curSessionPairId.second, m_curSessionPairId.first, vec ) ;
//
//	std::for_each( vec.begin(), vec.end(), boost::bind( &PvPRankMap::AddToPtrMap, &m_pvpRank, _1 ) ) ;
//}
//
//int SessionData::AddPvPRank( uint64 playerId, uint64 accountId, uint32 rank )
//{
//	if( m_pvpRank.GetElement( rank ) != NULL )
//	{
//		ASSERT( false ) ;
//		return 0 ;
//	}
//
//	if( sDataMgr.GetTable<PvPRankTable>()->CanFindByKey( playerId ) )
//	{
//		return 0 ;
//	}
//
//	PvPRankTable newRecord ;
//	newRecord.rank        =rank ;
//	newRecord.historyRank =rank ;
//	newRecord.playerId    =playerId ;
//	newRecord.accountId   =accountId ;
//	newRecord.regionId    =m_curSessionPairId.second ;
//	newRecord.serverId    =m_curSessionPairId.first ;
//
//	m_pvpRank.AddAndAddToCache( &newRecord ) ;
//
//	return rank ;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void SessionData::BrodCastCacheReceveMail(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg,std::set< uint64>& recIds)
{
	bool isNeedCheck = false;
	if(responMsg.mail().mail_type() == pb::MAIL_PLATGIFT || responMsg.mail().mail_type() == pb::MAIL_PLATFROM)
	{
		isNeedCheck = responMsg.is_server_mail();
	}
	uint32 lLv = responMsg.alv();
	uint32 hLv = responMsg.blv();
	uint32 lVLv = responMsg.avip();
	uint32 hVLv = responMsg.bvip();
	std::set< int> setChannel;
	Utility::SplitStr(responMsg.mail().channel(),setChannel,'|');
	bool checkChannel = false;
	if ((int)setChannel.size() > 0)
	{
		if((setChannel.count(0) == 0) && (setChannel.count(-1) == 0))
		{
			checkChannel = true;
		}
	}

	bool IsJustOnline = (responMsg.mail().send_type() == pb::MAIL_SEND_ONLINE);
	for( PlayerDataMapItr itr =m_playerMap.begin(); itr != m_playerMap.end(); ++itr )
	{
		PlayerDataPtr ptrPlayer = itr->second;
		if (isNeedCheck)
		{
			if (checkChannel)
			{
				if(setChannel.count(ptrPlayer->m_PlayerTable->channelId) == 0)
				{
					continue;
				}
			}
			uint32 curLv = ptrPlayer->m_PlayerTable->level;
			uint32 curVip = ptrPlayer->m_PlayerTable->vipLv;
			if( !(curLv >= lLv && curLv <= hLv && curVip >= lVLv && curVip <= hVLv))
			{
				continue;
			}
		}
		bool canSendMail = true;
		if (IsJustOnline && ptrPlayer->GetIsOffline())
		{
			canSendMail = false;
		}

		if (canSendMail)
		{
			if(ptrPlayer->AddMailAsync( responMsg.mail()))
			{
				if (!ptrPlayer->GetIsOffline())
				{
					pb::GS2C_Mail_MailList mailupdates;
					if(ptrPlayer->GetPlayerMailUpdate( mailupdates))
					{
						Send( pb::SD2G_CHECK_MAIL_RECIVERS_EXIST_RET, ptrPlayer->GetPlayerId(), mailupdates) ;
					}
				}
				recIds.insert(ptrPlayer->GetPlayerId());
			}
		}
		
	}
}


void SessionData::BrodCastReceveOfflineMail(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg,const std::set< uint64>& notIds)
{
	bool isNeedCheck = false;
	if(responMsg.mail().mail_type() == pb::MAIL_PLATGIFT || responMsg.mail().mail_type() == pb::MAIL_PLATFROM)
	{
		isNeedCheck = responMsg.is_server_mail();
	}
	std::set< int> setChannel;
	Utility::SplitStr(responMsg.mail().channel(),setChannel,'|');
	bool checkChannel = false;
	if ((int)setChannel.size() > 0)
	{
		if((setChannel.count(0) == 0) && (setChannel.count(-1) == 0))
		{
			checkChannel = true;
		}
	}
	int y =0, m =0, d =0 ;
	time_t loadLimit =::time( NULL ) - MAIL_NOLOGIN_CANT_RECEIVE_TIME;
	if( TimeT2Date( loadLimit, y, m, d ) )
	{
		std::string sql ;
		if (isNeedCheck)
		{
			uint32 lLv = responMsg.alv();
			uint32 hLv = responMsg.blv();
			uint32 lVLv = responMsg.avip();
			uint32 hVLv = responMsg.bvip();
			string channels;
			if(checkChannel && Utility::GetStringValueForSet(setChannel,channels,','))
			{
				Utility::FormatString( sql, "SELECT playerId FROM %s WHERE accountId != 0 AND lockReason != 2 AND level >= %u AND level <= %u AND vipLv >= %u AND vipLv <= %u AND regionId =%u  AND serverId =%u AND lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') AND channelId IN (%s) ORDER BY lastLogin DESC; "
					, PlayerTable::GetTableName(),lLv,hLv,lVLv,hVLv,m_curSessionPairId.second, m_curSessionPairId.first, y, m, d,channels.c_str()) ;
			}
			else
			{
				Utility::FormatString( sql, "SELECT playerId FROM %s WHERE accountId != 0 AND lockReason != 2 AND level >= %u AND level <= %u AND vipLv >= %u AND vipLv <= %u AND regionId =%u  AND serverId =%u AND lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') ORDER BY lastLogin DESC; "
					, PlayerTable::GetTableName(),lLv,hLv,lVLv,hVLv,m_curSessionPairId.second, m_curSessionPairId.first, y, m, d) ;
			}
			
		}
		else
		{
			Utility::FormatString( sql, "SELECT playerId FROM %s WHERE accountId != 0 AND lockReason != 2  AND regionId =%u AND serverId =%u AND lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') ORDER BY lastLogin DESC; "
				, PlayerTable::GetTableName(), m_curSessionPairId.second, m_curSessionPairId.first, y, m, d) ;
		}
		std::set< uint64> pIdSet;
		if(sDataMgr.GetPlayerIds(pIdSet,sql.c_str()))
		{
			std::set<uint64> AddMailSet;
			std::set_difference(pIdSet.begin(),pIdSet.end(),notIds.begin(),notIds.end(), std::inserter(AddMailSet,AddMailSet.end()));
			for (std::set<uint64>::iterator additer = AddMailSet.begin(); additer!= AddMailSet.end();++additer)
			{
				uint64 playerId = (*additer);
				AddOfflineMail(playerId,responMsg.mail());
			}
		}
	}
}


void SessionData::BrodCastPlayersReceveMail(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg,std::set< uint64>& recIds,bool jsOnline)
{
	bool isNeedCheck = false;
	if(responMsg.mail().mail_type() == pb::MAIL_PLATGIFT || responMsg.mail().mail_type() == pb::MAIL_PLATFROM)
	{
		isNeedCheck = responMsg.is_server_mail();
	}
	uint32 lLv = responMsg.alv();
	uint32 hLv = responMsg.blv();
	uint32 lVLv = responMsg.avip();
	uint32 hVLv = responMsg.bvip();
	std::set< int> setChannel;
	Utility::SplitStr(responMsg.mail().channel(),setChannel,'|');
	bool checkChannel = false;
	if ((int)setChannel.size() > 0)
	{
		if((setChannel.count(0) == 0) && (setChannel.count(-1) == 0))
		{
			checkChannel = true;
		}
	}

	for (std::set<uint64>::iterator additer = recIds.begin(); additer!= recIds.end();++additer)
	{
		uint64 playerId = (*additer);
		PlayerDataMapItr pItr =m_playerMap.find(playerId) ;
		if( pItr != m_playerMap.end() )
		{
			PlayerDataPtr ptrPlayer = pItr->second;
			if (isNeedCheck)
			{
				if (checkChannel)
				{
					if(setChannel.count(ptrPlayer->m_PlayerTable->channelId) == 0)
					{
						continue;
					}
				}
				uint32 curLv = ptrPlayer->m_PlayerTable->level;
				uint32 curVip = ptrPlayer->m_PlayerTable->vipLv;
				if( !(curLv >= lLv && curLv <= hLv && curVip >= lVLv && curVip <= hVLv))
				{
					continue;
				}
			}
			bool canSendMail = true;
			if (jsOnline && ptrPlayer->GetIsOffline())
			{
				canSendMail = false;
			}
			if(canSendMail)
			{
				if (ptrPlayer->AddMailAsync(responMsg.mail()))
				{
					if (!ptrPlayer->GetIsOffline())
					{
						pb::GS2C_Mail_MailList mailupdates;
						if(ptrPlayer->GetPlayerMailUpdate( mailupdates))
						{
							Send( pb::SD2G_CHECK_MAIL_RECIVERS_EXIST_RET, ptrPlayer->GetPlayerId(), mailupdates) ;
						}
					}
				}
			}
		}
		else
		{
			if(!jsOnline)
			{
				bool canSendMail = true;
				int y =0, m =0, d =0 ;
				time_t loadLimit =::time( NULL ) - MAIL_NOLOGIN_CANT_RECEIVE_TIME ;
				if( TimeT2Date( loadLimit, y, m, d ) )
				{
					if (isNeedCheck)
					{
						string channels;
						if(checkChannel && Utility::GetStringValueForSet(setChannel,channels,','))
						{
							canSendMail = PlayerTableCacheType::QueryIsTure("SELECT 1 FROM %s WHERE playerId =%llu AND lockReason != 2 AND level >= %u AND level <= %u AND vipLv >= %u AND vipLv <= %u  AND  lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') AND channelId IN (%s) LIMIT 1;"
								, PlayerTable::GetTableName(), playerId,lLv,hLv,lVLv,hVLv,y, m, d,channels.c_str());
						}
						else
						{
							canSendMail = PlayerTableCacheType::QueryIsTure("SELECT 1 FROM %s WHERE playerId =%llu AND lockReason != 2 AND level >= %u AND level <= %u AND vipLv >= %u AND vipLv <= %u  AND  lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') LIMIT 1;"
								, PlayerTable::GetTableName(), playerId,lLv,hLv,lVLv,hVLv,y, m, d);
						}
					}
					else
					{
						canSendMail = PlayerTableCacheType::QueryIsTure("SELECT 1 FROM %s WHERE playerId =%llu AND lockReason != 2  AND  lastLogin > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') LIMIT 1;"
							, PlayerTable::GetTableName(), playerId,  y, m, d);
					}
				}

				if (canSendMail)
				{
					AddOfflineMail(playerId,responMsg.mail());
				}
			}
		}
	}
}

void SessionData::ReceveMails(const pb::SD2G_Check_Mail_Recivers_Exist_Rspone& responMsg)
{
	bool IsJustOnline = (responMsg.mail().send_type() == pb::MAIL_SEND_ONLINE);
	if (responMsg.is_server_mail())
	{
		std::set< uint64> onlinePIds;
		BrodCastCacheReceveMail(responMsg,onlinePIds);
		if (!IsJustOnline)
		{
			BrodCastReceveOfflineMail(responMsg,onlinePIds);
		}
	}
	else
	{
		std::set< uint64> toSendSet;
		const SG2G_Mail_SendMail& platMail = responMsg.mail();
		std::copy(platMail.to_players().begin(),platMail.to_players().end(),std::inserter(toSendSet,toSendSet.begin()));
		BrodCastPlayersReceveMail(responMsg,toSendSet,IsJustOnline);
	}

}

bool SessionData::UpdateForbidenInfo(PlayerTable *pPlayerTable, const pb::SG2D_Forbiden_Player& info)
{
	bool modify = false;
	if (pPlayerTable)
	{
		switch(info.forbidid())
		{
		case  pb::ACCOUNT_OP_LOCK_SPEAK:
		case  pb::ACCOUNT_OP_LOCK:
			{
				if (info.forbidid() == pb::ACCOUNT_OP_LOCK_SPEAK)
				{
					pPlayerTable->lockReason = pb::ACCOUNT_LOCK_SPEAK;
				}
				else if (info.forbidid() == pb::ACCOUNT_OP_LOCK)
				{
					pPlayerTable->lockReason = pb::ACCOUNT_LOCK_TYPE_LOCKED;
				}
				if (info.time() > 0)
				{
					if (info.time() == 1)
					{
						pPlayerTable->lockTime = 1;
					}
					else
					{
						pPlayerTable->lockTime = sOS.GetRealTime() + (uint64)info.time();
					}
				}
				else
				{
					pPlayerTable->lockTime = 0;
				}
				modify = true;
			}
			break;
		case  pb::ACCOUNT_OP_UNLOCK:
		case  pb::ACCOUNT_OP_UNLOCK_SPEAK:
			{
				pPlayerTable->lockReason = pb::ACCOUNT_LOCK_TYPE_NONE;
				pPlayerTable->lockTime = 0;
				modify = true;
			}
			break;
		case  pb::ACCOUNT_OP_KICK_OFF_LINE:
			{
				return true;
			}
		}
	}
	return modify;
}



void SessionData::SetSession(Session *psession )
{
	m_curSession = psession;
	if (psession != NULL)
	{
		m_curSessionPairId = psession->GetPairId();
		m_curPlayerIdSeed  = GetGuidSeed( m_curSessionPairId.second, m_curSessionPairId.first, "player", "playerId", TYPE_PLAYER);
		OnGameServerRegist();
		m_syncToGamePIdList.clear();
		m_isSyncPTGFinish = false;
		PlayerDataMapItr endItr =m_playerMap.end() ;
		for( PlayerDataMapItr itr =m_playerMap.begin(); itr != endItr; ++itr )
		{
			PlayerDataPtr &pData =itr->second ;
			m_syncToGamePIdList.push_back(pData->GetPlayerId());
		}
	}
	else
	{
		m_isSyncPTGFinish = true;
		m_syncToGamePIdList.clear();
		m_curSessionPairId.first = 0;
		m_curSessionPairId.second = 0;
	}
}

void SessionData::UnLoadSessionData()
{
	m_curSession =NULL;
	m_playerTableList.clear();
	m_syncToGamePIdList.clear();
	m_CountryBaseTable.SaveMod();
	m_CountryBaseTable.RemoveAllEntry();
	for (int i=0; i< MONSTER_COUNTRY_ID;i++)
	{
		m_CountryCityTable[i].SaveMod();
		m_CountryCityTable[i].RemoveAllEntry();
	}
	for (int i=0; i< MAX_PLAYER_COUNTRY_ID;i++)
	{
		m_CountryLevelUpTaskPlayerTable[i].SaveMod();
		m_CountryLevelUpTaskPlayerTable[i].RemoveAllEntry();
		m_PalaceOfficeTable[i].SaveMod();
		m_PalaceOfficeTable[i].RemoveAllEntry();
	}
	time_t curTimeMS =sOS.TimeMS() ;
	for( PlayerDataMapItr itr =m_playerMap.begin(); itr != m_playerMap.end(); ++itr )
	{
		itr->second->SaveDirtyPart( curTimeMS ) ;
		sDataMgr.UnLoadPlayerData( itr->second.get() ) ;
		itr->second.reset();
	}
	m_playerMap.clear() ;
	m_isCachePlayerFinish = true;
	m_isSyncPTGFinish = true;
}

const SessionPairId& SessionData::GetPairId()
{
	return m_curSessionPairId;
}

void SessionData::Send( const int opCode, const ::google::protobuf::Message &msg ) const
{
	if (m_curSession)
	{
		m_curSession->Send(opCode,msg);
	}
}

void SessionData::Send( const int opCode, const uint64 sessionId, const ::google::protobuf::Message &msg ) const
{
	if (m_curSession)
	{
		m_curSession->Send(opCode,sessionId,msg);
	}
}

void SessionData::SavePlayerCache(uint64 playerId )
{
	PlayerDataMapItr pItr =m_playerMap.find(playerId) ;
	if( pItr != m_playerMap.end() )
	{
		PlayerDataPtr dataPtr = pItr->second;
		dataPtr->SaveDirtyPart(sOS.TimeMS());
	}
}

void SessionData::SaveAllPlayerCache()
{
	time_t curTimeMS =sOS.TimeMS() ;
	PlayerDataMapItr pItr =m_playerMap.begin();
	for(; pItr != m_playerMap.end();++pItr )
	{
		PlayerDataPtr dataPtr = pItr->second;
		dataPtr->SaveDirtyPart(curTimeMS);
	}
}

void SessionData::SaveAndRemoveAllPlayerCache()
{
	time_t curTimeMS =sOS.TimeMS() ;
	for( PlayerDataMapItr itr =m_playerMap.begin(); itr != m_playerMap.end(); ++itr )
	{
		itr->second->SaveDirtyPart( curTimeMS ) ;
		itr->second.reset();
	}
	m_playerMap.clear() ;
}


void SessionData::OnGameServerRegist()
{
	SyncCountryInfo();  //发送国家数据
	SyncPalaceInfo();   //发送皇城官职
	SyncCountryLevelUpTaskRankInfo(); //发送国家升级任务 排行榜
}

bool  SessionData::AddOfflineMail(uint64 playerId,const pb::SG2G_Mail_SendMail& sendMail)
{
	PlayerMailTable newPlayerMail;
	newPlayerMail.player_id = playerId;
	newPlayerMail.mail_id = sendMail.mail_id();
	newPlayerMail.mail_type = sendMail.mail_type();
	newPlayerMail.mail_state =0;//邮件状态0初始状态，1已收新邮件，2已读，3已读并已领附件
	newPlayerMail.send_time = ::time( NULL);
	newPlayerMail.items = sendMail.items();
	newPlayerMail.id = 0;
	return NULL != sDataMgr.SaveAndAddTableEntry<PlayerMailTable>(&newPlayerMail);
}

PlatfromMailTable* SessionData::AddNewPlatfromMailToTable( PlatfromMailTable &newMail )
{
	if (newMail.id > 0)
	{
		bool isExist = PlatfromMailTableCacheType::QueryIsTure("SELECT 1 FROM %s WHERE id =%llu LIMIT 1;", PlatfromMailTable::GetTableName(), newMail.id);
		if (isExist)
		{
			return NULL;
		}
	}
	//if ( sDataMgr.GetTable<PlatfromMailTable>()->GetMaxId() <= 20000 )
	//{
	//	sDataMgr.GetTable<PlatfromMailTable>()->SetMaxId(20000);
	//}
	newMail.region_id = m_curSessionPairId.second;
	newMail.server_id = m_curSessionPairId.first;
	return sDataMgr.SaveAndAddTableEntry<PlatfromMailTable>(&newMail);
}

void SessionData::SyncCountryInfo()
{
	pb::SD2G_Country_AllInfo msg;

	CachePtrMap<uint32, CountryBaseTable>::Iter it = m_CountryBaseTable.Begin();
	for (; it != m_CountryBaseTable.End(); ++it)
	{
		CountryBaseTable* table = it->second;
		if(table)
			table->SaveTo(*msg.add_base_info());
	}
	for (int i = 0; i < ARRAY_SIZE(m_CountryCityTable); ++i)
	{
		CachePtrMap<uint32, CountryCityTable>::Iter it = m_CountryCityTable[i].Begin();
		for (; it != m_CountryCityTable[i].End(); ++it)
		{
			CountryCityTable* table = it->second;
			if(table)
				table->SaveTo(*msg.add_city_lst());
		}
	}
	Send(pb::SD2G_COUNTRY_ALL_INFO, msg);
}
void SessionData::SyncCountryLevelUpTaskRankInfo()
{
	pb::Country_Levle_Up_Task_Rank msg;

	for (int i = 0; i < ARRAY_SIZE(m_CountryLevelUpTaskPlayerTable); ++i)
	{
		CachePtrMap<uint64, CountryLevelUpTaskPlayerTable>::Iter it = m_CountryLevelUpTaskPlayerTable[i].Begin();
		for (; it != m_CountryLevelUpTaskPlayerTable[i].End(); ++it)
		{
			CountryLevelUpTaskPlayerTable* table = it->second;
			if(table)
				table->SaveTo(*msg.add_player_lst());
		}
		msg.set_country(i + 1);
	}
	Send(pb::SD2G_COUNTRY_LEVLE_UP_TASK_RANK, msg);
}
void SessionData::SyncPalaceInfo()
{
	pb::GS2C_Palace_All_Info msg;
	for (int i = 0; i < ARRAY_SIZE(m_PalaceOfficeTable); ++i)
	{
		CachePtrMap<int, PalaceOfficeTable>::Iter it = m_PalaceOfficeTable[i].Begin();
		for (; it != m_PalaceOfficeTable[i].End(); ++it)
		{
			PalaceOfficeTable* table = it->second;
			if(table)
				table->SaveTo(*msg.add_office_lst());
		}
	}
	Send(pb::SD2G_PALACE_ALL_INFO, msg);
}

int SessionData::GetRegionId( void )
{
	return m_curSessionPairId.second ;
}

int SessionData::GetServerId( void )
{
	return m_curSessionPairId.first;
}