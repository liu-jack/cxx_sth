
#ifndef CLIENT_SESSION_H__
#define CLIENT_SESSION_H__

#include "../data/Account.h"
#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"
namespace pb
{
	class CVstarGameAccountCheck;
	class SVstarGameAccountCheck;
}

struct AccountInfo ;
struct MCResopnse;

class ClientSession : public Session
{
	DECLARE_OBJ_POOL(ClientSession)

public:
	typedef void (ClientSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteHandlers( void );

private:
	static HandlerArray s_handlers ;

public:
	ClientSession( void ) ;
	virtual ~ClientSession( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;

	AccountInfo* GetAccount( void ) {   return m_pAccount ;   }
	void SetAccount( AccountInfo *pAccount ) {   m_pAccount =pAccount ;   }

	uint64 SdkAccountInfoInit( AccountInfo &accountInfo ) ;

	const std::string& GetDyDeviceId( void ) const {   return m_dyDeviceId ;   }
	bool GetRequestPostData(std::string &reqUrl,std::string &psData, const pb::CVstarGameAccountCheck& info);
	bool SdkVerifyFromWeb(const pb::CVstarGameAccountCheck& info);
	bool PasserResponseData(const MCResopnse &psData, pb::SVstarGameAccountCheck& info);

private:
	bool         m_isPassSdkVerfy ;
	AccountInfo  m_sdkCheckAccountInfo ;

	std::string m_dyDeviceId ;
	AccountInfo *m_pAccount ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual bool HandleNetPack( NetPack &pack );

	virtual void HandlePing( NetPack &packet ) ;

	void HandleVersionCheck( NetPack &packet ) ;

	void HandleVstarGameAccountCheck( NetPack &packet ) ;

	void HandleSdkServerAccountCheck( NetPack &packet ) ;
	
	void HandleAccountCheck( NetPack &packet ) ;

	void HandleRegisterAccount( NetPack &packet ) ;

	void HandleChaneAccountInfo( NetPack &packet ) ;
	
	void HandleGetServerList( NetPack &packet ) ;

	void HandleSelectServer( NetPack &packet ) ;


} ;



#endif


