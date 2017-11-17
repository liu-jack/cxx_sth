#include "FriendInfo.h"
#include "Social.pb.h"
#include "../PlayerBaseData.h"
#include "../session/GameServerSession.h"
//#include "def/TypeDef.h"
uint32 DB_FriendInfo::GetPlayerId()
{
	ObjGUID curGuid(player_id);
	return Guid::LowGuid(curGuid);
}

uint32 DB_FriendInfo::GetFriendId()
{
	ObjGUID curGuid(friend_id);
	return Guid::LowGuid(curGuid);
}

void DB_FriendInfo::UpdateFrom(const pb::Friend_list_Record &friendList)
{
	friend_id = friendList.friend_guid();
	friend_type = friendList.friend_type();
	is_send_ap = friendList.is_send_ap();
	is_receive_ap = friendList.is_receive_ap();
	request_time = friendList.request_time();
}

//void DB_FriendInfo::SaveTo( pb::Friend_list_Record &friendList,GameServerSession* pSession )
//{
//	if (pSession)
//	{
//		pSession->WriteFriendInfo(friend_id,friendList);
//	}
//	friendList.set_friend_guid(friend_id);
//	friendList.set_friend_type(friend_type);
//	friendList.set_is_send_ap(is_send_ap > 0 ? true:false);
//	friendList.set_is_receive_ap(is_receive_ap > 0 ? true:false);
//	friendList.set_request_time(request_time);
//}

void DB_FriendInfo::InitData( uint64 playerId,uint64 friendId,uint32 friendtype )
{
	id = 0;
	player_id = playerId;
	friend_id = friendId;
	friend_type = friendtype;
	is_send_ap = 0;
	is_receive_ap = 0;
	request_time = time(NULL);
}
