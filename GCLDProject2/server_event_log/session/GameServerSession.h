
#ifndef GAME_SERVER_SESSION_H__
#define GAME_SERVER_SESSION_H__


#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"

class GameServerSession : public Session
{
	DECLARE_OBJ_POOL(GameServerSession)

public:
	typedef void (GameServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	GameServerSession( void ) ;
	virtual ~GameServerSession( void ) ;

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


