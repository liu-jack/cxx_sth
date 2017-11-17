
#ifndef SERVER_CLIENT_H__
#define SERVER_CLIENT_H__

#include <string>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "StopWatch.h"

#include "SessionTypeDef.h"
#include "TcpAsyncClient.h"
#include "NetFrameworkDefines.h"

#include "def/TypeDef.h"
#include "google/protobuf/message.h"

class NetPack ;

class ServerClient : public INetPackHandler
{
public:
	ServerClient ( void ) ;
	virtual ~ServerClient( void ) ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// virtual 
public:
	//继承于INetPackHandler的消息处理方法
	virtual void OnConnect( TcpAsyncConn *pConn ) ;
	virtual void OnRecv( TcpAsyncConn *pConn, NetPack *pPack ) ;
	virtual void OnSend( uint64 sessionId, NetPack *pPack ) ;
	virtual void OnClose( uint64 sessionId ) ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
public:
	void Stop( void ) ;
	void Update( uint64 nowTimeMS, int64 diffMS ) ;

	virtual bool IsEnqueueToCommonPacketList( NetPack* pRecvPack ) {   return true ;   }

	virtual void ClientConnect( void ) {}
	virtual void ClientDisconnect( void ) {}
	virtual void ClientUpdate( uint64 nowTimeMS, int64 diffMS ) =0 ;
	virtual bool HandlePacket( NetPack &packet ) =0 ;

	void ReConnect( void )  ;

	bool IsCurClient(const string &host, const string &port);

	void SetDstInfo( const string &host, const string &port ) ;

	const char* GetIP( void ) {   return m_host.c_str() ;   }
	const char* GetPort( void ) {   return m_port.c_str() ;   }

	bool IsSettingAddress( void ) {   return !m_host.empty() && !m_port.empty() ;   }

	bool Connect( const std::string& host, const string &port, std::string& errMsg ) ;

	bool Send( NetPack &packet ) ;
	bool Send( NetPack *pPacket ) ;
	bool Send( const int opCode, const ::google::protobuf::Message &msg ) ;
	bool Send( const int opCode, const uint64 sessionId, const ::google::protobuf::Message &msg ) ;
	bool Send( const int opCode, const uint64 sessionId, char clientType, const ::google::protobuf::Message &msg );
	bool Send( NetPack &packet, char clientType);
	inline bool CanSend( void ) {    return m_canSend ;    }
	inline void SetCanSend( bool canSend )
	{
		m_canSend =canSend ;
	}

	uint64 GetDelayTime( void ) {   return m_delayTime ;   }
	void SetDelayTime( uint64 time ) {   m_delayTime =time ;    }

	char GetClientType( void ) {   return m_clientType ;   }
	void SetClientType( char type ) ;

	char GetDstServerType( void ) {   return m_dstServetType ;   }
	void SetDstServerType( char type ) ;

	uint32 GetConnectCount( void ) {   return m_connectCount ;   }

	uint32 GetMaxConnectCount( void ) {   return m_maxConnectCount ;   }
	void SetMaxConnectCount( int count ) {   m_maxConnectCount =count ;   }

	const char* GetClientName( void ) {   return m_clientName.c_str() ;   }
	const char* GetServerName( void ) {   return m_dstServerName.c_str() ;   }

	void SetPingDiffMS( uint64 time ) {   m_pingWatch.SetMax( time ) ;   }

	void SetConnectNow( void ) {   m_reConnectWatch.SetDone() ;   }
	void SetReConnectDiffMS( uint64 time ) {   m_reConnectWatch.SetMax( time ) ;   }

	void ReleaseConnectCount();
		
private:
	char                              m_clientType ;
	char                              m_dstServetType ;

	std::string                       m_clientName ;
	std::string                       m_dstServerName ;

	std::string                       m_port ;
	std::string                       m_host ;
	uint64                            m_delayTime ;

	uint32                            m_connectCount ;			//连接次数
	uint32                            m_maxConnectCount ;		//最大重连次数

	boost::scoped_ptr<TcpAsyncClient> m_pTcpClient;
	volatile bool                     m_canSend ;

	StopWatch                         m_pingWatch ;
	StopWatch                         m_reConnectWatch ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:
	virtual void SendPing( void ) =0 ;

	void SendPing( int pintOpCode ) ;
	void HandlePong( NetPack &packet ) ;

private:
	DLockPacketQueue          m_packetTable ;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// static 
public:
	static uint64 GetDelayNoticeLimitTimeMS( void ) {   return s_delayNoticLimitTimeMS ;   }
	static void SetDelayNoticeLimitTimeMS( uint64 time ) {   s_delayNoticLimitTimeMS =time ;   }

private:
	static uint64                     s_delayNoticLimitTimeMS ;

};


#endif
