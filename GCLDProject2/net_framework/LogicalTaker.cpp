

#include "LogicalTaker.h"

#include "Logger.h"
#include "OS.h"
#include "Session.h"

#include "def/MmoAssert.h"
#include "utility/STLUtility.h"

#include "Opcode.pb.h"


LogicalTaker::LogicalTaker( void )
{
	m_sessionCount            =0 ;
	m_updateTimeMS            =0 ;
	m_preUpdateTimeMS         =0 ;
	m_notRecvPackLimitTimeMS  =5000 ;
	m_threadUpdateLimitTimeMS =100 ;
}


LogicalTaker::~LogicalTaker( void )
{
	Stop() ;

	//std::for_each( m_sessions.begin(), m_sessions.end(), DeleteSecondValue() ) ;
	for( SessionMap::iterator itr =m_sessions.begin(); itr != m_sessions.end(); ++itr )
	{
		itr->second->SetIsVerification( false ) ;
		delete itr->second ;
	}
	m_sessions.clear() ;

	//DeleteAllPtrInLFQueue<Session*>( m_pendingSession ) ;
	Session *pSession =NULL ;
	while( m_pendingSession.Dequeue( pSession ) )
	{
		pSession->SetIsVerification( false ) ;
		delete pSession ;
		pSession =NULL ;
	}
}


void LogicalTaker::Start( void )
{
	LLOG("LogicalTaker::Start");
	m_logicalThread.Start( boost::bind( &LogicalTaker::LogicalThread, this ) ) ;
}

void LogicalTaker::Stop( void )
{
	m_logicalThread.Stop() ;
}


void LogicalTaker::LogicalThread( void )
{
	NLOG( "LogicalTaker::LogicalThread() start running ~" ) ;

	sOS.SRand() ;
	m_preUpdateTimeMS =sOS.TimeMS() ;

	while( m_logicalThread.IsStart() )
	{
		uint64 begTime = sOS.TimeMS() ;

		int64 twiceUpdateDiff =begTime - m_preUpdateTimeMS ;
		ASSERT( twiceUpdateDiff >= 0 ) ;

		m_preUpdateTimeMS     =begTime ;

		TakePendingSession() ;
		TakeCloseSession() ; // Tips: TakeCloseSession() must after TakePendingSession()

		BeforeSeesionUpdate( begTime, twiceUpdateDiff ) ;

		// Session update
		SessionMapItr itr ;
		SessionMapItr tmpItr ;

		for( itr =m_sessions.begin(); itr != m_sessions.end(); /*++itr*/ )
		{
			tmpItr =itr++ ;
			Session *pSession =tmpItr->second ;

			if( pSession->GetIsHadClosed() )
			{
				continue ;
			}

			if( pSession->GetIsNeedClose() )
			{
				NLOG( "LogicalThread initiative close session %llu, type %s .", pSession->GetSessionId(), pSession->GetSessionTypeName() ) ;

				pSession->CloseSession() ;

				continue ;
			}


			pSession->Update( begTime, twiceUpdateDiff ) ;

			if( pSession->GetPreRecvTime() + m_notRecvPackLimitTimeMS < begTime )
			{
				int64 diffTime =begTime - pSession->GetPreRecvTime() ;
				WLOG( "LogicalThread, Will be closed id:%llu, type:%s,%lld not packet recv more than %lld MS !!!!", pSession->GetSessionId(), pSession->GetSessionTypeName(), diffTime, m_notRecvPackLimitTimeMS ) ;
				pSession->CloseSession() ;
			}
		}

		// After session update ;
		AfterSessionUpdate( begTime, twiceUpdateDiff ) ;

		int64 updateUseTime =sOS.TimeMS() - begTime ; ASSERT( updateUseTime >= 0 ) ;
		m_updateTimeMS = updateUseTime ;

		int64 diffTime = m_threadUpdateLimitTimeMS - m_updateTimeMS ;
		if( diffTime > 0 )
		{
			m_logicalThread.Sleep( static_cast<int>( diffTime ) ) ;
		}
		else
		{
			WLOG( "LogicalThread update time more than %llu ms use %llu ms", m_threadUpdateLimitTimeMS,m_updateTimeMS) ;
		}
	}

	NLOG( "LogicalThread is shoped!!!!" ) ;
}


void LogicalTaker::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	ASSERT( false ) ;
}

void LogicalTaker::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	ASSERT( false ) ;
}



void LogicalTaker::PutClostSessionId( uint64 sessionId )
{
	m_closeloseQueue.Enqueue( sessionId ) ;
}

void LogicalTaker::TakeCloseSession( void )
{
	uint64 sessionId =0 ;
	while( m_closeloseQueue.Dequeue( sessionId ) )
	{
		SessionMapItr itr =m_sessions.find( sessionId ) ;
		if( itr != m_sessions.end() )
		{
			delete itr->second ;
			itr->second =NULL ;
			m_sessions.erase( itr ) ;
			m_sessionCount =m_sessions.size() ;
		}

	}
}

void LogicalTaker::PutSeesion( Session *pSession )
{
	m_pendingSession.Enqueue( pSession ) ;
}

void LogicalTaker::TakePendingSession( void )
{
	Session *pSession =NULL ;
	while( m_pendingSession.Dequeue( pSession ) )
	{
		m_sessions.insert( std::make_pair( pSession->GetSessionId(), pSession ) ) ;
		m_sessionCount =m_sessions.size() ;
	}
}

