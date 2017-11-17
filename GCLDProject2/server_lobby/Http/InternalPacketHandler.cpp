
#include <assert.h>

#include "InternalPacketHandler.h"

//#include "ServerDef.h"
#include "memory_buffer/NetPack.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 内部线程间数据传递相关
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InternalPacketHandler::InternalPacketHandler( void )
{
}

InternalPacketHandler::~InternalPacketHandler( void )
{
	m_internalPacketAllocator.FreePacketList( m_internalPackets ) ;
	m_internalPacketAllocator.FreeAllPackets() ;
}

NetPack* InternalPacketHandler::AllocInternalPacket( uint16 opCode,size_t pSize)
{
	NetPack *m_TempData = m_internalPacketAllocator.AllocOnePacket(opCode,pSize);//得到一个空闲的Datapacket
	//m_TempData->setPosition( 0 );
	return m_TempData;
}


void InternalPacketHandler::FreeInternalPacket(NetPack* pDataPacket)
{
	m_internalPacketAllocator.FreePacket( pDataPacket ) ;
}

void InternalPacketHandler::PostInternalPacket( NetPack *pPacket )
{
	assert( pPacket != NULL ) ;
	assert( pPacket->GetPacketType() != 0 ) ;
	assert( pPacket->GetPacketType() < PACKET_TYPE_END ) ;
	//assert( pPacket->GetOpcode() != 0 ) ;
	m_internalPackets.Enqueue( pPacket ) ;
}

void InternalPacketHandler::ProcessInternalPackets( void )
{
	NetPack* p;
	while (m_internalPackets.Dequeue(p))
	{
		DispatchInternalPacket( p );

		FreeInternalPacket( p );
		delete p;
	}
}



void InternalPacketHandler::DispatchInternalPacket( NetPack *pPacket )
{
	static DispatchFunTable s_dispatchTable =GetInitDispatchTable() ;

	size_t packetType =pPacket->GetPacketType() ;
	if( packetType < s_dispatchTable.size() )
	{
		(this->*s_dispatchTable[packetType])( pPacket ) ;
	}
	else
	{
		DispatchUnknowInternalPacket( pPacket ) ;
	}
}	



InternalPacketHandler::DispatchFunTable InternalPacketHandler::GetInitDispatchTable( void )
{
	DispatchFunTable table ;
	//table.resize( jxSrvDef::ServerTypeCount ) ;

	//table[ jxSrvDef::InvalidServer ] = &InternalPacketHandler::DispatchUnknowInternalPacket ;	//未定义服务器类型
	//table[ jxSrvDef::GateServer    ] = &InternalPacketHandler::DispatchGateInternalPacket ;		//网关服务器
	//table[ jxSrvDef::LogServer     ] = &InternalPacketHandler::DispatchLoggerInternalPacket ;   //日志服务器
	//table[ jxSrvDef::SessionServer ] = &InternalPacketHandler::DispatchSessionInternalPacket ;	//会话服务器
	//table[ jxSrvDef::DBServer      ] = &InternalPacketHandler::DispatchDatabaseInternalPacket ;	//数据服务器
	//table[ jxSrvDef::GameServer    ] = &InternalPacketHandler::DispatchGameInternalPacket ;		//游戏服务器
	//table[ jxSrvDef::HttpServer    ] = &InternalPacketHandler::DispatchHttpInternalPacket ;		//http服务器返回的数据
	
	return table ;
}
