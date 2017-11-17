
#ifndef CROSS_SERVER_LOGICAL_THREAD_H__
#define CROSS_SERVER_LOGICAL_THREAD_H__

#include "net_framework/LogicalTaker.h"


class CrossServerLogicalTaker: public LogicalTaker
{
public:
	CrossServerLogicalTaker( void ) ;
	virtual ~CrossServerLogicalTaker( void ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
} ;


#endif