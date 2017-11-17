#pragma once

#include <string>
#include <boost/smart_ptr.hpp>

#include "Singleton.h"

#include "curl/MultiCurl.h"

namespace boost
{
	namespace asio
	{
		class io_service ;
	}
}

enum HttpRequestType
{
	HTTP_REQUEST_SDK_ACCOUNT_CHECK      =0,
	HTTP_REQUEST_GET_CONFIG_SERVER_LIST =1,
	HTTP_REQUEST_GET_VERSION_CHANNEL = 2,
	HTTP_REQUEST_GET_TESTER_ACCOUNT_IDS = 3,
} ;

class LoginServerHttpClients : public Singleton<LoginServerHttpClients>
{
private:
	friend class Singleton<LoginServerHttpClients>;

private:
	LoginServerHttpClients( void ) ;

public:
	virtual ~LoginServerHttpClients( void ) ;

	void Start( boost::asio::io_service &io ) ;

	MultiCurl& GetMultiCurl( void ) {   return *m_spMC ;   }
	const std::string& GetWebUrl( void ) {   return m_webUrl ;   }
	void SetWebUrl( const std::string &url ) {   m_webUrl =url ;   }

public:
	std::string                  m_webUrl ;

	boost::shared_ptr<MultiCurl> m_spMC ;
};

#define sHttps       (LoginServerHttpClients::Instance().GetMultiCurl())
#define sHttpClients (LoginServerHttpClients::Instance())
