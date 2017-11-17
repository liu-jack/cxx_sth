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

	//继承于INetPackHandler的消息处理方法
public:
	virtual void OnConnect( TcpAsyncConn *pConn ) ;
	virtual void OnRecv( TcpAsyncConn *pConn, NetPack *pPack ) ;
	virtual void OnSend( uint64 sessionId, NetPack *pPack ) ;
	virtual void OnClose( uint64 sessionId ) ;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TcpServer 监听端口
	void StartListenPort( const std::string &port ) ;

	//获取IP与端口
	const string& GetIP( void ) ;
	const string& GetPort( void ) ;

	//获取/设置服务器类型
	char GetServerType( void ) {   return m_serverType ;   }
	void SetServerType( char type ) {   m_serverType =type ;   SetServerTypeName( m_serverType ) ;   }

	//获取/设置服务器类型名
	void SetServerTypeName( char type ) ;
	const char* GetServerTypeName( void ) {   return m_serverTypeName.c_str() ;   }

	void SetPackPrintDiffCount( uint64 count ) ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread
public:
	void Start( void ) ;		//开启线程
	virtual void OnBeforeStart( void ) {}

	void Stop( void ) ;			//关闭线程
	virtual void OnAfterStop( void ) {}

	void WorldThread( void ) ;	//线程函数
	
	//会话
	virtual void BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS ) ;
	virtual void OnSessonUpdate( Session *pSession ) {}

	boost::shared_ptr<LogicalTaker>& GetLogicalTaker( void ) {   return m_pLogicalTaker ;   }
	void SetLogicalTaker( LogicalTaker *pLogicalTaker ) {   m_pLogicalTaker.reset( pLogicalTaker ) ;   }

private:
	CThread                         m_worldThread ;		//服务器主线程
	boost::shared_ptr<LogicalTaker> m_pLogicalTaker ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Config
public:
	const ConfigMgr& Config() { return m_config; }	//获得配置文件管理器

	//读取配置文件
	virtual bool InitGameConf( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;

private:
	ConfigMgr m_config;		//配置文件管理器


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Packet
public:
	void PostInputPacket( TcpAsyncConn *pConn, NetPack* pPacket ) ;

	void NoticeRecv( uint64 sessionId, NetPack *pPack ) ;
	void NoticeSend( uint64 sessionId, NetPack *pPack ) ;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Session
public:
	//获得所有会话个数
	size_t GetAllSessionSize( void )
	{
		return m_sessions.size() ;
	}


	size_t GetUpdateSessionSize( void )
	{
		return m_updateSessionMap.size() ;
	}

	//开启/关闭会话
	void OnOpenSession( TcpAsyncConn *pConn ) ;
	void OnCloseSession( uint64 sessionId ) ;

	//创建会话
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
	//向会话表添加会话
	bool AddSeesionToSeerionMap( SessionMap &sessionMap, Session *pSession ) ;
	//删除会话
	bool RemoveSessionFromSessionMap( SessionMap &sessionMap, Session *pSession ) ;

	void BroadcastMsgBySessionMap( SessionMap &sessionMap, const int opCode, const ::google::protobuf::Message &msg );
	//向所有会话广播
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
	uint64                                m_verfyLimitTimeMS ;		//识别等待时间
	uint64                                m_notRecvPackLimitTimeMS ;	//没有数据包通信关闭时间
	uint64                                m_threadUpdateLimitTimeMS ;	//线程循环时间间隔

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

