/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "def/TypeDef.h"

namespace pb
{
	class WannaBeStrongerInfo;
}
class Player;
struct BeStrongerInfo;
class GeneralRewardDB {
public:
	static void SendUpdateInfoToDb(Player& player,const uint32 charId,bool has_first_reward,uint32 operate_type);
	static void SendDeleteInfoToDb(uint32 operate_type);
};

class WannaBeStrongerDB{
public:
	static void SendUpdateInfoToDb(Player& player,pb::WannaBeStrongerInfo& msg);
	static void SendUpdateInfoToDb(Player& player,const uint32 groupId,const BeStrongerInfo& ref);
};
