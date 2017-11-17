
#ifndef SESSION_CENTER_H__
#define SESSION_CENTER_H__

#include <vector>

#include <boost/atomic.hpp>
#include <boost/smart_ptr.hpp>

#include "google/protobuf/message.h"

#include "SessionTypeDef.h"
#include "NetFrameworkDefines.h"

#include "def/TypeDef.h"


class Server ;
class NetPack ;
class TcpAsyncConn ;

class Session : public boost::noncopyable, public INetPackHandler
{
public:
	Session( void ) ;
	virtual ~Session( void ) ;

	void Init( void ) ;
	void Clear( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// virtual
public:
	virtual void OnConnect( TcpAsyncConn *pConn ) ;
	virtual void OnRecv( TcpAsyncConn *pConn, NetPack *pPack ) ;
	virtual void OnSend( uint64 sessionId, NetPack *pPack ) ;
	virtual void OnClose( uint64 sessionId ) ;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
public:
	const char* GetSessionTypeName( void ) ;

	bool GetIsSetSessionType( void ) ;

	uint64 GetSessionId( void ) const {   return m_sessionId ;   }
	void SetSessionId( uint64 id ) {   m_sessionId =id ;   }

	Server* GetServer( void ) const {   return m_pServer ;   }
	void SetServer( Server *pServer ) { m_pServer =pServer ;   }

	uint64 GetPreRecvTime( void ) const {   return m_preRecvPacketTime ;    }
	void SetPreRecvTime( uint64 time ) {   m_preRecvPacketTime =time ;    }

	uint64 GetDelayTime( void ) const {   return m_delayTime ;   }
	void SetDelayTime( uint64 time ) {   m_delayTime =time ;    }

	int GetSessionType( void ) const {   return m_sessionType ;   }
	void SetSessionType( int type ) {   m_sessionType =type ;   }

	bool GetIsVerification( void ) const {   return m_isVerification ;   }
	void SetIsVerification( bool isVerify ) {   m_isVerification =isVerify ;   }

	bool GetIsInWorldThread( void ) const {   return m_isInWorldThread ;   }
	void SetIsInWorldThread( bool isInWorldThread ) {   m_isInWorldThread =isInWorldThread ;   }

	bool GetIsServerSession( void ) const {   return m_isServerSession ;   }
	void SetIsServerSession( bool isServer ) {   m_isServerSession =isServer;   }

	bool GetIsHadClosed( void ) const {   return m_isClosed ;   }
	void SetIsHadClosed( bool isClose ) {   m_isClosed =isClose ;   }

	bool GetIsNeedClose( void ) const {   return m_isNeedClose ;   }
	void SetIsNeedClose( bool isClosed ) {   m_isNeedClose =isClosed ;   }

	const SessionAddress& GetAddress( void )  const {   return m_address ;   }
	void SetAddress( const SessionAddress &address ) {   m_address =address ;   }

	const SessionPairId& GetPairId( void ) const {    return m_pairId ;   }
	void SetPairId( const SessionPairId &pairId ) {   m_pairId =pairId ;   }

	uint64 GetWaitVerfyTime( void ) {   return m_waitVerfyTimeMS ;   }
	void SetWaitVerfyTime( uint64 timeMS ) {   m_waitVerfyTimeMS =timeMS ;   }

private:
	bool                  m_isClosed ;
	bool                  m_isNeedClose ;
	bool                  m_isServerSession ;
	bool                  m_isVerification ;
	bool                  m_isInWorldThread ;
	SessionId             m_sessionId ;
	SetssionType          m_sessionType ;
	Server               *m_pServer ;
	uint64                m_waitVerfyTimeMS ;
	uint64                m_delayTime ;
	uint64                m_preRecvPacketTime ;

	SessionPairId         m_pairId ;
	SessionAddress        m_address ;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:
	void Kick( int32 errorCode ) ;
	void CloseSession( void ) ;

	bool GetOnePacket( NetPack *&pPacket ) {   return m_packetTable.Dequeue( pPacket ) ;   }
	void PostInputPacket( NetPack *pPacket ) {   m_packetTable.Enqueue( pPacket ) ;   }

	void SetConn( TcpAsyncConn *pConn ) ;

	virtual bool HandleNetPack( NetPack &pack ) =0;

	void Send( NetPack &packet ) const ;
	void Send( NetPack *pPacket ) const ;
	void Send( NetPack &packet,const uint64 sessionId, char clientType) const;
	void Send( NetPack *pPacket,const uint64 sessionId, char clientType) const;
	void Send( const int opCode, const ::google::protobuf::Message &msg ) const ;
	void Send( const int opCode, const uint64 sessionId, const ::google::protobuf::Message &msg ) const ;
	void Send( const int opCode, const uint64 sessionId,char clientType, const ::google::protobuf::Message &msg ) const;

	void HandlePing( NetPack &packet, int pongOpCode ) ;


private:
	DLockPacketQueue                m_packetTable ;
	TcpAsyncConn*                   m_spConn ;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// static 
public:
	static uint64 GetDelayNoticeLimitTimeMS( void ) {   return s_delayNoticLimitTimeMS ;   }
	static void SetDelayNoticeLimitTimeMS( uint64 time ) {   s_delayNoticLimitTimeMS =time ;   }

	static uint32 GetTakePacketLimit( void ) {   return s_takePacketLimit ;   }
	static void SetTakePacketLimit( uint32 limit ) {   s_takePacketLimit =limit ;   }

private:
	static uint32                     s_takePacketLimit ;
	static uint64                     s_delayNoticLimitTimeMS ;
};

#endif
