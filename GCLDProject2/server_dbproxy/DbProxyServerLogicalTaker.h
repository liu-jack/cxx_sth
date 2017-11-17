
#ifndef CENTER_SERVER_LOGICAL_THREAD_H__
#define CENTER_SERVER_LOGICAL_THREAD_H__

#include "net_framework/LogicalTaker.h"


class DbProxyServerLogicalTaker: public LogicalTaker
{
public:
	DbProxyServerLogicalTaker( void ) ;
	virtual ~DbProxyServerLogicalTaker( void ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
} ;


#endif