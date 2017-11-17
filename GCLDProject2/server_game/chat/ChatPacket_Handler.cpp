#include "../object/Player.h"
#include "../GM/GmManager.h"
#include "memory_buffer/NetPack.h"
#include "Opcode.pb.h"
#include "ChatSpeaker.h"
using namespace pb;

OpHandler(CMSG_CHAT_MESSAGE)
{
	pb::C2GS_Chat msg;
	pack >> msg;

	if (IsShutUp())
	{
		SendErrorCode(pb::ErrBeShutUp);
		return;
	}

	CHAT_TYPE playerChatType = msg.type();
	if (playerChatType == CHAT_TYPE_COUNT)
	{
		if (msg.has_chat_info())
		{
			sGmMgr.HandleCommand(this, msg.chat_info());
		}
		return;
	}
	if (m_chatSpeaker.get())
	{
		if(m_chatSpeaker->Say(this,msg))
		{
			UpdateAllDailyQuestInfo(CHAT,1,0);//每日任务
		}
	}
}
