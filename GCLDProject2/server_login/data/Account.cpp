
#include "Account.h"

#include "Login.pb.h"
#include "InterServer.pb.h"

void AccountInfo::ReadFrom( const pb::CRegisterAccount &info )
{
	account       =info.account() ;
	password      =info.password() ;
	deviceId      =info.device_id() ;
	phoneNumber   =info.phone_number() ;
	channelId     =info.channel_id() ;
	platformId    =info.platform_id() ;
	accountType   =info.type();
}

bool AccountInfo::IsForbiden() const
{
	return lockReason == (uint32)pb::ACCOUNT_LOCK_TYPE_LOCKED;
}

void AccountRoleInfo::ReadFrom( const pb::SG2L_Update_Account_Login &info )
{
	accountId = info.account_id();
	playerId = info.player_id();
	regionId = info.region_id();
	serverId = info.server_id();
	name = info.name();
	headicon = info.headicon();
	level = info.level();
	viplevel = info.viplevel();
	lastLogin = info.login_time();
}

void AccountRoleInfo::Saveto( pb::RoleInfo &info )
{
	info.set_name(name);
	info.set_server_id(serverId);
	info.set_level(level);
	info.set_viplevel(viplevel);
	info.set_headicon(headicon);
	info.set_login_time(lastLogin);
}
