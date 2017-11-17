#include "EventTable.h"
#include "InterServer.pb.h"
void EventSchedulerTable::SaveTo( pb::SD2G_EventScheduler &event ) const
{
	event.set_event_id(id);
	event.set_event_type(event_type);
	event.set_send_type(send_type);
	event.set_platform(platform);
	event.set_channel(channel);
	event.set_begin_time(begin_time);
	event.set_end_time(end_time);
	event.set_mail_id(mail_id);
	event.set_receivers(receivers);
}

void EventSchedulerTable::LoadFrom( const pb::SD2G_EventScheduler &event )
{
	id		   = event.event_id();
	event_type = event.event_type();
	send_type  = event.send_type();
	platform   = event.platform();
	channel    = event.channel();
	begin_time = event.begin_time();
	end_time   = event.end_time();
	mail_id    = event.mail_id();
	receivers  = event.receivers();
}

void EventRecodeTable::SaveTo( pb::SD2G_EventRecord & record ) const
{
	record.set_record_id(id);
	record.set_event_id(event_id);
	record.set_player_id(player_id);
}

void EventRecodeTable::LoadFrom( const pb::SD2G_EventRecord & record )
{
	id = record.record_id();
	event_id = record.event_id();
	player_id = record.player_id();
}
