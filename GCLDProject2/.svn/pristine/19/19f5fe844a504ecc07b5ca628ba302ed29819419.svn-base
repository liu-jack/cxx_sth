#include "Player.h"
#include "memory_buffer/NetPack.h"
#include "Logger.h"


#include "Opcode.pb.h"
#include "Rank.pb.h"

#include "../server_client/DbProxyServerClient.h"
#include "../Map/WorldMap.h"
#include "../Map/country/country.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "ServerOpcode.pb.h"
#include "Login.pb.h"
#include "Enum.pb.h"
#include "../quest/QuestLogger.h"
#include "ActivityControl/ActivityMgr.h"
#include "Activity.pb.h"
#include "utility/TimeParser.h"
#include "object/PlayerDataManager.h"
#include "GameServer.h"
#include "GameServerLogicalTaker.h"
#include "reward/reward.h"
#include "Congratulate/Table/CongratulateTableMgr.h"
#include "Congratulate/CongratulateRewardLog.h"
#include "CrossLogic/PlayerCongratulateDB.h"
#include "session/PlayerPool.h"
#include "AutomaticUpdate/AutoUpdateGame.h"
#include "OS.h"
#include "DailyLottery/Table/DailyLotteryTableMgr.h"
#include "WannaBeStronger/WannaBeStrongerLog.h"
using namespace pb;



Player::HandlerArray& Player::PackHandlers()
{
    static HandlerArray s_handlers(CSMSG_COUNT);
    return s_handlers;
}

bool Player::HandlePack( NetPack& pack )
{
    HandlerArray& handlers = PackHandlers();
    pb::Opcode opCode = static_cast< pb::Opcode >( pack.GetOpcode() ) ;

#ifdef _MMO_SERVER_
	if (IsInCrossWar())
	{
		LLOG("[1002] Get a Cross Msg,  opcode=%d",(int)opCode);
		if(sPlayerDataMgr.CheckClientCrossWhiteList(opCode))	//符合白名单，则允许发送
		{
			LLOG("[1002] send to cross server");
			return SendCrossServerNetPack(pack);
		}
		else if(sPlayerDataMgr.CheckClientGameWhiteList(opCode))
		{
			LLOG("[1002] handle in local");
		}
		else
		{
			LLOG("[1002] a opcode not allowed to pass, code=%d",opCode);
			return false;
		}
	}
#endif

    if (handlers[opCode] == NULL)
    {
        ELOG ("Unhandled pack from %s, opName= [ %s ( %d ) ]", pack.GetPacketTypeName(), pack.GetOpcodeName(), opCode );
        return false;
    }

    (this->*handlers[opCode])(pack);
    return true;
}

#undef HandleOpcode
#define HandleOpcode(opCode) handlers[opCode] = &Player::Handle_##opCode;

void Player::RegisteAllHandlers()
{
    HandlerArray& handlers = PackHandlers();

    #include "PlayerNetPackOpcodes.h"
}

bool Player::IsCanCrossWarState()
{
	//TODO test
	//return true;

	if (m_pCountry != NULL)
	{
		return m_pCountry->IsCanCrossWarState();
	}
	else
	{
		m_pCountry = sWorldMap.GetCountry(GetCountryId());
		if (m_pCountry)
		{
			return	m_pCountry->IsCanCrossWarState();
		}
	}
	return false;
}

bool Player::SendCrossServerNetPack( NetPack& pack )
{
	if (m_pCountry != NULL)
	{
		pack.SetSessionId(GetGuid());
		return m_pCountry->SendCrossServerNetPack(pack);
	}
	else
	{
		m_pCountry = sWorldMap.GetCountry(GetCountryId());
		if (m_pCountry)
		{
			pack.SetSessionId(GetGuid());
			return	m_pCountry->SendCrossServerNetPack(pack);
		}
	}
	return false;
}

bool Player::SendCrossServerMsg( const int opCode, const ::google::protobuf::Message &msg,char clientType,uint64 sessionId /*= 0*/ )
{
	if (m_pCountry != NULL)
	{
		return m_pCountry->SendCrossServerMsg(opCode,msg,clientType,sessionId);
	}
	else
	{
		m_pCountry = sWorldMap.GetCountry(GetCountryId());
		if (m_pCountry)
		{
			return	m_pCountry->SendCrossServerMsg(opCode,msg,clientType,sessionId);
		}
	}
	return false;
}


OpHandler( CMSG_PING)
{

}

OpHandler( CMSG_PLAYER_CHANGE_NAME)
{
    static const uint32 changeNameCashGold = GET_BASE_DEF_UINT( pb::BD_PLAYER_CHANGE_NAME );
    pb::MSG_ChangeName msg; 
    pack >> msg;

    if ( !EnoughGold(changeNameCashGold))
    {
        msg.set_err_code( pb::ErrNotEnoughCurr);
        Send( SMSG_PLAYER_CHANGE_NAME_RESP, msg);
        return;
    }
	if(CheckHasName(msg.name()))
	{
		 msg.set_err_code( pb::ErrNameAlreadyExist ) ;
		 Send( SMSG_PLAYER_CHANGE_NAME_RESP, msg);
		 return;
	}
    msg.set_player_id( GetGuid());  
    sDbProxyClient.Send( pb::SG2D_PLAYER_CHANGE_NAME, SessionId(), msg ) ; 

}

OpHandler( CMSG_SET_NAME_AND_HEADID)
{
	pb::MSG_SetNameAndHeadId msg;
	pack >> msg;
	if(CheckHasName(msg.name()))
	{
		msg.set_err_code( pb::ErrNameAlreadyExist ) ;
		Send( SMSG_SET_NAME_AND_HEADID_RSP, msg);
		return;
	}
	msg.set_player_id(GetGuid());
	sDbProxyClient.Send(pb::SG2D_PLAYER_SET_NAME_AND_HEADID,SessionId(),msg);
}

#ifdef _MMO_SERVER_
OpHandler(CMSG_ACTIIVTY_START_ALL_INFO)
{
	pb::GS2C_Activity_All_Active_Info msg;
	const ActivityPack& ac_pack = sActivityMgr.GetActivityPack();
	for(std::map<uint64,cur_activity>::const_iterator iter = ac_pack.cur_activity_map_.begin();
		iter != ac_pack.cur_activity_map_.end();++iter)
	{
		pb::GS2C_Activity_Start* info = msg.add_info();
		info->set_activity_type(iter->second.activity_id_);
		info->set_open_way(iter->second.open_way_);
		std::pair<time_t,time_t> pair_time;
		sActivityMgr.GetActivityStartAndEndTime(iter,pair_time);
		info->set_activity_start_time(pair_time.first);
		info->set_activity_last_time(pair_time.second);
		info->set_activity_show_time(iter->second.last_show_date_);
		info->set_server_open_time(sActivityMgr.GetOpenTimeById(sActivityMgr.GetCurServerId()));
	}
	Send(pb::SMSG_ACTIVITY_START_ALL_INFO_RSP,msg);
}
#endif
OpHandler( CMSG_CONGRATULATE_ACCEPT)
{
	pb::C2GS_Congratulate_Accept msg;
	pack >> msg;
	pb::GS2C_Congratulate_Accept_Rsp sendmsg;
	uint64 to_player_id = 0;

	if(GetAttrUint(pb::PLAYER_FIELD_TODAY_CONGRATULATE_TIMES) < GET_BASE_DEF_UINT(pb::BD_CONGRATULATE_MAX_TIME))
	{
		to_player_id = m_CongratulateLog->GetToPlayerId(msg);
		const IntPairVec* vec = sCongratulateMgr.GetCongratulateReward(msg.table_id());
		if(vec)
		{
			sReward.Change(*this,*vec);
			for(IntPairVec::const_iterator iter = vec->begin();iter != vec->end();++iter)
			{
				pb::StReward * reward = sendmsg.add_reward();
				reward->set_type(iter->first);
				reward->set_value(iter->second);
			}
		}
		AddValue(pb::PLAYER_FIELD_TODAY_CONGRATULATE_TIMES,1);
		m_CongratulateLog->EraseFromCongratulateBeenMap(msg.rank(),msg.table_id());
		msg.set_operate_type(pb::DATA_DEL);
		CongratulateLogDB::SendInfoToCongratulateBeenDb(*this,msg);
		PlayerPtr to_player = sPlayerPool.GetPtrByPlayerId(to_player_id);
		if(to_player)
		{
			to_player->m_CongratulateLog->AddBeenCongratulated(*to_player,msg.table_id());
		}
	}
	Send(pb::SMSG_CONGRATULATE_ACCEPT_RSP,sendmsg);
}
OpHandler( CMSG_CONGRATULATE_CURRENT_CONGRATULATION)
{
	pb::GS2C_ListOfCurrentCanCongratulate msg;
	m_CongratulateLog->SaveTo(msg);
	Send(pb::SMSG_CONGRATULATE_CURRENT_CONGRATULATION_RSP,msg);
}

OpHandler( CMSG_SET_GUIDE_PROCESS)
{
    int process;
    pack >> process;

    SetGuideProcess( process);
}
OpHandler( CMSG_LOTTERY_DAILY)
{
	pb::GS2C_Lottery_Daily_Reward msg;
	if(GetLevel() >=GET_BASE_DEF_INT(pb::BD_DAILY_LOGIN_OPEN_LEVEL))
	{
		if(!TestFlag(pb::PLAYER_LOTTERY_DAILY_DONE))
		{
			SetFlag(pb::PLAYER_LOTTERY_DAILY_DONE);
			msg.add_table_id(sDailyLotteryTableMgr.GetTableId(TYPE_FIRST));
			msg.add_table_id(sDailyLotteryTableMgr.GetTableId(TYPE_SECOND));
			msg.add_table_id(sDailyLotteryTableMgr.GetTableId(TYPE_THIRD));
			sDailyLotteryTableMgr.GiveReward(*this,msg.table_id(0),msg.table_id(1),msg.table_id(2));
		}
	}
	Send(pb::SMSG_LOTTERY_DAILY_RSP,msg);
}

////////////Wanna be stronger
OpHandler( CMSG_WANNA_BE_STRONGER_ALL_INFO)
{
	pb::GS2C_WannaBeStrongerAllInfo msg;
	m_WannaBeStrongerLog->SaveTo(*this,msg);
	Send(pb::SMSG_WANNA_BE_STRONGER_ALL_INFO_RSP,msg);
}
OpHandler( CMSG_WANNA_BE_STRONGER_TAKE_REWARD)
{
	pb::C2GS_WannaBeStrongerReward msg; pack >> msg;
	pb::GS2C_WannaBeStrongerRewardRes sendmsg;
	m_WannaBeStrongerLog->TakeRewards(*this,msg.group_id(),sendmsg);
}
///////////////