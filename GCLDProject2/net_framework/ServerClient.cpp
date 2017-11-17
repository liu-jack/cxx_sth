#include <fstream>
#include <string>

#include "ServerClient.h"
#include "Logger.h"
#include "OS.h"
#include "IoServicePool.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"


#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "Session.h"


uint64 ServerClient::s_delayNoticLimitTimeMS =10;


ServerClient::ServerClient( void )
{
	m_connectCount =0 ;		//连接次数

	SetCanSend( false ) ; // 不让继续执行Send操作

	m_pTcpClient.reset( new TcpAsyncClient( sIoPool.GetServerIoService() ) ) ;		//重置tcp连接

	SetDelayTime( 0 ) ;		//延迟时间

	SetMaxConnectCount( ~0 ) ; // 默认设置重连无数次，子类根据需求改变

	SetClientType( PACKET_TYPE_END ) ;		//初始化子服务器类型
	SetDstServerType( PACKET_TYPE_END ) ;	//初始化目标服务器类型
}


ServerClient::~ServerClient( void )
{
	DeleteAllPtrInLFQueue<NetPack*>( m_packetTable ) ;
}

//断开连接
void ServerClient::Stop( void )
{
	//if (m_connectCount > 0)
	//{
	//	--m_connectCount;
	//}
	m_pTcpClient->Stop();
}

//子服务器更新
void ServerClient::Update( uint64 nowTimeMS, int64 diffMS )
{
	// Tips: whether had connect or not, the packet must be taker
	NetPack *pPacket =NULL ;
	uint32 takeCount = Session::GetTakePacketLimit();
	while(takeCount > 0 && m_packetTable.Dequeue( pPacket ) )		//取出一条消息
	{
		takeCount--;
		std::auto_ptr<NetPack> guard( pPacket );

		if( !HandlePacket( *pPacket ) )		//处理消息
		{
			ELOG ("Client type %s receive not handle packet from %s, opName= [ %s ( %d ) ]"
					, GetClientName(), pPacket->GetPacketTypeName() , pPacket->GetOpcodeName(), pPacket->GetOpcode() );

			ASSERT( false ) ;
		}
	}

	//未设置连接ip与端口  tcp连接正在断开  tcp连接正在启动  则退出
	if( !IsSettingAddress() || m_pTcpClient->IsStopping() || m_pTcpClient->IsStarting() )
	{
		return ;
	}

	//tcp连接已经关闭
	if( m_pTcpClient->IsStopped() )
	{
		m_reConnectWatch.Update( diffMS ) ;		//重连时间增加
		if( !m_reConnectWatch.Done() )			//m_tick >= m_max?		定时重连一次
		{
			return ;
		}

		m_reConnectWatch.Reset() ;	//计时器重置
		ReConnect() ;				//重连
		return ;
	}

	m_pingWatch.Update( diffMS ) ;			//增加心跳时间
	if( m_pingWatch.Done() )				//定时发送一次心跳
	{
		SendPing() ;
		m_pingWatch.Reset() ;
	}

	ClientUpdate( nowTimeMS, diffMS ) ;		//子服务器更新

}

//连接成功
void ServerClient::OnConnect( TcpAsyncConn *pConn )
{
	SetCanSend( true ) ;
	ASSERT( pConn->Ip() == GetIP() ) ;

	NLOG( "%s had connected %s server, (%s:%s), session id: %llu", GetClientName(), GetServerName(), GetIP(), GetPort(), pConn->Id() ) ;
	ClientConnect() ;
}

//收到消息
void ServerClient::OnRecv(TcpAsyncConn *pConn, NetPack* pPack)
{
	if( NetPack::IsNoticeRecvAndSend( *pPack ) )
	{
		NLOG( "[Recv] 2 %s to %s, %s( %u )", GetServerName(), GetClientName(), pPack->GetOpcodeName(), pPack->GetOpcode() );
	}

	char packType =pPack->GetPacketType() ;
	if( packType != GetDstServerType() )
	{
		ELOG( "%s client receive packet type is %s, not the %s", GetClientName(), pPack->GetPacketTypeName(), GetServerName() ) ;
		delete pPack ;
		pPack =NULL ;
		return ;
	}

	ASSERT( !pPack->IsClientPacket() ) ;

	m_packetTable.Enqueue( pPack ) ;
}

void ServerClient::OnSend( uint64 sessionId, NetPack *pPack )
{
	if( NetPack::IsNoticeRecvAndSend( *pPack ) )
	{
		ASSERT( !pPack->IsClientPacket() ) ;

		NLOG( "[Send] %s to %s, %s( %u )", GetClientName(), GetServerName(), pPack->GetOpcodeName(), pPack->GetOpcode() );
	}
}

void ServerClient::OnClose(uint64 sessionId)
{
	SetCanSend( false ) ; // 不让继续执行Send操作
	ClientDisconnect() ;

	NLOG( "%s disconnect %s server(%s:%s), session id: %llu", GetClientName(), GetServerName(), GetIP(), GetPort(), sessionId ) ;
}

bool ServerClient::Send( NetPack &packet, char clientType)
{
	packet.SetPacketType( clientType ) ;
	if( !CanSend() )
	{
		ELOG( "send packetet %s(%d) to %s faild !", packet.GetOpcodeName(),  packet.GetOpcode(), GetServerName() ) ;
		return false ;
	}

	m_pTcpClient->Send( packet ) ;
	return true ;
}

bool ServerClient::Send( NetPack &packet )
{
	packet.SetPacketType( GetClientType() ) ;
	if( !CanSend() )
	{
		ELOG( "send packetet %s(%d) to %s faild !", packet.GetOpcodeName(),  packet.GetOpcode(), GetServerName() ) ;
		return false ;
	}

	m_pTcpClient->Send( packet ) ;
	return true ;
}

bool ServerClient::Send( NetPack *pPacket )
{
	pPacket->SetPacketType( GetClientType() ) ;
	if( !CanSend() )
	{
		ELOG( "send packet %s(%d) to %s faild !", pPacket->GetOpcodeName(),  pPacket->GetOpcode(), GetServerName() ) ;
		delete pPacket ;
		return false ;
	}

	m_pTcpClient->Send( pPacket ) ;
	return true ;
}

bool ServerClient::Send( const int opCode, const ::google::protobuf::Message &msg )
{
	NetPack *pPacket =new NetPack( opCode, msg.ByteSize() + 2 ) ; // + 2, beacuse packet << msg will add a uint16 sign the msg size .
	*pPacket << msg;
	return Send( pPacket );
}

bool ServerClient::Send( const int opCode, const uint64 sessionId, const ::google::protobuf::Message &msg )
{
	NetPack *pPacket =new NetPack( opCode, msg.ByteSize() + 2 ) ; // + 2, beacuse pPacket << msg will add a uint16 sign the msg size .
	pPacket->SetSessionId( sessionId ) ;
	*pPacket << msg;
	return Send( pPacket );
}

bool ServerClient::Send( const int opCode, const uint64 sessionId, char clientType, const ::google::protobuf::Message &msg )
{
	NetPack *pPacket =new NetPack( opCode, msg.ByteSize() + 2 ) ; // + 2, beacuse pPacket << msg will add a uint16 sign the msg size .
	pPacket->SetSessionId( sessionId ) ;
	pPacket->SetPacketType(clientType);
	*pPacket << msg;
	if( !CanSend() )
	{
		ELOG( "send packet %s(%d) to %s faild !", pPacket->GetOpcodeName(),  pPacket->GetOpcode(), GetServerName() ) ;
		delete pPacket ;
		return false ;
	}
	m_pTcpClient->Send( pPacket ) ;
	return true ;
}


void ServerClient::ReConnect( void )
{
	LLOG("server reconnect");
	string error ;
	if( !Connect( m_host, m_port, error ) )
	{
		NLOG( "%s connect %s server,: %s:%s fail because %s !" ,GetClientName(), GetServerName(), GetIP(), GetPort(), error.c_str() ) ;
	}
}


bool ServerClient::IsCurClient( const string &host, const string &port )
{
	return (m_port == port) && (m_host == host);
}

void ServerClient::SetDstInfo( const string &host, const string &port )
{
	//LLOG("ServerClient::SetDstInfo host=%s,port=%s",host,port);
	m_port =port ;
	m_host =host ;
}


bool ServerClient::Connect(const std::string& host, const string &port, std::string& errMsg)
{
	ASSERT( m_pTcpClient->IsStopped() ) ;
	LLOG("connect port=%s",port.c_str());
	SetDstInfo( host, port ) ;
	
	if( m_connectCount < m_maxConnectCount )
	{
		++m_connectCount ;
		m_pTcpClient->Connect( host, port, this );
		return true ;
	}
	else
	{
		errMsg ="Max connect count limit" ;
		return false ;
	}
}


void ServerClient::SetClientType( char type )
{
	m_clientType =type ;
	m_clientName =NetPack::GetPacketTypeName( GetClientType() ) ;
}


void ServerClient::SetDstServerType( char type )
{
	m_dstServetType =type ;
	m_dstServerName =NetPack::GetPacketTypeName( GetDstServerType() ) ;
}



void ServerClient::SendPing( int pintOpCode )
{
	NetPack ping( pintOpCode, sizeof( uint64 ) * 3 ) ;
	ping << sOS.TimeMS() ;
	ping << GetDelayTime() ;
	Send( ping ) ;
}

void ServerClient::HandlePong( NetPack &packet )
{
	uint64 pongTimeMS =0 ;
	packet >> pongTimeMS ;

	int64 delayTime =sOS.TimeMS() - pongTimeMS ;

	ASSERT( delayTime >= 0 ) ;
	SetDelayTime( delayTime ) ;

	// Delay notice 
	if( GetDelayTime() > GetDelayNoticeLimitTimeMS() )
	{
		WLOG( "%s with %s(%s:%s), delay %lld MS !!!!", GetClientName(), GetServerName(), GetIP(), GetPort(), GetDelayTime() ) ;
	}
}

void ServerClient::ReleaseConnectCount()
{
	m_connectCount = m_maxConnectCount;
}