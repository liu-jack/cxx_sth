#include "GiftCodeTable.h"
#include "Web.pb.h"
#include "utility/TimeParser.h"
#include "utility/Utility.h"

void GiftCodeTable::LoadFrom( const pb::CreateGiftCode& info )
{
   giftId = info.giftid();
   giftCode.clear();
   serverId = 0;
   playerId = 0;
   platNo = info.platno();
   useTime = 0;
   mark = info.mark();
}

bool GiftCodeTable::CheckChannel( int channel )
{
	std::set<int> retSet;
	Utility::SplitStr(platNo, retSet, '|');
	if (retSet.find(channel) != retSet.end() || retSet.find(-1) != retSet.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GiftCodeTableUse::LoadFrom( const GiftCodeTable& table )
{
	id = table.id;
	giftId = table.giftId;
	giftCode = table.giftCode;
	serverId = table.serverId;
	playerId = table.playerId;
	platNo = table.platNo;
	useTime = table.useTime;
}
