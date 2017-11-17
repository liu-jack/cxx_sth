
#include "GameServerSession.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "object/Player.h"
#include "Opcode.pb.h"


GameServerSession::GameServerSession( void )
{
}

GameServerSession::~GameServerSession( void )
{
}

void GameServerSession::Update( int64 diffMS )
{
	// Handle packets
	Session::Update( diffMS ) ;
}

void GameServerSession::HandleNetPack( NetPack &pack )
{
    bool isServerPacket = pack.IsServerPacket() ;

    if ( !m_handler.get())
    {
        if ( !isServerPacket)
        {
            m_handler.reset( new Player());
        }
    }

    if ( m_handler.get())
    {
        m_handler->HandlePack( pack);
    }
}

