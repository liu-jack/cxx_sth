
#ifndef CENTER_SERVER_CLIENT_H__
#define CENTER_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "Singleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"


class NetPack ;

class CenterServerClient : public Singleton<CenterServerClient>, public ServerClient
{
private:
	friend class Singleton<CenterServerClient>;

public:
	typedef void (CenterServerClient::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );

private:
	static HandlerArray s_handlers ;

private:
	CenterServerClient ( void ) ;

public:
	virtual ~CenterServerClient( void ) ;


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
	void HandleCenterBroadcast( NetPack &packet ) ;
	void HandleGetEventLogServersRet( NetPack &packet ) ;
};

#define sCenterClient (CenterServerClient::Instance())

#endif
