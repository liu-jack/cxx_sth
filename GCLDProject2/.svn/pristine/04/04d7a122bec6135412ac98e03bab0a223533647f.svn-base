#include "object/Player.h"
#include "memory_buffer/NetPack.h"
#include "Logger.h"


#include "CrossLogic/PlayerOperationLogic.h"

#include "Opcode.pb.h"

using namespace pb;

namespace PlayerHandle
{
    extern void SendOpcodeWithErrCode( Player* player, int opcode, pb::CxGS_ERROR_CODE err_code);
}



OpHandler( CMSG_START_AUTO_RECOVER_HP)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerOperationLogic::HandlePlayerStartAutoIncreaseSolders( *this); 
	PlayerHandle::SendOpcodeWithErrCode( this, SMSG_STOP_AUTO_RECOVER_HP,  err_code);
}
