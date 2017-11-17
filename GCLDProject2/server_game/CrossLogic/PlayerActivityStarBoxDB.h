/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#pragma once

#include "def/TypeDef.h"

struct  ActivityStarBoxStruct;
class Player;
class PlayerActivityStarBoxDb{
public:
	static void SendUpdateInfoToDb(Player& player,const uint32 table_id,const  ActivityStarBoxStruct &ref,uint32 operate_type);
	static void SendDeleteInfoToDb(uint32 operate_type);
};