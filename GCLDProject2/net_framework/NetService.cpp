//
//#include "NetService.h"
//#include "def/MmoAssert.h"
//#include "memory_buffer/NetPack.h"
//#include "Logger.h"
//
//
//#include "Opcode.pb.h"
//#include "ServerOpcode.pb.h"
//
//
//NetService::NetService()
//:m_inClientNetStat( pb::CSMSG_COUNT, "Client Input" )
//,m_outClientNetStat( pb::CSMSG_COUNT, "Client Output" )
//,m_inServerNetStat( pb::SERVER_OPCODE_COUNT, "Server Input" )
//,m_outServerNetStat( pb::SERVER_OPCODE_COUNT, "Server Output" )
//{
//	m_serverType =0 ;
//}
//
//NetService::~NetService(void)
//{
//	Stop();
//}
//
//void NetService::SetPackPrintDiffCount( uint64 count )
//{
//	m_inClientNetStat.SetPrintDiffCount( count ) ;
//	m_outClientNetStat.SetPrintDiffCount( count ) ;
//	m_inServerNetStat.SetPrintDiffCount( count ) ;
//	m_outServerNetStat.SetPrintDiffCount( count ) ;
//}
//
//
//void NetService::StartListenPort(int port)
//{
//	m_pTcpSvr.reset(new TcpAsyncServer(port));
//	m_pTcpSvr->Start(this);
//}
//
//void NetService::Stop()
//{
//	if (m_pTcpSvr.get())
//	{
//		m_pTcpSvr->Stop();
//		m_pTcpSvr.reset();
//	}
//}
//
//void NetService::Send(int sessionId, NetPack& pack)
//{
//	if( IsNoticRecvAndSend( pack.IsClientPacket(), pack.Opcode() ) )
//	{
//		const char* pPacketTypeName =NetPack::GetPacketTypeName( pack.GetPacketType() ) ;
//
//		if( pack.IsClientPacket() )
//		{
//			pb::Opcode opCode =static_cast< pb::Opcode >( pack.Opcode() ) ;
//			const char *opName = pb::Opcode_Name( opCode ).c_str();
//
//			NLOG ("[Send] %s, session id: %d, %s(%d)", pPacketTypeName, sessionId, opName, opCode );
//			m_outClientNetStat.Add(pack);
//		}
//		else
//		{
//			pb::ServerOpcode opCode =static_cast< pb::ServerOpcode >( pack.Opcode() ) ;
//			const char *opName = pb::ServerOpcode_Name( opCode ).c_str();
//
//			NLOG ("[Send] %s, session id: %d, %s(%d)", pPacketTypeName, sessionId, opName, opCode );
//			m_outServerNetStat.Add(pack);
//		}
//	}
//
//	pack.SetPacketType( GetServerType() ) ;
//
//	m_pTcpSvr->Send(sessionId, pack);
//}
//
//
//void NetService::OnConnect(int sessionId, const std::string &ip)
//{
//	NLOG("Session(%d), IP:%s, connected\n", sessionId, ip.c_str());
//}
//
//void NetService::OnRecv(int sessionId, NetPack* pRecvPack)
//{
//	if( IsNoticRecvAndSend( pRecvPack->IsClientPacket(), pRecvPack->Opcode() ) )
//	{
//		const char* pPacketTypeName =NetPack::GetPacketTypeName( pRecvPack->GetPacketType() ) ;
//		if( pRecvPack->IsClientPacket() )
//		{
//			pb::Opcode opCode =static_cast< pb::Opcode >( pRecvPack->Opcode() ) ;
//			const char *opName = pb::Opcode_Name( opCode ).c_str();
//
//			NLOG ("[Recv] %s, session id: %d, %s(%d)", pPacketTypeName, sessionId, opName, opCode );
//			m_inClientNetStat.Add(*pRecvPack);
//		}
//		else
//		{
//			pb::ServerOpcode opCode =static_cast< pb::ServerOpcode >( pRecvPack->Opcode() ) ;
//			const char *opName = pb::ServerOpcode_Name( opCode ).c_str();
//
//			NLOG ("[Recv] %s, session id: %d, %s(%d)", pPacketTypeName, sessionId, opName, opCode );
//			m_inServerNetStat.Add(*pRecvPack);
//		}
//	}
//}
//
//void NetService::OnClose(int sessionId)
//{
//}
//
//
//bool NetService::IsNoticRecvAndSend( bool isClientPacket, int opCode )
//{
//	if( isClientPacket )
//	{
//		switch( opCode )
//		{
//		case pb::CMSG_PING: case pb::SMSG_PONG:
//			{
//				return false ;
//			}
//		}
//	}
//	else
//	{
//		switch( opCode )
//		{
//		case pb::S_PING: case pb::S_PONG:
//			{
//				return false ;
//			}
//		}
//	}
//
//	return true ;
//}
//
//
//
