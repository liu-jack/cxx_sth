/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once

#include "def/TypeDef.h"
#include "DailyQuest/daily_quest_def.h"
struct  DailyQuestStruct;
class Player;
class DailyQuestDb {
public:
	static void SendUpdateInfoToDb(Player& player,const uint32 quest_id,const  DailyQuestStruct &ref);
	static void SendUpdateInfoToDb(Player& player,const uint32 quest_id);
};