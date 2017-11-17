#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Dungeon.pb.h"
#include "PlayerDungeonLog.h"
#include "Table/DungeonTableMgr.h"
#include "../character/CharacterStorage.h"
using namespace pb;

OpHandler(CMSG_DUNGEON_ALL_INFO)
{
    GS2C_Dungeon_All_Info msg;
    m_DungeonLog->SaveTo(msg);
    Send(SMSG_DUNGEON_ALL_INFO, msg);
}
OpHandler(CMSG_DUNGEON_BUILD_BY_TOKEN)
{
    m_DungeonLog->Build_Token(*this);
}
OpHandler(CMSG_DUNGEON_BUILD_BY_SILVER)
{
    C2GS_Dungeon_Build_By_Silver msg; pack >> msg;

    m_DungeonLog->Build_Silver(*this, msg.is_fast());
}
OpHandler(CMSG_DUNGEON_LEVELUP)
{
    m_DungeonLog->LevelUp(*this);
}
OpHandler(CMSG_DUNGEON_CANE)
{
	uint32 player_exp = 0;
	pb::GS2C_Dungeon_Cane msg;
    msg.set_success(m_DungeonLog->Cane(*this,player_exp));
	msg.set_player_exp(player_exp);
	Send(pb::SMSG_DUNGEON_CANE_RSP,msg);
	AddXP(player_exp);
}
OpHandler(CMSG_DUNGEON_CANE_FAST_LEVELUP)
{
    C2GS_Dungeon_Cane_Fase_LevelUp msg; pack >> msg;

    m_DungeonLog->CaneFastLevelUp(*this, msg.dst_lv());
}
OpHandler(CMSG_DUNGEON_ESCAPE)
{
    C2GS_Dungeon_Escape msg; pack >> msg;

    int idx = msg.hero_idx();
    if (Character* hero = m_characterStorage->MutableCharacter(idx))
    {
        int seconds = hero->EscapeFromDungeon();

        GS2C_Dungeon_Escape msgBack;
        msgBack.set_hero_tid(hero->GetID());
        msgBack.set_need_second(seconds);
        Send(SMSG_DUNGEON_ESCAPE, msgBack);
    }
}
OpHandler(CMSG_DUNGEON_TRY_CANE)
{
    C2GS_Dungeon_Try_Cane req; pack >> req;

    m_DungeonLog->TryCane(*this, req.dst_lv());
}