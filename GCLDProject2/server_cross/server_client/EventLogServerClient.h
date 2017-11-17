
#ifndef EVENT_LOG_SERVER_CLIENT_H__
#define EVENT_LOG_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "Singleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"

#include "Enum.pb.h"
#include "StopWatch.h"
#include "def/EventLogDefine.h"
namespace pb
{
    class Server_Event;
}

class NetPack ;
class PlayerSession ;

class EventLogServerClient : public Singleton<EventLogServerClient>, public ServerClient
{
private:
	friend class Singleton<EventLogServerClient>;

public:
	typedef void (EventLogServerClient::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );

private:
	static HandlerArray s_handlers ;

private:
	EventLogServerClient ( void ) ;

public:
	virtual ~EventLogServerClient( void ) ;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// virtual 
private:
	virtual void ClientConnect( void ) ;
	virtual void ClientDisconnect( void ) ;
	virtual void ClientUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual bool HandlePacket( NetPack &packet ) ;


public:
	virtual void SendPing( void ) ;
	void SendRegisterInfo( void ) ;

	static void AddInt32Value( pb::Server_Event& evt, int32 value ) ;
	static void AddUInt32Value( pb::Server_Event& evt, uint32 value ) ;

	static void AddInt64Value( pb::Server_Event& evt, int64 value ) ;
	static void AddUInt64Value( pb::Server_Event& evt, uint64 value ) ;
	static void AddFloatValue( pb::Server_Event& evt, float value ) ;

	static void AddStringValue( pb::Server_Event& evt, const string &value ) ;

    void SendEvent(const pb::Server_Event& evt);
    void SendEvent( EventLogInfo& evtMsg, int paramCount, va_list& params, bool withFmt = false);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
private:
   EventLogs m_eventLogs;
   StopWatch m_watchLogSend;
public:
	virtual void HandlePong( NetPack &packet ) ;
	void HandleRegisterRet( NetPack &packet ) ;
private:
	void SendEventLogs();

};

#define sEventLogClient (EventLogServerClient::Instance())

#endif
