#pragma once

#include <assert.h>

#include <vector>
#include <memory>
#include <boost/function.hpp>
#include "memory_buffer/NetPack.h"

class NetPackDispatcher
{
public:
	typedef boost::function<void (uint64)>                     SeesionCloseFunction;
	typedef boost::function<void (uint64, const std::string&)> SeesionOpenFunction;
	typedef boost::function<void (uint64, NetPack*)>           PacketFunction;

public:
	NetPackDispatcher( void )
	{
		m_defaultPacketFun = boost::bind( &NetPackDispatcher::DefaultHandlePacket, this, _1, _2 ) ;

		m_sessionOpenFun   = boost::bind( &NetPackDispatcher::DefaultSessionOpenFunction, this, _1, _2 ) ;
		m_sessionCloseFun  = boost::bind( &NetPackDispatcher::DefaultSessionCloseFunction, this, _1) ;
	}

	// Set handler
public:
	void RegistePacketHandler(const PacketFunction& defaultFun)
	{
		m_defaultPacketFun = defaultFun;
	}

	void RegisteOpenSessionHandle( const SeesionOpenFunction &sessionFunction )
	{
		m_sessionOpenFun =sessionFunction ;
	}

	void RegisteCloseSessionHandle( const SeesionCloseFunction &sessionFunction )
	{
		m_sessionCloseFun =sessionFunction ;
	}

	// Handler
public:
	void OpenSessionHandle( uint64 sessionId, const std::string &ip )
	{
		m_sessionOpenFun( sessionId, ip ) ;
	}

	void CloseSessionHandle( uint64 sessionId )
	{
		m_sessionCloseFun( sessionId ) ;
	}

	void HandlePacket(uint64 sessionId, NetPack* pPacket)
	{
		m_defaultPacketFun(sessionId, pPacket );
	}

private:
	PacketFunction              m_defaultPacketFun;
	SeesionOpenFunction         m_sessionOpenFun;
	SeesionCloseFunction        m_sessionCloseFun;

private:
	void DefaultSessionOpenFunction( uint64 sessinId, const std::string &ip )
	{
		assert( false ) ;
	}

	void DefaultSessionCloseFunction( uint64 sessinId )
	{
		assert( false ) ;
	}

	void DefaultHandlePacket(uint64 sessionId, NetPack* pPacket)
	{
		assert( false ) ;
	}
};
