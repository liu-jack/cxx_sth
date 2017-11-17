#ifndef FRIENDINFO_TABLE_H__
#define FRIENDINFO_TABLE_H__

#include "structDef.h"
namespace pb
{
	class SD2G_EventRecord;
	class SD2G_EventScheduler;
	class Friend_list_Record;
}
//set byte alignment 
#pragma pack(push,1)

enum FriendType
{
	LIST_FRIEND=0,
	LIST_IGNORES,
	LIST_INVITES,
	LIST_RECOMMENDS,
	LIST_ALL,
	LIST_ALL_BUT_RECOMMENDS,
	LIST_ALL_BUT_IGNORES,
};
class GameServerSession;
struct DB_FriendInfo : public BaseStruct
{
public:
	

public:
	INIT_GAMEDATA(DB_FriendInfo);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey( void ) const
	{
		return id ;
	}

	const Key GetLogicalKey( void ) const
	{
		return friend_id ;
	}
	

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "friend_guid" ;
	}

	static const char* GetTableName()
	{
		return "player_friend";
	}

	static const char* GetFormat()
	{
		return "bbbuuut";
	}
	uint32 GetPlayerId();
	uint32 GetFriendId();
	void InitData(uint64 playerId,uint64 friendId,uint32 friendtype);
	//void SaveTo(pb::Friend_list_Record &friendList,GameServerSession* pSession);
	void UpdateFrom(const pb::Friend_list_Record &friendList);
public:
	IdType      id ;
	uint64      player_id;
	uint64      friend_id;
	uint32		friend_type;
	uint32		is_send_ap;
	uint32		is_receive_ap;
	time_t      request_time;
} ;


//struct FriendInfo : public BaseStruct
//{
//public:
//	
//
//public:
//	INIT_GAMEDATA_AND_EXTRA(FriendInfo,extraData);
//public:
//	static const bool HasIndex()
//	{
//		return true;
//	}
//
//	const Key& GetKey( void ) const
//	{
//		return id;
//	}
//
//	const Key& GetLogicalKey( void ) const
//	{
//		return friend_guid;
//	}
//
//	static const char* GetDbIncreaseIdName()
//	{
//		return "id";
//	}
//
//	static const char* GetLogicalKeyName( void )
//	{
//		return "friend_guid" ;
//	}
//
//	static const char* GetTableName()
//	{
//		return "FriendInfo";
//	}
//
//	static const char* GetFormat()
//	{
//		return "bbbsuusuuutt";
//	}
//
//	uint32 GetFriendId();
//	uint32 GetVipLevel();
//	uint32 HeadIcon();
//	void   SaveTo(pb::Friend_list_Record &friendList);
//	void   LoadFrom(const pb::Friend_list_Record &friendList);
//	void   LoadFrom(const DB_FriendInfo &info);
//	void   WriteDBFriendInfo(DB_FriendInfo& info);
//
//	bool		IsModify() { return extraData.IsModify();}
//	void		SetModify() { extraData.SetModify();}
//	void		ResetModify() { extraData.ResetModify();}
//
//	void        SetOnline() { extraData.SetOnline();}
//	void        ResetOnline() { extraData.ResetOnline();}
//	bool        IsOnline()  { return extraData.IsOnline();}
//public:
//	IdType id;
//	uint64 player_guid;
//	uint64 friend_guid;
//	string friend_name;
//	//uint32 friend_id;
//	//uint32 head_icon;
//	//uint32 vip_level;
//	uint32 level;
//	uint32 power;
//	string guild_name;
//	uint32 friend_type;
//	uint32 is_send_ap;
//	uint32 is_receive_ap;
//	//bool   is_online;
//	time_t last_onlin_time; //最后在线时间
//	time_t request_time;//好友请求时间
//
//	ExtraDataStruct extraData;
//} ;

#pragma pack(pop)

#endif

