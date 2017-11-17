#ifndef GUILDINFO_TABLE_H__
#define GUILDINFO_TABLE_H__

#include "structDef.h"
namespace pb
{
	class GxDB_GuildInfo;
	class GxDB_GMember_Info;
}
//set byte alignment 
#pragma pack(push,1)

struct GuildInfoTable : public BaseStruct
{
public:
	INIT_GAMEDATA_AND_EXTRA(GuildInfoTable,extraData);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey( void ) const
	{
		return guild_id;
	}

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetTableName()
	{
		return "guilds";
	}

	static const char* GetFormat()
	{
		return "bbuuustuuuustbsbsuuuts";
	}

	void   SaveTo(pb::GxDB_GuildInfo &Info);
	void   LoadFrom(const pb::GxDB_GuildInfo &Info);

	bool		IsModify() { return extraData.IsModify();}
	void		SetModify() { extraData.SetModify();}
	void		ResetModify() { extraData.ResetModify();}

public:
	IdType      id ;
	uint64		guild_id;
	uint32      region_id;
	uint32      server_id;
	uint32      icon;
	string      name;
	time_t		create_time;
	uint32		level;
	uint32		exp;
	uint32		prestige;
	uint32		active;
	string      notice;
	time_t      notice_time;
	uint64      notice_creater_id;
	string      notice_creater_name;
	uint64		guild_owner_id;
	string		guild_owner_name;
	uint32		members_count;
	uint32      members_limit;
	uint32		score;
	time_t		last_rank_time;
	string      logs;

	ExtraDataStruct extraData;
} ;


struct GuildMemberInfoTable: public BaseStruct
{
public:
	INIT_GAMEDATA_AND_EXTRA(GuildMemberInfoTable,extraData);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return id;
	}

	const Key GetLogicalKey( void ) const
	{
		return player_id;
	}

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "player_id" ;
	}

	static const char* GetTableName()
	{
		return "guild_member";
	}

	static const char* GetFormat()
	{
		return "bbsbsuttuuucctuuss";
	}

	void   SaveTo(pb::GxDB_GMember_Info &member);
	void   LoadFrom(const pb::GxDB_GMember_Info &member);

	bool		IsModify() { return extraData.IsModify();}
	void		SetModify() { extraData.SetModify();}
	void		ResetModify() { extraData.ResetModify();}

	void        SetOnline() { extraData.SetOnline();}
	void        ResetOnline() { extraData.ResetOnline();}
	bool        IsOnline()  { return extraData.IsOnline();}
public:
	IdType id;
	uint64 player_id;
	string player_name;
	uint64 guild_id;
	string guild_name;
	uint32 title;
	time_t join_time;
	time_t last_active_time;
	uint32 guild_contributions;
	uint32 level;
	uint32 vip_level;
	bool   is_requester;
	bool   is_inviter;
	time_t request_expire_time;
	uint32 power;
	uint32 icon;
	string is_send_ap;
	string is_receive_ap;

	ExtraDataStruct extraData;
} ;

#pragma pack(pop)

#endif

