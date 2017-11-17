#pragma once
#include "def/TypeDef.h"

typedef uint32 ProgramID;
typedef uint32 VictimType;
typedef uint32 SellId;

enum SELL_VICTIM_TYPE
{
     SELL_VICTIM_NORMAL = 0,
     SELL_VICTIM_HERO   = 1,
     SELL_VICTIM_EQUIP  = 2,
     SELL_VICTIM_HONOR  = 3,
     SELL_VICTIM_BOUNTY = 4,
     SELL_VICTIM_GUILD  = 5, 
     SELL_VICTIM_AWAKE  = 6,
     SELL_VICTIM_PROMO  = 7,
     SELL_VICTIM_VIP_GIFT_ONCE = 8,
     SELL_VICTIM_VIP_GIFT_CYCLE = 9,
     SELL_VICTIM_COUNT,
};
