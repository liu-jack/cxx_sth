#include "../object/Player.h"
#include "stage/StageLogger.h"

#include "CrossLogic/PlayerStageLogic.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"

#include "Opcode.pb.h"
#include "Stage.pb.h"
using namespace pb;

#define  PRE_JUDGE_INIT_STAGE_LOGGER  if(!m_stageLogger.get()) { return;}
OpHandler(CMSG_ALL_STAGE_INFO)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	GS2C_AllStageInfo reponMsg;
	m_stageLogger->SaveStageLoggerTo(reponMsg);
	Send(SMSG_ALL_STAGE_INFO, reponMsg);
}

OpHandler(CMSG_PRE_BATTLE)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	uint32 stage_level_id;
	pack >> stage_level_id;

    int preBattleId = 0;
    
	uint32 go_battle_state = m_stageLogger->CheckGoBattleState(stage_level_id, *this);
    if ( go_battle_state == pb::ErrCommonSuccess)
    {
        preBattleId = GenPrebattleID();        
    }
    else
    {
        ResetPrebattleId();
    }

	NetPack packMsg((uint16)SMSG_PRE_BATTLE_RESULT);
	packMsg << go_battle_state << preBattleId;
	Send(packMsg);
}

OpHandler(CMSG_BATTLE_DATA)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	C2GS_BattleData battleData;
	pack >> battleData;
   
    // 注释掉以下 检测的原因 是因为在 CMSG_PRE_BATTLE 的时候已经检测过， 并产生了个 prebattle id
    //if ( m_stageLogger->CheckGoBattleState( battleData.stage_level_id(), *this) != ErrCommonSuccess)
    //    return;

    GS2C_BattleResult battleResult;
    battleResult.set_err_code( pb::ErrCommonFail);
    PlayerStageLogic::CalculateBattleResult( *this, battleData, battleResult);
    Send(SMSG_BATTLE_RESULT, battleResult);
	
}
