
#ifndef CENTER_SERVER_CLIENT_H__
#define CENTER_SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "Singleton.h"

#include "net_framework/ServerClient.h"
#include "net_framework/NetFrameworkDefines.h"

namespace pb{
	class WebPay;
}
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
	void SendLockedAccount( uint64 accountId, const string& accountName, int locktype, int lockTime);
	void DoWebPay( const pb::WebPay& info, uint64 sessionId = 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual void HandlePong( NetPack &packet ) ;

	void HandleRegisterRet( NetPack &packet ) ;
	void HandleGetLoginServersRet( NetPack &packet ) ;
	void HandleGetCrossServersRet( NetPack &packet );
	void HandleWebGetPlayerList( NetPack &packet ) ;
	void HandleGetEventLogServersRet( NetPack &packet ) ;
	void HandleWebPay( NetPack &packet ) ;

	void HandleSendServersMail( NetPack &packet ) ;
	void HandleSendPlayerMail( NetPack &packet ) ;
	void HandlePlayerForbid( NetPack &packet ) ;
	void HandleSetPlayerVip( NetPack &packet ) ;
	void HandleReCreatePayOrder( NetPack &packet ) ;
	void HandleSendServersNotice( NetPack &packet ) ;
	void HandleServerHotResReload( NetPack &packet ) ;
	void HandleExchangeGiftCodeRes( NetPack &packet ) ;
	void HandleFeedBackRes( NetPack &packet ) ;
	void HandleGetPlayerOnlineCount( NetPack &packet ) ;
	void HandleGetGameserverInfo( NetPack &packet ) ;
	void HandleServerPayItemHotReload( NetPack &packet ) ;
	void HandleSaveDbproxyCache( NetPack &packet ) ;
	
	
	

};

#define sCenterClient (CenterServerClient::Instance())

#endif
