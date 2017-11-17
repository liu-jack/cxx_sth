
#include <assert.h>

#include "MultiCurl.h"

#include "Logger.h"


MultiCurl::MultiCurl( boost::asio::io_service &io ) : m_asio( io ), m_timer( m_asio )
{
	m_runningCount =0 ;

	curl_global_init(CURL_GLOBAL_ALL);

	m_pMulti = curl_multi_init();

	curl_multi_setopt( m_pMulti, CURLMOPT_SOCKETFUNCTION, MultiSockCB );
	curl_multi_setopt( m_pMulti, CURLMOPT_SOCKETDATA, this );

	curl_multi_setopt( m_pMulti, CURLMOPT_TIMERFUNCTION, MultiTimeCB );
	curl_multi_setopt( m_pMulti, CURLMOPT_TIMERDATA, this );

	curl_version_info_data *pCurlInfo =curl_version_info( CURLVERSION_NOW ) ;
	if( pCurlInfo != NULL )
	{
		NLOG( "libcurl version is : %s; ", pCurlInfo->version ) ;
	}
}

MultiCurl::~MultiCurl( void )
{
	curl_multi_cleanup( m_pMulti );
	curl_global_cleanup();

	DeleteAllPtrInLFQueue< MCRequest* >( m_newRequest ) ;
	DeleteAllPtrInLFQueue< MCRequest* >( m_finishRequest ) ;
}



/* Update the event timer after curl_multi library calls */
int MultiCurl::MultiTimeCB(CURLM *multi, long timeout_ms, MultiCurl *pMC)
{
	//NLOG( "MultiTimeCB: timeout_ms %ld", timeout_ms);

	/* cancel running timer */
	pMC->m_timer.cancel();

	if(timeout_ms > 0)
	{
		/* update timer */
		pMC->m_timer.expires_from_now(boost::posix_time::millisec(timeout_ms));
		pMC->m_timer.async_wait(boost::bind(&TimeCB, _1, pMC));
	}
	else
	{
		/* call timeout function immediately */
		boost::system::error_code error; /*success*/
		TimeCB(error, pMC);
	}

	return 0;
}

/* Die if we get a bad CURLMcode somewhere */
void MultiCurl::MCodeOrDie(const char *where, CURLMcode code)
{
	if(CURLM_OK != code)
	{
		const char *s;
		switch(code)
		{
		case CURLM_CALL_MULTI_PERFORM: s = "CURLM_CALL_MULTI_PERFORM"; break;
		case CURLM_BAD_HANDLE:         s = "CURLM_BAD_HANDLE";         break;
		case CURLM_BAD_EASY_HANDLE:    s = "CURLM_BAD_EASY_HANDLE";    break;
		case CURLM_OUT_OF_MEMORY:      s = "CURLM_OUT_OF_MEMORY";      break;
		case CURLM_INTERNAL_ERROR:     s = "CURLM_INTERNAL_ERROR";     break;
		case CURLM_UNKNOWN_OPTION:     s = "CURLM_UNKNOWN_OPTION";     break;
		case CURLM_LAST:               s = "CURLM_LAST";               break;
		default:
			s = "CURLM_unknown";
			break;

		case CURLM_BAD_SOCKET:
			s = "CURLM_BAD_SOCKET";
			/* ignore this error */
			return;
		}

		ELOG( "ERROR: %s returns %s", where, s);
	}
}

/* Check for completed transfers, and remove their easy handles */
void MultiCurl::CheckMultiInfo(MultiCurl *pMC)
{
	//char *eff_url;
	CURLMsg *msg        =NULL;
	int msgs_left       =0;
	MCRequest *pRequest =NULL;
	CURL *easy          =NULL;
	CURLcode res        =CURLE_OK ;

	//NLOG( "REMAINING: %d", pMC->m_runningCount);

	while((msg = curl_multi_info_read(pMC->m_pMulti, &msgs_left)))
	{
		if(msg->msg == CURLMSG_DONE)
		{
			easy = msg->easy_handle;
			res  = msg->data.result;

			curl_easy_getinfo(easy, CURLINFO_PRIVATE, &pRequest);
			//curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);

			pRequest->m_response.m_result =res ;

			curl_multi_remove_handle(pMC->m_pMulti, easy);

			curl_easy_cleanup(easy);

			pMC->PuthFinishRequest( pRequest ) ;
		}
	}
}

/* Called by asio when there is an action on a socket */
void MultiCurl::EventCB(MultiCurl *pMC, socket_ptr &tcp_socket, curl_socket_t s, CURL *e, int action, const boost::system::error_code &err)
{
	//NLOG( "EventCB: action=%d", action);

	assert( tcp_socket->sock.native_handle() == s ) ;

	int msparam =0 ;
	if( err )
	{
		msparam =CURL_CSELECT_ERR ;
		ELOG("MultiCurl::EventCB(): socket=%d action=%d \nERROR=%s", s, action, err.message().c_str()) ;
	}
	else
	{
		msparam =action ;
	}

	CURLMcode rc ;
	rc =curl_multi_socket_action(pMC->m_pMulti, tcp_socket->sock.native_handle(), msparam, &pMC->m_runningCount);

	MCodeOrDie("EventCB: curl_multi_socket_action", rc);
	CheckMultiInfo(pMC);

	if(pMC->m_runningCount <= 0)
	{
		//NLOG( "last transfer done, kill timeout");
		pMC->m_timer.cancel();
	}
	else
	{
		int action_continue = (tcp_socket->mask) & action;
		if (action_continue)
		{
			NLOG("continue read or write: %d", action_continue);
			SetSock(tcp_socket, s, e, action_continue, pMC); // continue read or write
		}
	}
}


/* Called by asio when our timeout expires */
void MultiCurl::TimeCB(const boost::system::error_code & error, MultiCurl *pMC)
{
	if(!error)
	{
		//NLOG( "TimeCB: ");

		CURLMcode rc;
		rc = curl_multi_socket_action(pMC->m_pMulti, CURL_SOCKET_TIMEOUT, 0, &pMC->m_runningCount);

		MCodeOrDie("TimeCB: curl_multi_socket_action", rc);
		CheckMultiInfo(pMC);
	}
}

/* Clean up any data */
//void MultiCurl::RemSock(int *f, MultiCurl *pMC)
//{
//	//NLOG( "RemSock: ");
//	if(f)
//	{
//		free(f);
//	}
//}

void MultiCurl::SetSock(socket_ptr &tcp_socket, curl_socket_t s, CURL*e, int act, MultiCurl*pMC)
{
	//NLOG( "SetSock: socket=%d, act=%d, fdp=%p", s, act, fdp);

	assert(tcp_socket->sock.native_handle() == s);

	SocketMapItr it = pMC->m_sockeets.find(s);
	if(it == pMC->m_sockeets.end())
	{
		NLOG( "socket %d is a c-ares socket, ignoring", s);
		return;
	}

	if( act == CURL_POLL_IN )
	{
		//NLOG("watching for socket to become readable");

		tcp_socket->sock.async_read_some(boost::asio::null_buffers(),
				boost::bind(&EventCB, pMC,
					tcp_socket, s, e,
					act, _1));
	}
	else if( act == CURL_POLL_OUT )
	{
		//NLOG("watching for socket to become writable");

		tcp_socket->sock.async_write_some(boost::asio::null_buffers(),
				boost::bind(&EventCB, pMC,
					tcp_socket, s, e,
					act, _1));
	}
	else if( act == CURL_POLL_INOUT )
	{
		//NLOG("watching for socket to become readable & writable");

		tcp_socket->sock.async_read_some(boost::asio::null_buffers(),
				boost::bind(&EventCB, pMC,
					tcp_socket, s, e,
					CURL_POLL_IN, _1));

		tcp_socket->sock.async_write_some(boost::asio::null_buffers(),
				boost::bind(&EventCB, pMC,
					tcp_socket, s, e,
					CURL_POLL_OUT, _1));
	}

	//SocketMapItr it = pMC->m_sockeets.find(s);
	//if(it == pMC->m_sockeets.end())
	//{
	//	//NLOG( "socket %d is a c-ares socket, ignoring", s);
	//	return;
	//}
	//boost::asio::ip::tcp::socket * tcp_socket = it->second;
	//if(act == CURL_POLL_IN)
	//{
	//	//NLOG( "watching for socket to become readable");

	//	tcp_socket->async_read_some(boost::asio::null_buffers(), boost::bind(&EventCB, pMC, tcp_socket, act));
	//}
	//else if (act == CURL_POLL_OUT)
	//{
	//	//NLOG( "watching for socket to become writable");

	//	tcp_socket->async_write_some(boost::asio::null_buffers(), boost::bind(&EventCB, pMC, tcp_socket, act));
	//}
	//else if(act == CURL_POLL_INOUT)
	//{
	//	//NLOG( "watching for socket to become readable & writable");

	//	tcp_socket->async_read_some(boost::asio::null_buffers(),
	//		boost::bind(&EventCB, pMC, tcp_socket, act));

	//	tcp_socket->async_write_some(boost::asio::null_buffers(),
	//		boost::bind(&EventCB, pMC, tcp_socket, act));
	//}
}

//void MultiCurl::AddSock(curl_socket_t s, CURL *easy, int action, MultiCurl *pMC)
//{
//	/* fdp is used to store current action */
//	int *fdp = (int *) calloc(sizeof(int), 1);
//
//	SetSock(fdp, s, easy, action, pMC);
//	curl_multi_assign(pMC->m_pMulti, s, fdp);
//}

/* CURLMOPT_SOCKETFUNCTION */
int MultiCurl::MultiSockCB(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
	//NLOG( "MultiSockCB: socket=%d, what=%d, sockp=%p", s, what, sockp);

	MultiCurl *pMC = (MultiCurl*) cbp;
	int *actionp = (int *) sockp;

	const char *whatstr[] = { "none", "IN", "OUT", "INOUT", "REMOVE"};
	//NLOG( "socket callback: s=%d e=%p what=%s ", s, e, whatstr[what] );

	SocketMapItr it = pMC->m_sockeets.find( s );

	if( it == pMC->m_sockeets.end() )
	{
		if(actionp)
		{
			NLOG("socket( %u ) closed already before remove CURL_POLL_REMOVE event. bug???", s);
			return 0;
		}
		else
		{
			NLOG("we don't know how to create asio::ip::tcp::socket without this fd's protocol family, please recompiled libcurl without c-ares");
			NLOG("socket( %d ) is a c-ares socket, ignoring", s);
			return 0; // don't poll this fd, will cause c-ares read dns response until timeout
		}
	}

	socket_ptr &tcp_socket = it->second;

	if ( !actionp )
	{
		actionp = &(tcp_socket->mask);

		curl_multi_assign(pMC->m_pMulti, s, actionp);
	}

	if ( what == CURL_POLL_REMOVE )
	{
		NLOG("remsock: socket=%d", s);
	}
	else
	{
		NLOG("Changing action from %s to %s", whatstr[*actionp], whatstr[what]);
		SetSock(tcp_socket, s, e, what & (~*actionp), pMC); // only add new instrest
	}

	*actionp = what;
	return 0;
}

/* CURLOPT_WRITEFUNCTION */
size_t MultiCurl::WriteCB(void *ptr, size_t size, size_t nmemb, void *data)
{
	MCRequest *pRequest =reinterpret_cast<MCRequest*>( data ) ;
	size_t written = size * nmemb ;

	pRequest->m_response.m_data.append( reinterpret_cast<char*>( ptr ), written ) ;

	return written;
}

size_t MultiCurl::HeaderWriteCB( void *ptr, size_t size, size_t nmemb, void *data)
{
	MCRequest *pRequest =reinterpret_cast<MCRequest*>( data ) ;
	size_t written = size * nmemb ;

	std::string header((char*)ptr, written);

	if( pRequest->m_response.m_responseReason.empty() )  // http code
	{
		std::string::size_type statusPos = header.find(' ');
		if( statusPos == std::string::npos )
		{
			return 0;
		}

		statusPos = header.find_first_not_of(' ', statusPos+1);
		if( statusPos == std::string::npos )
		{
			return 0;
		}

		std::string::size_type reasonPos = header.find(' ', statusPos+1);
		if( reasonPos == std::string::npos )
		{
			return 0;
		}

		reasonPos = header.find_first_not_of(' ', reasonPos+1);
		if( reasonPos == std::string::npos )
		{
			return 0;
		}

		pRequest->m_response.m_responseCode = atoi((char*)ptr+statusPos);
		pRequest->m_response.m_responseReason = header.substr(reasonPos);
		pRequest->m_response.m_responseReason.erase(pRequest->m_response.m_responseReason.find_last_not_of("\r\n")+1);

		pRequest->m_response.m_data.clear();
	}
	else
	{
		std::string::size_type pos = header.find(':');
		if( pos != std::string::npos )
		{
			std::string key = header.substr(0, pos);
			std::string value = header.substr(pos+1);

			key.erase(0, key.find_first_not_of(" \t\r\n"));
			key.erase(key.find_last_not_of(" \t\r\n") + 1);
			
			value.erase(0, value.find_first_not_of(" \t\r\n"));
			value.erase(value.find_last_not_of(" \t\r\n") + 1);

			if( key == "Content-Length" )
			{
				pRequest->m_response.m_data.reserve(atoi(value.c_str()));
			}
		}
	}
	return written;
}


/* CURLOPT_PROGRESSFUNCTION */
int MultiCurl::ProgCB(void *p, double dltotal, double dlnow, double ult, double uln)
{
	//MCRequest *pRequest =reinterpret_cast<MCRequest*>( p ) ;
	//NLOG( "Progress: %s (%pMC/%pMC)", pRequest->m_url.c_str(), dlnow, dltotal);
	//NLOG( "Progress: %s (%pMC)", pRequest->m_url.c_str(), ult);

	return 0;
}

/* CURLOPT_OpenSocketFUNCTION */
curl_socket_t MultiCurl::OpenSocket( void *clientp, curlsocktype purpose, struct curl_sockaddr *address )
{
	//NLOG( "OpenSocket :");

	MultiCurl *pMC = reinterpret_cast<MultiCurl*>( clientp ) ;

	curl_socket_t sockfd = CURL_SOCKET_BAD;

	/* restrict to IPv4 */
	if(purpose == CURLSOCKTYPE_IPCXN && address->family == AF_INET)
	{
		/* create a tcp socket object */
		socket_ptr tcp_socket( new SocketInfo( pMC->m_asio ) ) ;

		/* open it and get the native handle*/
		boost::system::error_code ec;
		tcp_socket->sock.open(boost::asio::ip::tcp::v4(), ec);

		if(ec)
		{
			/* An error occurred */
			ELOG( "Couldn't open socket [ %s ]; ERROR: Returning CURL_SOCKET_BAD to signal error", ec.message().c_str() );
		}
		else
		{
			sockfd = tcp_socket->sock.native_handle();
			NLOG( "Opened socket %d", sockfd);

			/* save it for monitoring */
			size_t size = pMC->m_sockeets.size() ;
			pMC->m_sockeets.insert(std::make_pair(sockfd, tcp_socket)) ;
			assert(size + 1 == pMC->m_sockeets.size()) ;
		}
	}

	return sockfd;
}

/* CURLOPT_CloseSocketFUNCTION */
int MultiCurl::CloseSocket( void *clientp, curl_socket_t item )
{
	//NLOG( "CloseSocket : %d", item);

	MultiCurl *pMC = reinterpret_cast<MultiCurl*>( clientp ) ;

	SocketMapItr it = pMC->m_sockeets.find(item);

	if(it != pMC->m_sockeets.end())
	{
		assert(it->second->sock.native_handle() == item);
		it->second->mask = 0;

		// close or cancel will cancel any asynchronous send, receive or connect operations 
		// Caution: on Windows platform, if connect host timeout, the event_cb will pending forever. Must be canceled manually
		it->second->sock.cancel(); 
		pMC->m_sockeets.erase(it);
	}
	else
	{
		assert( false ) ;
	}

	return 0;
}


int MultiCurl::OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//printf("[TEXT]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
		printf("[DATA_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
		printf("[DATA_OUT]%s\n", pData);
	}
	return 0;
}

void MultiCurl::PostRequest( void )
{
	m_asio.post( boost::bind( &MultiCurl::TakeNewRequest, this ) ) ;
}


/* Create a new easy handle, and add it to the global curl_multi */
void MultiCurl::TakeNewRequest( void )
{
	MCRequest *pRequest =NULL;

	while( GetRequest( pRequest ) )
	{
		CURLMcode rc;

		pRequest->m_easy = curl_easy_init();
		if(!pRequest->m_easy)
		{
			ELOG( "curl_easy_init() failed, exiting!" );
			delete pRequest ;
			return ;
		}

		pRequest->m_mgr = this;
		CURLcode res = curl_easy_setopt(pRequest->m_easy, CURLOPT_URL, pRequest->m_url.c_str());
		if( res != CURLE_OK )
		{
			ELOG( "curl_easy_setopt( url ) fail, return %u!", res );
			return ;
		}

		// read
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_READFUNCTION, NULL);

		// write
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_WRITEFUNCTION, WriteCB);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_WRITEDATA, pRequest);

		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_HEADERFUNCTION, HeaderWriteCB);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_WRITEHEADER, pRequest);

		// for error
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_VERBOSE, 1L);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_ERRORBUFFER, pRequest->m_response.m_error);

		// when need open it, commit without debug
		//curl_easy_setopt(pRequest->m_easy, CURLOPT_DEBUGFUNCTION, OnDebug);

		// self data when finish get it
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_PRIVATE, pRequest);

		// show the down process ;
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_NOPROGRESS, 1L);          // To set CURLOPT_NOPROGRESS 0 to open CURLOPT_PROGRESSFUNCTION 
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_PROGRESSFUNCTION, ProgCB);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_PROGRESSDATA, pRequest);

		//// speed
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_LOW_SPEED_TIME, 3L);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_LOW_SPEED_LIMIT, 10L);

		// timeout for the connect phase
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_CONNECTTIMEOUT, 16L); 

		/* call this function to get a socket */
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_OPENSOCKETFUNCTION, OpenSocket);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_OPENSOCKETDATA, this);

		/* call this function to close a socket */
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_CLOSESOCKETFUNCTION, CloseSocket);
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_CLOSESOCKETDATA, this);

		/** 
		* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。 
		* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。 
		*/
		res = curl_easy_setopt(pRequest->m_easy, CURLOPT_NOSIGNAL, 1L);

		if( !pRequest->m_data.empty() )
		{
			res = curl_easy_setopt(pRequest->m_easy, CURLOPT_POST, 1);
			res = curl_easy_setopt(pRequest->m_easy, CURLOPT_POSTFIELDS, pRequest->m_data.c_str());
			res = curl_easy_setopt(pRequest->m_easy, CURLOPT_POSTFIELDSIZE, pRequest->m_data.size());
		}

		if( pRequest->m_isHttps )
		{
			if( pRequest->m_caPath.empty() )
			{
				res = curl_easy_setopt(pRequest->m_easy, CURLOPT_SSL_VERIFYPEER, false);
				res = curl_easy_setopt(pRequest->m_easy, CURLOPT_SSL_VERIFYHOST, false);
			}
			else  
			{
				//缺省情况就是PEM，所以无需设置，另外支持DER  
				//curl_easy_setopt(pRequest->m_easy,CURLOPT_SSLCERTTYPE,"PEM");
				res = curl_easy_setopt(pRequest->m_easy, CURLOPT_SSL_VERIFYPEER, true);
				res = curl_easy_setopt(pRequest->m_easy, CURLOPT_CAINFO, pRequest->m_caPath.c_str());
			}
		}

		NLOG("Adding easy %p to multi %p (%s)", pRequest->m_easy, m_pMulti, pRequest->m_url.c_str());

		rc = curl_multi_add_handle( m_pMulti, pRequest->m_easy ) ;

		MCodeOrDie("NewConn: curl_multi_add_handle", rc);

		/* note that the add_handle() will set a time-out to trigger very soon so
		that the necessary socket_action() call will be called by this app */
	}
}


const char* MultiCurl::UrlEncode(const std::string& srcUrl, std::string& dstUrl)
{
	char* dst = curl_easy_escape(NULL, srcUrl.c_str(), srcUrl.length());
	if( dst != NULL )
	{
		dstUrl = dst;
		curl_free(dst);
	}
	return dstUrl.c_str();
}

const char* MultiCurl::UrlDecode(const std::string& srcUrl, std::string& dstUrl)
{
	int outlength;
	char* dst = curl_easy_unescape(NULL, srcUrl.c_str(), srcUrl.length(), &outlength);
	if( dst != NULL )
	{
		dstUrl.assign(dst, dst+outlength);
		curl_free(dst);
	}
	return dstUrl.c_str();
}

void MultiCurl::AppendEncodedParams(std::string &result, const std::string& paramName, const std::string& paramValue)
{
	if( paramName.empty() || paramValue.empty() )
	{
		return;
	}

	std::string encodedParamValue;
	UrlEncode(paramValue, encodedParamValue);

	result.append(paramName);
	result.append(encodedParamValue);
}

void MultiCurl::AppendParams(std::string &result, const std::string& paramName, const std::string& paramValue)
{
	if( paramName.empty() || paramValue.empty() )
	{
		return;
	}

	//string encodedParamValue;
	//UrlEncode(paramValue, encodedParamValue);

	result.append(paramName);
	//result.append(encodedParamValue);

	result.append(paramValue);
}
