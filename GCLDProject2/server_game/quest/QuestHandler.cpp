#include "object/Player.h"
#include "quest/QuestLogger.h"
#include "memory_buffer/NetPack.h"
#include "Quest.pb.h"
#include "Opcode.pb.h"
#include "Enum.pb.h"
#include "Item.pb.h"


namespace PlayerHandle
{
    extern void SendOpcodeWithErrCode( Player* player, int opcode, pb::CxGS_ERROR_CODE err_code);
}

using namespace pb;

             
OpHandler( CMSG_ALL_QUEST)
{
    if ( !m_questLogger.get())
        return;
    GS2C_QuestList questList;
    m_questLogger->Save(this,questList);
    Send( SMSG_ALL_QUEST, questList);
}

OpHandler( CMSG_TAKE_QUEST_REWARD)
{
    if ( !m_questLogger.get())
        return;

    unsigned questId = 0;
    pack >> questId;
	uint32 player_exp = 0;
    pb::GS2C_LootResult msg;
    CxGS_ERROR_CODE code = (CxGS_ERROR_CODE)m_questLogger->TakeQuestReward(this,questId, *msg.mutable_loot_list(),player_exp);
	NLOG("ERROR_CODE is %d",code);
    msg.set_err_code( code);
    Send(SMSG_TAKE_QUEST_REWARD_RESP, msg);  
	AddXP(player_exp);
}
