#include "ChatSpeaker.h"
#include "IChatChannel.h"
#include "ChatChannel_Country.h"
#include "ChatChannel_World.h"
#include "ChatChannel_SysTem.h"
#include "ChatChannel_BC.h"
#include "def/MmoAssert.h"
#include "Opcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "object/Player.h"
ChatSpeaker::ChatSpeaker()
:m_channels(pb::CHAT_TYPE_COUNT)
{
	m_mapChatTimeStamp =0 ;
	m_mapChatCounter   =0 ;

	m_channels[pb::CHAT_COUNTRY]= new ChatChannel_Country(); // todo
	m_channels[pb::CHAT_WORLD]= new ChatChannel_World();
	m_channels[pb::CHAT_SYSTEM]= new ChatChannel_SysTem();
	m_channels[pb::CHAT_COMBAT_BC]= new ChatChannel_BC();
}

ChatSpeaker::~ChatSpeaker()
{
}

bool ChatSpeaker::Say(Player* speaker, const pb::C2GS_Chat& msg )
{
	MMO_ASSERT(speaker);

	if (msg.type() < pb::CHAT_COUNTRY || msg.type() >= pb::CHAT_TYPE_COUNT)
	{
		// Hacker
		return false;
	}

	if(IsInMapChatCDTime())
	{
		if( m_mapChatCounter >= 6 )//static const uint32 s_map_chat_max_counter = 6;
		{
			NetPack pck(pb::SMSG_SERVERCHAT_CDTIME_ERROR);
			pck << static_cast<uint32>( msg.type() ) ;
			speaker->Send(pck);
			return false;
		}
		else
		{
			++m_mapChatCounter;
		}
	}
	else
	{
		m_mapChatCounter = 1;
	}

	return m_channels[msg.type()]->Say(speaker, msg);
}


bool ChatSpeaker::IsInMapChatCDTime()
{
	bool bRtn = true;
	if( m_mapChatTimeStamp != 0 )
	{
		time_t currTime = sOS.GetRealTime();
		if( currTime - m_mapChatTimeStamp > 30 )//time_t s_map_chat_interval = 30;
		{
			m_mapChatTimeStamp = currTime;
			bRtn = false;
		}
	}
	else
	{
		m_mapChatTimeStamp = sOS.GetRealTime();
		bRtn = false;
	}
	return bRtn;
}
