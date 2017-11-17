#include "ChatChannel_World.h"
#include "Opcode.pb.h"
#include "Social.pb.h"
#ifdef  _MMO_SERVER_
#include "../GameServer.h"
#endif
bool  ChatChannel_World::Say( Player* speaker, const pb::C2GS_Chat& msg )
{
	pb::GS2C_Chat_Respon respon;
	FillChatMsg(speaker,msg,respon);
#ifdef  _MMO_SERVER_
	sGameSvr.BroadcastMsgToPlayer(pb::SMSG_CHAT_MESSAGE,respon);
#endif
	return true;
}