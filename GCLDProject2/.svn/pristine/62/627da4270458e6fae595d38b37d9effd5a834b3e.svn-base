#ifndef _SYSEVENT_H
#define _SYSEVENT_H
#include "def/TypeDef.h"
#include "Social.pb.h"
#include "SimpleSingleton.h"
#include "memory_buffer/NetPack.h"
#include "Opcode.pb.h"
#include "InterServer.pb.h"
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP

using namespace pb;
class MailEvent;
class SysEventMgr: public SimpleSingleton<SysEventMgr>
{
public:
	SysEventMgr();
	~SysEventMgr();
private:

#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint64, MailEvent> EventMailMap;
#else
	typedef PtrHashMap<uint64, MailEvent> EventMailMap;
#endif
public:
	void Init();
	void SaveMailTo(pb::Mail_UserMailInfo& msg);
	const MailEvent* GetEventMailById(uint64 mailId);
	void Update( uint64 nowTimeMS, int64 diffMS);
	void SendPlatFormMail( pb::SG2D_Check_Mail_Recivers_Exist& checkMsg,std::set<uint64>& playerIds);
	void SendSysEventMail( std::set<uint64>& playerIds,uint64 eventId);
	void SendJustTipsMail(const uint64 player_id,const uint64 eventId,string& items,const int val1 = 0,const int val2 = 0);
private:
	EventMailMap m_eventMailMap;
};


#define sSysEventMgr (SysEventMgr::Instance())

#endif /* _SYSEVENT_H */
