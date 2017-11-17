/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "def/TypeDef.h"

struct  GrowUpQuestStruct;
struct  GrowUpQuestGroupStruct;
class Player;
class PlayerGrowUpQuestDb {
public:
	static void SendUpdateInfoToDb(Player& player,const uint32 id,const  GrowUpQuestStruct &ref,uint32 operate_type);
	static void SendUpdateGroupInfoToDb(Player& player,const uint32 group_id,const GrowUpQuestGroupStruct& ref,uint32 operate_type);
	static void SendDeleteAllInfo(Player& player,uint32 operate_type);
};