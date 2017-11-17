#ifndef MULTI_CURL_H__
#define MULTI_CURL_H__

#include <map>
#include <string>

#include <curl/curl.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include "lock_free/LockFreeQueue.h"
#include "memory_buffer/MemoryPool.h"

class MultiCurl ;


/* Information associated with a specific easy handle */


struct MCResopnse
{
public:
	MCResopnse( void )
	{
		m_result =CURL_LAST ;
		m_responseCode =0 ;
		::memset( m_error, 0, CURL_ERROR_SIZE * sizeof( char ) ) ;
	}

public:
	std::string m_data ;

	int         m_responseCode ;
	std::string m_responseReason ;
	int         m_result ;
	char        m_error[CURL_ERROR_SIZE];
} ;

struct MCRequest
{
	DECLARE_OBJ_POOL(MCRequest)

public:
	MCRequest( const std::string &url ="", const std::string &postData ="" )
		: m_url( url ),m_data( postData )
	{
		m_isHttps =false ;
		m_id      =0 ;
		m_opType  =0 ;
		m_easy    =NULL ;
		m_mgr     =NULL ;
	}

	~MCRequest( void )
	{
	}

public:
	bool          m_isHttps ; // HTTPS 请求,无证书版本

	int           m_id ;
    int           m_opType;
	
	std::string   m_url ;    // 如果 m_isHttps 为 true, 则为 https://xxx.xx.xxx 否则为 http://xxx.xx.xxx
	std::string   m_data ;   // post 时才需要使用，get 直接在url上附带数据，使用如下格式para1=val1&2=val2&…

	std::string   m_caPath ; // 为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性

	MCResopnse    m_response ;

	CURL         *m_easy;
	MultiCurl    *m_mgr;
} ;



class MultiCurl
{
public:
	typedef struct SocketInfo
	{
		public:
			SocketInfo(boost::asio::io_service &io) : sock(io), mask(0) { }

		public:
			boost::asio::ip::tcp::socket sock;
			int mask; // is used to store current action 
	} SocketInfo;

	typedef boost::shared_ptr<SocketInfo> socket_ptr;
	typedef std::map< curl_socket_t, socket_ptr > SocketMap ;
	typedef SocketMap::iterator SocketMapItr  ;

public:
	explicit MultiCurl( boost::asio::io_service &io ) ;

	virtual ~MultiCurl( void ) ;

public:
	void Request( MCRequest *pRequest )
	{
		m_newRequest.Enqueue( pRequest ) ;
	}

	void PostRequest( void ) ;

	bool GetRequest( MCRequest *&pRequest )
	{
		return m_newRequest.Dequeue( pRequest ) ;
	}

	MCRequest* GetFinishRequest( void )
	{
		MCRequest *pRet =NULL ;
		m_finishRequest.Dequeue( pRet ) ;
		return pRet ;
	}

	void PuthFinishRequest( MCRequest *pRequest )
	{
		m_finishRequest.Enqueue( pRequest ) ;
	}

	const char* UrlEncode(const std::string& srcUrl, std::string& dstUrl);

	const char* UrlDecode(const std::string& srcUrl, std::string& dstUrl);

	void AppendEncodedParams(std::string &result, const std::string& paramName, const std::string& paramValue);

	void AppendParams(std::string &result, const std::string& paramName, const std::string& paramValue);
private:

	/* Create a new easy handle, and add it to the global curl_multi */
	void TakeNewRequest( void ) ;

private:
	ServerQueue<MCRequest*>      m_newRequest ;
	ServerQueue<MCRequest*>      m_finishRequest ;

	// curl
	CURLM                       *m_pMulti;
	int                          m_runningCount ;
	SocketMap                    m_sockeets ;

	// server
	boost::asio::io_service     &m_asio ;
	boost::asio::deadline_timer  m_timer ;


public:
	/* Update the event timer after curl_multi library calls */
	static int MultiTimeCB(CURLM *multi, long timeout_ms, MultiCurl *pMC) ;

	/* Die if we get a bad CURLMcode somewhere */
	static void MCodeOrDie(const char *where, CURLMcode code) ;

	/* Check for completed transfers, and remove their easy handles */
	static void CheckMultiInfo(MultiCurl *pMC) ;

	/* Called by asio when there is an action on a socket */
	static void EventCB(MultiCurl *pMC, socket_ptr &tcp_socket, curl_socket_t s, CURL *e, int action, const boost::system::error_code &err) ;

	/* Called by asio when our timeout expires */
	static void TimeCB(const boost::system::error_code & error, MultiCurl *pMC) ;

	/* Clean up any data */
	//static void RemSock(int *f, MultiCurl *pMC) ;

	static void SetSock(socket_ptr &tcp_socket, curl_socket_t s, CURL*e, int act, MultiCurl *pMC) ;

	//static void AddSock(curl_socket_t s, CURL *easy, int action, MultiCurl *pMC) ;

	/* CURLMOPT_SOCKETFUNCTION */
	static int MultiSockCB(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp) ;

	/* CURLOPT_WRITEFUNCTION */
	static size_t WriteCB(void *ptr, size_t size, size_t nmemb, void *data) ;

	static size_t HeaderWriteCB( void *ptr, size_t size, size_t nmemb, void *data) ;

	/* CURLOPT_PROGRESSFUNCTION */
	static int ProgCB(void *p, double dltotal, double dlnow, double ult, double uln);

	/* CURLOPT_OpenSocketFUNCTION */
	static curl_socket_t OpenSocket( void *clientp, curlsocktype purpose, struct curl_sockaddr *address ) ;

	/* CURLOPT_CloseSocketFUNCTION */
	static int CloseSocket( void *clientp, curl_socket_t item ) ;

	static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *) ;
} ;


#endif
