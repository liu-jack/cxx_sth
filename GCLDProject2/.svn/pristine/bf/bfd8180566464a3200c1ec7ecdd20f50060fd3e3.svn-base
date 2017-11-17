/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef INCLUDE_CONGRATULATE_DB_H
#define INCLUDE_CONGRATULATE_DB_H
#include "def/TypeDef.h"
namespace pb
{
	class C2GS_Congratulate_Accept;
}
class Player;
class CongratulateLog;
class CongratulateLogDB
{
public:
	static void SendInfoToSelfCongratulateDb(Player& player, CongratulateLog& log,const uint64 act_time,const uint32 operate_type);
	static void SendInfoToCongratulateBeenDb(Player& player, const pb::C2GS_Congratulate_Accept& msg);
};



#endif ////INCLUDE_CONGRATULATE_DB_H