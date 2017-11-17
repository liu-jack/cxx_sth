#include "ChatChannel_BC.h"
#include "Social.pb.h"
#include "Opcode.pb.h"
#include "object/Player.h"
#ifdef  _MMO_SERVER_
#include "GameServer.h"
#endif

bool  ChatChannel_BC::Say( Player* speaker, const pb::C2GS_Chat& msg )
{
	if (!speaker)
	{
		return false;
	}

	pb::GS2C_Chat_Respon respon;
	FillChatMsg(speaker,msg,respon);
	#ifdef  _MMO_SERVER_
		//sGameSvr.SendMsgToPlayer(speaker->GetGuid(),pb::SMSG_CHAT_MESSAGE,respon);
		speaker->Send(pb::SMSG_CHAT_MESSAGE,respon);
	#endif
	return true;
}