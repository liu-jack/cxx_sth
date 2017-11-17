#include "MapLogicTable.h"
#include "MapLogic.pb.h"

void MapLogicTable::SaveTo(pb::GS2C_MapLogic_Init& msg)
{
    msg.set_country_id(country_id);
    msg.set_take_box(take_box);
	for(int i=0; i<ARRAY_SIZE(unlock_fog_city); i++)
	{
		if(unlock_fog_city[i]>0)
			msg.add_unlock_fog_city(unlock_fog_city[i]);
	}
    msg.set_country_donate_time(country_donate_time);
    msg.set_monster_donate_time(monster_donate_time);
    pb::GS2C_MapLogic_ArmyResource* pMsg = msg.mutable_army_resource();
    pMsg->set_update_day(army_resource_update_day);
    pMsg->set_update_hour(army_resource_update_hour);
    for (int i = 0; i < ARRAY_SIZE(army_resource_item); ++i) {
        pb::PairValue* pair = pMsg->add_resource_lst();
        pair->set_first(army_resource_item[i].first);
        pair->set_second(army_resource_item[i].second);
    }
	for (int i = 0; i < ARRAY_SIZE(tonden_begin_time); ++i) {
		pb::PairTime* pair = msg.add_tonden_hero_begin_time();
        //if (tonden_begin_time[i] > 0) {
            pair->set_first(tonden_hero[i]);
            pair->set_second(tonden_begin_time[i]);
        //}
    }
	msg.set_tonden_cdend_time(tonden_donate_end_time);
}
void MapLogicTable::LoadFrom(const pb::GS2C_MapLogic_Init& msg)
{
    country_id = msg.country_id();
    take_box = msg.take_box();
	for(int i=0; i<msg.unlock_fog_city_size(); i++)
	{
		if(i<ARRAY_SIZE(unlock_fog_city))
			unlock_fog_city[i] = msg.unlock_fog_city(i);
	}
    country_donate_time = msg.country_donate_time();
    monster_donate_time = msg.monster_donate_time();
    const pb::GS2C_MapLogic_ArmyResource& msgData = msg.army_resource();
    army_resource_update_day = msgData.update_day();
    army_resource_update_hour = msgData.update_hour();
    for (int i = 0; i < ARRAY_SIZE(army_resource_item) && i < msgData.resource_lst_size(); ++i) {
        const pb::PairValue& pair = msgData.resource_lst(i);
        army_resource_item[i].first = pair.first();
        army_resource_item[i].second = pair.second();
    }

    for (int i = 0; i < msg.tonden_hero_begin_time_size(); ++i) {
        const pb::PairTime& pair = msg.tonden_hero_begin_time(i);
        //if (pair.first() < ARRAY_SIZE(tonden_begin_time)) {
        //    tonden_begin_time[pair.first()] = pair.second();
        //}
		tonden_hero[i] = pair.first();
		tonden_begin_time[i] = pair.second();
    }
	tonden_donate_end_time = msg.tonden_cdend_time();
}
void MapLogicCityEventTable::SaveTo(pb::CityEvent& msg)
{
    msg.set_event_id(event_id);
    msg.set_happen_time(happen_time);
    msg.set_daily_times(daily_times);
    msg.set_city_id(city_id);
    msg.set_is_begin(is_begin == 1);
	msg.set_iscelebrity(is_celebrity == 1);
    msg.set_left_num(left_num);
    msg.set_event_end_time(event_end_time);
}
void MapLogicCityEventTable::LoadFrom(const pb::CityEvent& msg)
{
    event_id = msg.event_id();
    happen_time = msg.happen_time();
    daily_times = msg.daily_times();
    city_id = msg.city_id();
    is_begin = msg.is_begin() ? 1 : 0;
	is_celebrity = msg.iscelebrity() ? 1:0;
    left_num = msg.left_num();
    event_end_time = msg.event_end_time();
}

void FogRecMap::SaveTo(pb::Struct_FOG_INFO& msg)
{
	msg.set_fog_id(fog_id);
	msg.set_uniqueid(uniqueID);
	for (int i = 0; i < ARRAY_SIZE(monster_map); ++i) 
	{
		pb::PairValue* pair = msg.add_guard_hp();
		pair->set_first(monster_map[i].first);
		pair->set_second(monster_map[i].second);
	}
	msg.set_guard_up_hp(monsterAllHp);
}

void FogRecMap::LoadFrom(const pb::Struct_FOG_INFO& msg)
{
	fog_id = msg.fog_id();
	uniqueID = msg.uniqueid();
	for (int i = 0; i < ARRAY_SIZE(monster_map) && i < msg.guard_hp_size(); ++i) 
	{
		const pb::PairValue& pair = msg.guard_hp(i);
		monster_map[i].first = pair.first();
		monster_map[i].second = pair.second();
	}
	monsterAllHp = msg.guard_up_hp();
}