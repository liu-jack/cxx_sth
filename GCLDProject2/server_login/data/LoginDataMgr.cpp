
#include <stdlib.h>
#include <algorithm>
#include "../LoginServer.h"
#include "LoginDataMgr.h"
#include "MySqlSingleton.h"
#include "InterServer.pb.h"
#include "OS.h"


LoginDataMgr::LoginDataMgr( void )
{

}


LoginDataMgr::~LoginDataMgr( void )
{
	m_mutiAccountRoles.clear();
	m_accountInfoMap.clear();
}



bool LoginDataMgr::InitData( uint32 regionId )
{
	m_accountInfos.reset(new AccountInfoCacheType());
	m_accountInfos->SetDB( &sDatabase ) ;
	m_accountInfos->InitMaxId(regionId) ;

	m_accountRolesInfo.reset(new AccountRoleInfoCacheType());
	m_accountRolesInfo->SetDB(&sDatabase);
	m_accountRolesInfo->InitMaxId(regionId);

	const ConfigMgr &config =sLoginSvr.Config() ;

	int y =0, m =0, d =0 ;
	uint32 limitDays =config.GetInt("Self.max_account_cache_days_limit", 7) ;
	if( limitDays < 1 )
	{
		limitDays =1 ;
	}

	time_t loadLimit =::time( NULL ) - limitDays * 24 * 60 * 60 ;
	if( TimeT2Date( loadLimit, y, m, d ) )
	{
		const size_t limitCount = config.GetInt("Self.max_account_cache_count_limit", 3000) ;

		std::string sql ;
		Utility::FormatString( sql, "SELECT * FROM %s WHERE lastLoginTime > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') LIMIT %u ; "
			, AccountInfo::GetTableName(), y, m, d, limitCount ) ;

		std::vector<AccountInfo*> vecAccounts ;
		m_accountInfos->SqlLoadAndAddData( sql.c_str(), &vecAccounts ) ;
		for( std::vector<AccountInfo*>::iterator itr =vecAccounts.begin(); itr != vecAccounts.end(); ++itr )
		{
			AccountInfo* acInfo = (*itr);
			ActupleId acId(acInfo->channelId,acInfo->platformId,acInfo->account.c_str());
			m_accountInfoMap.insert(std::make_pair( acId, acInfo));
			NLOG( "account id: %llu, name: %s add to cache ~!", (uint64)(*itr)->id, (*itr)->account.c_str() ) ;
		}

		NLOG( "total %u account add to cache .", vecAccounts.size() ) ;
			
		const char* rtN = AccountRoleInfo::GetTableName();
		const char* atN = AccountInfo::GetTableName();
		Utility::FormatString( sql, "SELECT * FROM %s WHERE %s.accountId IN (SELECT id FROM %s WHERE %s.lastLoginTime > STR_TO_DATE( '%u-%u-%u','%%Y-%%m-%%d') ) LIMIT %u;",
			rtN,rtN,atN,atN, y, m, d, limitCount ) ;
		NLOG( "load roles sql:%s",sql.c_str() ) ;
		std::vector<AccountRoleInfo*> vecRoles;
		m_accountRolesInfo->SqlLoadAndAddData( sql.c_str(), &vecRoles);
		for( std::vector<AccountRoleInfo*>::iterator itr =vecRoles.begin(); itr != vecRoles.end(); ++itr )
		{
			bool isInsert = m_mutiAccountRoles.insert((*itr)).second;
			if( !isInsert )
			{
				NLOG( "insert muti roles:id: %llu, account id: %llu,player id: %llu, name: %s fail !",(uint64)(*itr)->id, (uint64)(*itr)->accountId,(uint64)(*itr)->playerId,(*itr)->name.c_str() ) ;
			}
		}
	}
	else
	{
		ELOG( " Login server add all account to cache~~~~!!!!!" ) ;
		std::vector<AccountInfo*> vecAccounts ;
		m_accountInfos->SqlLoadAndAddData(NULL,&vecAccounts) ;
		for( std::vector<AccountInfo*>::iterator itr =vecAccounts.begin(); itr != vecAccounts.end(); ++itr )
		{
			AccountInfo* acInfo = (*itr);
			ActupleId acId(acInfo->channelId,acInfo->platformId,acInfo->account.c_str());
			m_accountInfoMap.insert(std::make_pair( acId, acInfo));
		}
		std::vector<AccountRoleInfo*> vecRoles;
		m_accountRolesInfo->SqlLoadAndAddData(NULL,&vecRoles) ;
		for( std::vector<AccountRoleInfo*>::iterator itr =vecRoles.begin(); itr != vecRoles.end(); ++itr )
		{
			bool isInsert = m_mutiAccountRoles.insert((*itr)).second;
			if( !isInsert )
			{
				NLOG( "insert muti roles:id: %llu, account id: %llu,player id: %llu, name: %s fail !",(uint64)(*itr)->id, (uint64)(*itr)->accountId,(uint64)(*itr)->playerId,(*itr)->name.c_str() ) ;
			}
		}
	}
	return true ;
}

void LoginDataMgr::ReleaseAccount( AccountInfo *pAccountInfo)
{
	if( pAccountInfo == NULL )
	{
		return ;
	}
	int	remainCount =DeductAccountInfoUse( pAccountInfo->id ) ;
	ASSERT( remainCount >= 0 ) ;
	if( remainCount == 0 )
	{
		ActupleId acId(pAccountInfo->channelId,pAccountInfo->platformId,pAccountInfo->account.c_str());
		m_accountInfoMap.erase(acId);
		RemoveRoleInfoByAcId(pAccountInfo->id);
		m_accountInfos->RemoveAndSaveData( pAccountInfo ) ;
	}
}

void LoginDataMgr::SaveModifyAccount( AccountInfo* pInfo )
{
	m_accountInfos->WriteEntry(pInfo);
}


void LoginDataMgr::SaveModifyRoleInfo( AccountRoleInfo* pInfo )
{
	m_accountRolesInfo->WriteEntry(pInfo);
}

AccountRoleInfo* LoginDataMgr::AddAndSaveRoleInfo( AccountRoleInfo& pInfo )
{
	pInfo.id =m_accountRolesInfo->IncreaseMaxId() ;
	AccountRoleInfo* newRole = m_accountRolesInfo->SaveAndAddEntry( &pInfo ) ;
	if (newRole)
	{
		bool isInsert = m_mutiAccountRoles.insert(newRole).second;
		if( !isInsert )
		{
			NLOG( "AddAndSaveRoleInfo roles:id: %llu, account id: %llu,player id: %llu, name: %s fail !",newRole->id, newRole->accountId,newRole->playerId,newRole->name.c_str() ) ;
		}
	}
	return newRole;
}

AccountInfo* LoginDataMgr::AddNewAccount( AccountInfo &newAccount )
{
	newAccount.id =m_accountInfos->IncreaseMaxId() ;
	
	std::transform( newAccount.account.begin(), newAccount.account.end(), newAccount.account.begin(), ToUpper() );

	return m_accountInfos->SaveAndAddEntry( &newAccount ) ;
}

AccountInfo* LoginDataMgr::GetAccount(uint64 accoundId)
{
	AccountInfo *pInfo = NULL;
	if (accoundId > 0)
	{
		string sql ;
		pInfo =m_accountInfos->GetEntryByKey( accoundId ) ;
		if( pInfo == NULL )
		{
			std::vector<AccountInfo*> vec ;
			Utility::FormatString( sql, "SELECT * FROM %s WHERE id = %llu;", AccountInfo::GetTableName(), accoundId ) ;
			if( m_accountInfos->SqlLoadAndAddData( sql.c_str(), &vec, FlagReload ) )
			{
				pInfo =vec[0] ;
			}
		}
		if (pInfo)
		{
			ActupleId acId(pInfo->channelId,pInfo->platformId,pInfo->account.c_str());
			m_accountInfoMap.insert(std::make_pair( acId, pInfo));

			Utility::FormatString( sql, "SELECT * FROM %s WHERE  accountId = %llu;"
				, AccountRoleInfo::GetTableName(), pInfo->id ) ;
			NLOG( "load role sql:%s",sql.c_str() ) ;
			std::vector<AccountRoleInfo*> vecRoles;
			m_accountRolesInfo->SqlLoadAndAddData( sql.c_str(), &vecRoles, FlagReload ) ;
			for( std::vector<AccountRoleInfo*>::iterator itr =vecRoles.begin(); itr != vecRoles.end(); ++itr )
			{
				bool isInsert = m_mutiAccountRoles.insert((*itr)).second;
				if( !isInsert )
				{
					NLOG( "insert muti role:id: %llu, account id: %llu,player id: %llu, name: %s fail !",(uint64)(*itr)->id, (uint64)(*itr)->accountId,(uint64)(*itr)->playerId,(*itr)->name.c_str() ) ;
				}
			}
		}

		if (pInfo && pInfo->IsForbiden())
		{
			if (pInfo->lockTime > 1 && pInfo->lockTime < sOS.GetRealTime())
			{
				pInfo->lockReason = (uint32)pb::ACCOUNT_LOCK_TYPE_NONE;;
				pInfo->lockTime = 0;
				m_accountInfos->WriteEntry(pInfo);
			}
		}
	}

	return pInfo ;
}

AccountInfo* LoginDataMgr::GetAccount( uint32 channelId, uint32 platformId, std::string account )
{
	ActupleId acid(channelId,platformId,account);
	AccountInfo *pInfo = NULL;
	AccountInfoMap::iterator iterAcInfo = m_accountInfoMap.find(acid);
	if (iterAcInfo != m_accountInfoMap.end())
	{
		pInfo = iterAcInfo->second;
	}
	else
	{
		std::transform( account.begin(), account.end(), account.begin(), ToUpper() );
		string sql ;
		std::vector<AccountInfo*> vec ;
		Utility::FormatString( sql, "SELECT * FROM %s WHERE channelId = %d AND platformId = %d AND account LIKE '%s' LIMIT 1;", AccountInfo::GetTableName(),channelId,platformId,account.c_str()) ;
		if( m_accountInfos->SqlLoadAndAddData( sql.c_str(), &vec, FlagReload ) )
		{
			pInfo =vec[0] ;
		}
		if (pInfo == NULL)
		{
			if ((channelId == 0) && (platformId == 0))
			{
				Utility::FormatString( sql, "SELECT * FROM %s WHERE  account LIKE '%s' LIMIT 1;", AccountInfo::GetTableName(),account.c_str()) ;
				if( m_accountInfos->SqlLoadAndAddData( sql.c_str(), &vec, FlagReload ) )
				{
					pInfo =vec[0] ;
				}
			}
		}

		if (pInfo)
		{
			ActupleId acId(pInfo->channelId,pInfo->platformId,pInfo->account.c_str());
			m_accountInfoMap.insert(std::make_pair( acId, pInfo));

			Utility::FormatString( sql, "SELECT * FROM %s WHERE  accountId = %llu;"
				, AccountRoleInfo::GetTableName(), pInfo->id ) ;
			NLOG( "load role sql:%s",sql.c_str() ) ;
			std::vector<AccountRoleInfo*> vecRoles;
			m_accountRolesInfo->SqlLoadAndAddData( sql.c_str(), &vecRoles, FlagReload ) ;
			for( std::vector<AccountRoleInfo*>::iterator itr =vecRoles.begin(); itr != vecRoles.end(); ++itr )
			{
				bool isInsert = m_mutiAccountRoles.insert((*itr)).second;
				if( !isInsert )
				{
					NLOG( "insert muti role:id: %llu, account id: %llu,player id: %llu, name: %s fail !",(uint64)(*itr)->id, (uint64)(*itr)->accountId,(uint64)(*itr)->playerId,(*itr)->name.c_str() ) ;
				}
			}
		}
	}
	
	if (pInfo && pInfo->IsForbiden())
	{
		if (pInfo->lockTime > 1 && pInfo->lockTime < sOS.GetRealTime())
		{
			pInfo->lockReason = (uint32)pb::ACCOUNT_LOCK_TYPE_NONE;;
			pInfo->lockTime = 0;
			m_accountInfos->WriteEntry(pInfo);
		}
	}
	return pInfo ;

}

void LoginDataMgr::LockAccount( uint64 AccountId, const string& strName, uint32 type, time_t locktime)
{
	time_t      toLockTime = 0;
	uint32 toLockType = (uint32)pb::ACCOUNT_LOCK_TYPE_NONE;
	if (type == (uint32)pb::ACCOUNT_OP_LOCK)
	{ 
		toLockType = (uint32)pb::ACCOUNT_LOCK_TYPE_LOCKED;
		if (locktime == 1)
		{
			toLockTime = 1;
		}
		else if (locktime > 1)
		{
			toLockTime = sOS.GetRealTime() + locktime;
		}
	}
	else if (type == (uint32)pb::ACCOUNT_OP_UNLOCK)
	{ 
		toLockType = (uint32)pb::ACCOUNT_LOCK_TYPE_NONE;
		toLockTime = 0;
	}
	else
	{
		return;
	}

	AccountInfo *curAcountInfo = GetAccount( AccountId ) ;
	if( curAcountInfo)
	{
		curAcountInfo->lockReason = toLockType;
		curAcountInfo->lockTime = toLockTime;
		if (GetAccountInfoUse(AccountId) == 0)
		{
			ReleaseAccount(curAcountInfo);
		}
		else
		{
			m_accountInfos->WriteEntry(curAcountInfo);
		}
	}
}

void LoginDataMgr::GetRoleInfosByAccountId( uint64 accoundId, AccoutRoleInfoMap& mapRef )
{
	RolesByAccountId &byId =m_mutiAccountRoles.get<TAId>() ;
	RangeAccountIdRolesItr rangRoles = byId.equal_range( accoundId );
	for (RolesByAccountIdItr iter = rangRoles.first;  iter != rangRoles.second; ++iter)
	{
		AccountRoleInfo* info = *iter;
		SessionPairId pairId(info->serverId,info->regionId);
		mapRef[pairId] = info;
	}
}

AccountRoleInfo* LoginDataMgr::GetRoleInfoByPlayerId( uint64 playerId )
{
	RolesByPlayerId &byId =m_mutiAccountRoles.get<TPId>() ;
	RolesByPlayerIdItr iter = byId.find( playerId );
	if( iter != byId.end() )
	{
		return *iter;
	}
	else
	{
		return NULL;
	}
}

void LoginDataMgr::RemoveRoleInfoByAcId( uint64 accoundId )
{
	RolesByAccountId &byId = m_mutiAccountRoles.get<TAId>() ;
	RolesByAccountIdItr iter = byId.find(accoundId);
	if( iter != byId.end() )
	{
		AccountRoleInfo* curRole = *iter;
		if (curRole->IsModify())
		{
			m_accountRolesInfo->RemoveAndSaveData(curRole);
			curRole->ResetModify();
		}
		else
		{
			m_accountRolesInfo->RemoveKeyEntry(curRole);
		}
		byId.erase(iter);
		iter = byId.find(accoundId);
	}
}
