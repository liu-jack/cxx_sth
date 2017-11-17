//#pragma once
//
//#include <boost/function.hpp>
//#include <boost/smart_ptr.hpp>
//
//#include "SimpleSingleton.h"
//#include "TcpAsyncServer.h"
//
//#include "NetPackDispatcher.h"
//
//#include "NetPackClientCount.h"
//#include "NetPackServerCount.h"
//
//
//class NetService : public INetPackHandler
//{
//public:
//	void StartListenPort(int port);
//	void Stop( void );
//
//	TcpAsyncServer*  GetServer( void ) {   return m_pTcpSvr.get();   }
//
//	int GetPort( void ) {   return m_pTcpSvr->GetPort() ;   }
//	const string& GetIP( void ) {   return m_pTcpSvr->GetIP() ;   }
//
//	char GetServerType( void ) {   return m_serverType ;   }
//	void SetServerType( char type ) {   m_serverType =type ;   }
//
//	void SetPackPrintDiffCount( uint64 count ) ;
//
//private:
//	NetService( void );
//	virtual ~NetService( void );
//
//	virtual void OnConnect(int sessionId, const std::string &ip);
//	virtual void OnRecv(int sessionId, NetPack* pRecvPack);
//	virtual void OnClose(int sessionId);
//
//	bool IsNoticRecvAndSend( bool isClientPacket, int opCode ) ;
//
//private:
//	char                                  m_serverType ;
//
//	boost::scoped_ptr<TcpAsyncServer>     m_pTcpSvr;
//
//	NetPackClientCount                    m_inClientNetStat;
//	NetPackClientCount                    m_outClientNetStat;
//
//	NetPackServerCount                    m_inServerNetStat;
//	NetPackServerCount                    m_outServerNetStat;
//};
//
//
