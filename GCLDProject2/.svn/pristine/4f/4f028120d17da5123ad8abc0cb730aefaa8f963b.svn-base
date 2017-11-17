#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H
#include "def/TypeDef.h"
#include "Social.pb.h"
#include "SimpleSingleton.h"
#include <boost/unordered_map.hpp>
using namespace pb;
const uint32 OFFLINECHAT_TIME_SECOND = 604800;//7 days
class Player;
class ChatHandler: public SimpleSingleton<ChatHandler>
{
  public:
    ChatHandler();
    ~ChatHandler();
	void AddOfflineChat(GxDB_ChatInfoData& ofchat);
	void SendOfflineChats(Player* pPlayer);
private:
	typedef boost::unordered_multimap<uint64, GxDB_ChatInfoData> OffLineChatMap; 
	typedef OffLineChatMap::iterator OffLineChatMapItr ;
	OffLineChatMap m_offLineChats;
};

#define sChatHandle (ChatHandler::Instance())

#endif /* CHAT_HANDLER_H */
