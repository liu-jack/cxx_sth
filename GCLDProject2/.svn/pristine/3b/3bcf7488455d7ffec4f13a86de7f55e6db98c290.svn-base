

#include "HttpManager.h"
#include "mongoose.h"

HttpMsgTool::HttpMsgTool( http_message *pMsg )
{
	m_pHttpMsg            = pMsg;
}

HttpMsgTool::~HttpMsgTool( void )
{

}

HttpRequestType HttpMsgTool::GetRequestType( void ) const
{
	//数据错误
	if( m_pHttpMsg == NULL || m_pHttpMsg->method.p == NULL )
	{
		return HttpRequestTypeUnknow;
	}

	if( mg_vcmp( &m_pHttpMsg->method, "GET" ) == 0 )
	{
		return HttpRequestTypeGet ;
	}
	else if( mg_vcmp( &m_pHttpMsg->method, "POST" ) == 0 )
	{
		return HttpRequestTypePost ;
	}
	else
	{
		return HttpRequestTypeUnknow ;
	}
}

void HttpMsgTool::GetHttpStrVar( const char *pName, std::string &str ) const
{
	if( m_pHttpMsg == NULL || pName == NULL )
	{
		return;
	}

	char var[CHAR_HTTP_VAR_MAX] ="";

	mg_str *pMgStr =NULL ;
	switch( GetRequestType() )
	{
	case HttpRequestTypePost:
		pMgStr =&m_pHttpMsg->body ;
		break ;

	case HttpRequestTypeGet:
		pMgStr =&m_pHttpMsg->query_string ;
		break ;

	default:
		assert( false ) ;
		return ;
	}

	if( pMgStr->p == NULL ||  pMgStr->len <= 0 )
	{
		return ;
	}

	int len =mg_get_http_var( pMgStr, pName, var, sizeof(var) ); // 获取变量
	if( len > 0 )
	{
		str.assign( var, len ) ;    //如果数据正确
	}
}

int HttpMsgTool::GetHttpIntVar( const char *pName, int def ) const
{
	std::string str ;
	GetHttpStrVar( pName, str ) ;
	if( str.empty() )
	{
		return def ;
	}

	return atoi( str.c_str() ) ;
}


double HttpMsgTool::GetHttpDoubleVar( const char *pName, double def ) const
{
	std::string str ;
	GetHttpStrVar( pName, str ) ;
	if( str.empty() )
	{
		return def ;
	}

	return strtod( str.c_str(), NULL ) ;
}

long long int HttpMsgTool::GetHttpLongLongIntVar( const char *pName, long long int def ) const
{
	std::string str ;
	GetHttpStrVar( pName, str ) ;
	if( str.empty() )
	{
		return def ;
	}

	long long int retVal =def;

	sscanf( str.c_str(), "%lld", &retVal ) ;

	return retVal ;
}


void HttpMsgTool::GetUrl( std::string &str ) const
{
	if(  m_pHttpMsg->uri.len > 1 )
	{
		str.assign( m_pHttpMsg->uri.p + 1, m_pHttpMsg->uri.len - 1 ) ;
	}
}

void HttpMsgTool::GetCookie(const char *pName, std::string &str ) const
{
	if( m_pHttpMsg == NULL || pName == NULL || m_pHttpMsg->query_string.p == NULL )
	{
		return;
	}

	struct mg_str *cookie_header = mg_get_http_header( m_pHttpMsg, "cookie" );    //获取Cookie 整条
	if (cookie_header == NULL)
	{
		return;
	}

	char var[CHAR_HTTP_COOKIE_MAX] ="";    //Cookie 最大更长
	int len =mg_http_parse_header(cookie_header, pName, var, sizeof(var));
	if( len > 0 )
	{
		str.assign( var, len ) ;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HttpParamTool::HttpParamTool( HttpRequestType type, const std::string &url, const std::string &externHeaders ) 
	: m_url( url ), m_externHeaders( externHeaders )
{
	m_paramLen =0 ;
	m_type     =type ;
	switch( m_type )
	{
	case HttpRequestTypeUnknow:
		{
			assert( false ) ;
		} break ;

	case HttpRequestTypeGet:
		{
			AddParam( "%s%s", m_url.c_str(), "?" ) ; 
		} break ;

	case HttpRequestTypePost:
		{
			//m_externHeaders.append( "Content-Type: text/html\r\n" ) ;
			//m_externHeaders.append( "Content-Type: multipart/form-data\r\n" ) ;
		} break ;
	}

	// 注意，这里末尾需要有 \r\n 因为函数没加
	m_externHeaders.append( "Content-Type: application/x-www-form-urlencoded\r\n" ) ;
}

HttpParamTool::~HttpParamTool( void )
{
}

bool HttpParamTool::CheckIsOk( void )
{
	return !m_url.empty() ;
}


bool HttpParamTool::AddParam( const char *pName, const int val )
{
	return AddParam( "%s=%d&", pName, val ) ;
}

bool HttpParamTool::AddParam( const char *pName, const long long val )
{
	return AddParam( "%s=%lld&", pName, val ) ;
}

bool HttpParamTool::AddParam( const char *pName, const std::string &val )
{
	return AddParam("%s=%s&", pName, val.c_str() ) ;
}

bool HttpParamTool::AddParam( const char *pName, const double val )
{
	return AddParam( "%s=%lf&", pName, val ) ;
}


template< class T >
bool HttpParamTool::AddParam( const char *pFormat, const char *pName, const T &val )
{
	const int leftLen =CHAR_HTTP_PARAM_MAX - m_paramLen ;
	if( leftLen <= 4 )
	{
		return false ;
	}

	int len =snprintf( m_paramBuf + m_paramLen, leftLen, pFormat, pName, val ) ;
	assert( len >= 0 ) ;
	m_paramLen += len ;

	return true ;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HttpManager::HttpManager( void )
{
	SetIsRunning( false ) ;
	m_waitMillisecond =0;
}

HttpManager::~HttpManager( void )
{
}

bool HttpManager::Init( const char *pListPort, bool isPerThreadForConnect, mg_event_handler_t pHandler )
{
	if( pListPort == NULL || pHandler == NULL )
	{
		return false ;
	}

	mg_mgr_init( &m_mgr, this ) ;
	m_pConnect = mg_bind( &m_mgr, pListPort, pHandler ) ;
	mg_set_protocol_http_websocket(m_pConnect) ;

	if( isPerThreadForConnect )
	{
		/* For each new connection, execute ev_handler in a separate thread */
		mg_enable_multithreading(m_pConnect);
	}

	return true ;
}

void HttpManager::RegisterHttpEndpoint( const char *pUrl, mg_event_handler_t pHandler )
{
	mg_register_http_endpoint( m_pConnect, pUrl, pHandler ) ;
}


void HttpManager::WorkThread( void )
{
	while( IsRunning() )
	{
		mg_mgr_poll( &GetMgMgr(), GetWaitMillisecond() ) ;
		WhenRunning() ;
	}
}


void HttpManager::Release( void )
{
	mg_mgr_free( &m_mgr );
}

void* Loop( void *pData )
{
	HttpManager *pMgr =static_cast<HttpManager*>( pData ) ;
	pMgr->WorkThread() ;
	return pData ;
}

bool HttpManager::StartListion( int waitMillisecond )
{
	SetIsRunning( true ) ;
	m_waitMillisecond =waitMillisecond ;
	mg_start_thread( Loop, this ) ;

	return true ;
}


mg_connection* HttpManager::SendHttpRequest( mg_event_handler_t pHandler, HttpParamTool &paramTool, void *pUserData )
{
	if( !paramTool.CheckIsOk() )
	{
		return NULL ;
	}

	const char *pExternHeaders = paramTool.m_externHeaders.empty() ? NULL : paramTool.m_externHeaders.c_str() ;

	mg_connect_opts opts;
	memset(&opts, 0, sizeof(opts));
	opts.user_data =pUserData ;

	switch( paramTool.m_type )
	{
	case HttpRequestTypeGet:
		{
			if( paramTool.m_paramLen > 1 )
			{
				char *pLastChar =&paramTool.m_paramBuf[ paramTool.m_paramLen - 1 ] ;
				if( *pLastChar == '&' )
				{
					*pLastChar ='\0' ;
				}
			}

			return mg_connect_http_opt( &m_mgr, pHandler, opts, paramTool.m_paramBuf, pExternHeaders, NULL ) ;
		} break ;

	case HttpRequestTypePost:
		{
			if( paramTool.m_paramLen > 1 )
			{
				char *pLastChar =&paramTool.m_paramBuf[ paramTool.m_paramLen - 1 ] ;
				if( *pLastChar == '&' )
				{
					*pLastChar ='\0' ;
				}
			}

			return mg_connect_http_opt( &m_mgr, pHandler, opts, paramTool.m_url.c_str(), pExternHeaders, paramTool.m_paramBuf ) ;
		} break ;

	default:
		return NULL ;
	}

}


void HttpManager::ReplyError( mg_connection *pConn, int errCode, const char *pReason )
{
	mg_http_send_error( pConn, errCode, pReason ) ;
}


void HttpManager::ReplyStr( mg_connection *pConn, const std::string &str )
{
	// 注意，这里末尾不能有 \r\n 因为函数加了
	//mg_send_head( pConn, 200, result.size(), "Content-Type: text/html" ) ;
	mg_send_head( pConn, 200, str.size(), "Content-Type: application/x-www-form-urlencoded" ) ;
	mg_send( pConn, &str[0], str.size() ) ;
}
