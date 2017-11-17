

#ifndef INTERNAL_PACKET_HANDLER_H__
#define INTERNAL_PACKET_HANDLER_H__


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 内部线程间数据传递相关
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PacketAllocator.h"

//class NetPack ;

class InternalPacketHandler
{
public:
	InternalPacketHandler( void ) ;
	virtual ~InternalPacketHandler( void ) ;

	// 线程间数据传分配
public:
	NetPack* AllocInternalPacket( uint16 opCode,size_t pSize);
	void FreeInternalPacket( NetPack* pPacket ) ;

	// 以下使用的 NetPack 由 AllocInternalPacket() 分配, FreeInternalPacket() 回收
public:
	void ProcessInternalPackets( void ) ;

	void DispatchInternalPacket( NetPack *pPacket ) ;

	void PostInternalPacket( NetPack *pPacket ) ;

	inline size_t GetInternalMessageCount( void )
	{
		return m_internalPackets.Size();
	}

	// 虚函数
public:
	virtual void DispatchUnknowInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

	virtual void DispatchLoggerInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

	virtual void DispatchGameInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

	virtual void DispatchGateInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

	virtual void DispatchSessionInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

	virtual void DispatchDatabaseInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

	virtual void DispatchHttpInternalPacket( NetPack *pPacket ) {  assert( false ) ; }

private:
	FLockPacketQueue m_internalPackets ;         // 线程间交换信息表
	PacketAllocator            m_internalPacketAllocator ;

	// 静态
public:
	typedef void ( InternalPacketHandler::* DispatchFunctionPtr )( NetPack *pPacket ) ;
	typedef std::vector< DispatchFunctionPtr > DispatchFunTable ;
	 
public:
	static DispatchFunTable GetInitDispatchTable( void ) ;
} ;

#endif