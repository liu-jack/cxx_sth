#include "SysEventMgr.h"

#include "OS.h"
#include "GameServer.h"
#include "DbTool.h"
#include "../object/Player.h"
#include "ServerOpcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "InterServer.pb.h"
#include "../server_game/server_client/DbProxyServerClient.h"
#include "datastruct/struct_event.h"
#include "SysEvent/MailEvent.h"
#include "Social.pb.h"
#include "game_data/DbStringMgr.h"
#include "def/ConstDef.h"
#include "utility/Utility.h"
using namespace pb;
//extern SortPlayerDistanceNearBy;
///////////////class DynamicChannelInfo ////////////////////
SysEventMgr::SysEventMgr()
{
}

SysEventMgr::~SysEventMgr()
{

}

void SysEventMgr::Init()
{
	FOREACH_DB_ITEM(eventMail, Event_mail)
	{
		uint64 mailId = eventMail->id;
		MailEvent* pStage = new MailEvent(*eventMail);
		m_eventMailMap[mailId] = pStage;
	}
}

const MailEvent* SysEventMgr::GetEventMailById(uint64 mailId)
{
	EventMailMap::const_iterator mailIter = m_eventMailMap.find(mailId);
	if (mailIter != m_eventMailMap.end())
	{
		return mailIter->second;
	}
	else
	{
		return NULL;
	}
}

void SysEventMgr::SaveMailTo( pb::Mail_UserMailInfo& msg )
{
	if (msg.mail_type() == pb::MAIL_SYSMAIL)
	{
		const MailEvent* tempMail = GetEventMailById(msg.mail_id());
		if (tempMail)
		{
			tempMail->SaveTo(msg);
		}
	}
}


void SysEventMgr::Update( uint64 nowTimeMS, int64 diffMS)
{

}

void SysEventMgr::SendSysEventMail( std::set<uint64>& playerIds,uint64 eventId)
{
	if (playerIds.size() == 0)
	{
		return;
	}
	pb::SG2D_Check_Mail_Recivers_Exist checkMsg;
	SG2G_Mail_SendMail* pSendMail = checkMsg.mutable_mail();
	pSendMail->set_mail_type(pb::MAIL_SYSMAIL);
	pSendMail->set_send_type(pb::MAIL_SEND_ALL);
	pSendMail->set_title("testTTT");
	pSendMail->set_content("testCCCC");
	pSendMail->set_mail_id(eventId);
	checkMsg.set_is_server_mail(false);
	for (std::set<uint64>::iterator iterPlayer = playerIds.begin(); iterPlayer != playerIds.end(); ++iterPlayer)
	{
		pSendMail->add_to_players(*iterPlayer);
	}
	sDbProxyClient.Send(SG2D_CHECK_MAIL_RECIVERS_EXIST,checkMsg);
}


void SysEventMgr::SendPlatFormMail(pb::SG2D_Check_Mail_Recivers_Exist& checkMsg,std::set<uint64>& playerIds)
{
	SG2G_Mail_SendMail* pSendMail = checkMsg.mutable_mail();
	pSendMail->set_mail_id(0);
	//pSendMail->set_mail_type(pb::MAIL_PLATFROM);
	//pSendMail->set_send_type(sendType);
	//pSendMail->set_title(title);
	//pSendMail->set_content(content);
	//pSendMail->set_items()
	for (std::set<uint64>::iterator iterPlayer = playerIds.begin(); iterPlayer != playerIds.end(); ++iterPlayer)
	{
		pSendMail->add_to_players(*iterPlayer);
	}
	pSendMail->set_begin_time(0);
	pSendMail->set_end_time(0);

	sDbProxyClient.Send(SG2D_CHECK_MAIL_RECIVERS_EXIST,checkMsg);
}

void SysEventMgr::SendJustTipsMail(const uint64 player_id,const uint64 eventId,string& items,const int val1/* = 0*/,const int val2/* = 0*/)
{
	if (player_id == 0 || items.empty())
	{
		return;
	}
	pb::SG2D_Check_Mail_Recivers_Exist checkMsg;
	SG2G_Mail_SendMail* pSendMail = checkMsg.mutable_mail();
	pSendMail->set_mail_type(pb::MAIL_JUST_TIPS);
	pSendMail->set_send_type(pb::MAIL_SEND_ALL);
	pSendMail->set_title("testTTT");
	pSendMail->set_content("testCCCC");
	pSendMail->set_mail_id(eventId);
	checkMsg.set_is_server_mail(false);
	pSendMail->add_to_players(player_id);
#define MAX_BUF 1024
	if(eventId == EVENT_TYPE_EXPLOIT || eventId == EVENT_TYPE_MILITARY_POWER_RANK || eventId == EVENT_TYPE_CONGRATULATE)
	{
		char buf[MAX_BUF] = {0};
		Utility::convert(buf,val1);
		if(items.empty())
		{
			items += string(buf);
		}
		else
		{
			items += "*" + string(buf);
		}
	}
	else if(eventId == EVENT_TYPE_CONTIN_OCCUPY_CITY)
	{
		char buf[MAX_BUF] = {0};
		Utility::convert(buf,val1);
		if(items.empty())
		{
			items += string(buf);
		}
		else
		{
			items += "*" + string(buf);
		}
		char buf_val2[MAX_BUF] = {0};
		Utility::convert(buf_val2,val2);
		items += "*" + string(buf_val2);
	}
	pSendMail->set_items(items);
	sDbProxyClient.Send(SG2D_CHECK_MAIL_RECIVERS_EXIST,checkMsg);
}