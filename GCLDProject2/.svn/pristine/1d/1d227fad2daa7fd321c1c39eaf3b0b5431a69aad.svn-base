#include "ChatChannel_Country.h"
#include "../object/Player.h"
//#include "../Guild/GuildHandler.h"
#include "Opcode.pb.h"
#include "Social.pb.h"
#ifdef  _MMO_SERVER_
#include "../GameServer.h"
#endif
bool ChatChannel_Country::Say( Player* speaker, const pb::C2GS_Chat& msg )
{
	if (!speaker)
	{
		return false;
	}
	//if (!speaker->GetGuildId())
	//{
	//	speaker->SendGError(NotInGuild);
	//	return;
	//}
	//pb::GS2C_Chat_Respon respon;
	//FillChatMsg(speaker,msg,respon);
	//sGuildHandler.HandleGuildChat(respon);
	pb::GS2C_Chat_Respon respon;
	FillChatMsg(speaker,msg,respon);
	#ifdef  _MMO_SERVER_
		sGameSvr.BroadcastMsgToCountryPlayer(speaker->GetCountryId(),pb::SMSG_CHAT_MESSAGE,respon);
	#endif
	return true;
}
