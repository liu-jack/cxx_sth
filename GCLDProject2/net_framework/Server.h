#pragma once

#include <boost/smart_ptr.hpp>

#include "ConfigMgr.h"
#include "LogicalTaker.h"

#include "Thread.h"
#include "SessionTypeDef.h"
#include "NetFrameworkDefines.h"

#include "NetPackClientCount.h"
#include "NetPackServerCount.h"

class NetPack;
class Session ;
class TcpAsyncConn ;
class TcpAsyncServer ;

class Server : public INetPackHandler
{
public:
	Server( void ) ;
	virtual ~Server( void ) ;

	//�̳���INetPackHandler����Ϣ������
public:
	virtual void OnConnect( TcpAsyncConn *pConn ) ;
	virtual void OnRecv( TcpAsyncConn *pConn, NetPack *pPack ) ;
	virtual void OnSend( uint64 sessionId, NetPack *pPack ) ;
	virtual void OnClose( uint64 sessionId ) ;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TcpServer �����˿�
	void StartListenPort( const std::string &port ) ;

	//��ȡIP��˿�
	const string& GetIP( void ) ;
	const string& GetPort( void ) ;

	//��ȡ/���÷���������
	char GetServerType( void ) {   return m_serverType ;   }
	void SetServerType( char type ) {   m_serverType =type ;   SetServerTypeName( m_serverType ) ;   }

	//��ȡ/���÷�����������
	void SetServerTypeName( char type ) ;
	const char* GetServerTypeName( void ) {   return m_serverTypeName.c_str() ;   }

	void SetPackPrintDiffCount( uint64 count ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	void Start( void ) ;		//�����߳�
	virtual void OnBeforeStart( void ) {}

	void Stop( void ) ;			//�ر��߳�
	virtual void OnAfterStop( void ) {}

	void WorldThread( void ) ;	//�̺߳���
	
	//�Ự
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void OnSessonUpdate( Session *pSession ) {}

	boost::shared_ptr<LogicalTaker>& GetLogicalTaker( void ) {   return m_pLogicalTaker ;   }
	void SetLogicalTaker( LogicalTaker *pLogicalTaker ) {   m_pLogicalTaker.reset( pLogicalTaker ) ;   }

private:
	CThread                         m_worldThread ;		//���������߳�
	boost::shared_ptr<LogicalTaker> m_pLogicalTaker ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Config
public:
	const ConfigMgr& Config() { return m_config; }	//��������ļ�������

	//��ȡ�����ļ�
	virtual bool InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;

private:
	ConfigMgr m_config;		//�����ļ�������


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:
	void PostInputPacket( TcpAsyncConn *pConn, NetPack* pPacket ) ;

	void NoticeRecv( uint64 sessionId, NetPack *pPack ) ;
	void NoticeSend( uint64 sessionId, NetPack *pPack ) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Session
public:
	//������лỰ����
	size_t GetAllSessionSize( void )
	{
		return m_sessions.size() ;
	}


	size_t GetUpdateSessionSize( void )
	{
		return m_updateSessionMap.size() ;
	}

	//����/�رջỰ
	void OnOpenSession( TcpAsyncConn *pConn ) ;
	void OnCloseSession( uint64 sessionId ) ;

	//�����Ự
	virtual Session* CreateSesion( int type ) =0 ;

	virtual bool ChangeToLogicalTacker( Session *pSession ) ;

	bool AddSession( Session *pSession ) ;

	Session* GetSession( uint64 sessionId )
	{
		SessionMapItr itr =m_sessions.find( sessionId ) ;
		return ( itr != m_sessions.end() ? itr->second : NULL ) ;
	}

	void SetVerfyLimitTime( uint64 time ) {   m_verfyLimitTimeMS =time ;   }
	void SetThreadUpdateLimitTimeMS( uint64 time ) {   m_threadUpdateLimitTimeMS =time ;   }
	void SetNotRecvPackLimitTimeMS( uint64 time ) {   m_notRecvPackLimitTimeMS =time ;   }
	void SetJustConnectLimintTimeMS( uint64 time ) ; 

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Common data
protected:
	//��Ự����ӻỰ
	bool AddSeesionToSeerionMap( SessionMap &sessionMap, Session *pSession ) ;
	//ɾ���Ự
	bool RemoveSessionFromSessionMap( SessionMap &sessionMap, Session *pSession ) ;

	void BroadcastMsgBySessionMap( SessionMap &sessionMap, const int opCode, const ::google::protobuf::Message &msg );
	//�����лỰ�㲥
	void BroadcastBySessionMap( SessionMap &sessionMap, NetPack &packet ) ;

	void BroadcastByServerIdSessionMap( SessionByServerIdMap &sessionMap, NetPack &packet );
private:
	void TakeNewSession( void ) ;
	void TakeCloseSession( void ) ;

	uint32 GetTakeNewSessionLimit( void ) {   return m_takeNewSessionLimit ;   }
	void SetTakeNewSessionLimit( uint32 limit ) {   m_takeNewSessionLimit =limit ;   }

private:
	char                                  m_serverType ;
	uint32                                m_takeNewSessionLimit ;
	uint64                                m_updateTimeMS ;
	uint64                                m_preUpdateTimeMS ;
	uint64                                m_verfyLimitTimeMS ;		//ʶ��ȴ�ʱ��
	uint64                                m_notRecvPackLimitTimeMS ;	//û�����ݰ�ͨ�Źر�ʱ��
	uint64                                m_threadUpdateLimitTimeMS ;	//�߳�ѭ��ʱ����

	std::string                           m_serverTypeName ;

	SessionMap                            m_sessions ;
	SessionMap                            m_updateSessionMap ;

	MLockSessionQueue                     m_newSession ;
	MLockSessionIdQueue                   m_closeQueue ;

	boost::scoped_ptr<TcpAsyncServer>     m_pTcpSvr ;

	//NetPackClientCount                    m_inClientNetStat ;
	//NetPackClientCount                    m_outClientNetStat ;

	//NetPackServerCount                    m_inServerNetStat ;
	//NetPackServerCount                    m_outServerNetStat ;
};

