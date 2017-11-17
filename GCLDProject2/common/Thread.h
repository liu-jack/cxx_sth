#ifndef THREAD_H__
#define THREAD_H__

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

class CThread
{
public:
	CThread( void ):m_isStarted( false )
	{
	}

	~CThread( void )
	{
		if( IsStart() )
		{
			Stop() ;
		}
	}

public:
	bool IsStart( void )
	{
		return m_isStarted ;
	}

	void Start( const boost::function< void ( void ) > &fun )
	{
		if( !IsStart() )
		{
			m_isStarted =true ;
			m_thread.reset( new boost::thread( fun ) );
		}
	}

	void Stop( void )
	{
		m_isStarted =false ;
		if( m_thread )
		{
			m_thread->join();
			m_thread.reset();
		}
	}

	void Sleep( int ms )
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds( ms ) ) ;
	}

private:
	bool                             m_isStarted ;
	boost::scoped_ptr<boost::thread> m_thread ;
};

#endif
