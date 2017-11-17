//跨服战的消息
#include "Opcode.pb.h"
#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Map/country/country.h"
#include "Map/WorldMap.h"
#include "CrossWar.pb.h"

using namespace pb;

OpHandler(CMSG_CROSS_WAR_STATE)
{
	GS2C_Notice_CrossWar_State sendmsg;
	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
	{
		sendmsg.set_curstate(country->GetCrossState());
		sendmsg.set_stateendtime(country->cross_war_state_end_time);
		LLOG("[1002] Notice All Client");
	}
	else
	{
		sendmsg.set_curstate((CROSS_STATE)0);
		sendmsg.set_stateendtime(0);
	}
	Send(SMSG_NOTICE_CROSS_UPDATE,sendmsg);
}

OpHandler(CMSG_DONATION_GOODS)
{
	GC2S_Donate_Goods getMsg; pack >> getMsg;

	uint32 donate_type = getMsg.item_type();

	GS2C_DONATE_RET msg;
	msg.set_item_type(Office_Troops);
	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
	{
		LLOG("[1002] get country");
		if(country->GetCrossState() == State_Prepare)		//该玩家所属国家处于筹备阶段
		{
			LLOG("[1002] state correct");
			country->SendCrossServerNetPack(pack);
			return;
		}
		msg.set_ret(Cro_War_State_Dismatch);
	}
	msg.set_ret(Cro_Common_Err);
	Send(SMSG_DONATION_GOODS_RET,msg);
}

//OpHandler(CMSG_DONATION_TROOPS_ORDER)
//{
//	LLOG("[1002] handle CMSG_DONATION_TROOPS_ORDER");
//	CR2C_DONATE_TROOPS_RET msg;
//	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
//	{
//		LLOG("[1002] get country");
//		if(country->GetCrossState() == State_Prepare)		//该玩家所属国家处于筹备阶段
//		{
//			LLOG("[1002] state correct");
//			country->SendCrossServerNetPack(pack);
//			return;
//		}
//		msg.set_ret(Cro_War_State_Dismatch);
//	}
//	msg.set_ret(Cro_Common_Err);
//	Send(SMSG_DONATION_TROOPS_ORDER_RET,msg);
//}
//
//OpHandler(CMSG_DONATION_SHADOW)
//{
//	LLOG("[1002] handle CMSG_DONATION_SHADOW");
//	CR2C_DONATE_SHADOW_RET msg;
//	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
//	{
//		LLOG("[1002] get country");
//		if(country->GetCrossState() == State_Prepare)		//该玩家所属国家处于筹备阶段
//		{
//			LLOG("[1002] state correct");
//			country->SendCrossServerNetPack(pack);
//			return;
//		}
//		msg.set_ret(Cro_War_State_Dismatch);
//	}
//	msg.set_ret(Cro_Common_Err);
//	Send(SMSG_DONATION_SHADOW_RET,msg);
//}