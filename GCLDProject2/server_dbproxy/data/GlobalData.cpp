
//#include<sstream>
//#include <algorithm>
//#include <boost/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/typeof/typeof.hpp>
//#include <boost/smart_ptr.hpp>
//#include <boost/dynamic_bitset.hpp>
#include "GlobalData.h"
//#include "../PlayerData.h"
//#include "Logger.h"
//#include "OS.h"
//#include "def/MmoAssert.h"
//#include "utility/STLUtility.h"
#include "memory_buffer/NetPack.h"
//#include "DbProxyDataMgr.h"
//#include "Enum.pb.h"
#include "InterServer.pb.h"
//#include "Social.pb.h"
//#include "FriendInfo.h"
//#include <boost/regex.hpp>
//#include "PlayerTable.h"
#include "../session/GameServerSession.h"
#include "utility/MsgTool.h"
//#include "utility/STLUtility.h"
#include "ServerOpcode.pb.h"
#include "Palace.pb.h"
//#include "Base.h"
//const time_t MAX_OFFLINETIME_FOR_RCOMMMEND = 604800;   //离线时间小于一周玩家才被推荐加好友
//const size_t FRIEND_COUNT_LIMIT            = 40;


GlobalData::GlobalData()
{

}

GlobalData::~GlobalData()
{
}



void GlobalData::Init()
{
}


void GlobalData::OnGameServerRegist(GameServerSession* pSession)
{
	SyncActivityInfo(pSession);//send activity info
	SyncActivityCountryOccupyCityInfo(pSession);
}

void GlobalData::UnloadGlobalData()
{
	//m_ActivityProtoTable.SaveMod();
	m_ActivityProtoTable.RemoveAllEntry();
	//m_ActivityPlanTable.SaveMod();
	m_ActivityPlanTable.RemoveAllEntry();
	//m_ActivityRewardTable.SaveMod();
	m_ActivityRewardTable.RemoveAllEntry();
	//m_ServerOpenTimeTable.SaveMod()
	m_ServerOpenTimeTable.RemoveAllEntry();
	//m_ActivityCountryOccupyCityTable.SaveMod();
	m_ActivityCountryOccupyCityTable.RemoveAllEntry();
}

//bool   GlobalData::SearchStringForKey(const string& dstStr, const string& strKey)
//{
//	boost::regex expression(strKey);
//	boost::smatch what;
//	if( boost::regex_search(dstStr,what,expression) )
//	{
//		string temp = what[0];
//		return true;
//	}
//	return false;
//}

void GlobalData::UpdateActivityAllInfo( const pb::SC2G_activity_package& info )
{
	for (int i= 0; i< MODIFY_TYPE_COUNT; i++)
	{
		vec_modify[i].clear();
	}
	for(int i = 0;i < info.activity_proto_size();++i)
	{
		const pb::SC2G_activity_proto& ac_proto_info = info.activity_proto(i);
		ActivityProtoTable* ac_proto = m_ActivityProtoTable.GetElement(ac_proto_info.table_id());
		if(ac_proto)
		{
			if(ac_proto->IsModified(ac_proto_info))
			{
				ac_proto->LoadFrom(ac_proto_info);
				vec_modify[MODIFY_TYPE_PROTO][ac_proto_info.table_id()] = true;
				m_ActivityProtoTable.SaveOneMod(ac_proto);
			}
		}
		else
		{
			ActivityProtoTable cell;
			cell.LoadFrom(ac_proto_info);
			vec_modify[MODIFY_TYPE_PROTO][ac_proto_info.table_id()] = true;
			m_ActivityProtoTable.AddAndAddToCache(&cell);
		}
	}
	for(int i = 0;i < info.activity_plan_size();++i)
	{
		const pb::SC2G_activity_plan& ac_plan_info = info.activity_plan(i);
		ActivityPlanTable* ac_plan = m_ActivityPlanTable.GetElement(ac_plan_info.table_id());
		if(ac_plan)
		{
			if(ac_plan->IsModified(ac_plan_info))
			{
				ac_plan->LoadFrom(ac_plan_info);
				vec_modify[MODIFY_TYPE_PLAN][ac_plan_info.table_id()] = true;
				m_ActivityPlanTable.SaveOneMod(ac_plan);
			}
		}
		else
		{
			ActivityPlanTable cell;
			cell.LoadFrom(ac_plan_info);
			vec_modify[MODIFY_TYPE_PLAN][ac_plan_info.table_id()] = true;
			m_ActivityPlanTable.AddAndAddToCache(&cell);
		}
	}
	for(int i = 0;i < info.server_open_time_size();++i)
	{
		const pb::SC2G_server_open_time& server_open_time = info.server_open_time(i);
		ServerOpenTimeTable* ser_op_time = m_ServerOpenTimeTable.GetElement(server_open_time.table_id());
		if(ser_op_time)
		{
			if(ser_op_time->IsModified(server_open_time))
			{
				ser_op_time->LoadFrom(server_open_time);
				vec_modify[MODIFY_TYPE_OPEN_TIME][server_open_time.table_id()] = true;
				m_ServerOpenTimeTable.SaveOneMod(ser_op_time);
			}
		}
		else
		{
			ServerOpenTimeTable cell;
			cell.LoadFrom(server_open_time);
			vec_modify[MODIFY_TYPE_OPEN_TIME][server_open_time.table_id()] = true;
			m_ServerOpenTimeTable.AddAndAddToCache(&cell);
		}
	}
	for(int i = 0;i < info.activity_reward_size();++i)
	{
		const pb::SC2G_activity_reward& ac_reward_info = info.activity_reward(i);
		ActivityRewardTable* ac_reward = m_ActivityRewardTable.GetElement(ac_reward_info.table_id());
		if(ac_reward)
		{
			if(ac_reward->IsModified(ac_reward_info))
			{
				ac_reward->LoadFrom(ac_reward_info);
				vec_modify[MODIFY_TYPE_REWARD][ac_reward_info.table_id()] = true;
				m_ActivityRewardTable.SaveOneMod(ac_reward);
			}
		}
		else
		{
			ActivityRewardTable cell;
			cell.LoadFrom(ac_reward_info);
			vec_modify[MODIFY_TYPE_REWARD][ac_reward_info.table_id()] = true;
			m_ActivityRewardTable.AddAndAddToCache(&cell);
		}
	}
}

void GlobalData::SyncActivityInfo(GameServerSession* pSession)
{
	if (pSession)
	{
		pb::SC2G_activity_package msg;
		CachePtrMap<uint64, ActivityProtoTable>::Iter it1 = m_ActivityProtoTable.Begin();
		for (; it1 != m_ActivityProtoTable.End(); ++it1)
		{
			ActivityProtoTable* table = it1->second;
			if(table)
				table->SaveTo(*msg.add_activity_proto());
		}
		CachePtrMap<uint64, ActivityPlanTable>::Iter it2 = m_ActivityPlanTable.Begin();
		for (; it2 != m_ActivityPlanTable.End(); ++it2)
		{
			ActivityPlanTable* table = it2->second;
			if(table)
				table->SaveTo(*msg.add_activity_plan());
		}
		CachePtrMap<uint64, ActivityRewardTable>::Iter it4 = m_ActivityRewardTable.Begin();
		for (; it4 != m_ActivityRewardTable.End(); ++it4)
		{
			ActivityRewardTable* table = it4->second;
			if(table)
				table->SaveTo(*msg.add_activity_reward());
		}
		CachePtrMap<uint64, ServerOpenTimeTable>::Iter it3 = m_ServerOpenTimeTable.Begin();
		for (; it3 != m_ServerOpenTimeTable.End(); ++it3)
		{
			ServerOpenTimeTable* table = it3->second;
			if(table)
				table->SaveTo(*msg.add_server_open_time());
		}
		pSession->Send(pb::SD2G_LOAD_ACTIIVTY_INFO,msg);
	}
}

void GlobalData::SyncActivityCountryOccupyCityInfo(GameServerSession* pSession)
{
	if (pSession)
	{
		pb::SD2G_Activity_Occupy_City_Num msg;
		CachePtrMap<uint64, ActivityCountryOccupyCityTable>::Iter it1 = m_ActivityCountryOccupyCityTable.Begin();
		for (; it1 != m_ActivityCountryOccupyCityTable.End(); ++it1)
		{
			ActivityCountryOccupyCityTable* table = it1->second;
			if(table)
				table->SaveTo(*msg.add_info());
		}
		pSession->Send(pb::SD2G_ACTIVITY_COUNTRY_OCCUPY_CITY,msg);
	}
}
