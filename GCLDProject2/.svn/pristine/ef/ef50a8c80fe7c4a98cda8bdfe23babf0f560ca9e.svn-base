#pragma once

#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include "def/TypeDef.h"
class TcpAsyncConn ;

class ITcpConnMgr
{
	public:
		virtual ~ITcpConnMgr() {}

		virtual void Remove( uint64 sessionId ) = 0;
		virtual void HadRecvPacket( uint64 sessionId ) {}

		virtual boost::asio::io_service& GetIoService( void ) = 0;
};

