#pragma once
#include <string>
#include "ptr_container/PtrVector.h"
#include "def/TypeDef.h"
#include "Social.pb.h"
class IChatChannel;
class Player;
class ChatSpeaker
{
	typedef PtrVector<IChatChannel>	Channels;

public:
	ChatSpeaker();
	~ChatSpeaker();	

	bool Say
	(
		Player*				speaker, 
		const pb::C2GS_Chat& msg
	);
private:
	bool IsInMapChatCDTime();
private:
	Channels	m_channels;
	time_t		m_mapChatTimeStamp;
	uint32		m_mapChatCounter;
	
};