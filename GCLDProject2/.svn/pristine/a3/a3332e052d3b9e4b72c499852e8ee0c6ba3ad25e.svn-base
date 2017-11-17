
#ifndef LOGICAL_THREAD_H__
#define LOGICAL_THREAD_H__

#include <map>

#include "Thread.h"
#include "SessionTypeDef.h"

#include "def/TypeDef.h"
#include "lock_free/LockFreeQueue.h"

class Session ;

class LogicalTaker
{
public:
	LogicalTaker( void ) ;
	virtual ~LogicalTaker( void ) ;

	void Start( void ) ;
	void Stop( void ) ;

	void LogicalThread( void ) ;

	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;

	void PutSeesion( Session *pSession ) ;
	void PutClostSessionId( uint64 sessionId ) ;

	void TakeCloseSession( void ) ;
	void TakePendingSession( void ) ;

	size_t GetSessionCount( void ) {   return m_sessionCount ;   }
	uint64 GetUpdateUseTimeMS( void ) {   return m_updateTimeMS ;   }

	void SetNotRecvPackLimitTimeMS( uint64 time ) {   m_notRecvPackLimitTimeMS =time ;   }
	void SetThreadUpdateLimitTimeMS( uint64 time ) {   m_threadUpdateLimitTimeMS =time ;}

	virtual Session* GetSession( uint64 sessionId )
	{
		SessionMapItr itr =m_sessions.find( sessionId ) ;
		return ( itr != m_sessions.end() ? itr->second : NULL ) ;
	}


protected:
	CThread              m_logicalThread ;

	size_t               m_sessionCount ;
	uint64               m_updateTimeMS ;
	uint64               m_preUpdateTimeMS ;
	uint64               m_notRecvPackLimitTimeMS ;
	uint64               m_threadUpdateLimitTimeMS ;

	SessionMap           m_sessions ;

	DLockSessionQueue    m_pendingSession ;
	DLockSessionIdQueue  m_closeloseQueue ;
} ;


#endif
