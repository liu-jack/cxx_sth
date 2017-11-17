#pragma once

#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include "detail/TcpAsyncConn.h"
#include "detail/NetFrameworkImplDetail.h"

class TcpAsyncServer : public ITcpConnMgr
{
	public:
		typedef std::map< int, TcpAsyncConn::Ptr > ConnectMap ;
		typedef ConnectMap::iterator ConnectMapItr ;

		typedef std::map< int, TcpAsyncConn::WPtr > ConnectWMap ;
		typedef ConnectWMap::iterator               ConnectWMapItr ;

		
		typedef std::multimap< time_t, TcpAsyncConn::Ptr > MConnectMap ;
		typedef MConnectMap::iterator MConnectMapItr ;

public:
			TcpAsyncServer( const std::string &port, const std::string &fixIP ="" );
			virtual ~TcpAsyncServer( void );

	public:
			virtual void Remove( uint64 sessionId ) ;
			//virtual void HadRecvPacket( int sessionId ) ;
			virtual boost::asio::io_service& GetIoService( void ) {   return m_ioSvc ;   }

	public:
			void Start( INetPackHandler *pPackHandler );
			void Stop( void );
			bool GetIsOpen( void ) {   return m_isOpen ;   }

			std::string &GetPort( void ) {   return m_port ;   }
			const string& GetIP( void ) {   return m_ip ;   }

			void SetJustConnectLimintTimeMS( uint64 time ) {   m_justConnectLimitTimeMS  =time ;   }

	private:
			void StartAccept( void );
			void HandleAccept( TcpAsyncConn::Ptr pNewConn, const boost::system::error_code& error );
			void SyncStop( void );

			void CheckWaitFirstPacketSessions( uint64 nowTimeMS ) ;

	private:
			bool                                m_isOpen;
			int                                 m_idSeed;
			std::string                         m_port ;
			INetPackHandler                    *m_pPackHandler;
			boost::asio::io_service            &m_ioSvc ;
			std::string                         m_ip ;
			boost::asio::ip::tcp::acceptor      m_acceptor;

			uint64                              m_justConnectLimitTimeMS ;

			ConnectMap                          m_conns ;
			ConnectMap                          m_justConnectConns ;
			MConnectMap                         m_waiForCloseConns ;
};

