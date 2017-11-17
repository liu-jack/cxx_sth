#include "MailEvent.h"
#include "datastruct/struct_event.h"
#include "Mail.pb.h"
MailEvent::MailEvent( const Event_mail& dbEvent )
:m_dbMailEvent(dbEvent)
{

}

MailEvent::~MailEvent()
{

}

uint32 MailEvent::titleId() const
{
	return m_dbMailEvent.title;
}

uint32 MailEvent::contentId() const
{
	return m_dbMailEvent.content;
}

void MailEvent::SaveTo( pb::Mail_UserMailInfo &mail ) const
{
	mail.set_mail_id((uint64)m_dbMailEvent.id);
	mail.set_title_id(m_dbMailEvent.title);
	mail.set_mail_type(pb::MAIL_SYSMAIL);//事件类型：1，后台邮件, 2固定邮件
	mail.set_content_id(m_dbMailEvent.content);
	mail.set_items(m_dbMailEvent.items);
}