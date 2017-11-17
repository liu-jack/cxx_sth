#ifndef DB_EVENT_H_
#define DB_EVENT_H_
#include "def/TypeDef.h"
namespace pb{
	class Mail_UserMailInfo;
};
struct Event_mail;
class MailEvent
{
public:
	MailEvent(const Event_mail& dbEvent);
	~MailEvent();
	uint32 titleId() const;
	uint32 contentId() const;
	void SaveTo(pb::Mail_UserMailInfo &mail ) const;
private:
	const Event_mail&       m_dbMailEvent;
};
#endif //DB_EVENT_H_