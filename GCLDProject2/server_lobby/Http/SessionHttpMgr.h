#ifndef SESSION_HTTP_MANAGER_H__
#define SESSION_HTTP_MANAGER_H__
#include "def/TypeDef.h"
#include "mongoose.h"
//#include "HttpManager.h"
#include "InternalPacketHandler.h"
class SessionHttpManager //: public HttpManager, public InternalPacketHandler
{
public:
	SessionHttpManager( void ) ;
	virtual ~SessionHttpManager( void ) ;

	virtual void WhenRunning( void ) ;

	void SetSessionServer( InternalPacketHandler *pServer )
	{
		m_pSSServer =pServer ;
	}

	InternalPacketHandler* GetSessionServer( void )
	{
		return m_pSSServer ;
	}

	void SetSleepTime( uint64 time )
	{
		m_sleepTimeMS =time ;
	}

	void SetTimeEvent( mg_connection *pConnect, int seconds )
	{
		pConnect->ev_timer_time =mg_time() + seconds ;
	}

	bool AddWaitingConnect( mg_connection *pConnect, int waitTimeSeconds )
	{
		bool isInsert =m_waitFinishConnect.insert( pConnect ).second ;
		if( isInsert && waitTimeSeconds != 0 )
		{
			SetTimeEvent( pConnect, waitTimeSeconds ) ;
		}
		return isInsert ;
	}

	void CheckWaitingConnect( int count )
	{
		assert( m_waitFinishConnect.size() == count ) ;
	}

	bool RemoveWaitingConnect( mg_connection *pConnect )
	{
		return m_waitFinishConnect.erase( pConnect ) == 1 ;
	}

	void SetLoginUrl( const std::string &url )
	{
		m_loginUrl =url ;
	}

	const std::string& GetLoginUrl( void )
	{
		return m_loginUrl ;
	}

public:
	std::set< mg_connection* > m_waitFinishConnect ;

public:
	static SessionHttpManager& GetInstance( void ) ;

	static int GetConnectId( void )
	{
		return ++s_connectId ;
	}

public:
	virtual void DispatchSessionInternalPacket( NetPack *pPacket ) ;


private:
	static int      s_connectId ;

private:
	uint64          m_preUpdateTimeMS ;
	uint64          m_sleepTimeMS ;
	std::string     m_loginUrl ;
	InternalPacketHandler *m_pSSServer;    //所属会话服务器
} ;

#define sHttpMgr SessionHttpManager::GetInstance()

void HttpMainEventhandler( mg_connection *pConn, int eventId, void *pMsg ) ;
void HttpLoginEventhandler( mg_connection *pConn, int eventId, void *pMsg ) ;

#endif