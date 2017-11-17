#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"
#include "AutoCountryWar.pb.h"

OpHandler(CMSG_QUICK_TRAINING)
{
	pb::C2GS_Quick_Training msg; pack >> msg;
	pb::GS2C_QuickTraining_Ret sendmsg;
	m_AutoCountryWarLog->QuickTraining(this, msg, sendmsg);
	Send(pb::SMSG_QUICK_TRAINING_RET, sendmsg);
}

OpHandler(CMSG_START_AUTO_COUNTRY_WAR)
{
	pb::C2GS_Start_Auto_Country_War msg; pack >> msg;
	pb::GS2C_Start_AC_Ret sendmsg;
	m_AutoCountryWarLog->StartAutoCountryWar(this, msg, sendmsg);
	Send(pb::SMSG_START_AC_RET, sendmsg);

	pb::GS2C_Auto_Country_War_Info infomsg;
	m_AutoCountryWarLog->SaveTo(infomsg);
	Send(pb::SMSG_GET_AUTO_COUNTRY_WAR_DATA, infomsg);
}

OpHandler(CMSG_REAUTO_COUNTRY_WAR)
{
	pb::C2GS_Reauto_Country_War msg; pack >> msg;
	pb::GS2C_REAUTO_HERO_Ret sendmsg;
	m_AutoCountryWarLog->ReautoHero(this,msg,sendmsg);
	Send(pb::SMSG_REAUTO_HERO_RET, sendmsg);

	pb::GS2C_Auto_Country_War_Info infomsg;
	m_AutoCountryWarLog->SaveTo(infomsg);
	Send(pb::SMSG_GET_AUTO_COUNTRY_WAR_DATA, infomsg);
}

OpHandler(CMSG_CHANGE_DESCITY)
{
	pb::C2GS_Change_Target_City msg; pack >> msg;
	pb::GS2C_Change_Des_Ret sendmsg;
	m_AutoCountryWarLog->SetDesCity(this,msg,sendmsg);
	Send(pb::SMSG_CHANGE_DESCITY_RET, sendmsg);

	pb::GS2C_Auto_Country_War_Info infomsg;
	m_AutoCountryWarLog->SaveTo(infomsg);
	Send(pb::SMSG_GET_AUTO_COUNTRY_WAR_DATA, infomsg);
}

OpHandler(CMSG_QUIT_AUTO_COUNTRY_WAR)
{
	m_AutoCountryWarLog->Stop(true);

	pb::GS2C_Auto_Country_War_Info infomsg;
	m_AutoCountryWarLog->SaveTo(infomsg);
	Send(pb::SMSG_GET_AUTO_COUNTRY_WAR_DATA, infomsg);
}

OpHandler(CMSG_GET_AUTO_COUNTRY_WAR_DATA)
{
	pb::GS2C_Auto_Country_War_Info sendmsg;
	m_AutoCountryWarLog->SaveTo(sendmsg);
	Send(pb::SMSG_GET_AUTO_COUNTRY_WAR_DATA, sendmsg);
}