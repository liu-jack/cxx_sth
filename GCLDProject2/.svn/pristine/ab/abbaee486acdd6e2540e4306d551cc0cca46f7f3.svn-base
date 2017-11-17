#include "IChatChannel.h"
#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "def/MmoAssert.h"
#include "Social.pb.h"
#include "Opcode.pb.h"
#include "OS.h"

void IChatChannel::FillChatMsg( Player* speaker, const pb::C2GS_Chat& msg, pb::GS2C_Chat_Respon& respon)
{
	MMO_ASSERT (msg.type() >= pb::CHAT_COUNTRY && msg.type() < pb::CHAT_TYPE_COUNT);
	respon.set_type(msg.type());
	respon.set_chat_info(msg.chat_info());
	respon.set_facial_id(msg.facial_id());
	respon.set_to_name(msg.to_name());
	respon.set_to_vip_lv(msg.vip_level());
	respon.set_to_card_proid(msg.card_proid());
	respon.set_to_guid(msg.to_guid());

	respon.set_from_guid(speaker->GetGuid());
	respon.set_from_name(speaker->CharName());
	respon.set_from_vip_lv(speaker->GetVipLevel());
	respon.set_from_card_proid(speaker->GetLeaderProtoId());
	respon.set_send_time(sOS.GetRealTime());
}
