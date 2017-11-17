
#ifndef CROSS_SERVER_SESSION_H__
#define CROSS_SERVER_SESSION_H__


#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"


class CrossServerSession : public Session
{
	DECLARE_OBJ_POOL(CrossServerSession)

public:
	typedef void (CrossServerSession::*NetPackHandler)(NetPack&);
	typedef std::vector<NetPackHandler> HandlerArray;

public:
	static HandlerArray RegisteServerHandlers( void );

private:
	static HandlerArray s_serverHandlers ;

public:
	CrossServerSession( void ) ;
	virtual ~CrossServerSession( void ) ;

	virtual void Update( uint64 nowTimeMS, int64 diffMS ) ;
public:
	typedef std::vector< std::string > ServerNameTable ;
	typedef ServerNameTable::iterator ServerNameTableItr  ;

	ServerNameTable& GetServerNames( void ) {   return m_strNames ;   }

private:
	ServerNameTable m_strNames ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Handler
public:
	virtual bool HandleNetPack( NetPack &pack );

	virtual void HandlePing( NetPack &packet ) ;

	void HandleRegister( NetPack &packet ) ;

	void HandleGetEventLogServers( NetPack &packet ) ;

} ;



#endif


