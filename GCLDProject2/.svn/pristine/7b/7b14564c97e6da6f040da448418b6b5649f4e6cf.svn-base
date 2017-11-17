
#ifndef LOBBY_SERVER_SESSION_H__
#define LOBBY_SERVER_SESSION_H__


#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"

class LobbyServerSession : public Session
{
	DECLARE_OBJ_POOL(LobbyServerSession)

public:
	typedef void (LobbyServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	LobbyServerSession( void ) ;
	virtual ~LobbyServerSession( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual bool HandleNetPack( NetPack &pack );

	virtual void HandlePing( NetPack &packet ) ;

	void HandleRegister( NetPack &packet ) ;

	void HandleServerEvent( NetPack &packet ) ;
	
} ;



#endif


