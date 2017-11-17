#pragma once

#include <string>
#include "def/TypeDef.h"
#include "Social.pb.h"
class Player;
class NetPack;

class IChatChannel
{
public:
	virtual ~IChatChannel(){}

	virtual bool Say(Player* speaker, const pb::C2GS_Chat& msg) { return false;}

protected:
	virtual void FillChatMsg(Player* speaker, const pb::C2GS_Chat& msg, pb::GS2C_Chat_Respon& respon);
};