#include <fstream>
#include <string>

#include "EventLogServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../LoginServer.h"

#include "def/MmoAssert.h"
#include "memory_buffer/NetPack.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"

#include "boost/bind/bind.hpp"


EventLogServerClient::HandlerArray EventLogServerClient::s_handlers =RegisteHandlers() ;


EventLogServerClient::HandlerArray EventLogServerClient::RegisteHandlers( void )
{
	HandlerArray handlers( pb::SERVER_OPCODE_COUNT ) ;

	handlers[ pb::S_PONG ]                =&EventLogServerClient::HandlePong ;
	handlers[ pb::SL2E_REGISTER_RET ]     =&EventLogServerClient::HandleRegisterRet ;

	return handlers ;
}


EventLogServerClient::EventLogServerClient( void )
{
	SetClientType( PACKET_TYPE_SERVER_LOGIN ) ;
	SetDstServerType( PACKET_TYPE_SERVER_EVENT ) ;

	SetPingDiffMS( sLoginSvr.Config().GetInt( "EventLogServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sLoginSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
}

EventLogServerClient::~EventLogServerClient( void )
{
}



void EventLogServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{

}

bool EventLogServerClient::HandlePacket( NetPack &packet )
{
	size_t opCode = packet.GetOpcode() ;

	if( opCode >= s_handlers.size() )
	{
		ASSERT( false ) ;
		return false ;
	}

	NetPackHandler packetFun = s_handlers[ opCode ] ;
	if( packetFun )
	{
		(this->*packetFun)( packet ) ;
		return true ;
	}

	return false ;
}

void EventLogServerClient::ClientConnect( void )
{
	SendRegisterInfo() ;
}

void EventLogServerClient::ClientDisconnect( void )
{
	SetDstInfo( "", "" ) ;
}

void EventLogServerClient::SendPing( void )
{
	ServerClient::SendPing( pb::S_PING ) ;
}

void EventLogServerClient::HandlePong( NetPack &packet )
{
	ServerClient::HandlePong( packet ) ;
}

void EventLogServerClient::SendRegisterInfo( void )
{
	pb::SL2E_RegisterInfo msg ;

	msg.set_ip( sLoginSvr.GetIP() ) ;
	msg.set_port( sLoginSvr.GetPort() ) ;
	msg.set_region_id(sLoginSvr.GetRegionId());
	msg.set_server_id(0);

	Send( pb::SL2E_REGISTER, msg ) ;
}

void EventLogServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SL2E_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;
		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s) fail ! %s .",GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

void AddInt32Value( int32 value )
{
}

void AddUInt32Value( int32 value )
{
}


void EventLogServerClient::AddInt32Value( pb::Server_Event& evt, int32 value )
{
	pb::EventParm *pParam =evt.add_params() ;
	pParam->set_parm_type( pb::EventParm::INT32 ) ;
	pParam->set_value_int32( value ) ;
}

void EventLogServerClient::AddUInt32Value( pb::Server_Event& evt, uint32 value )
{
	pb::EventParm *pParam =evt.add_params() ;
	pParam->set_parm_type( pb::EventParm::UINT32 ) ;
	pParam->set_value_uint32( value ) ;
}

void EventLogServerClient::AddInt64Value( pb::Server_Event& evt, int64 value )
{
	pb::EventParm *pParam =evt.add_params() ;
	pParam->set_parm_type( pb::EventParm::INT64 ) ;
	pParam->set_value_int64( value ) ;
}

void EventLogServerClient::AddUInt64Value( pb::Server_Event& evt, uint64 value )
{
	pb::EventParm *pParam =evt.add_params() ;
	pParam->set_parm_type( pb::EventParm::UINT64 ) ;
	pParam->set_value_uint64( value ) ;
}

void EventLogServerClient::AddFloatValue( pb::Server_Event& evt, float value )
{
	pb::EventParm *pParam =evt.add_params() ;
	pParam->set_parm_type( pb::EventParm::FLOAT ) ;
	pParam->set_value_float( value ) ;
}

void EventLogServerClient::AddStringValue( pb::Server_Event& evt, const string &value )
{
	pb::EventParm *pParam =evt.add_params() ;
	pParam->set_parm_type( pb::EventParm::STRING ) ;
	pParam->set_value_str( value ) ;
}



void EventLogServerClient::SendEvent( pb::Server_Event& evtMsg )
{
    Send( pb::SL2E_EVENT, evtMsg ) ; 
}

void EventLogServerClient::SendEvent( pb::Server_Event& evtMsg, int paramCount, va_list& params, bool withFmt /*= false*/ )
{
    evtMsg.set_time( sOS.GetRealTime() );

    for(; paramCount; paramCount--)
    {
        if ( withFmt)
        {
            const char * paramFmt = va_arg(params, const char *);
            ::pb::EventParm* evtParm = evtMsg.add_params();
            if ( evtParm == NULL )
                return;

            if (paramFmt[0] == 'i')
            {
                evtParm->set_parm_type( pb::EventParm_ParmType_INT32);
                evtParm->set_value_int32( va_arg(params, int32));
            }
            else if (paramFmt[0] == 'u')
            {
                evtParm->set_parm_type( pb::EventParm_ParmType_UINT32);
                evtParm->set_value_uint32( va_arg(params, uint32));
            }
            else if (paramFmt[0] == 'b')
            {
                evtParm->set_parm_type( pb::EventParm_ParmType_INT64);
                evtParm->set_value_int64( va_arg(params, int64));
            }
            else if (paramFmt[0] == 'l')
            {
                evtParm->set_parm_type( pb::EventParm_ParmType_UINT64);
                evtParm->set_value_uint64( va_arg(params, uint64));
            }
			else if (paramFmt[0] == 'f')
			{
				evtParm->set_parm_type( pb::EventParm_ParmType_FLOAT);
				evtParm->set_value_float( (float)va_arg(params, double));
			}
            else if (paramFmt[0] == 's')
            {
                evtParm->set_parm_type( pb::EventParm_ParmType_STRING);
                evtParm->set_value_str( va_arg(params, const char *));
            }
            else
            {
                ASSERT(0);
                return;
            }
        } 
        else
        {
            int paramValue = va_arg(params, int);
            ::pb::EventParm* evtParm = evtMsg.add_params();
            if (evtParm)
            {
                evtParm->set_parm_type( pb::EventParm_ParmType_INT32);
                evtParm->set_value_int32( paramValue );
            }
        }
    }

	SendEvent( evtMsg ) ;
}

