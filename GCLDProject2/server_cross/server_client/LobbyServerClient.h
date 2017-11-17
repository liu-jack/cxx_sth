
#ifndef LOBBY_SERVER_CLIENT_H__
#define LOBBY_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "Singleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"


class NetPack ;

class LobbyServerClient : public Singleton<LobbyServerClient>, public ServerClient
{
private:
	friend class Singleton<LobbyServerClient>;

public:
	typedef void (LobbyServerClient::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );

private:
	static HandlerArray s_handlers ;

public:
	LobbyServerClient ( void ) ;

public:
	virtual ~LobbyServerClient( void ) ;


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
	void HandleWarState( NetPack &packet ) ;
	void HandleWarStart( NetPack &packet ) ;
	void HandleDevides( NetPack &packet ) ;

};

#define sLobbyClient (LobbyServerClient::Instance())

#endif
