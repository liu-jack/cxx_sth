#include "chat_handler.h"

#include "OS.h"
#ifdef  _MMO_SERVER_
#include "GameServer.h"
#endif
#include "../object/Player.h"
#include "Opcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "Social.pb.h"
#include "GameServer.pb.h"
using namespace pb;
static const uint32 MAXOFLINECHATRECORDS = 10;
static const uint32 MAXCHATMESSAGESIZE = 200;
//extern SortPlayerDistanceNearBy;
///////////////class DynamicChannelInfo ////////////////////
ChatHandler::ChatHandler()
{
}

ChatHandler::~ChatHandler()
{

}

void ChatHandler::AddOfflineChat(GxDB_ChatInfoData& ofchat)
{
	uint32 nChatSize = ofchat.chat().chat_info().length();
	if (nChatSize > MAXCHATMESSAGESIZE)
	{
		return;
	}
	
	m_offLineChats.insert(std::make_pair(ofchat.chat().to_guid(),ofchat));
	OffLineChatMapItr playIter = m_offLineChats.begin();
	while(playIter != m_offLineChats.end())
	{
		GxDB_ChatInfoData& plchat = playIter->second;
        if (plchat.expire_time() < static_cast<uint64>( sOS.GetRealTime() ) )
		{
			m_offLineChats.erase(playIter++);
		}
		else
			++playIter;
	}

	uint32 nChatCount = m_offLineChats.count(ofchat.chat().to_guid());
	if (nChatCount > MAXOFLINECHATRECORDS)
	{
		OffLineChatMapItr toDelItr;
		bool bFind = false;
		uint64 curEarliestTime = sOS.GetRealTime()+OFFLINECHAT_TIME_SECOND;
		std::pair< OffLineChatMapItr, OffLineChatMapItr > pItr =m_offLineChats.equal_range( ofchat.chat().to_guid()) ;
		while( pItr.first != pItr.second )
		{
			GxDB_ChatInfoData& plchat = pItr.first->second;
			if (plchat.expire_time() < curEarliestTime)
			{
				curEarliestTime = plchat.expire_time();
				toDelItr = pItr.first;
				bFind = true;
				++pItr.first;
			}
			else
				++pItr.first;
		}
		if (bFind)
		{
			m_offLineChats.erase(toDelItr);
		}
	}

}

void ChatHandler::SendOfflineChats(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}
	OffLineChatMapItr playIter = m_offLineChats.find(pPlayer->GetGuid());
	if (playIter != m_offLineChats.end())
	{
		std::pair< OffLineChatMapItr, OffLineChatMapItr > pItr =m_offLineChats.equal_range( pPlayer->GetGuid()) ;
		while( pItr.first != pItr.second )
		{
			GxDB_ChatInfoData& plchat = pItr.first->second;
			GS2C_Chat_Respon respon;
			respon.CopyFrom(plchat.chat());
			respon.set_to_card_proid(pPlayer->GetLeaderProtoId());
			respon.set_to_vip_lv(pPlayer->GetVipLevel());
			respon.set_to_name(pPlayer->CharName());
#ifdef  _MMO_SERVER_
			sGameSvr.SendMsgToPlayer(plchat.chat().to_guid(),pb::SMSG_CHAT_MESSAGE,respon);
#endif
			m_offLineChats.erase(pItr.first++);
		}
	}
}
