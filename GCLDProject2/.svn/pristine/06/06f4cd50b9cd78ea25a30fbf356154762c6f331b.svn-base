#include "NetPack.h"

#include "Logger.h"
#include "common/Opcode.pb.h"
#include "common/ServerOpcode.pb.h"
#include "def/MmoAssert.h"

void NetPack::InitHeadInfo( void )
{
	ASSERT( m_buf.wpos() == 0 && m_buf.wpos() == m_buf.rpos() ) ;
	m_buf.resize( GetHeaderSize() ) ; 

	// Set the read pos .
	m_buf.rpos( m_buf.wpos() ) ;
	ASSERT( m_buf.wpos() == GetHeaderSize() ) ;
}

void NetPack::InitHeadInfo( char type, uint16 bodySize, uint16 opCode, uint64 sessionId )
{
	InitHeadInfo() ;

	SetPacketType( type ) ;
	ASSERT( GetPacketType() == type ) ;

	SetBodySize( bodySize ) ;
	ASSERT( GetBodySize() == bodySize ) ;

	SetOpCode( opCode ) ;
	ASSERT( GetOpcode() == opCode ) ;

	SetSessionId( sessionId ) ;
	ASSERT( GetSessionId() == sessionId ) ;
	
	// Set the read pos .
	m_buf.rpos( m_buf.wpos() ) ;

	// Check right .
	ASSERT( m_buf.wpos() == m_buf.size() ) ;
	ASSERT( m_buf.size() == GetHeaderSize() ) ;
}

const char* NetPack::GetOpcodeName( void )
{
	if( IsClientPacket() )
	{
		pb::Opcode opCode =static_cast< pb::Opcode >( GetOpcode() ) ;
		return pb::Opcode_Name( opCode ).c_str();
	}
	else
	{
		pb::ServerOpcode opCode =static_cast< pb::ServerOpcode >( GetOpcode() ) ;
		return pb::ServerOpcode_Name( opCode ).c_str();
	}
}


const char* NetPack::GetPacketTypeName( void )
{
	return GetPacketTypeName( GetPacketType() ) ;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// static
std::string              NetPack::s_unkouwName            ="Unknow Type" ;
NetPack::PackTypeNameMap NetPack::s_typeName              =GetInitTypeName() ;
std::vector<char>        NetPack::s_clientOpCodeIsNotice  ;
std::vector<char>        NetPack::s_serverOpCodeIsNotice  ;

const char* NetPack::GetPacketTypeName( int type )
{
	PackTypeNameMap::iterator itr =s_typeName.find( type ) ;
	return itr != s_typeName.end() ? itr->second.c_str() : s_unkouwName.c_str() ;
}


NetPack::PackTypeNameMap NetPack::GetInitTypeName( void )
{
	PackTypeNameMap names ;

	names[PACKET_TYPE_CLIENT]         = "Client" ;
	names[PACKET_TYPE_CLIENT_CROSS]   = "ClientToCr" ;
	names[PACKET_TYPE_CROSS_CLIENT]   = "CrossToCl" ;
	names[PACKET_TYPE_SERVER_LOGIN]   = "Login"  ;
	names[PACKET_TYPE_SERVER_GAME]    = "Game"   ;
	names[PACKET_TYPE_SERVER_GATEWAY] = "Gateway";
	names[PACKET_TYPE_SERVER_DBPROXY] = "DbProxy";
	names[PACKET_TYPE_SERVER_CENTER]  = "Center" ;
	names[PACKET_TYPE_SERVER_EVENT]   = "Event" ;
	names[PACKET_TYPE_SERVER_WEB]     = "Web" ;
	names[PACKET_TYPE_SERVER_LOBBY]   = "Lobby";
	names[PACKET_TYPE_SERVER_CROSS]   = "Cross";

	return names ;
}

const bool NetPack::IsNoticeRecvAndSend( NetPack &packet )
{
	static std::vector<char>* isNotice[2] = { &s_clientOpCodeIsNotice, &s_serverOpCodeIsNotice } ;

	int index =!packet.IsClientPacket() ;
	return (*isNotice[index])[packet.GetOpcode()] > 0 ;
}

void NetPack::ClientOpCodeNoticeInit( void )
{
	s_clientOpCodeIsNotice.resize( pb::CSMSG_COUNT, true ) ;
	s_clientOpCodeIsNotice[pb::CMSG_PING] =false ;
	s_clientOpCodeIsNotice[pb::SMSG_PONG] =false ;
}
 
void NetPack::ServerOpCodeNoticeInit( void )
{
	s_serverOpCodeIsNotice.resize( pb::SERVER_OPCODE_COUNT, true ) ;

	s_serverOpCodeIsNotice[pb::S_PING] =false ;
	s_serverOpCodeIsNotice[pb::S_PONG] =false ;
}

void NetPack::GetMsg( google::protobuf::Message *pMsg )
{
	m_buf >> *pMsg ;
}


void NetPack::SetMsg( google::protobuf::Message *pMsg )
{
	m_buf << *pMsg ;
}