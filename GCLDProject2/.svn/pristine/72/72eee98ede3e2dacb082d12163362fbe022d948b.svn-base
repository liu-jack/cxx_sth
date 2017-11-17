#include "CenterServerHttpClients.h"




CenterServerHttpClients::CenterServerHttpClients( void )
{
}

CenterServerHttpClients::~CenterServerHttpClients( void )
{
}

void CenterServerHttpClients::Start( boost::asio::io_service &io )
{
	m_spMC.reset( new MultiCurl( io ) ) ;
}
