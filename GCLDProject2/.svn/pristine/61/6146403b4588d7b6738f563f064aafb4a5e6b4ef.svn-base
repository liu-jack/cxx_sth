
#ifndef LOGIN_SERVER_SESSION_H__
#define LOGIN_SERVER_SESSION_H__


#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"


class LoginServerSession : public Session
{
	DECLARE_OBJ_POOL(LoginServerSession)

public:
	typedef void (LoginServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	LoginServerSession( void ) ;
	virtual ~LoginServerSession( void ) ;

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


