
#ifndef CENTER_SERVER_SESSION_H__
#define CENTER_SERVER_SESSION_H__

#include "net_framework/Session.h"
#include "memory_buffer/MemoryPool.h"


class ISimpleHandler;

class GameServerSession : public Session
{
	DECLARE_OBJ_POOL(GameServerSession)

public:
	GameServerSession( void ) ;
	virtual ~GameServerSession( void ) ;

	virtual void Update( int64 diffMS );
	virtual void HandleNetPack( NetPack &pack ) ;

private:
	boost::scoped_ptr<ISimpleHandler> m_handler;

} ;



#endif


