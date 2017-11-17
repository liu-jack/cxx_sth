#pragma once
#include <string>
#include "def/TypeDef.h"
#include "Mail.pb.h"
class Player;
struct PayMailInfo;
namespace pb
{
	class Mail_UserMailInfo;
	class SG2D_MailInfoData;
	//class Mail_AttachCurency;
	//class Mail_AttachItem;
}
class Mail
{
public:
	static Mail* CreateMailFrom(uint64 playerId, const pb::Mail_UserMailInfo& msg);

	Mail();
	//explicit Mail(uint64 mailId,);
	~Mail();

	void SetId(uint64 id);
	void SetOpened();

	bool MoveAttachmentsToBag(Player* player);
	bool MoveGiftsToBag( Player* player );
	bool IsTaked();
	bool IsNew();
	uint64 Id() const { return m_id; }
	uint64 PlayerId() const { return m_playerId; }
    int GetMailType() const{ return m_type;}
    int GetMailReadState() const;

	void SetPlayerId( uint64 id) { m_playerId = id;} 
    void SetMailReadState(int mailState);
	
    void SaveToUserMailInfo(pb::Mail_UserMailInfo& userMail);    
	bool IsMailType(int mType);
	void SaveTo(pb::SG2D_MailInfoData& msg) const;
	void LoadFrom(const pb::Mail_UserMailInfo& msg);
	void LoadFrom(const PayMailInfo *pPayMailInfo );
	time_t SendTime() { return m_sendTime;}
	int GetContentId() const;
	void UpdateFrom(const pb::Mail_UserMailInfo& msg);
	const std::string& GetItems() { return m_items;}
private:
	uint64			m_id;//
	uint64			m_playerId;
	uint64          m_mailId;
	int			m_mailState;
	int			m_type;	
	int			m_operate;
	time_t		m_sendTime;
	int			m_title_id;
	int			m_content_id;
	std::string	m_title;
	std::string	m_content;
	std::string	m_items;
	bool        m_isTaked;
};
