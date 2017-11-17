#ifndef ACCOUNT_H__
#define ACCOUNT_H__

#include "structDef.h"


namespace pb
{
	class CRegisterAccount ;
	class SG2L_Update_Account_Login;
	class RoleInfo;
}

//set byte alignment 
#pragma pack(push,1)

struct AccountInfo : public BaseStruct
{
public:
	INIT_GAMEDATA( AccountInfo ) ;

public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return id ;
	} 

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "id" ;
	}

	static const char* GetTableName()
	{
		return "accounts";
	}

	static const char* GetFormat()
	{
		return "bssssttuusuuutu";
	}

	void ReadFrom( const pb::CRegisterAccount &info ) ;
	bool IsForbiden() const;

public:
	IdType      id ;
	std::string account ;
	std::string password ;
	std::string deviceId ;
	std::string phoneNumber ;
	time_t      regTime ;
	time_t      lastLoginTime ;
	uint32      channelId ;
	uint32      platformId ;
	std::string lastIp ;
	uint32      lastRegionId ;
	uint32      lastServerId ;
	uint32      lockReason ;
	time_t      lockTime;
	uint32      accountType ;

} ;


struct AccountRoleInfo : public BaseStruct
{
public:
	INIT_GAMEDATA_AND_EXTRA( AccountRoleInfo,extraData) ;

public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return id ;
	} 

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "id" ;
	}

	static const char* GetTableName()
	{
		return "roles";
	}

	static const char* GetFormat()
	{
		return "bbbuusuuut";
	}

	uint64 GetPlayerId() const
	{
		return playerId;
	}
	uint64 GetAccountId() const
	{
		return accountId;
	}

	void ReadFrom( const pb::SG2L_Update_Account_Login &info ) ;
	void Saveto( pb::RoleInfo &info ) ;
	bool		IsModify() { return extraData.IsModify();}
	void		SetModify() { extraData.SetModify();}
	void		ResetModify() { extraData.ResetModify();}


public:
	IdType      id ;
	uint64      accountId;
	uint64      playerId;
	uint32      regionId ;
	uint32      serverId ;
	std::string name ;
	uint32      headicon ;
	uint32      level;
	uint32      viplevel;
	time_t      lastLogin;

	ExtraDataStruct extraData;
} ;

#pragma pack(pop)

#endif

