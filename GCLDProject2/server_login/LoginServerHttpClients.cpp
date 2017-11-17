#include "LoginServerHttpClients.h"




LoginServerHttpClients::LoginServerHttpClients( void )
{
}

LoginServerHttpClients::~LoginServerHttpClients( void )
{
}

void LoginServerHttpClients::Start( boost::asio::io_service &io )
{
	m_spMC.reset( new MultiCurl( io ) ) ;
}
