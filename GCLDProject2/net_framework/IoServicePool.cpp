
#include "IoServicePool.h"

#include <signal.h>

#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "Logger.h"
#include "def/MmoAssert.h"


IoServicePool::IoServicePool( void )
: m_nextIoServiceIndex(0)
, m_oneIoServiceThreadCount( 1 )
{
	// Server io service init .
	m_spServerIoService.reset( new boost::asio::io_service() ) ;
	m_spServerWork.reset( new boost::asio::io_service::work( *m_spServerIoService ) ) ;
	m_spServerIoThread.reset( new boost::thread( boost::bind(&boost::asio::io_service::run, m_spServerIoService ) ) ) ;

	std::vector<int> vec ;

#ifndef WIN32
	vec.push_back( SIGPIPE ) ;
#endif

	AddIoSign( m_spServerIoService.get(), vec ) ;

}

IoServicePool::~IoServicePool( void )
{
}

void IoServicePool::Run( int ioServiceSize )
{
	if( ioServiceSize <= 0 || m_oneIoServiceThreadCount <= 0 )
	{
		throw std::runtime_error("IoServicePool size <= 0 or thread per io_service <= 0.") ;
	}

	m_oneIoServiceThreadCount =1 ;
	WLOG( "Current just support each io_service one thread." ) ;

	std::vector<int> vec ;

#ifndef WIN32
	vec.push_back( SIGPIPE ) ;
#endif

	// Give all the io_services work to do so that their run() functions will not
	// exit until they are explicitly stopped.
	for( int i = 0; i < ioServiceSize; ++i )
	{
		SpIoServicePtr spIoService( new boost::asio::io_service() ) ;

		SpWorkPtr spWork( new boost::asio::io_service::work( *spIoService ) ) ;

		m_ioServices.push_back( spIoService ) ;
		m_work.push_back( spWork ) ;

		AddIoSign( spIoService.get(), vec ) ;
	}

	// Create a pool of threads to run all of the io_services.
	for( size_t i = 0; i < m_ioServices.size() ; ++i )
	{
		for( int j =0; j < m_oneIoServiceThreadCount;  ++j )
		{
			SpThread spThread( new boost::thread( boost::bind(&boost::asio::io_service::run, m_ioServices[i] ) ) ) ;
			m_threads.push_back(spThread) ;

			//SocketCuntInfo &countInfo =m_socketCount[ spThread->get_id() ] ;
			//countInfo.count           =0 ;
			//countInfo.spIoService     =m_ioServices[i] ;
		}
	}

	NLOG( "Notice:------------------------------------------------------------------" ) ;
	NLOG( "hardware_concurrency() is: %u", boost::thread::hardware_concurrency() ) ;
	//NLOG( "physical_concurrency() is: %u", boost::thread::physical_concurrency() ) ;

	NLOG( "Current use %u io_service, each one use %u threads.", m_ioServices.size(), m_oneIoServiceThreadCount ) ;
	NLOG( "-------------------------------------------------------------------------" ) ;
}

void IoServicePool::Stop( void )
{
	// Explicitly stop all io_services.
	for( std::size_t i = 0; i < m_ioServices.size() ; ++i )
	{
		m_ioServices[i]->stop() ;
		m_work[i].reset() ;         // 要在 io_service 之前把对应的 io_service::work 先释放掉，不然会在shared_ptr释放是不返回。
	}

	// Wait for all threads in the pool to exit.
	for( std::size_t i = 0; i < m_threads.size() ; ++i )
	{
		m_threads[i]->join() ;
		m_threads[i].reset() ;
	}

	m_spServerIoService->stop() ;
	m_spServerWork.reset() ;

	m_spServerIoThread->join() ;
	m_spServerIoThread.reset() ;
}

boost::asio::io_service& IoServicePool::GetIoService( void )
{
	// Use a round-robin scheme to choose the next io_service to use.
	boost::asio::io_service& io_service =*m_ioServices[ m_nextIoServiceIndex++ ] ;
	
	if( m_nextIoServiceIndex == m_ioServices.size() )
	{
		m_nextIoServiceIndex = 0;
	}

	return io_service;
}

void IoServicePool::AddIoSign( IoServicePtr ptr, const std::vector<int> &signsTable )
{
	SpSign spSign ;

	SignMap::iterator sItr =m_signs.find( ptr ) ;
	if( sItr != m_signs.end() )
	{
		spSign =sItr->second ;
	}
	else
	{
		spSign.reset( new boost::asio::signal_set( *ptr ) ) ;
		m_signs.insert( std::make_pair( ptr, spSign ) ) ;
	}

	for( std::vector<int>::const_iterator itr =signsTable.begin(); itr != signsTable.end(); ++itr )
	{
		spSign->add( *itr ) ;
	}

	spSign->async_wait( boost::bind( &IoServicePool::HandlerSigns, this, _1, _2 ) ) ;
}

void IoServicePool::HandlerSigns( const boost::system::error_code& error, int signal_number )
{
	if( !error )
	{
		ELOG( "Get sign %d ?????????????????????", signal_number ) ;
	}
}

void IoServicePool::GetAllThreadId( std::vector< boost::thread::id > &vec )
{
	vec.reserve( m_threads.size() * 2 ) ;

	GetNetPacketThreadId( vec ) ;
	vec.push_back( m_spServerIoThread->get_id() ) ; 
}

void IoServicePool::GetNetPacketThreadId( std::vector< boost::thread::id > &vec )
{
	vec.reserve( m_threads.size() * 2 ) ;

	for( std::vector< SpThread >::iterator itr =m_threads.begin(); itr != m_threads.end(); ++itr )
	{
		vec.push_back( (*itr)->get_id() ) ;
	}
}

//boost::asio::io_service& IoServicePool::GetIoServiceByCount( void )
//{
//	int min                    =0 ;
//	SocketCuntInfo *pCountInfo =NULL ;
//	SocketCountMapItr endItr   =m_socketCount.end() ;
//
//	for( SocketCountMapItr itr =m_socketCount.begin(); itr != endItr; ++itr )
//	{
//		if( itr->second.count < min )
//		{
//			pCountInfo =&itr->second ;
//		}
//	}
//
//	DLOG( "IoServicePool::GetIoServiceByCount( %x )", ptr ) ;
//
//	return *ptr ;
//}
//
//void IoServicePool::AddUseCount( boost::asio::io_service &io )
//{
//	SocketCountMapItr itr =m_socketCount.find( &io ) ;
//	if( itr == m_socketCount.end() )
//	{
//		ASSERT( false ) ;
//		ELOG( "IoServicePool::AddUseCount() can not find io_service ?? " ) ;
//		return ;
//	}
//
//	if( ++itr->second < 0 )
//	{
//		ASSERT( false ) ;
//		ELOG( "IoServicePool::AddUseCount() ++use_count is bigger than max( int ) ?? " ) ;
//		itr->second =0 ;
//	}
//}
//
//void IoServicePool::ReduceUseCount( boost::asio::io_service &io )
//{
//	SocketCountMapItr itr =m_socketCount.find( &io ) ;
//	if( itr == m_socketCount.end() )
//	{
//		ASSERT( false ) ;
//		ELOG( "IoServicePool::ReduceUseCount() can not find io_service ?? " ) ;
//		return ;
//	}
//
//	if( --itr->second < 0 )
//	{
//		ASSERT( false ) ;
//		ELOG( "IoServicePool::AddUseCount() is less than 0 ???? " ) ;
//		itr->second =0 ;
//	}
//
//}



