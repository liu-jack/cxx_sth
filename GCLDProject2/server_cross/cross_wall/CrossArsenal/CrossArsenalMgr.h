#pragma once
#include "def/TypeDef.h"
#include "CrossWar.pb.h"

class Player;
namespace pb
{
	class GS2C_Cross_Ammo_Infos;
	class GS2C_Buy_Ammo_Rsp;
}


class CrossArsenalMgr
{
	typedef std::map<pb::ArsenalType, uint32> ArsenalRecMap;
public:
	CrossArsenalMgr();
	static CrossArsenalMgr& Instance(){ static CrossArsenalMgr T; return T; }

	void Init();

	void SaveTo(pb::GS2C_Cross_Ammo_Infos& msg, uint32 countryid);

	void BuyArsenalItems(Player& player, pb::ArsenalType type, pb::GS2C_Buy_Ammo_Rsp& msg);

	uint32 GetRestBuyTimes(pb::ArsenalType type, uint32 countryid);
public:
	std::map<uint32, ArsenalRecMap> m_arsenalRec;	//¹ú¼Ò£¬¼ÇÂ¼

};

#define sCrossArsenalMgr CrossArsenalMgr::Instance()