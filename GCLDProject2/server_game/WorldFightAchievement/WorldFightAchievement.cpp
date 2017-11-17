/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "WorldFightAchievement.h"
#include "Table/WorldFightAchievementMgr.h"
#include "World.pb.h"
#include "../object/Player.h"
#include "Opcode.pb.h"
#include "CrossLogic/PlayerWorldFightAchievementDB.h"
WorldFightAchievementLog::WorldFightAchievementLog()
{
	const WorldFightAchievementMap& achieve_map = sWorldFightAchievementMgr.getWorldFightAchievementMap();
	for(WorldFightAchievementMap::const_iterator iter = achieve_map.begin();iter != achieve_map.end();++iter)
	{
		achievement_attack_map_[iter->first] = AchievementInfo(CITY_ATTACK_CITY);
		achievement_occupy_map_[iter->first] = AchievementInfo(CITY_OCCUPY_CITY);
	}
}

void WorldFightAchievementLog::SaveTo(pb::GS2C_World_Achievement_All_Info& msg)
{
	for(AchievementInfoMap::iterator iter = achievement_attack_map_.begin();iter != achievement_attack_map_.end();++iter)
	{
		pb::World_Achievement_Info* info = msg.add_info();
		info->set_id(iter->first);
		info->set_type(iter->second.type);
		info->set_state(iter->second.state);
	}
	for(AchievementInfoMap::iterator iter = achievement_occupy_map_.begin();iter != achievement_occupy_map_.end();++iter)
	{
		pb::World_Achievement_Info* info = msg.add_info();
		info->set_id(iter->first);
		info->set_type(iter->second.type);
		info->set_state(iter->second.state);
	}
}


AchievementInfo* WorldFightAchievementLog::FindInfoInMap(const uint32 id,const uint32 type)
{
	if(type == CITY_ATTACK_CITY)
	{
		Iter iter = achievement_attack_map_.find(id);
		if(iter != achievement_attack_map_.end())
		{
			return &iter->second;
		}
		return NULL;
	}
	else 
	{
		Iter iter = achievement_occupy_map_.find(id);
		if(iter != achievement_occupy_map_.end())
		{
			return &iter->second;
		}
		return NULL;
	}
}



void WorldFightAchievementLog::UpdateCityState(Player* player,const uint32 city_id,const uint32 type,const uint32 state)
{
	pb::World_Achievement_Info msg;
	msg.set_id(city_id);
	msg.set_type(type);
	if(sWorldFightAchievementMgr.IsInvalidCityId(city_id)) return;
	if(type == CITY_ATTACK_CITY)
	{
		achievement_attack_map_[city_id].set_state(state);
		PlayerWorldFightAchievementDB::UpdateAchievementInfoToDb(*player,city_id,type,state);
		msg.set_state(achievement_attack_map_[city_id].state);
	}
	else if(type == CITY_OCCUPY_CITY)
	{
		achievement_occupy_map_[city_id].set_state(state);
		PlayerWorldFightAchievementDB::UpdateAchievementInfoToDb(*player,city_id,type,state);
		if(state == STATE_CAN_TAKE)
		{
			achievement_attack_map_[city_id].set_state(state);
			PlayerWorldFightAchievementDB::UpdateAchievementInfoToDb(*player,city_id,CITY_ATTACK_CITY,state);
		}
		msg.set_state(achievement_occupy_map_[city_id].state);
	}
	player->Send(pb::SMSG_WORLD_FIGHT_ACHIEVE_STATE_CHANGE,msg);
}


size_t WorldFightAchievementLog::GetCanTakeRewardCount(const uint32 type)
{
	size_t count = 0;
	if(type == CITY_ATTACK_CITY)
	{
		for(AchievementInfoMap::iterator iter = achievement_attack_map_.begin();iter != achievement_attack_map_.end();++iter)
		{
			if(iter->second.state >= STATE_CAN_TAKE) count++;
		}
		return count;
	}
	else if(type == CITY_OCCUPY_CITY)
	{
		for(AchievementInfoMap::iterator iter = achievement_occupy_map_.begin();iter != achievement_occupy_map_.end();++iter)
		{
			if(iter->second.state >= STATE_CAN_TAKE) count++;
		}
		return count;
	}
	return 0;
}

void WorldFightAchievementLog::LoadFrom(const pb::GxDB_World_Achievement& msg)
{
	for(int i = 0;i <msg.info_size();++i)
	{
		const pb::GxDB_World_Achievement_Info& info = msg.info(i);
		uint32 city_id = info.city_id();
		if((info.type() >> SHIFT_4) == CITY_OCCUPY_CITY)
		{
			achievement_occupy_map_[city_id].set_state(info.state() >> SHIFT_4);
			if((info.type() & 0xf) != 0)
			{
				achievement_attack_map_[city_id].set_state(info.state() & 0xf);
			}
		}
		else
		{
			achievement_attack_map_[city_id].set_state(info.state() & 0xf);
		}
	}
}

