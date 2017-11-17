#include "ChatChannel_SysTem.h"

//#include "OS.h"
//#include "../object/Player.h"
//#include "memory_buffer/NetPack.h"
//#include "../chat/chat_handler.h"
#include "Social.pb.h"
#include "Opcode.pb.h"
#ifdef  _MMO_SERVER_
#include "GameServer.h"
#endif
//#include "../Social/SocialHandler.h"
bool  ChatChannel_SysTem::Say( Player* speaker, const pb::C2GS_Chat& msg )
{
	if (!speaker)
	{
		return false;
	}
	/*bool isBlackMan = sSocialHandler.IsBlackListPlayer(speaker->GetGuid(),msg.to_guid());
	if (isBlackMan)
	{
		return;
	}*/
//	pb::GS2C_Chat_Respon respon;
//	FillChatMsg(speaker,msg,respon);
//#ifdef  _MMO_SERVER_
//	if(sGameSvr.IsPlayerOnline(msg.to_guid()))
//	{
//		sGameSvr.SendMsgToPlayer(msg.to_guid(), pb::SMSG_CHAT_MESSAGE, respon);
//		speaker->Send(pb::SMSG_CHAT_MESSAGE,respon);
//	}
//	else
//#endif
//	{
//		speaker->Send(pb::SMSG_CHAT_MESSAGE,respon);
//		//´æÀëÏßÏûÏ¢¡£
//		pb::GxDB_ChatInfoData info;
//		pb::GS2C_Chat_Respon* chat = info.mutable_chat();
//		info.set_expire_time( sOS.GetRealTime() + OFFLINECHAT_TIME_SECOND);
//		chat->CopyFrom(respon);
//		sChatHandle.AddOfflineChat(info);
//	}

	pb::GS2C_Chat_Respon respon;
	FillChatMsg(speaker,msg,respon);
	#ifdef  _MMO_SERVER_
		sGameSvr.BroadcastMsgToPlayer(pb::SMSG_CHAT_MESSAGE,respon);
	#endif
	return true;
}
