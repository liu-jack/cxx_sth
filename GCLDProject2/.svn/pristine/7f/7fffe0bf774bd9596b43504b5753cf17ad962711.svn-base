#include <memory>
#include "Mail.h"
#include "def/MmoAssert.h"
#include "mail_const.h"
#include "InterServer.pb.h"
//#include "Mail.pb.h"
#include "../object/Player.h"
#include "../item/ItemArray.h"
#include "item/ItemManager.h"
#include "../item/LogicItemProto.h"
#include "SysEvent/SysEventMgr.h"
#include "Loot/LootList.h"
#include "Opcode.pb.h"
#include "data/PayItemTable.h"
#include "../GameServer.h"
#include "utility/Utility.h"
Mail::Mail()
:m_id(0)
,m_playerId(0)
,m_mailState(MAIL_NEW)
,m_type(pb::MAIL_SYSMAIL)
,m_sendTime(0)
,m_title_id(0)
,m_content_id(0)
,m_title("")
,m_content("")
,m_items("")
,m_isTaked(false)
{
}

Mail::~Mail()
{
}

void Mail::SetId( uint64 id )
{
	MMO_ASSERT(m_id == 0);

	m_id = id;
}

void Mail::SaveTo( pb::SG2D_MailInfoData& msg ) const
{
	msg.set_player_id(m_playerId);
	msg.set_id(m_id);
	msg.set_mail_id(m_mailId);
	msg.set_mail_type(m_type);
	msg.set_mail_state(m_mailState);
	msg.set_send_time(m_sendTime);
	msg.set_is_take(m_isTaked);
}

Mail* Mail::CreateMailFrom(uint64 playerId, const pb::Mail_UserMailInfo& msg )
{
	std::auto_ptr<Mail> mail(new Mail());
	if (msg.mail_type() == pb::MAIL_SYSMAIL)
	{
		pb::Mail_UserMailInfo sysMsg;
		sysMsg.CopyFrom(msg);
		sSysEventMgr.SaveMailTo(sysMsg);
		mail->LoadFrom(sysMsg);
	}
	else if (msg.mail_type() == pb::MAIL_PLATFROMFIX)
	{
		PayMailInfo *pPayMailInfo =sGameSvr.m_payItemsInfo.GetPayMailInfo( msg.mail_id() ) ;
		mail->LoadFrom(msg);
		mail->LoadFrom(pPayMailInfo);
	}
	else
	{
		mail->LoadFrom(msg);
	}
	mail->SetPlayerId(playerId);
	if(msg.mail_operate() == MAIL_OPREATE_ADD)
	{
		mail->SetMailReadState(MAIL_NEW);
	}
	else
	{
		mail->SetMailReadState(msg.mail_state());
	}
	return mail.release();
}

void Mail::UpdateFrom(const pb::Mail_UserMailInfo& msg)
{
	if (msg.mail_type() == pb::MAIL_SYSMAIL)
	{
		pb::Mail_UserMailInfo sysMsg;
		sysMsg.CopyFrom(msg);
		sSysEventMgr.SaveMailTo(sysMsg);
		LoadFrom(sysMsg);
	}
	else if (msg.mail_type() == pb::MAIL_PLATFROMFIX)
	{
		PayMailInfo *pPayMailInfo =sGameSvr.m_payItemsInfo.GetPayMailInfo( msg.mail_id() ) ;
		LoadFrom(msg);
		LoadFrom(pPayMailInfo);
	}
	else
	{
		LoadFrom(msg);
	}
}
int Mail::GetContentId() const{
	return m_content_id;
}

void Mail::LoadFrom(const PayMailInfo *pPayMailInfo )
{
	if (!pPayMailInfo)
	{
		return;
	}
	m_mailId	= pPayMailInfo->id;
	m_type		= (int)pb::MAIL_PLATFROMFIX;
	m_title		= pPayMailInfo->title;
	m_content	= pPayMailInfo->content;
	m_items     = pPayMailInfo->items;
}

void Mail::LoadFrom(const pb::Mail_UserMailInfo& msg )
{
	m_id		= msg.id();	
	m_mailId    = msg.mail_id();
	m_type		= msg.mail_type();
	m_operate	= msg.mail_operate();
	m_mailState	= msg.mail_state();
	m_sendTime	= msg.send_time();
	m_title		= msg.title();
	m_content	= msg.content();
	m_title_id  = msg.title_id();
	m_content_id = msg.content_id();
	m_items = msg.items();
	m_isTaked = msg.is_take();
}

bool Mail::MoveAttachmentsToBag( Player* player )
{
	if (!player || IsTaked())
	{
		return false;
	}
	if (m_type == pb::MAIL_SYSMAIL || m_type == pb::MAIL_PLATFROMFIX)
	{
		ItemID2Count curItemMap;
		Utility::SplitItemStr(m_items,curItemMap);
		if ((int)curItemMap.size() > 0)
		{
			player->CreateSysMailItem(IR_MAIL_GET,curItemMap);
		}
		m_isTaked = true;
	}
	else if (m_type == pb::MAIL_PLATFROM)
	{
		IntV3Vec itemsMap;
		Utility::SplitStr3(m_items,itemsMap);
		if ((int)itemsMap.size() > 0)
		{
			player->CreateMailItem(IR_MAIL_GET,itemsMap);
		}
		m_isTaked = true;
	}
	else if (m_type == pb::MAIL_PLATGIFT)
	{
		return false;
	}
	else
	{
		m_isTaked = true;
	}
	return true;
}

bool Mail::MoveGiftsToBag( Player* player )
{
	if (!player || IsTaked())
	{
		return false;
	}
	if (m_type == pb::MAIL_PLATGIFT)
	{
		IntV3Vec itemsMap;
		Utility::SplitStr3(m_items,itemsMap);
		if ((int)itemsMap.size() > 0)
		{
			player->CreateMailItem(IR_MAIL_GET,itemsMap);
		}
		m_isTaked = true;
	}
	return m_isTaked;
}

bool Mail::IsTaked()
{
	return m_isTaked;
}

bool Mail::IsNew()
{
	return m_mailState == MAIL_NEW || m_mailState == MAIL_INIT;
}

void Mail::SetOpened()
{
	m_mailState = MAIL_READ;
}

void Mail::SaveToUserMailInfo(pb::Mail_UserMailInfo& userMail)
{
	userMail.set_id(m_id);
	userMail.set_mail_id(m_mailId);
	userMail.set_mail_type(m_type);
	userMail.set_mail_state(m_mailState);
	userMail.set_mail_operate(m_operate);
	userMail.set_send_time(m_sendTime);
	userMail.set_title(m_title);
	userMail.set_content(m_content);
	userMail.set_title_id(m_title_id);
	userMail.set_content_id(m_content_id);
	userMail.set_items(m_items);
	userMail.set_is_take(m_isTaked);
}

bool Mail::IsMailType(int mType)
{
  return m_type == mType;
}

void Mail::SetMailReadState(int mailState)
{
	if (mailState == MAIL_NEW || mailState == MAIL_READ )
	{
		m_mailState = mailState;
	}
}

int Mail::GetMailReadState() const
{
	return m_mailState;
}
