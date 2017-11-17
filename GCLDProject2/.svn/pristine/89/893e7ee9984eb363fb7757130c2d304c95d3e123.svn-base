
#ifndef WEB_SERVER_SESSION_H__
#define WEB_SERVER_SESSION_H__


#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"


class WebServerSession : public Session
{
	DECLARE_OBJ_POOL(WebServerSession)

public:
	typedef void (WebServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	WebServerSession( void ) ;
	virtual ~WebServerSession( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual bool HandleNetPack( NetPack &pack );

	virtual void HandlePing( NetPack &packet ) ;

	void HandleRegister( NetPack &packet ) ;

	void HandleGetServerList( NetPack &packet ) ;

	void HandleGetPayerList( NetPack &packet ) ;

	void HandleWebPay( NetPack &packet ) ;

	void HandleSendServersMail( NetPack &packet ) ;
	void HandleSendPlayerMail( NetPack &packet ) ;
	void HandlePlayerForbid( NetPack &packet ) ;
	void HandleSetPlayerVip( NetPack &packet ) ;
	void HandleRecreatePayOrder( NetPack &packet ) ;
	void HandleSendServersNotice( NetPack &packet ) ;
	void HandleServersHotResReload( NetPack &packet ) ;
	void HandleCreatGiftCode( NetPack &packet ) ;
	void HandleGetPlayerOnlineCount( NetPack &packet ) ;
	void HandleGetServerInfo( NetPack &packet ) ;
	
	
} ;



#endif


