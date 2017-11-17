
#ifndef HTTP_MANAGER_H__
#define HTTP_MANAGER_H__

#include <map>
#include <string>

#include "mongoose.h"

// Tips:
/*
 * === Core API: TCP/UDP/SSL
 *
 * NOTE: Mongoose manager is single threaded. It does not protect
 * its data structures by mutexes, therefore all functions that are dealing
 * with a particular event manager should be called from the same thread,
 * with exception of the `mg_broadcast()` function. It is fine to have different
 * event managers handled by different threads.
 */
// 根据以上所说，不可以多线程同时，一个 event managers 只可以一个线程调用
// 然后虽然连接此服务器的，可以每个处理开一个新线程，不过由服务武器发起的连接，都由主工作线程回调

enum HttpRequestType
{
	HttpRequestTypeGet,
	HttpRequestTypePost,
	HttpRequestTypeUnknow
} ;

const int CHAR_HTTP_VAR_MAX = 128;
const int CHAR_HTTP_COOKIE_MAX = 400;
const int CHAR_HTTP_PARAM_MAX = 512;

class HttpMsgTool
{
public:
	HttpMsgTool( http_message *pMsg );
	~HttpMsgTool( void );

	HttpRequestType GetRequestType( void ) const;

	//获取变量
	void GetHttpStrVar( const char *pName, std::string &str ) const;
	int GetHttpIntVar( const char *pName, int def ) const;
	double GetHttpDoubleVar( const char *pName, double def ) const;
	long long int GetHttpLongLongIntVar( const char *pName, long long int def ) const ;

	void GetUrl( std::string &str ) const;

	void GetCookie( const char *pName, std::string &str ) const;        //获取Cookie


private:
	http_message *m_pHttpMsg ;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HttpParamTool
{
public:
	HttpParamTool( HttpRequestType type, const std::string &url, const std::string &externHeaders ) ;
	~HttpParamTool( void ) ;

	bool CheckIsOk( void ) ;

public:
	bool AddParam( const char *pName, const int val ) ;
	bool AddParam( const char *pName, const long long val ) ;
	bool AddParam( const char *pName, const std::string &val ) ;
	bool AddParam( const char *pName, const double val ) ;

private:
	template< class T >
	bool AddParam( const char *pFormat, const char *pName, const T &val ) ;

public:
	HttpRequestType m_type ;
	std::string     m_url ;
	std::string     m_externHeaders ;
	int             m_paramLen ;
	char            m_paramBuf[ CHAR_HTTP_PARAM_MAX + 1] ;
} ;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HttpManager
{
public:
	HttpManager( void ) ;
	virtual ~HttpManager( void ) ;

public:
	// 初始化
	// @param pListPort: 监听的端口
	// @param isPerThreadForConnect: 是否对每个连接都建立一个新的线程去处理
	// @param eventFun : 当有事件时回调的函数
	bool Init( const char *pListPort, bool isPerThreadForConnect, mg_event_handler_t pHandler ) ;

	void Release( void ) ;

	// server part
public:
	// Tips: 
	// 1:请求处理不是线程安全的，所有请求与请求返回，都要在 WorkThread() 里面处理, 之类需要与其他线程交互，需要自己加锁
	void WorkThread( void ) ;

	// 2: 由线程函数 WorkThread() 调用
	virtual void WhenRunning( void ) =0;

public:
	void RegisterHttpEndpoint( const char *pUrl, mg_event_handler_t pHandler ) ;
	
	// 开始监听端口并开启工作主线程
	// @param waitMillisecond : 表示当没事件时，select() 函数睡眠多久
	bool StartListion( int waitMillisecond ) ;

	inline bool IsRunning( void )
	{
		return m_isRuning ;
	}

	inline void SetIsRunning( bool bVal )
	{
		m_isRuning =bVal ;
	}

	inline mg_mgr& GetMgMgr( void )
	{
		return m_mgr ;
	}

	inline int GetWaitMillisecond( void )
	{
		return m_waitMillisecond ;
	}

	void ReplyError( mg_connection *pConn, int errCode, const char *pReason ) ;
	void ReplyStr( mg_connection *pConn, const std::string &str ) ;


	void SetIsPerThreadEachConnect( bool value ) {    m_preThreadOneConnect =value;   }
	bool GetIsPerThreadEachConnect(void ) {    return m_preThreadOneConnect ;   }

	void SetListenPort( const std::string &str ) {   m_listenPort =str ;   }
	const std::string& GetListenPort( void ) {   return m_listenPort ;   }

	// client part
public:
	mg_connection* SendHttpRequest( mg_event_handler_t pHandler, HttpParamTool &paramTool, void *pUserData ) ;

public:
	bool                m_preThreadOneConnect ;
	bool                m_isRuning ;
	int                 m_waitMillisecond ;
	std::string         m_listenPort ;
	mg_mgr              m_mgr;
	mg_connection      *m_pConnect;
} ;

//#define sHttpMgr HttpManager::GetInstance()

#endif
