#include "GameServerHttpClients.h"




GameServerHttpClients::GameServerHttpClients( void )
{
}

GameServerHttpClients::~GameServerHttpClients( void )
{
}

void GameServerHttpClients::Start( boost::asio::io_service &io )
{
	m_spMC.reset( new MultiCurl( io ) ) ;
}
