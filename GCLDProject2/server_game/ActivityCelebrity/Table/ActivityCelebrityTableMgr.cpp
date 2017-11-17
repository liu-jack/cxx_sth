/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "ActivityCelebrityTableMgr.h"
#include "DbTool.h"
#include "System.h"
#include "utility/Utility.h"

ActivityVisitFamousReward::ActivityVisitFamousReward(const DB_ActivityVisitFamousReward& db)
:db_activity_visit_famous_reward_(db)
{

}

uint32 ActivityVisitFamousReward::Id() const
{
	return db_activity_visit_famous_reward_.id;
}

uint32 ActivityVisitFamousReward::NeedRenown() const
{
	return db_activity_visit_famous_reward_.need_renown;
}

string ActivityVisitFamousReward::Reward() const
{
	return db_activity_visit_famous_reward_.reward_list;
}

ActivityVisitFamousNpc::ActivityVisitFamousNpc(const DB_ActivityVisitFamousNpc& db)
:db_activity_visit_famous_npc_(db)
{

}

uint32 ActivityVisitFamousNpc::Id() const
{
	return db_activity_visit_famous_npc_.id;
}

string ActivityVisitFamousNpc::NpcList() const
{
	return db_activity_visit_famous_npc_.reward_list;
}

uint32 ActivityVisitFamousNpc::CountryId() const
{
	return db_activity_visit_famous_npc_.country_id;
}

uint32 ActivityVisitFamousNpc::Weight() const
{
	return db_activity_visit_famous_npc_.weight;
}

uint32 ActivityVisitFamousNpc::ArmyHZ() const
{
	return db_activity_visit_famous_npc_.amy_hz;
}

ActivityVisitFamousEvent::ActivityVisitFamousEvent(const DB_ActivityVisitFamousEvent& db)
:db_activity_visit_famous_event_(db)
{
	
}

uint32 ActivityVisitFamousEvent::Id() const
{
	return db_activity_visit_famous_event_.id;
}

uint32 ActivityVisitFamousEvent::EventId() const
{
	return db_activity_visit_famous_event_.event_id;
}

uint32 ActivityVisitFamousEvent::Weight() const
{
	return db_activity_visit_famous_event_.weight;
}

string ActivityVisitFamousEvent::LeftImage() const
{
	return db_activity_visit_famous_event_.left_image;
}

uint32 ActivityVisitFamousEvent::ImageNameId() const
{
	return db_activity_visit_famous_event_.image_name_id;
}

ActivityCelebrityMgr::ActivityCelebrityMgr()
{
	::memset(&celebrity_info,0,sizeof(celebrity_info));
}

void ActivityCelebrityMgr::Init()
{
	FOREACH_DB_ITEM(ptr1,DB_ActivityVisitFamousReward)
	{
		ActivityVisitFamousReward * reward = new ActivityVisitFamousReward(*ptr1);
		activity_visit_famous_reward_map_[ptr1->id] = reward;
	}
	FOREACH_DB_ITEM(ptr2,DB_ActivityVisitFamousNpc)
	{
		ActivityVisitFamousNpc * npc = new ActivityVisitFamousNpc(*ptr2);
		activity_visit_famous_npc_map_[ptr2->id] = npc;
		if(npc->CountryId() == pb::COUNTRY_WORM)
		{
			npc_worm_.push_back(std::make_pair(npc->Id(),npc->Weight()));
		}
		if(npc->CountryId() == pb::COUNTRY_MAN)
		{
			npc_human_.push_back(std::make_pair(npc->Id(),npc->Weight()));
		}
		if(npc->CountryId() == pb::COUNTRY_GOD)
		{
			npc_god_.push_back(std::make_pair(npc->Id(),npc->Weight()));
		}
	}
	FOREACH_DB_ITEM(ptr3,DB_ActivityVisitFamousEvent)
	{
		ActivityVisitFamousEvent * event = new ActivityVisitFamousEvent(*ptr3);
		activity_visit_famous_event_map_[ptr3->id] = event;
		event_vec_.push_back(std::make_pair(event->Id(),event->Weight()));
	}
}


const ActivityVisitFamousReward* ActivityCelebrityMgr::getActivityVisitFamousReward(const uint32 Table_Id)
{
	return activity_visit_famous_reward_map_.pfind(Table_Id);
}

const ActivityVisitFamousNpc* ActivityCelebrityMgr::getActivityVisitFamousNpc(const uint32 Table_Id)
{
	return activity_visit_famous_npc_map_.pfind(Table_Id);
}

const ActivityVisitFamousEvent* ActivityCelebrityMgr::getActivityVisitFamousEvent(const uint32 Table_Id)
{
	return activity_visit_famous_event_map_.pfind(Table_Id);
}

const ActivityVisitFamousRewardMap& ActivityCelebrityMgr::getActivityVisitFamousRewardMap() const
{
	return activity_visit_famous_reward_map_;
}

int ActivityCelebrityMgr::getTableIdForEvent() const
{
	return System::RandWeight(event_vec_);
}

int ActivityCelebrityMgr::getTableIdForNpc(const uint32 id) const
{
	if(id == pb::COUNTRY_WORM)
	{
		return System::RandWeight(npc_human_);
	}
	if(id == pb::COUNTRY_MAN)
	{
		return System::RandWeight(npc_god_);
	}
	if(id == pb::COUNTRY_GOD)
	{
		return System::RandWeight(npc_worm_);
	}
	return 0;
}

uint32 ActivityCelebrityMgr::GetNpcFreshTime(const uint32 country_id)
{
	uint32 table_id = getTableIdForNpc(country_id);
	const ActivityVisitFamousNpc* npc = getActivityVisitFamousNpc(table_id);
	ASSERT(npc != NULL);
	return npc->ArmyHZ() * 60 * 1000;
}

void ActivityCelebrityMgr::GetNpc(const uint32 country_id,uint32 &npc_country_Id,IntPair& p_)
{
	uint32 table_id = getTableIdForNpc(country_id);
	const ActivityVisitFamousNpc* npc = getActivityVisitFamousNpc(table_id);
	Utility::SplitStr(npc->NpcList(),p_,'|');
	npc_country_Id = npc->CountryId();
}
void ActivityCelebrityMgr::FillInfomation(uint32 country_id,uint32 city_id,uint64 next_fresh_time,Npc* npc)
{
	celebrity_info[country_id-1].city_id = city_id;
	celebrity_info[country_id-1].next_fresh_time = next_fresh_time;
	celebrity_info[country_id-1].item_id = npc->GetExtraAwardID();
}

void ActivityCelebrityMgr::setCityIdToZero(uint32 country_id)
{
	celebrity_info[country_id-1].city_id = 0;
}

uint32 ActivityCelebrityMgr::getCityId(uint32 country_id)
{
	return celebrity_info[country_id-1].city_id;
}

infomation* ActivityCelebrityMgr::getInfomation(const uint32 countryId)
{
	return &celebrity_info[countryId-1];
}
uint32 ActivityCelebrityMgr::getRandEventId() const
{
	int table_id = sActivityCelebrityMgr.getTableIdForEvent();
	const ActivityVisitFamousEvent* event = sActivityCelebrityMgr.getActivityVisitFamousEvent(table_id);
	if(event)
	{
		return event->EventId();
	}
	else
	{
		return 0;
	}
}
