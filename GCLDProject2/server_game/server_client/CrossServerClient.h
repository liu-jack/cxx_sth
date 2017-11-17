
#ifndef CROSS_SERVER_CLIENT_H__
#define CROSS_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "SimpleSingleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"


class NetPack ;

class CrossServerClient : public ServerClient
{
public:
	typedef void (CrossServerClient::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );
#ifdef _MMO_SERVER_
	void OnRecv( TcpAsyncConn *pConn, NetPack *pPack ) ;
	void OnSend( uint64 sessionId, NetPack *pPack ) ;
#endif

private:
	static HandlerArray s_handlers ;

public:
	CrossServerClient ( void ) ;

public:
	virtual ~CrossServerClient( void ) ;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// virtual 
private:
	virtual void ClientConnect( void ) ;
	virtual void ClientUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual bool HandlePacket( NetPack &packet ) ;


public:
	virtual void SendPing( void ) ;
	void SendRegisterInfo( void ) ;


	void HandleRegisterCrossPlayerRet( NetPack &packet );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual void HandlePong( NetPack &packet ) ;
	void HandleRegisterRet( NetPack &packet ) ;
	void HandleUpdatePlayerValue( NetPack &packet ) ;
	void HandleQuitCrossWar( NetPack &packet ) ;
	void HandleCrossResultCountry( NetPack &packet ) ;

	void HandleUpdatePlayerCharValue( NetPack &packet ) ;
	void HandleEndCrossWar( NetPack &packet ) ;
	void CanChangeHeros(NetPack &packet);

	void GetAwardFromCross(NetPack &packet);
};

#endif
