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


class GameServerHttpClients : public Singleton<GameServerHttpClients>
{
private:
	friend class Singleton<GameServerHttpClients>;

private:
	GameServerHttpClients( void ) ;

public:
	virtual ~GameServerHttpClients( void ) ;

	void Start( boost::asio::io_service &io ) ;

	MultiCurl& GetMultiCurl( void ) {   return *m_spMC ;   }

	const std::string& GetWebUrl( void ) {   return m_webUrl ;   }
	void SetWebUrl( const std::string &url ) {   m_webUrl =url ;   }

public:
	std::string                  m_webUrl ;

	boost::shared_ptr<MultiCurl> m_spMC ;
};

#define sHttps       (GameServerHttpClients::Instance().GetMultiCurl())
#define sHttpClients (GameServerHttpClients::Instance())
