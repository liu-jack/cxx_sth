#include "GuildInfo.h"
#include "Social.pb.h"
#include "def/TypeDef.h"
#include<sstream>
void GuildMemberInfoTable::SaveTo( pb::GxDB_GMember_Info &member )
{
	member.set_playerid(player_id);
	member.set_player_name(player_name);
	member.set_guild_id(guild_id);
	member.set_guild_name(guild_name);
	member.set_title(title);
	member.set_jointime(join_time);
	member.set_lastactivetime(last_active_time);
	member.set_guildcontributions(guild_contributions);
	member.set_level(level);
	member.set_isonline(IsOnline());
	member.set_vip_level(vip_level);
	member.set_request_flag(is_requester);
	member.set_invite_flag(is_inviter);
	member.set_request_expire_time(request_expire_time);
	member.set_power(power);
	member.set_head_icon(icon);
	std::vector<std::string> attachment_info_vec;
	char* p = strtok(const_cast<char*>(is_send_ap.c_str()),",");
	while(p != NULL)
	{
		attachment_info_vec.push_back(p);
		p = strtok(NULL,",");
	}
	for (size_t i = 0;i < attachment_info_vec.size();i++)
	{
		std::string apinfo = attachment_info_vec[i];
		std::stringstream time_value;
		time_value << apinfo;
		uint64 cureventtime;
		time_value >> cureventtime;
		member.add_is_send_ap(cureventtime);
	}
	attachment_info_vec.clear();
	p = strtok(const_cast<char*>(is_receive_ap.c_str()),",");
	while(p != NULL)
	{
		attachment_info_vec.push_back(p);
		p = strtok(NULL,",");
	}
	for (size_t i = 0;i < attachment_info_vec.size();i++)
	{
		std::string apinfo = attachment_info_vec[i];
		std::stringstream time_value;
		time_value << apinfo;
		uint64 cureventtime;
		time_value >> cureventtime;
		member.add_is_receive_ap(cureventtime);
	}
}

void GuildMemberInfoTable::LoadFrom( const pb::GxDB_GMember_Info &member )
{
	player_id = member.playerid();
	player_name = member.player_name();
	guild_id = member.guild_id();
	guild_name = member.guild_name();
	title = member.title();
	join_time = member.jointime();
	last_active_time = member.lastactivetime();
	guild_contributions = member.guildcontributions();
	level = member.level();
	if (member.isonline())
	{
		SetOnline();
	}
	else
	{
		ResetOnline();
	}
	vip_level = member.vip_level();
	is_requester = member.request_flag();
	is_inviter = member.invite_flag();
	request_expire_time = member.request_expire_time();
	power = member.power();
	icon = member.head_icon();
	is_send_ap.clear();
	int events_size  = member.is_send_ap_size();
	for (int j = 0; j < events_size;j++)
	{
		uint64 playerId = member.is_send_ap(j);
		char temp[64];
		memset(temp,0,64);
		SPRINTF(temp,"%llu,",playerId);
		is_send_ap.append(temp);
	}
	is_receive_ap.clear();
	events_size  = member.is_receive_ap_size();
	for (int j = 0; j < events_size;j++)
	{
		uint64 playerId = member.is_receive_ap(j);
		char temp[64];
		memset(temp,0,64);
		SPRINTF(temp,"%llu,",playerId);
		is_receive_ap.append(temp);
	}
}

void GuildInfoTable::SaveTo( pb::GxDB_GuildInfo &Info )
{
	Info.set_guild_id(guild_id);
	Info.set_icon(icon);
	Info.set_name(name);
	Info.set_create_time(create_time);
	Info.set_level(level);
	Info.set_exp(exp);
	Info.set_prestige(prestige);
	Info.set_active(active);
	Info.set_notice(notice);
	Info.set_notice_time(notice_time);
	Info.set_notice_creater_id(notice_creater_id);
	Info.set_notice_creater_name(notice_creater_name);
	Info.set_guild_owner_id(guild_owner_id);
	Info.set_guild_owner_name(guild_owner_name);
	Info.set_members_count(members_count);
	Info.set_members_limit(members_limit);
	Info.set_score(score);
	Info.set_last_rank_time(last_rank_time);

	std::vector<std::string> attachment_info_vec;
	char* p = strtok(const_cast<char*>(logs.c_str()),",");
	while(p != NULL)
	{
		attachment_info_vec.push_back(p);
		p = strtok(NULL,",");
	}
	pb::GS2C_Guild_Dynamic_Event_Log* logData = Info.mutable_logs();
	for (size_t i = 0;i < attachment_info_vec.size();i++)
	{
		std::string attach_item_info = attachment_info_vec[i];
		int first_token = attach_item_info.find_first_of('|');
		std::string eventType = attach_item_info.substr(0,first_token);

		std::string left_str = attach_item_info.substr(first_token+1,attach_item_info.size());
		int second_token = left_str.find_first_of('|');
		std::string id_value = left_str.substr(0,second_token);

		std::string remaining = left_str.substr(second_token+1,left_str.size());
		int third_token = remaining.find_first_of('|');
		std::string playerId = remaining.substr(0,third_token);

		std::string remainagain = remaining.substr(third_token+1,remaining.size());
		int again_token = remainagain.find_first_of('|');
		std::string stritemguid = remainagain.substr(0,again_token);

		std::string resting = remainagain.substr(again_token+1,remainagain.size());
		int fourth_token = resting.find_first_of('|');
		std::string event_time = resting.substr(0,fourth_token);

		std::string resright = resting.substr(fourth_token+1,resting.size());
		int fiveth_token = resright.find_first_of('|');
		std::string curplayer_name = resright.substr(0,fiveth_token);

		std::string str_value = resright.substr(fiveth_token+1,resright.size());
		pb::GS2C_Guild_Dynamic_Event* eventlog = logData->add_guild_evens();
		if (eventlog)
		{
			eventlog->set_guild_id(id);
			eventlog->set_guild_name(name);
			eventlog->set_event_type((pb::GUILD_DYNAMIC_EVENT)atoi(eventType.c_str()));
			std::stringstream player_value;
			player_value << playerId;
			uint64 curplayerId;
			player_value >> curplayerId;
			std::stringstream item_value;
			item_value << stritemguid;
			uint64 curitemguid;
			item_value >> curitemguid;
			eventlog->set_item_guid(curitemguid);
			eventlog->set_player_id(curplayerId);
			eventlog->set_id_value(atoi(id_value.c_str()));
			eventlog->set_str_value(str_value);
			std::stringstream time_value;
			time_value << event_time;
			uint64 cureventtime;
			time_value >> cureventtime;
			eventlog->set_event_time(cureventtime);
			eventlog->set_player_name(curplayer_name);
		}
	}
}

void GuildInfoTable::LoadFrom( const pb::GxDB_GuildInfo &Info )
{
	guild_id = Info.guild_id();
	icon = Info.icon();
	name = Info.name();
	create_time = Info.create_time();
	level = Info.level();
	exp = Info.exp();
	prestige = Info.prestige();
	active = Info.active();
	notice = Info.notice();
	notice_time = Info.notice_time();
	notice_creater_id = Info.notice_creater_id();
	notice_creater_name = Info.notice_creater_name();
	guild_owner_id = Info.guild_owner_id();
	guild_owner_name = Info.guild_owner_name();
	members_count = Info.members_count();
	members_limit = Info.members_limit();
	score = Info.score();
	last_rank_time = Info.last_rank_time();
	logs.clear();
	const pb::GS2C_Guild_Dynamic_Event_Log & logEvents = Info.logs();
	int events_size  = logEvents.guild_evens_size();
	for (int j = 0; j < events_size;j++)
	{
		const pb::GS2C_Guild_Dynamic_Event& guildEvent = logEvents.guild_evens(j);
		int eventType = guildEvent.event_type();
		uint64 playerId = guildEvent.player_id();
		string playerName = guildEvent.player_name();
		int intValue = guildEvent.id_value();
		string strValue = guildEvent.str_value();
		uint64 eventTime = guildEvent.event_time();
		uint64 itemGuid = guildEvent.item_guid();
		char temp[192];
		memset(temp,0,192);
		SPRINTF(temp,"%d|%d|%llu|%llu|%llu|%s|%s,",eventType,intValue,playerId,itemGuid,eventTime,playerName.c_str(),strValue.c_str());
		logs.append(temp);
	}

}
