#include "MailTable.h"
#include "Mail.pb.h"
//#include "CommomDef.pb.h"
#include "InterServer.pb.h"

void PlatfromMailTable::LoadFrom( const pb::SG2G_Mail_SendMail &mail )
{
	id = mail.mail_id();
	title = mail.title();
	content = mail.content();
	items = mail.items();
	mail_type = mail.mail_type();
}

void PlatfromMailTable::SaveTo( pb::SG2G_Mail_SendMail &mail )
{
	mail.set_mail_id(id);
	mail.set_title(title);
	mail.set_content(content);
	mail.set_items(items);
	mail.set_mail_type(mail_type);//事件类型：1，后台邮件, 2事件邮件
}

void PlatfromMailTable::SaveTo( pb::Mail_UserMailInfo &mail )
{
	mail.set_title(title);
	mail.set_content(content);
	mail.set_items(items);
	mail.set_mail_type(mail_type);//事件类型：1，后台邮件, 2事件邮件
}

void PlayerMailTable::LoadFrom( const pb::Mail_UserMailInfo &mail )
{
	id = mail.id();
	mail_id = mail.mail_id();
	mail_type = mail.mail_type();
	mail_state = mail.mail_state();
	send_time = mail.send_time();
	is_take = mail.is_take() ? 1:0;
	if(!mail.items().empty())
		items = mail.items();
}

void PlayerMailTable::LoadFrom( const pb::SG2D_MailInfoData &mail )
{
	player_id = mail.player_id();
	mail_id = mail.mail_id();
	mail_type = mail.mail_type();
	mail_state = mail.mail_state();
	send_time = mail.send_time();
	is_take = mail.is_take() ? 1:0;
	if(!mail.items().empty())
		items = mail.items();
}

void PlayerMailTable::SaveTo( pb::Mail_UserMailInfo &mail )
{
	mail.set_id(id);
	mail.set_mail_id(mail_id);
	mail.set_mail_type(mail_type);
	mail.set_mail_state(mail_state);
	mail.set_send_time(send_time);
	mail.set_mail_operate(0);
	mail.set_is_take(is_take > 0 ? true : false);
	mail.set_items(items);
}

bool PlayerMailTable::IsAutoTake()
{
	return mail_type != pb::MAIL_PLATGIFT;
}

bool PlayerMailTable::IsModify()
{
	return extraData.IsModify();
}

void PlayerMailTable::SetModify()
{
	extraData.SetModify();
}

void PlayerMailTable::ResetModify()
{
	extraData.ResetModify();
}
