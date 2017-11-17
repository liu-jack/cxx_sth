#include <fstream>
#include <string>

#include "EventLogServerClient.h"
#include "Logger.h"
#include "OS.h"

#include "../GameServer.h"
#include "../object/Player.h"
#include "../session/PlayerSession.h"

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
	handlers[ pb::SG2E_REGISTER_RET ]     =&EventLogServerClient::HandleRegisterRet ;

	return handlers ;
}


EventLogServerClient::EventLogServerClient( void )
{
	SetClientType( PACKET_TYPE_SERVER_GAME ) ;
	SetDstServerType( PACKET_TYPE_SERVER_EVENT ) ;

	SetPingDiffMS( sGameSvr.Config().GetInt( "EventLogServer.ping_time_ms", 5000 ) ) ;
	SetReConnectDiffMS( sGameSvr.Config().GetInt( "Self.reconnect_diff_time_ms", 5000 ) ) ;
	m_watchLogSend.SetMax(sGameSvr.Config().GetInt( "EventLogServer.log_send_time_ms", 60000 ));
}

EventLogServerClient::~EventLogServerClient( void )
{
}



void EventLogServerClient::ClientUpdate( uint64 nowTimeMS, int64 diffMS )
{
	m_watchLogSend.Update(diffMS);
	if (m_watchLogSend.Done())
	{
		SendEventLogs();
		m_watchLogSend.Reset();
	}
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
	pb::SG2E_RegisterInfo msg ;

	msg.set_server_id( sGameSvr.GetServerId() ) ;
	msg.set_region_id( sGameSvr.GetRegionId() ) ;

	msg.set_ip( sGameSvr.GetIP() ) ;
	msg.set_port( sGameSvr.GetPort() ) ;

	Send( pb::SG2E_REGISTER, msg ) ;
}

void EventLogServerClient::HandleRegisterRet( NetPack &packet )
{
	pb::SG2E_RegisterInfoRet retInfo ;
	packet >> retInfo ;

	switch( retInfo.result() )
	{
	case pb::SERVER_REGISTER_OK:
		{
			NLOG( "%s regiter %s,(%s:%s) success !", GetClientName(), GetServerName(), GetIP(), GetPort() ) ;
		} break ;

	default:
		{
			NLOG( "%s regiter %s,(%s:%s)  fail !: %s .", GetClientName(), GetServerName(), GetIP(), GetPort(), pb::RegisterResult_Name( retInfo.result() ).c_str() ) ;
		}
	}
}

void EventLogServerClient::SendEventLogs()
{
	pb::Server_Events logsMsg;
	m_eventLogs.SaveTo(logsMsg);
	int nLog = logsMsg.logs_size();
	for (int i=0; i < nLog; i++)
	{
		const pb::Server_Event& pSEvent = logsMsg.logs(i);
		SendEvent(pSEvent);
	}
	m_eventLogs.ClearAll();
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

void EventLogServerClient::SendEvent(const pb::Server_Event& evtMsg )
{
    Send( pb::SG2E_EVENT, evtMsg ) ; 
}

void EventLogServerClient::SendEvent( EventLogInfo& evtMsg, int paramCount, va_list& params, bool withFmt /*= false*/ )
{
    evtMsg.time = sOS.GetRealTime();

    for(; paramCount; paramCount--)
    {
		EventParmSt evtParm;
        if ( withFmt)
        {
            const char * paramFmt = va_arg(params, const char *);
            if (paramFmt[0] == 'i')
            {
                evtParm.parm_type = pb::EventParm_ParmType_INT32;
                evtParm.value_int32 = va_arg(params, int32);
            }
            else if (paramFmt[0] == 'u')
            {
                evtParm.parm_type =  pb::EventParm_ParmType_UINT32;
                evtParm.value_uint32 = va_arg(params, uint32);
            }
            else if (paramFmt[0] == 'b')
            {
                evtParm.parm_type = pb::EventParm_ParmType_INT64;
                evtParm.value_int64 = va_arg(params, int64);
            }
            else if (paramFmt[0] == 'l')
            {
                evtParm.parm_type =  pb::EventParm_ParmType_UINT64;
                evtParm.value_uint64 = va_arg(params, uint64);
            }
			else if (paramFmt[0] == 'f')
			{
				evtParm.parm_type = pb::EventParm_ParmType_FLOAT;
				evtParm.value_double = va_arg(params, double);
			}
            else if (paramFmt[0] == 's')
            {
                evtParm.parm_type =  pb::EventParm_ParmType_STRING;
                evtParm.value_str =  va_arg(params, const char *);
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
            evtParm.parm_type = pb::EventParm_ParmType_INT32;
            evtParm.value_int32 = paramValue;
        }
		evtMsg.params.push_back(evtParm);
    }

	m_eventLogs.AddEventLog(evtMsg);
	//SendEvent( evtMsg ) ;
}

//uint64         "l"
//int64          "b"
//uint32         "u"
//int32          "i"
//const char*    "s"
void EventLogServerClient::SendEventInfo( uint32 eventId, PlayerSession &session, int paramCount, ... )
{
	EventLogInfo evtMsg;
	evtMsg.event_id = eventId;
	evtMsg.account_id =  session.GetAccountId();
	evtMsg.player_id = session.GetPlayerId();
	va_list vlist;
	va_start(vlist, paramCount);

	SendEvent( evtMsg, paramCount, vlist, true);

	va_end(vlist);
}

void EventLogServerClient::SendEventInfo( uint32 eventId, uint64 playerId, uint64 accountId,int paramCount, ... )
{
	EventLogInfo evtMsg;
	evtMsg.event_id = eventId;
	evtMsg.account_id =  accountId;
	evtMsg.player_id = playerId;

	va_list vlist;
	va_start(vlist, paramCount);

	SendEvent( evtMsg, paramCount, vlist, true);

	va_end(vlist);
}
