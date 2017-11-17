
#ifndef EVENT_LOG_SERVER_SESSION_H__
#define EVENT_LOG_SERVER_SESSION_H__


#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"


class EventLogServerSession : public Session
{
	DECLARE_OBJ_POOL(EventLogServerSession)

public:
	typedef void (EventLogServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	EventLogServerSession( void ) ;
	virtual ~EventLogServerSession( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual bool HandleNetPack( NetPack &pack );

	virtual void HandlePing( NetPack &packet ) ;

	void HandleRegister( NetPack &packet ) ;

} ;



#endif


