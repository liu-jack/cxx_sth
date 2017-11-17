
#ifndef LOBBY_SERVER_LOGICAL_THREAD_H__
#define LOBBY_SERVER_LOGICAL_THREAD_H__

#include "net_framework/LogicalTaker.h"


class LobbyServerLogicalTaker: public LogicalTaker
{
public:
	LobbyServerLogicalTaker( void ) ;
	virtual ~LobbyServerLogicalTaker( void ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
} ;


#endif