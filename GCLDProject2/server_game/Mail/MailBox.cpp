#include <set>
#include <memory>
#include "MailBox.h"
#include "Mail.h"
#include "../object/Player.h"
//#include "../server_game/GameServer.h"
#include "def/MmoAssert.h"
#include "mail_const.h"
#include "SysEvent/SysEventMgr.h"
#include "SysEvent/MailEvent.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "../server_client/DbProxyServerClient.h"

MailBox::MailBox()
{
}

MailBox::~MailBox( void )
{
}
void MailBox::HandleSend(Player* player,const pb::SG2G_Mail_SendMail& msg)
{
	pb::SG2G_Mail_SendMail toSendMail;
	toSendMail.CopyFrom(msg);
	toSendMail.clear_to_players();
	toSendMail.add_to_players(player->GetGuid());
	sDbProxyClient.SendPlayerPack(*player, pb::SG2D_MAIL_RECEVE_MAIL,toSendMail);
}

void MailBox::HandleMailOperate(Player* player,pb::GS2C_Mail_MailList& message)
{
	//邮件操作0无操作，1添加，2删除,3,更新
	pb::GS2C_Mail_Option_respon mailRespon;
	pb::GS2C_Mail_NewMail newMails;
	int nOperate = message.mail_list_size();
	for (int i=0; i< nOperate; i++)
	{
		pb::Mail_UserMailInfo* curOperate = message.mutable_mail_list(i);
		if (curOperate->mail_operate() == MAIL_OPREATE_ADD)
		{
			std::auto_ptr<Mail> mail(Mail::CreateMailFrom(player->GetGuid(),*curOperate));
			if (mail.get())
			{
				if(mail->MoveAttachmentsToBag(player))
				{
					/*pb::SG2D_MailsOperate mailOperateToDB;
					mailOperateToDB.set_operate_type(MAIL_OPREATE_UPDATE);
					pb::SG2D_MailInfoData *updateMail = mailOperateToDB.add_mail_info_data();
					mail->SaveTo(*updateMail);
					UpdateMailToDb(player,mailOperateToDB);*/
					//目前所有这样邮件都是自动领取的，所以暂时不必要为领取单独发更新操作
				}
				Mail_UserMailInfo* curNewMail = newMails.add_new_mail();
				mail->SaveToUserMailInfo(*curNewMail);
				InboxAdd(mail.release());
			}
		}
		else if (curOperate->mail_operate() == MAIL_OPREATE_DEL)
		{
			MailInbox::iterator it = m_inbox.find(curOperate->mail_id());
			if (it != m_inbox.end())
			{
				pb::GS2C_Mail_Option_respon_opt_result* opres = mailRespon.add_opts_res();
				opres->set_opt_id(curOperate->mail_id());
				opres->set_opt_res(OPTION_SUCCESS);
				Mail* mail = it->second;
				if (mail->IsTaked())
				{
					m_inbox.erase(it);
				}
				else
				{
					if(!mail->MoveAttachmentsToBag(player))
					{
						mail->MoveGiftsToBag(player);
					}
					m_inbox.erase(it);
				}
			}
			else
			{
				std::auto_ptr<Mail> mail(Mail::CreateMailFrom(player->GetGuid(),*curOperate));
				if (mail.get())
				{
					if (!mail->IsTaked())
					{
						if(!mail->MoveAttachmentsToBag(player))
						{
							mail->MoveGiftsToBag(player);
						}
					}
				}
			}
		}
		else if (curOperate->mail_operate() == MAIL_OPREATE_UPDATE)
		{
			MailInbox::iterator it = m_inbox.find(curOperate->mail_id());
			if (it != m_inbox.end())
			{
				Mail* mail = it->second;
				mail->UpdateFrom(*curOperate);
			}
		}
	}

	if (mailRespon.opts_res_size() > 0)
	{
		player->Send(pb::SMSG_MAIL_DELMAIL_RESPON,mailRespon);
	}

	if (newMails.new_mail_size() > 0)
	{
		player->Send(pb::SMSG_MAIL_HASNEWMAIL, newMails);
	}

	if (m_inbox.empty())
	{
		SyncMails(player);
	}
}

void MailBox::HandleGetList(Player* player)
{
	if (m_inbox.empty())
	{
		SyncMails(player);
	}
	else
	{
		pb::GS2C_Mail_MailList msgRespone;
		for (MailInbox::iterator it =m_inbox.begin(); it!=m_inbox.end(); ++it)
		{
			Mail* mail = it->second;
			pb::Mail_UserMailInfo * usemailinfo = msgRespone.add_mail_list();
			mail->SaveToUserMailInfo(*usemailinfo);
		}
		player->Send(pb::SMSG_MAIL_GETMAILLIST_RESPON, msgRespone);
	}
}

void MailBox::HandleReadMail(Player* player,const pb::C2GS_Mail_ReadMail& message)
{
	int nMsgCount = message.mail_ids_size();
	if (nMsgCount<=0)
	{
		return;
	}
	bool isRead = false;
	pb::SG2D_MailsOperate mailOperateToDB;
	mailOperateToDB.set_operate_type(MAIL_OPREATE_UPDATE);
	pb::GS2C_Mail_Option_respon mailRespon;
	for(int i = 0; i< nMsgCount; i++)
	{
		uint64 mailId = message.mail_ids(i);
		MailInbox::iterator it = m_inbox.find(mailId);
		if (it != m_inbox.end())
		{
			Mail* mail = it->second;
			mail->SetMailReadState(MAIL_READ);
			pb::GS2C_Mail_Option_respon_opt_result* opres = mailRespon.add_opts_res();
			opres->set_opt_id(mailId);
			opres->set_opt_res(OPTION_SUCCESS);
			pb::SG2D_MailInfoData* pToDbData = mailOperateToDB.add_mail_info_data();
			mail->SaveTo(*pToDbData);
			isRead = true;
		}
		else
		{
			pb::GS2C_Mail_Option_respon_opt_result* opres = mailRespon.add_opts_res();
			opres->set_opt_id(mailId);
			opres->set_opt_res(OPTION_FAIL);
		}
	}
	player->Send(pb::SMSG_MAIL_READMAIL_RESPON, mailRespon);

	if (isRead)
	{
		UpdateMailToDb(player,mailOperateToDB);
	}
}

void MailBox::HandleDelete(Player* player, const pb::C2GS_Mail_DelMail& message)
{//1添加，2删除,3,更新
	pb::GS2C_Mail_Option_respon mailRespon;
	int delMailCount = message.mail_ids_size();
	if (delMailCount<=0)
	{
		m_inbox.clear();
		pb::GS2C_Mail_Option_respon_opt_result* opres = mailRespon.add_opts_res();
		opres->set_opt_id(0);
		opres->set_opt_res(OPTION_SUCCESS);
		player->Send(pb::SMSG_MAIL_DELMAIL_RESPON, mailRespon);
		return;
	}
	pb::SG2D_MailsOperate mailOperateToDB;
	mailOperateToDB.set_operate_type(MAIL_OPREATE_DEL);
	bool isModify = false;
	for(int j=0; j< delMailCount; j++)
	{
		uint64 mailId = message.mail_ids(j);
		MailInbox::iterator it = m_inbox.find(mailId);
		if (it != m_inbox.end())
		{
			Mail* toDelMail = it->second;
			pb::GS2C_Mail_Option_respon_opt_result* opres = mailRespon.add_opts_res();
			opres->set_opt_id(mailId);
			pb::SG2D_MailInfoData *delMail = mailOperateToDB.add_mail_info_data();
			toDelMail->SaveTo(*delMail);
			m_inbox.erase(it);
			opres->set_opt_res(OPTION_SUCCESS);
			isModify = true;
		}
		else
		{
			pb::GS2C_Mail_Option_respon_opt_result* opres = mailRespon.add_opts_res();
			opres->set_opt_id(mailId);
			opres->set_opt_res(OPTION_FAIL);
		}
	}
	player->Send(pb::SMSG_MAIL_DELMAIL_RESPON, mailRespon);

	if (isModify)
	{
		UpdateMailToDb(player,mailOperateToDB);
	}
}


void MailBox::HandlePickAttachments(Player* player,const pb::C2GS_Mail_RecvMailAward& message)
{
	if (!player)
	{
		return;
	}
	pb::GS2C_Mail_RecvMailAward award_data;
	uint64 mailId = message.mail_id();
	MailInbox::iterator it = m_inbox.find(mailId);
	award_data.set_opt_res(0);
	if (it != m_inbox.end())
	{
		Mail* mail = it->second;
		if (mail->IsMailType((int)pb::MAIL_PLATGIFT))
		{
			if (mail->MoveGiftsToBag(player))
			{
				pb::SG2D_MailsOperate mailOperateToDB;
				mailOperateToDB.set_operate_type(MAIL_OPREATE_DEL);
				pb::SG2D_MailInfoData *updateMail = mailOperateToDB.add_mail_info_data();
				mail->SaveTo(*updateMail);
				UpdateMailToDb(player,mailOperateToDB);

				player->AddEventLogFMT( pb::EVENT_PLAYER_TAKE_GIFT_BAG,
					2,
					"i", player->GetLevel(),
					"s", mail->GetItems().c_str()
					);
			}
			else
			{
				award_data.set_opt_res(1);
			}
		}
		else if (mail->MoveAttachmentsToBag(player))
		{
			pb::SG2D_MailsOperate mailOperateToDB;
			mailOperateToDB.set_operate_type(MAIL_OPREATE_UPDATE);
			pb::SG2D_MailInfoData *updateMail = mailOperateToDB.add_mail_info_data();
			mail->SaveTo(*updateMail);
			UpdateMailToDb(player,mailOperateToDB);
		}
		else
		{
			award_data.set_opt_res(1);
		}
	}
	award_data.set_mail_id(mailId);
	player->Send(pb::SMSG_MAIL_RECVMAILAWARD_RESPON,award_data);
}

void MailBox::InboxAdd( Mail* mail )
{
	MMO_ASSERT(mail);
	MMO_ASSERT(mail->Id() > 0);
	if(m_inbox.find(mail->Id()) != m_inbox.end())
	{
		m_inbox.erase(mail->Id());
	}
	m_inbox[mail->Id()] = mail;
}

void MailBox::LoadFrom(Player* player,pb::GS2C_Mail_MailList& msg )
{
	m_inbox.clear();
	MMO_ASSERT(m_inbox.empty());
	pb::GS2C_Mail_MailList message;
	message.CopyFrom(msg);
	msg.clear_mail_list();
	int nOperate = message.mail_list_size();
	for (int i=0; i< nOperate; i++)
	{
		pb::Mail_UserMailInfo* curOperate = message.mutable_mail_list(i);
		if (curOperate->mail_operate() == MAIL_OPREATE_ADD || curOperate->mail_operate() == MAIL_OPREATE_UPDATE)
		{
			std::auto_ptr<Mail> mail(Mail::CreateMailFrom(player->GetGuid(),*curOperate));
			if (mail.get())
			{
				mail->MoveAttachmentsToBag(player);
				Mail_UserMailInfo* curNewMail = msg.add_mail_list();
				mail->SaveToUserMailInfo(*curNewMail);
				InboxAdd(mail.release());
			}
		}
		else if (curOperate->mail_operate() == MAIL_OPREATE_DEL)
		{
			std::auto_ptr<Mail> mail(Mail::CreateMailFrom(player->GetGuid(),*curOperate));
			if (mail.get())
			{
				if (!mail->IsTaked())
				{
					if(!mail->MoveAttachmentsToBag(player))
					{
						mail->MoveGiftsToBag(player);
					}
				}
			}
		}
	}

	if (m_inbox.empty())
	{
		SyncMails(player);
	}
}

bool MailBox::InboxEmpty() const
{
	return m_inbox.empty();
}

void MailBox::HandleGetNewMailCount(Player* player)
{
	int newsysMail=0;
	for (MailInbox::const_iterator it =m_inbox.begin(); it!=m_inbox.end(); ++it)
	{
		Mail* mail = it->second;
		if (mail->GetMailReadState() == MAIL_NEW)
		{
			newsysMail++;
		}
	}
	NetPack packMsg((uint16)pb::SMSG_MAIL_GETNEWMAILCOUNT_RESPON);
	packMsg << newsysMail;
	player->Send(packMsg);
}

void MailBox::SyncMails(Player* player)
{
	pb::SG2D_MailsOperate mailOperateToDB;
	mailOperateToDB.set_operate_type(MAIL_OPREATE_UPDATE);
	sDbProxyClient.SendPlayerPack(*player,pb::SG2D_MAIL_UPDATEMAILINFO,mailOperateToDB);
}

void MailBox::UpdateMailToDb(Player* player,const pb::SG2D_MailsOperate& operate)
{
	sDbProxyClient.SendPlayerPack(*player,pb::SG2D_MAIL_UPDATEMAILINFO,operate);
}

const MailInbox& MailBox::GetMailBox() const{
	return m_inbox;
}
