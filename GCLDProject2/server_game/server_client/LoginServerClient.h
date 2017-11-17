
#ifndef LOGIN_SERVER_CLIENT_H__
#define LOGIN_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "SimpleSingleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"


class NetPack ;

class LoginServerClient : public ServerClient
{
public:
	typedef void (LoginServerClient::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );

private:
	static HandlerArray s_handlers ;

public:
	LoginServerClient ( void ) ;

public:
	virtual ~LoginServerClient( void ) ;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// virtual 
private:
	virtual void ClientConnect( void ) ;
	virtual void ClientUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual bool HandlePacket( NetPack &packet ) ;


public:
	virtual void SendPing( void ) ;
	void SendRegisterInfo( void ) ;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual void HandlePong( NetPack &packet ) ;
	void HandleRegisterRet( NetPack &packet ) ;

};

#endif
