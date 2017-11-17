/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "../object/Player.h"
#include "reward/reward.h"
#include "Activity.pb.h"
#include "table/ActivityRankTableMgr.h"
#include "Opcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "Rank.pb.h"
#include "ActivityControl/ActivityMgr.h"
extern uint32 activity_occupy_city_god;
extern uint32 activity_occupy_city_human;
extern uint32 activity_occupy_city_worm;
using namespace pb;
const static uint32 SHOW_NUMBER_PLAYER = 200;
#ifdef _MMO_SERVER_
OpHandler(CMSG_COUNTRY_OCCUPY_CITY_INFO)
{
	GS2C_CountryOccupyCityNum msg;
	msg.set_nr_god(activity_occupy_city_god);
	msg.set_nr_human(activity_occupy_city_human);
	msg.set_nr_worm(activity_occupy_city_worm);
	Send(SMSG_COUNTRY_OCCUPY_CITY_INFO_RSP,msg);
}
OpHandler(CMSG_PERSONAL_OCCUPY_CITY_INFO)
{
	GS2C_PersonalOccupyCityNum msg;
	size_t i = 1;
	ranksortMap::const_iterator iter = sActivityRankTableMgr.occupation_rank_map_.begin();
	for(;iter != sActivityRankTableMgr.occupation_rank_map_.end() && iter->first.ranknum_ != 0;++iter,++i)
	{
		if(i <= SHOW_NUMBER_PLAYER)
		{
			pb::ShowData* show_data = msg.add_rank_of_take_city();
			show_data->set_rank(i);
			show_data->set_name(iter->second);
			show_data->set_lv(iter->first.level_);
			show_data->set_value(iter->first.ranknum_);
			show_data->set_vip_lv(iter->first.vip_level_);
			show_data->set_country_id(iter->first.country_id_);
			show_data->set_headiconid(iter->first.headIconId_);
			if(iter->first.playerid_ == GetGuid())
			{
				pb::ShowData* self_data = msg.mutable_self_info();
				self_data->set_rank(i);
				self_data->set_name(iter->second);
				self_data->set_lv(iter->first.level_);
				self_data->set_vip_lv(iter->first.vip_level_);
				self_data->set_value(iter->first.ranknum_);
				self_data->set_country_id(iter->first.country_id_);
				self_data->set_headiconid(iter->first.headIconId_);
			}
		}
		else
		{
			break;
		}
	}
	if(!msg.has_self_info() && GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM) != 0)
	{
		pb::ShowData* self_data = msg.mutable_self_info();
		self_data->set_rank(i);
		self_data->set_name(CharName());
		self_data->set_lv(GetLevel());
		self_data->set_vip_lv(GetVipLevel());
		self_data->set_value(GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM));
		self_data->set_country_id(GetCountryId());
		self_data->set_headiconid(GetHeadIconId());
	}
	Send(SMSG_PERSONAL_OCCUPY_CITY_INFO_RSP,msg);
}
OpHandler(CMSG_ADD_SOLDIER_INFO)
{
	GS2C_PersonalAddSoldier msg;
	size_t i = 1;
	ranksortMap::const_iterator iter = sActivityRankTableMgr.add_solider_rank_map_.begin();
	for(;iter != sActivityRankTableMgr.add_solider_rank_map_.end()&& iter->first.ranknum_ != 0;++iter,++i)
	{
		if(i <= SHOW_NUMBER_PLAYER)
		{
			pb::ShowData* show_data = msg.add_rank_of_add_soldier();
			show_data->set_rank(i);
			show_data->set_name(iter->second);
			show_data->set_lv(iter->first.level_);
			show_data->set_vip_lv(iter->first.vip_level_);
			show_data->set_value(iter->first.ranknum_);
			show_data->set_country_id(iter->first.country_id_);
			show_data->set_headiconid(iter->first.headIconId_);
			if(iter->first.playerid_ == GetGuid())
			{
				pb::ShowData* self_data = msg.mutable_self_info();
				self_data->set_rank(i);
				self_data->set_name(iter->second);
				self_data->set_lv(iter->first.level_);
				self_data->set_vip_lv(iter->first.vip_level_);
				self_data->set_value(iter->first.ranknum_);
				self_data->set_country_id(iter->first.country_id_);
				self_data->set_headiconid(iter->first.headIconId_);
			}
		}
		else
		{
			break;
		}

	}
	if(!msg.has_self_info() && GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER) != 0)
	{
		pb::ShowData* self_data = msg.mutable_self_info();
		self_data->set_rank(i);
		self_data->set_name(CharName());
		self_data->set_lv(GetLevel());
		self_data->set_vip_lv(GetVipLevel());
		self_data->set_value(GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER));
		self_data->set_country_id(GetCountryId());
		self_data->set_headiconid(GetHeadIconId());
	}
	Send(SMSG_ADD_SOLDIER_INFO_RSP,msg);
}

#endif