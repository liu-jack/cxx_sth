/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "../object/Player.h"
#include "reward/reward.h"
#include "Activity.pb.h"
#include "Table/ActivityCelebrityTableMgr.h"
#include "Opcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "utility/Utility.h"
#include "Map/Table/MapTableMgr.h"
#include "Map/PlayerMapLogic.h"
#include "ActivityControl/ActivityMgr.h"
#include "OS.h"
using namespace pb;
/*
when city id is ZERO,it means there is no event happened.
*/
#ifdef _MMO_SERVER_
OpHandler(CMSG_CELECRITY_ALL_INFO)
{
	GS2C_Celebrity_All_Info sendmsg;
	uint32 country_id = GetCountryId();
	infomation* info = sActivityCelebrityMgr.getInfomation(country_id);
	if(info)
	{
		sendmsg.set_city_id(info->city_id);
		if(info->city_id == 0 && info->next_fresh_time < sOS.TimeSeconds())
		{
			sendmsg.set_next_fresh_time(info->next_fresh_time + sActivityCelebrityMgr.GetNpcFreshTime(country_id) / 1000);
		}
		else
		{
			sendmsg.set_next_fresh_time(info->next_fresh_time);
		}
		sendmsg.set_item_id(info->item_id);
	}
	pb::CelebrityEvent * ce_event = sendmsg.mutable_event_info();
	if(m_worldMapLogic)
	{
		const MapEvent* event = sMapTableMgr.getCelebrityMapEvent(m_worldMapLogic->m_CelebrityCityEventLst.first);
		if(event)
		{
			m_worldMapLogic->PackEventInfo(*event,*ce_event);
		}
		ce_event->set_event_id(m_worldMapLogic->m_CelebrityCityEventLst.first);
		ce_event->set_city_id(m_worldMapLogic->m_CelebrityCityEventLst.second.cityId);
		ce_event->set_next_fresh_time(m_worldMapLogic->m_CelebrityCityEventLst.second.endTime);
		ce_event->set_daily_times(m_worldMapLogic->m_CelebrityCityEventLst.second.dailyTimes);
	}
	Send(pb::SMSG_CELEBRITY_ALL_INFO_RSP,sendmsg);
}
OpHandler(CMSG_CELEBRITY_TAKE_REWARD)
{
	sActivityMgr.MarkTakeInActivity(pb::ACTIVITY_MAKE_FRIENDS,GetGuid());
	pb::C2GS_Celebrity_Take_Reward msg; pack >> msg;
	uint32 Id = msg.id();
	pb::GS2C_Celebrity_Take_Reward_Rsp sendmsg;
	uint32 curRenown = GetCurrency(eRenown);
	const ActivityVisitFamousReward* reward = sActivityCelebrityMgr.getActivityVisitFamousReward(Id);
	sendmsg.set_id(Id);
	IntPairVec vec;
	if(!IsTakeVisitFamousReward(Id))
	{
		sendmsg.set_ret(2);//已经领过
		Send(pb::SMSG_CELEBRITY_TAKE_REWARD_RSP,sendmsg);
		return;
	}
	if(reward)
	{
		if(curRenown < reward->NeedRenown())
		{
			sendmsg.set_ret(1);
			Send(pb::SMSG_CELEBRITY_TAKE_REWARD_RSP,sendmsg);
			return;
		}
		TakeVisitFamousReward(Id);
		Utility::SplitStr2(reward->Reward(),vec);
		for(IntPairVec::iterator iter = vec.begin();iter != vec.end();++iter)
		{
			pb::StReward *reward_ = sendmsg.add_reward();
			reward_->set_type(iter->first);
			reward_->set_value(iter->second);
		}
		sReward.Change(*this,vec);
		sendmsg.set_ret(0);
	}
	else
	{
		sendmsg.set_ret(1);
	}
	Send(pb::SMSG_CELEBRITY_TAKE_REWARD_RSP,sendmsg);
}

#endif