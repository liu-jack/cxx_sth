#include "WorldMapEvent.h"
#include "InterServer.pb.h"

void PersonalECTable::SaveTo(pb::SG2D_PersonalEC& msg)
{
	msg.set_player_id(player_id);
	msg.set_last_fresh_time(last_fresh_time);
	msg.set_current_step(current_step);
	msg.set_is_take_chain_award(is_take_chain_award);
	msg.set_current_event_id(current_event_id);
	msg.set_city_id(city_id);
	msg.set_current_completion(current_completion);
	msg.set_is_take_event_award(is_take_event_award);
}

void PersonalECTable::LoadFrom(const pb::SG2D_PersonalEC& msg)
{
	player_id = msg.player_id();
	last_fresh_time = msg.last_fresh_time();
	current_step = msg.current_step();
	is_take_chain_award = msg.is_take_chain_award();
	current_event_id = msg.current_event_id();
	city_id = msg.city_id();
	current_completion = msg.current_completion();
	is_take_event_award = msg.is_take_event_award();
}