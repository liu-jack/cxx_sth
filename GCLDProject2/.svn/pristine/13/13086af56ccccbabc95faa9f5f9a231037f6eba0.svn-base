
#pragma once
#include <string>
#include "struct_base.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
index string		'k'
*/


//set byte alignment 
#pragma pack(push,1)


struct GuildanceDialogueProto
{
	INIT_GAMEDATA(GuildanceDialogueProto)
		static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "guidance_dialogue_proto";
	}
	static const char* GetFormat()
	{
		return "uusuu";
	}
	uint32 id;
	uint32 image_position;
	string image_name;
	uint32 dialogue_id;
	uint32 image_name_id;
};



struct GuildanceProto
{
	INIT_GAMEDATA(GuildanceProto)
		static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "guidance_proto";
	}
	static const char* GetFormat()
	{
		return "uuuuusuussusuuu";
	}
	uint32 id;
	uint32 main_quest_id;
	uint32 guidance_button_id;
	uint32 dialogue_id;
	uint32 ui_view_id;
	string ui_view_id2;
	uint32 arrow_direction;
	uint32 is_need_record;
	string record_type;
	string comment;
	uint32 main_ui_flag;
	string end_type;
	uint32 guidance_type;
	uint32 arrow_type;
	uint32 arrow_string_id;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Item Table

//#define GUILD_TITLE_NUM 4
//
//struct Guild_Title
//{
//	std::string title;
//	uint32 title_authority;
//	uint32 title_num_limit;
//};
//
//enum GUILD_TITLE_TYPE
//{
//	TT_MASTER = 0,
//	TT_SUBMASTER = 1,
//	TT_OFFICER = 2,
//	TT_MEMBER = 3,
//};
//
//enum GUILD_TITLE_AUTHORITY_TYPE
//{
//	TAT_NONE = 0,
//	TAT_CHAT = 1 << 0,
//	TAT_INVITE = 1 << 1,
//	TAT_KICK = 1 << 2,
//	TAT_UPGRADE = 1 << 3,
//	TAT_CHANGE_MASTER = 1 << 4,
//};
//
//enum GUILD_TITLE_AUTHORITY
//{
//	TA_MASTER = TAT_CHAT | TAT_INVITE | TAT_KICK | TAT_UPGRADE | TAT_CHANGE_MASTER,
//	TA_SUBMASTER = TAT_CHAT | TAT_INVITE | TAT_KICK | TAT_UPGRADE,
//	TA_OFFICER = TAT_CHAT | TAT_INVITE | TAT_KICK,
//	TA_MEMBER = TAT_CHAT,
//};
//
//struct GuildInformation
//{
//	INIT_GAMEDATA(GuildInformation)
//
//	void Save(IStreamBuffer& buf) const
//	{
//		int version = 1;
//		buf << version;
//		if (version >= 1)
//		{
//			buf << Id;
//			buf << name;
//			buf << constructTime;
//			buf << level;
//			buf << controlArea1;
//			buf << controlArea2;
//			buf << controlArea3;
//			buf << controlArea4;
//			buf << guildPoint;
//			buf << prestige;
//			buf << honor;
//			buf << prestigeRank;
//			buf << honorRank;
//			buf << titles[0].title;
//			buf << titles[0].title_authority;
//			buf << titles[0].title_num_limit;
//			buf << titles[1].title;
//			buf << titles[1].title_authority;
//			buf << titles[1].title_num_limit;
//			buf << titles[2].title;
//			buf << titles[2].title_authority;
//			buf << titles[2].title_num_limit;
//			buf << titles[3].title;
//			buf << titles[3].title_authority;
//			buf << titles[3].title_num_limit;
//			buf << memberNum;
//			buf << tip;
//		}
//	};
//	bool Load(IStreamBuffer& buf)
//	{
//		if (buf.GetRemainSize() > 0)
//		{
//			int beginVersion = 1;
//			int curVersion = 1;
//			int version;
//			buf >> version;
//			if (version >= beginVersion && version <= curVersion)
//			{
//				buf >> Id;
//				buf >> name;
//				buf >> constructTime;
//				buf >> level;
//				buf >> controlArea1;
//				buf >> controlArea2;
//				buf >> controlArea3;
//				buf >> controlArea4;
//				buf >> guildPoint;
//				buf >> prestige;
//				buf >> honor;
//				buf >> prestigeRank;
//				buf >> honorRank;
//				buf >> titles[0].title;
//				buf >> titles[0].title_authority;
//				buf >> titles[0].title_num_limit;
//				buf >> titles[1].title;
//				buf >> titles[1].title_authority;
//				buf >> titles[1].title_num_limit;
//				buf >> titles[2].title;
//				buf >> titles[2].title_authority;
//				buf >> titles[2].title_num_limit;
//				buf >> titles[3].title;
//				buf >> titles[3].title_authority;
//				buf >> titles[3].title_num_limit;
//				buf >> memberNum;
//				buf >> tip;
//				return true;
//			}
//		}
//		return false;
//	};
//
//	static const char* GetFormat()
//	{
//		return "usuuuuuuuuuuusuusuusuusuuus";
//	}
//	uint32 Id;
//	std::string name;
//	uint32 constructTime;
//	uint32 level;
//	uint32 controlArea1;
//	uint32 controlArea2;
//	uint32 controlArea3;
//	uint32 controlArea4;
//	uint32 guildPoint;
//	uint32 prestige;
//	uint32 honor;
//	uint32 prestigeRank;
//	uint32 honorRank;
//	Guild_Title titles[GUILD_TITLE_NUM];
//	uint32 memberNum;//it's useless now !li.ye
//	std::string tip;
//};
//
////#define GUILD_MEMBER_MAX_NUM 8
//
//struct GMember_Info
//{
//	uint32 guild;
//	std::string remark;
//	uint32 title;
//	uint32 joinTime;
//};
//
//struct GMember_Info_Client
//{
//	std::string name;
//	std::string remark;
//	uint32 title;
//	uint8 race;
//	uint8 level;
//	uint16 area;
//	uint32 lastLogin;
//	bool online;
//	uint8 gender;
//	uint32 guidLow;
//	uint8 playerClass;
//	float x;
//	float y;
//	float z;
//};
//
//struct Guild_Member
//{
//	INIT_GAMEDATA(Guild_Member)
//	static const bool HasIndex()
//	{
//		return true;
//	}
//	const uint32 GetKey() const
//	{
//		return player;
//	}
//	static const char* GetKeyName()
//	{
//		return "player";
//	}
//	static const char* GetTableName()
//	{
//		return "guildmember";
//	}
//	static const char* GetFormat()
//	{
//		return "uusuu";
//	}
//	uint32 player;
//	GMember_Info member;
//};

#pragma pack(pop)