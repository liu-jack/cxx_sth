#pragma once

#include "IChatChannel.h"

class ChatChannel_SysTem : public IChatChannel
{
public:
	virtual bool Say(Player* speaker,const pb::C2GS_Chat& msg);

};