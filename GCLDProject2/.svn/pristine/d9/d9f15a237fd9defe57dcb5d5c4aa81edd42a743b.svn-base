/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/


#include "Congratulate/CongratulateRewardLog.h"
#include "Congratulate/Table/CongratulateTableMgr.h"
#include "Rank.pb.h"
#include "session/PlayerPool.h"
#include "../object/Player.h"
#include "OS.h"
#include "AutomaticUpdate/AutoUpdateGame.h"
#include "CrossLogic/PlayerCongratulateDB.h"
#include "Opcode.pb.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
CongratulateLog::CongratulateLog()
{
// 	const CongratulateConfigMap& congratulate_config_map = sCongratulateMgr.getCongratulateConfigMap();
// 	for(CongratulateConfigMap::const_iterator  iter = congratulate_config_map.begin();iter != congratulate_config_map.end();++iter)
// 	{
// 		congratulate_struct_map_[iter->first] = CongratulateStruct();
// 	}
}

bool CongratulateLog::isInvalidActiveTimeInStructMap(const uint64 active_time)
{
	CongratulateStructMap::iterator iter = congratulate_struct_map_.find(active_time);
	if(iter == congratulate_struct_map_.end())
	{
		return false;
	}
	return true;
}	

bool CongratulateLog::isInvalidActiveTimeInBeenStructMap(const uint64 active_time )
{
	CongratulateBeenStructMap::iterator iter = congratulate_been_struct_map_.find(active_time);
	if(iter == congratulate_been_struct_map_.end())
	{
		return true;
	}
	return false;
}

void CongratulateLog::SaveTo(const uint64 active_time,pb::GxDB_CongratulateInfo& msg)
{
	if(!isInvalidActiveTimeInStructMap(active_time)) return;
	CongratulateStruct& ref = congratulate_struct_map_[active_time];
	msg.set_table_id(ref.table_id_);
	msg.set_is_achieve(ref.is_achieve_);
	//msg.set_is_reward(ref.is_reward_);
	msg.set_been_congratulated_count(ref.been_congratulated_count_);
	msg.set_active_time(active_time);
}


void CongratulateLog::SaveTo(const uint64 active_time,pb::C2GS_Congratulate_Accept& msg)
{
	if(isInvalidActiveTimeInBeenStructMap(active_time)) return;
	CongratulateBeenStruct& ref = congratulate_been_struct_map_[active_time];
	msg.set_table_id(ref.table_id_);
	msg.set_rank(ref.rank_);
	msg.set_suit_id(ref.suit_id_);
	msg.set_to_player_id(ref.to_player_id_);
	msg.set_congratulate_type(ref.congratulate_type_);
	msg.set_active_time(active_time);
}

void CongratulateLog::SaveTo(pb::GS2C_ListOfCurrentCanCongratulate& msg)
{
	if(congratulate_been_struct_map_.empty()) return;
	CongratulateBeenStructMap::iterator iter = congratulate_been_struct_map_.begin();
	pb::GS2C_Achieve_Congratulate *info = msg.mutable_info();
	SaveTo(iter->first,*info->mutable_info());
	Player* to_player = sPlayerPool.GetByPlayerId(iter->second.to_player_id_);
	if(to_player)
	{
		info->set_to_player_name(to_player->CharName());
	}
}

void CongratulateLog::SaveTo(const CongratulateStruct& ref,pb::GxDB_CongratulateInfo& msg)
{
	msg.set_been_congratulated_count(ref.been_congratulated_count_);
	msg.set_is_achieve(ref.is_achieve_);
	msg.set_table_id(ref.table_id_);
}

void CongratulateLog::LoadFrom(const pb::GxDB_CongratulateRewardLog& msg)
{
	for(int i = 0;i < msg.info_size();++i)
	{
		const pb::GxDB_CongratulateInfo& info = msg.info(i);
		congratulate_struct_map_[info.active_time()] = CongratulateStruct(info.is_achieve(),info.been_congratulated_count(),info.table_id());
	}
}


void CongratulateLog::LoadFrom(const pb::GxDB_CongratulateBeenCongratualted& msg)
{
	for(int i = 0;i < msg.info_size();++i)
	{
		const pb::C2GS_Congratulate_Accept& info = msg.info(i);
		LoadFrom(info);
	}
}

void CongratulateLog::LoadFrom(const pb::C2GS_Congratulate_Accept& msg)
{
	congratulate_been_struct_map_[msg.active_time()] = CongratulateBeenStruct(msg.to_player_id(),msg.rank(),msg.suit_id(),msg.congratulate_type(),msg.table_id());
}

void CongratulateLog::EraseFromCongratulateBeenMap(const uint32 rank,const uint32 table_id)
{
	for(CongratulateBeenStructMap::iterator iter = congratulate_been_struct_map_.begin();iter != congratulate_been_struct_map_.end();++iter)
	{
		if(iter->second.table_id_ == table_id && iter->second.rank_ == rank)
		{
			congratulate_been_struct_map_.erase(iter);
			return;
		}
	}
}

void CongratulateLog::SetAchieve(const uint64 activeTime,const uint32 table_id)
{
	CongratulateStruct& ref = congratulate_struct_map_[activeTime];
	ref.table_id_ = table_id;
	ref.is_achieve_ =  1;
}

// void CongratulateLog::SetTakeReward(const uint32 table_id)
// {
// 	for(CongratulateStructMap::iterator iter = congratulate_struct_map_.begin();iter != congratulate_struct_map_.end();++iter)
// 	{
// 		if(iter->second.table_id_ == table_id && iter->second.is_achieve_)
// 		{
// 			iter->second.is_reward_ =  1;
// 		}
// 	}
// }

void CongratulateLog::AddBeenCongratulated(Player& player,const uint32 table_id)
{
	for(CongratulateStructMap::iterator iter = congratulate_struct_map_.begin();iter != congratulate_struct_map_.end();++iter)
	{
		if(iter->second.table_id_ == table_id)
		{
			iter->second.been_congratulated_count_++;
			pb::GxDB_CongratulateInfo msg;
			msg.set_active_time(iter->first);
			msg.set_operate_type(pb::DATA_UPDATE);
			SaveTo(iter->second,msg);
			sDbProxyClient.SendPlayerPack(player, pb::SG2D_CONGRATULATE_UPDATE, msg);
			return;
		}
	}
}

CongratulateStructMap& CongratulateLog::getCongratulateStructMap()
{
	return congratulate_struct_map_;
}

uint64 CongratulateLog::GetToPlayerId(const pb::C2GS_Congratulate_Accept& msg) const
{
	for(CongratulateBeenStructMap::const_iterator iter = congratulate_been_struct_map_.begin();iter != congratulate_been_struct_map_.end();++iter)
	{
		if(iter->second.table_id_ == msg.table_id() && iter->second.rank_ == msg.rank())
		{
			return iter->second.to_player_id_;
		}
	}
	return 0;
}

void CongratulateLog::ResetCongratulateData(Player& player)
{
	player.SetValueInt(pb::PLAYER_FIELD_TODAY_CONGRATULATE_TIMES,0);
}

void CongratulateLog::ClearCongratulateBeenStructMap(Player& player,const uint64 act_time)
{
	bool Need_update = false;
	const uint64 timenow = sOS.TimeSeconds();
	CongratulateBeenStructMap::const_iterator iter = congratulate_been_struct_map_.find(act_time);
	if(iter != congratulate_been_struct_map_.end())
	{
		if((timenow - iter->first) >= ONE_DAY_SECOND)
		{
			pb::C2GS_Congratulate_Accept msg;
			msg.set_operate_type(pb::DATA_DEL);
			SaveTo(iter->first,msg);
			CongratulateLogDB::SendInfoToCongratulateBeenDb(player,msg);
			congratulate_been_struct_map_.erase(iter++);
			Need_update = true;
		}
	}
	if(Need_update)
	{
		pb::GS2C_ListOfCurrentCanCongratulate msg;
		SaveTo(msg);
		player.Send(pb::SMSG_CONGRATULATE_CURRENT_CONGRATULATION_RSP,msg);
	}
	if(iter != congratulate_been_struct_map_.end())
	{
#ifdef _MMO_SERVER_
		sAutoUpdateGame.AddEvent(boost::bind(&Player::When24HoursToPlayerPasted, &player,iter->first),(U32)(ONE_DAY_SECOND - (timenow - iter->first)));
#endif
	}
}

const uint64 CongratulateLog::GetCongratulateStructMapBeginTime() const
{
	if(congratulate_struct_map_.empty())
	{
		return 0;
	}
	return congratulate_struct_map_.begin()->first;
}

const uint64 CongratulateLog::GetCongratulateBeenStructMapBeginTime() const
{
	if(congratulate_been_struct_map_.empty())
	{
		return 0;
	}
	return congratulate_been_struct_map_.begin()->first;
}
