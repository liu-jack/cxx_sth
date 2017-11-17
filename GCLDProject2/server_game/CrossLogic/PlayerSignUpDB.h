/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma once
#include "def/TypeDef.h"
struct SignUpStruct;
class Player;
class PlayerSignUpDB
{
public:
	static void SendInfoToDB(Player& player,uint32 day,SignUpStruct& ref);
};