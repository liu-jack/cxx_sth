
/*
*Add BY:zhoulunhao 
*CREATE TIME:2017.9.27
*MAIL:zhoulunhao@hotmail.com
*/
#include "ContinueOccupyCityLog.h"
#include "Table/ContinueOccupyCityMgr.h"
#include "Fuben.pb.h"
#include "../object/Player.h"
#include "OS.h"
#include "CrossLogic/PlayerContinueOccupyCityDB.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Opcode.pb.h"
#include "Social.pb.h"
#include "game_data/DbStringMgr.h"
#include "chat/ChatSpeaker.h"
#include "SysEvent/SysEventMgr.h"
#include "def/ConstDef.h"
 
#define ONE_MINUTES_SECOND	(GET_BASE_DEF_UINT(pb::BD_SERIAL_COUNTRY_ADD_TIME))
#define FIVE_MINUTES_SECOND  (GET_BASE_DEF_UINT(pb::BD_SERIAL_COUNTRY_MAX_TIME))
ContinueOccupyCityLog::ContinueOccupyCityLog()
{
	queue_of_reward_ = sContinueOccupyCityTableMgr.getQueue();
}

void ContinueOccupyCityLog::ActiveConitinOccupyCity(Player& player,const time_t active_time)
{
	log_of_continue_occupy_city_[active_time].finish_time_ =  FIVE_MINUTES_SECOND + (uint64)active_time;
	log_of_continue_occupy_city_[active_time].n_contin_city_ = 1;
	ContinueOccupyCityDB::SendInfoToDb(player,*this);
}

bool ContinueOccupyCityLog::IsInContinueOccupyCity()
{
	if(log_of_continue_occupy_city_.empty())  return false;
	LogOfContinueOccupyCityMap::reverse_iterator iter = log_of_continue_occupy_city_.rbegin();
	return sOS.TimeSeconds() <= iter->second.finish_time_;
}

void ContinueOccupyCityLog::UpdateCityNum(Player& player,int value)
{
	if(log_of_continue_occupy_city_.empty())  return;
	LogOfContinueOccupyCityMap::reverse_iterator iter = log_of_continue_occupy_city_.rbegin();
	uint32 diff_time = static_cast<uint32>(iter->second.finish_time_ - sOS.TimeSeconds());
	iter->second.finish_time_  = MIN(FIVE_MINUTES_SECOND,ONE_MINUTES_SECOND + diff_time) + sOS.TimeSeconds();
	iter->second.n_contin_city_ += value;
	pb::GS2C_ContinueOccupyCitySuccess msg;
	msg.set_con_occupy_citys(iter->second.n_contin_city_);
	ContinueOccupyCityDB::SendInfoToDb(player,*this);
	if(!queue_of_reward_.empty())
	{
		if(iter->second.n_contin_city_ >= queue_of_reward_.front().first)
		{
			if(queue_of_reward_.front().second != 0)
			{
				msg.set_exploit_value(queue_of_reward_.front().second);
				player.AddExploitValue(queue_of_reward_.front().second);
#ifdef _MMO_SERVER_
				string items;
				sSysEventMgr.SendJustTipsMail(player.GetGuid(),EVENT_TYPE_CONTIN_OCCUPY_CITY,items,queue_of_reward_.front().first,queue_of_reward_.front().second);
#endif
			}
			queue_of_reward_.pop_front();
		}
		else
		{
			msg.set_need_to_occupy_city(queue_of_reward_.front().first - iter->second.n_contin_city_);
		}
	}
	player.Send(pb::SMSG_CONTINUE_CITY_SUCCESS,msg);
	if(iter->second.n_contin_city_ >= GET_BASE_DEF_UINT(pb::BD_SERIAL_COUNTRY_BROADCAST_CONDITION1))
	{
		uint32 diffValue = iter->second.n_contin_city_ - GET_BASE_DEF_UINT(pb::BD_SERIAL_COUNTRY_BROADCAST_CONDITION1);
		if((diffValue % GET_BASE_DEF_UINT(pb::BD_SERIAL_COUNTRY_BROADCAST_CONDITION2)) == 0)
		{
			pb::C2GS_Chat msg;
			char temp[1024] = {0},buf[1024] = {0};
			CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_CONTINUE_OCCUPY_CITY,temp,LANG_CN);
			SPRINTF(buf,temp,player.CharName().c_str(),(int)iter->second.n_contin_city_);
			msg.set_type(pb::CHAT_SYSTEM);
			msg.set_chat_info(buf);
			player.m_chatSpeaker->Say(&player,msg);
		}
	}
}


void ContinueOccupyCityLog::SaveTo(pb::GS2C_ContinueOccupyCity& msg)
{
	if(log_of_continue_occupy_city_.empty())  return;
	LogOfContinueOccupyCityMap::reverse_iterator iter = log_of_continue_occupy_city_.rbegin();
	pb::BaseContinueMsg* info = msg.mutable_info();
	info->set_finish_time(iter->second.finish_time_);
	info->set_active_time(iter->first);
	info->set_con_occupy_citys(iter->second.n_contin_city_);
}

void ContinueOccupyCityLog::SaveTo(pb::BaseContinueMsg& msg)
{
	if(log_of_continue_occupy_city_.empty())  return;
	LogOfContinueOccupyCityMap::reverse_iterator iter = log_of_continue_occupy_city_.rbegin();
	msg.set_active_time(iter->first);
	msg.set_con_occupy_citys(iter->second.n_contin_city_);
	msg.set_finish_time(iter->second.finish_time_);
}

void ContinueOccupyCityLog::LoadFrom(const pb::GxDB_ContinueOccupyCityInfo& msg)
{
	for(int i = 0;i < msg.info_size();++i)
	{
		const pb::BaseContinueMsg& info = msg.info(i);
		log_of_continue_occupy_city_[info.active_time()] = LogOfContinueOccupyCity(info.finish_time(),info.con_occupy_citys());
	}
}

void ContinueOccupyCityLog::ResetLogMap(Player& player)
{
	uint32 today_max_value = getMaxContinueOccupyCity();
	//player.setValueInt(pb::);
	if(!log_of_continue_occupy_city_.empty())
	{
		//ContinueOccupyCityDB::SendInfoToDb(player,*this);
		log_of_continue_occupy_city_.clear();
	}
	queue_of_reward_ = sContinueOccupyCityTableMgr.getQueue();
	player.ResetContinueOccupyCityNum();
}

uint32 ContinueOccupyCityLog::getMaxContinueOccupyCity()
{
	uint32 max_value = 0;
	for(LogOfContinueOccupyCityMap::iterator iter = log_of_continue_occupy_city_.begin();iter != log_of_continue_occupy_city_.end();++iter)
	{
		if(iter->second.n_contin_city_ > max_value)
			max_value = iter->second.n_contin_city_;
	}
	return max_value;
}
