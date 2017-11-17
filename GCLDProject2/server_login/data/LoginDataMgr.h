#ifndef LOGIN_DATA_MGR_H__
#define LOGIN_DATA_MGR_H__

struct ToUpper
{
	int operator()(int c)  
	{  
		return toupper(c);  
	}
};

struct ToLower
{
	int operator()(int c)  
	{  
		return tolower(c);  
	}
};


#include "Singleton.h"

#include "MySqlTableCache.h"

#include "Account.h"
#include "AccountDataContainer.h"

TABLE_TYPEDEF( AccountInfo )
TABLE_TYPEDEF( AccountRoleInfo )


class LoginDataMgr: public Singleton<LoginDataMgr>
{
private:
	friend class Singleton<LoginDataMgr> ;

private:
	LoginDataMgr( void ) ;

public:
	~LoginDataMgr( void ) ;

public:
	bool InitData( uint32 regionId ) ;

	void ReleaseAccount( AccountInfo *pAccountInfo) ;
	void SaveModifyAccount( AccountInfo* pInfo );
	void SaveModifyRoleInfo( AccountRoleInfo* pInfo );
	AccountRoleInfo* AddAndSaveRoleInfo( AccountRoleInfo& pInfo );
	AccountInfo* AddNewAccount( AccountInfo &newAccount ) ;
	AccountInfo* GetAccount(uint64 accoundId) ;
	AccountInfo* GetAccount(uint32 channelId, uint32 platformId, std::string accound) ;

	void LockAccount( uint64 AccountId, const string& strName, uint32 type, time_t locktime);
	AccountInfoCache GetAccontTable( void ) {   return m_accountInfos ;   }

	void GetRoleInfosByAccountId(uint64 accoundId, AccoutRoleInfoMap& mapRef);
	AccountRoleInfo* GetRoleInfoByPlayerId(uint64 playerId);
	void RemoveRoleInfoByAcId(uint64 accoundId);



private:
	AccountInfoCache   m_accountInfos ;
	AccountRoleInfoCache m_accountRolesInfo;
	MutiAccountRoles   m_mutiAccountRoles;
	AccountInfoMap     m_accountInfoMap;

public:
	typedef uint64 AccountId ;
	int AddAcciontInfoUse( AccountId id ) {   return ++m_accountInfoUseCount[id] ;   }
	int DeductAccountInfoUse( AccountId id ){   int &value =m_accountInfoUseCount[id] ;   return value == 0 ? value : --value ;    }
	int GetAccountInfoUse(AccountId id) { return m_accountInfoUseCount[id]; }

public:
	std::map< AccountId, int > m_accountInfoUseCount ;
};


#define sDataMgr (LoginDataMgr::Instance())


#endif
