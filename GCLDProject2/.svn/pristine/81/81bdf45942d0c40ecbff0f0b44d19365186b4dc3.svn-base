#pragma once

#include <string>
#include "def/TypeDef.h"
class NetPack;

class TcpAsyncConn ;

class INetPackHandler
{
	public:
		virtual ~INetPackHandler( void ) {}

		virtual void OnConnect( TcpAsyncConn *pConn ) = 0;
		virtual void OnRecv( TcpAsyncConn *pConn, NetPack *pPack ) = 0;
		virtual void OnSend( uint64 sessionId, NetPack *pPack ) = 0;
		virtual void OnClose( uint64 sessionId ) = 0;
};





