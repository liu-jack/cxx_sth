
#ifndef CENTER_SERVER_LOGICAL_THREAD_H__
#define CENTER_SERVER_LOGICAL_THREAD_H__
#include <google/protobuf/message.h>
#include "net_framework/LogicalTaker.h"
#include "memory_buffer/NetPack.h"
#include <boost/scoped_ptr.hpp>
#include "session/PlayerSessionContainer.h"


class Session ;
class PlayerSession ;
class MapScriptGroup;
class FormulaScriptHolder;
class LuaScript;
class Player;
class JsonTool;
typedef boost::shared_ptr<Player> PlayerPtr ;
class GameServerLogicalTaker: public LogicalTaker
{
public:
	GameServerLogicalTaker( void ) ;
	virtual ~GameServerLogicalTaker( void ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Player Session
public:
	void AddPlayerSession( PlayerSession *pPlayerSession ) ;
	void RemovePlayerSession( PlayerSession *pPlayerSession ) ;
	bool IsPlayerOnline(uint64 playerId);
	void SetAllPlayerDataIsLost( void ) ;

	Player* GetPlayerPtr( uint64 playerId ) ;
	Player* GetPlayerPtrByName( const std::string &name ) ;
	bool  CheckHasName(const std::string& name);
	void GetUpdateTimeInfo(std::string& info);


private:
	PlayerSessionTable m_playerSessions ;
	uint64             m_beforUpTime;
	uint64             m_endUpTime;
	uint64             m_endbeforTime;
	boost::scoped_ptr<JsonTool> m_logicSerInfo;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lua
public:
    LuaScript* CreateScript( const std::string& scriptName, bool showErr);
    bool CallLuaFormula( const char* formulaName, float& result, const std::vector<float>& param_vec);    // 不定参数为float 类型

private:
    boost::scoped_ptr< MapScriptGroup>          m_scriptGroup;
    boost::scoped_ptr< FormulaScriptHolder>     m_formulaHolder;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Broadcas
public:
	void BroadcastMsgToPlayer(uint16 opcode, const ::google::protobuf::Message& msg);
	void BroadcastMsgToContryPlayers(uint32 countryId,uint16 opcode, const ::google::protobuf::Message& msg);
	bool SendMsgToPlayer(uint64 playerId, uint16 opcode, const ::google::protobuf::Message& msg);
	bool SendMsgToPlayer(uint64 playerId, NetPack &packet);
	void BroadcastServerMsg(const int opCode, const ::google::protobuf::Message &msg);
	void PostPlayerMsg(uint64 playerId, uint16 opcode, const ::google::protobuf::Message& msg);
	void PostInputSessionMsg(PlayerSession* pSession,uint16 opcode, const ::google::protobuf::Message& msg);
	void PostPlayerNetPack(uint64 playerId, NetPack *packet);
} ;


#endif
