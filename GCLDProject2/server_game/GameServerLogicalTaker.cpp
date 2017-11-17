

#include "GameServerLogicalTaker.h"

#include "Logger.h"
#include "OS.h"

#include "object/Player.h"
#include "session/PlayerSession.h"
#include "server_client/DbProxyServerClient.h"
#include "server_client/CenterServerClient.h"
#include "def/MmoAssert.h"
#include "utility/STLUtility.h"
#include "SysEvent/SysEventMgr.h"
#include "LuaScript/MapScriptGroup.h"
#include "LuaScript/FormulaScriptHolder.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "Social.pb.h"
#include "InterServer.pb.h"
#include "Service/ServiceMgr.h"
#include "Activity/ActMgr.h"
#include "Npc/Npc.h"
#include "character/Character.h"
//#include "AutomaticUpdate/AutomaticUpdate.h"
#include "AutomaticUpdate/AutoUpdateGame.h"
#include "session/PlayerPool.h"
#include "json/JsonTool.h"

GameServerLogicalTaker::GameServerLogicalTaker( void )
: m_scriptGroup(new MapScriptGroup())
, m_formulaHolder(new FormulaScriptHolder("formula", m_scriptGroup.get()))
,m_beforUpTime(0)
,m_endUpTime(0)
,m_endbeforTime(0)
,m_logicSerInfo(new JsonTool())
{
    if ( m_formulaHolder.get())
    {
        m_formulaHolder->Init();
    }

    //vector<float> a;
    //a.push_back( 1);
    //a.push_back( 2);
    //float result = 0;
    //CallLuaFormula("plus", result,a);
    //NLOG("test formula :%f", result);
}


GameServerLogicalTaker::~GameServerLogicalTaker( void )
{
}


void GameServerLogicalTaker::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	uint64 begintime =sOS.TimeMS();
	sCenterClient.Update( nowTimeMS, diffMS ) ;
	sDbProxyClient.Update( nowTimeMS,diffMS ) ;
	uint64 temp1 = sOS.TimeMS();
	uint64 up1 =sOS.DiffTimeMS(begintime);
	sSysEventMgr.Update( nowTimeMS,diffMS ) ;
	uint64 temp2 = sOS.TimeMS();
	uint64  up2 = sOS.DiffTimeMS(temp1);
	sAutoUpdateGame.Update(nowTimeMS,diffMS);
	temp1 = sOS.TimeMS();
	uint64  up3 =sOS.DiffTimeMS(temp2);
	ServiceMgr::RunAllService((uint32)diffMS, (uint32)nowTimeMS);
	temp2 = sOS.TimeMS();
	uint64 up4 =sOS.DiffTimeMS(temp1);
	Npc::_DeleteNpc();
	temp1 = sOS.TimeMS();
	uint64 up5 = sOS.DiffTimeMS(temp2);
	Character::_DeleteShadow();
	temp2 = sOS.TimeMS();
	uint64 up6 =sOS.DiffTimeMS(temp1);
	sActMgr._Update();
	temp1 = sOS.TimeMS();
	uint64 up7 =sOS.DiffTimeMS(temp2);
	sPlayerPool.UpdateOffLinePlayer(nowTimeMS,diffMS);
	uint64 up8 =sOS.DiffTimeMS(temp1);
	uint64 total =sOS.DiffTimeMS(begintime);
	uint32 pNum = sPlayerPool.GetOfflinePlayerCount();
	if (total > m_beforUpTime)
	{
		m_beforUpTime = total;
		m_logicSerInfo->WriteUint64("DbProxy",up1);
		m_logicSerInfo->WriteUint64("EventMgr",up2);
		m_logicSerInfo->WriteUint64("AutoUpdate",up3);
		m_logicSerInfo->WriteUint64("ServiceMgr",up4);
		m_logicSerInfo->WriteUint64("DeleteNpc",up5);
		m_logicSerInfo->WriteUint64("DeleteShadow",up6);
		m_logicSerInfo->WriteUint64("sActMgr",up7);
		m_logicSerInfo->WriteUint64("UpdateOffLine",up8);
		m_logicSerInfo->WriteInt("OffLines",pNum);
		m_logicSerInfo->WriteUint64("logTime",total);
	}
	m_endbeforTime = sOS.TimeMS();
	
	//NLOG( "--------GameLogical up1:%llu,up2:%llu,up3:%llu,up4:%llu,up5:%llu,up6:%llu,up7:%llu,up8:%llu,offlines:%u,total:%llu",up1,up2,up3,up4,up5,up6,up7,up8,pNum,total) ;
}

void GameServerLogicalTaker::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	uint64 total =sOS.DiffTimeMS(nowTimeMS);
	uint64 session =sOS.DiffTimeMS(m_endbeforTime);
	int sesscount = (int)GetSessionCount();
	if (total > m_endUpTime)
	{
		m_endUpTime = total;
		m_logicSerInfo->WriteUint64("SesTime",session);
		m_logicSerInfo->WriteInt("players",sesscount);
		m_logicSerInfo->WriteUint64("Total",total);
	}
	//NLOG( "--------GameLogical onlines:%d,sesTime:%llu,total:%llu",sesscount,session,total);
}




void GameServerLogicalTaker::AddPlayerSession( PlayerSession *pPlayerSession )
{
	ASSERT( pPlayerSession != NULL && pPlayerSession->GetPlayer() ) ;

	// when quick login and login is faster than close will can find .
	PlayerSessionByPlayerId    &byPlayerId =m_playerSessions.get<TPlayerSessionByPlayerId>() ;
	PlayerSessionByPlayerIdItr itr         =byPlayerId.find( pPlayerSession->GetPlayerId() ) ;
	if( itr != byPlayerId.end() )
	{
		byPlayerId.erase( itr ) ;
	}


	bool isInsert =m_playerSessions.insert( pPlayerSession ).second ;
	if( !isInsert )
	{
		ELOG( "GameServerLogicalTaker::AddPlayerSession() faild~~~~~~~?????????" ) ;
		//ASSERT( false ) ;
	}
}

void GameServerLogicalTaker::RemovePlayerSession( PlayerSession *pPlayerSession )
{
	ASSERT( pPlayerSession != NULL ) ;
	PlayerSessionBySessionId    &bySessionId =m_playerSessions.get<TPlayerSessionBySessionId>() ;
	PlayerSessionBySessionIdItr itr          =bySessionId.find( pPlayerSession->GetSessionId() ) ;
	if( itr != bySessionId.end() )
	{
		bySessionId.erase( itr ) ;
	}
}

void GameServerLogicalTaker::SetAllPlayerDataIsLost( void )
{
	for( PlayerSessionTableItr itr = m_playerSessions.begin(); itr != m_playerSessions.end(); ++itr )
	{
		if( (*itr)->GetPlayer() )
		{
			(*itr)->GetPlayer()->SetDataIsLost( true ) ;
		}
	}
}


bool GameServerLogicalTaker::IsPlayerOnline( uint64 playerId )
{
	return GetPlayerPtr( playerId ) != NULL ;
}

Player* GameServerLogicalTaker::GetPlayerPtr( uint64 playerId )
{
	PlayerSessionByPlayerId    &byPlayerId =m_playerSessions.get<TPlayerSessionByPlayerId>() ;
	PlayerSessionByPlayerIdItr itr         =byPlayerId.find( playerId ) ;

	return itr != byPlayerId.end() ? (*itr)->GetPlayerPtr() : NULL ;
}


Player* GameServerLogicalTaker::GetPlayerPtrByName( const std::string &name )
{
	PlayerSessionByPlayerName    &byPlayerName =m_playerSessions.get<TPlayerSessionByPlayerName>() ;
	PlayerSessionByPlayerNameItr itr           =byPlayerName.find( name ) ;

	return itr != byPlayerName.end() ? (*itr)->GetPlayerPtr() : NULL ;
}


bool GameServerLogicalTaker::CheckHasName( const std::string& name )
{
	PlayerSessionByPlayerName    &byPlayerName =m_playerSessions.get<TPlayerSessionByPlayerName>() ;
	PlayerSessionByPlayerNameItr itr           =byPlayerName.find( name ) ;
	return itr != byPlayerName.end() ? true : false ;
}

void GameServerLogicalTaker::GetUpdateTimeInfo(std::string& info)
{
	m_logicSerInfo->WriteToSting(info);
	m_beforUpTime = 0;
	m_endUpTime = 0;
	m_endbeforTime = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Broadcas
void GameServerLogicalTaker::PostPlayerMsg(uint64 playerId, uint16 opcode, const ::google::protobuf::Message& msg)
{
	PlayerSessionByPlayerId    &byPlayerId =m_playerSessions.get<TPlayerSessionByPlayerId>() ;
	PlayerSessionByPlayerIdItr itr         =byPlayerId.find( playerId ) ;
	if( itr != byPlayerId.end() )
	{
		PostInputSessionMsg((*itr),opcode,msg);
	}
}

void GameServerLogicalTaker::PostInputSessionMsg(PlayerSession* pSession,uint16 opcode, const ::google::protobuf::Message& msg)
{
	if (pSession)
	{
		NetPack *pPacket =new NetPack( opcode, msg.ByteSize() + 2 ) ;
		pPacket->SetPacketType(PACKET_TYPE_CLIENT);
		pPacket->SetSessionId(pSession->GetSessionId());
		*pPacket << msg;
		pSession->PostInputPacket(pPacket);
	}
}



void GameServerLogicalTaker::PostPlayerNetPack( uint64 playerId, NetPack *packet )
{
	std::auto_ptr<NetPack> guard( packet );
	PlayerSessionByPlayerId    &byPlayerId =m_playerSessions.get<TPlayerSessionByPlayerId>() ;
	PlayerSessionByPlayerIdItr itr         =byPlayerId.find( playerId ) ;
	if( itr != byPlayerId.end() )
	{
		packet->SetPacketType(PACKET_TYPE_CLIENT);
		packet->SetSessionId( (*itr)->GetSessionId());
		(*itr)->PostInputPacket(guard.release());
	}
}

void GameServerLogicalTaker::BroadcastServerMsg( const int opCode, const ::google::protobuf::Message &msg )
{
	for (PlayerSessionTableItr itr = m_playerSessions.begin(); itr != m_playerSessions.end(); ++itr)
	{
		PostInputSessionMsg((*itr),opCode,msg);
	}
}
void GameServerLogicalTaker::BroadcastMsgToPlayer(uint16 opcode, const ::google::protobuf::Message& msg)
{
	std::for_each( m_playerSessions.begin(), m_playerSessions.end(), boost::bind( &Session::Send, _1, opcode, boost::ref( msg ) ) ) ;
}

void GameServerLogicalTaker::BroadcastMsgToContryPlayers( uint32 countryId,uint16 opcode, const ::google::protobuf::Message& msg )
{
	PlayerSessionByCountryId &byId =m_playerSessions.get<TPlayerSessionByCountryId>() ;
	RangeCountryIdItr rangPlats = byId.equal_range( countryId );
	for (PlayerSessionByCountryIdItr iter = rangPlats.first;  iter != rangPlats.second; ++iter)
	{
		(*iter)->Send( opcode, msg ) ;
	}
}

bool GameServerLogicalTaker::SendMsgToPlayer(uint64 playerId, NetPack &packet)
{
	PlayerSessionByPlayerId    &byPlayerId =m_playerSessions.get<TPlayerSessionByPlayerId>() ;
	PlayerSessionByPlayerIdItr itr         =byPlayerId.find( playerId ) ;
	if( itr != byPlayerId.end() )
	{
		(*itr)->Send(packet) ;
		return true;
	}
	return false;
}

bool GameServerLogicalTaker::SendMsgToPlayer(uint64 playerId, uint16 opcode, const ::google::protobuf::Message& msg)
{
	PlayerSessionByPlayerId    &byPlayerId =m_playerSessions.get<TPlayerSessionByPlayerId>() ;
	PlayerSessionByPlayerIdItr itr         =byPlayerId.find( playerId ) ;
	if( itr != byPlayerId.end() )
	{
		(*itr)->Send( opcode, msg ) ;
		 return true;
	}
	return false;
}

LuaScript* GameServerLogicalTaker::CreateScript( const std::string& scriptName, bool showErr)
{
    if (m_scriptGroup.get())
    {
        return m_scriptGroup->Create(scriptName, showErr);
    }
    return NULL;
}

bool GameServerLogicalTaker::CallLuaFormula( const char* formulaName, float& result, const std::vector<float>& param_vec)
{
    if ( !m_formulaHolder.get())
    {
        return false;
    }
    return m_formulaHolder->Call( formulaName, param_vec, result);
}
