#pragma once

#include "IChatChannel.h"

class ChatChannel_BC : public IChatChannel
{
public:
	virtual bool Say(Player* speaker,const pb::C2GS_Chat& msg);

};