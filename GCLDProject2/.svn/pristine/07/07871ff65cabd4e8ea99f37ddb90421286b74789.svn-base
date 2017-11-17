#include "../object/Player.h"
#include "MailPacket_Handler.h"
#include "MailBox.h"
#include "memory_buffer/NetPack.h"
#include "utility/Utility.h"
#include "Web.pb.h"
#include "../server_client/CenterServerClient.h"
#include "ServerOpcode.pb.h"

OpHandler(CMSG_MAIL_GETNEWMAILCOUNT)
{
	m_mailBox->HandleGetNewMailCount(this);
}

OpHandler(CMSG_MAIL_GETMAILLIST)
{
	m_mailBox->HandleGetList(this);
}

OpHandler(CMSG_MAIL_READMAIL)
{
	pb::C2GS_Mail_ReadMail msgRecve;
	pack >> msgRecve;
	m_mailBox->HandleReadMail(this,msgRecve);
}

OpHandler(CMSG_MAIL_DELMAIL)
{
	pb::C2GS_Mail_DelMail  msgRecve;
	pack >> msgRecve;
	m_mailBox->HandleDelete(this,msgRecve);
}

OpHandler(CMSG_MAIL_RECVMAILAWARD)
{
	pb::C2GS_Mail_RecvMailAward  msgRecve;
	pack >> msgRecve;
	m_mailBox->HandlePickAttachments(this,msgRecve);
}

OpHandler(SMSG_MAIL_UPDATE)
{
	pb::GS2C_Mail_MailList  msgUpdate;
	pack >> msgUpdate;
	m_mailBox->HandleMailOperate(this,msgUpdate);
}

OpHandler(CMSG_EXCHANGE_GIFT_CODE)
{
	pb::C2GS_Exchange_Gift_Code  msg;
	pack >> msg;

	pb::SG2C_Exchange_Gift_Code gameMsg;
	gameMsg.set_gift_code(msg.giftcode());
	gameMsg.set_player_id(GetGuid());
	gameMsg.set_channel(GetChannelId());
	sCenterClient.Send(pb::SG2C_EXCHANGE_GIFT_CODE,gameMsg);
}


OpHandler(CMSG_FEEDBACK)
{
	pb::FeedbackInfo  msg;
	pack >> msg;

	pb::SG2C_FeedBack gameMsg;
	gameMsg.set_playerid(GetGuid());
	gameMsg.set_player_name(CharName());
	gameMsg.set_feedback(msg.feedback());
	sCenterClient.Send(pb::SG2C_FEEDBACK,gameMsg);
}


