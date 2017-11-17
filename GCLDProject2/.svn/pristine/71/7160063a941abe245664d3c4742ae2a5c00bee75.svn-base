#pragma once
#include "def/TypeDef.h"
#include "Mail.pb.h"
#include "InterServer.pb.h"
#include "ptr_container/PtrMap.h"
#include "Mail.h"
class Player;
class Mail;
typedef PtrMap<uint64, Mail>	MailInbox;
class MailBox
{
public:
	MailBox(/*Player* player*/);
	~MailBox( void );

	bool InboxEmpty() const;
	void HandleSend(Player* player,const pb::SG2G_Mail_SendMail& msg);
	void HandleGetList(Player* player);
	void HandleReadMail(Player* player,const pb::C2GS_Mail_ReadMail& message);
	void HandleDelete(Player* player,const pb::C2GS_Mail_DelMail& message);
	void HandlePickAttachments(Player* player,const pb::C2GS_Mail_RecvMailAward& message);
	void HandleMailOperate(Player* player,pb::GS2C_Mail_MailList& message);
	void HandleGetNewMailCount(Player* player);
	void SyncMails(Player* player);
	void InboxAdd(Mail* mail);
	void LoadFrom(Player* player,pb::GS2C_Mail_MailList& msg);
	const MailInbox& GetMailBox() const;

private:
	void UpdateMailToDb(Player* player,const pb::SG2D_MailsOperate& operate);
private:
	MailInbox		m_inbox;
};
