/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "ContinueOccupyCity/ContinueOccupyCityLog.h"
#include "Fuben.pb.h"

using namespace pb;
#ifdef _MMO_SERVER_
OpHandler(CMSG_CONTINUE_OCCUPY_CITY)
{
	pb::GS2C_ContinueOccupyCity msg;
	m_ContinueOccupyCityLog->SaveTo(msg);
	Send(SMSG_CONTINUE_OCCUPY_CITY_RSP,msg);
}
#endif