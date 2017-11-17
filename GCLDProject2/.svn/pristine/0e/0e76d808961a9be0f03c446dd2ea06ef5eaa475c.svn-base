#ifndef __DATA_DATASTRUCT_STRUCTBASE_H__
#define __DATA_DATASTRUCT_STRUCTBASE_H__

#pragma once

#include "multi_language/multi_lang_def.h"

#include "def/TypeDef.h"
#ifndef _TOOL
#include "memory_buffer/ByteBuffer.h"
#endif
//#define  BIN_BASE_VERSION 0	//GE1
#define  BIN_BASE_VERSION 100	//GE2

#define INIT_MEMBER_DATA \
		uint32 offset =0;\
		char* structpointer = (char*)this;\
		const char* pFormat = GetFormat();\
		for(uint32 i = 0; i< strlen(pFormat); i++ )\
		{\
			switch( pFormat[i])\
			{\
			case 'k':\
			case 'u':\
				*(uint32*)&structpointer[offset] = 0;\
				offset += sizeof(uint32);\
				break;\
			case 'i':\
				*(int32*)&structpointer[offset] = 0;\
				offset += sizeof(int32);\
				break;\
			case 'f':\
				*(float*)&structpointer[offset] = 0.0f;\
				offset += sizeof(float);\
				break;\
			case 's':\
				*(std::string*)&structpointer[offset] = "";\
				offset += sizeof(std::string);\
				break;\
			case 'c':\
				*(uint8*)&structpointer[offset] = 0;\
				offset += sizeof(uint8);\
				break;\
			case 'h':\
				*(uint16*)&structpointer[offset] = 0;\
				offset += sizeof(uint16);\
				break;\
			case 'l':\
				*(int64*)&structpointer[offset] = 0;\
				offset += sizeof(int64);\
				break;\
			case 'b':\
				*(uint64*)&structpointer[offset] = 0;\
				offset += sizeof(uint64);\
				break;\
            case 't':\
                *(int64*)&structpointer[offset] = 0;\
                offset += sizeof(int64);\
                break;\
            }\
		}

#define DECLARE_EXTRA_SIZE_BY_MEMBER(class_name,member_name) \
	static uint32 ExtraSize()\
	{\
		class_name* p = NULL;\
		uint32 offset = (uint32)((uint64)(&p->member_name));\
		return sizeof(class_name) -offset;\
	}

#define DECLARE_EXTRA_SIZE_ZERO \
	static uint32 ExtraSize()\
	{\
		return 0;\
	}

#define INIT_GAMEDATA(classname) \
	classname()\
	{\
		INIT_MEMBER_DATA; \
	};\
	DECLARE_EXTRA_SIZE_ZERO




template< class _T >
void Copy( _T* left, const _T* right )
{
	uint32 offset =0;
	char* structLeftpointer = (char*)left;
	char* structRightpointer = (char*)right;
	const char* pFormat = _T::GetFormat();
	for(uint32 i = 0; i< strlen(pFormat); i++ )
	{
		switch( pFormat[i])
		{
		case 'k':
		case 'u':
			*(uint32*)&structLeftpointer[offset] = *(uint32*)&structRightpointer[offset];
			offset += sizeof(uint32);
			break;
		case 'i':
			*(int32*)&structLeftpointer[offset] = *(int32*)&structRightpointer[offset];
			offset += sizeof(int32);
			break;
		case 'f':
			*(float*)&structLeftpointer[offset] = *(float*)&structRightpointer[offset];
			offset += sizeof(float);
			break;
		case 's':
			*(std::string*)&structLeftpointer[offset] = *(std::string*)&structRightpointer[offset];
			offset += sizeof(std::string);
			break;
		case 'c':
			*(uint8*)&structLeftpointer[offset] = *(uint8*)&structRightpointer[offset];
			offset += sizeof(uint8);
			break;
		case 'h':
			*(uint16*)&structLeftpointer[offset] = *(uint16*)&structRightpointer[offset];
			offset += sizeof(uint16);
			break;
		case 'l':
			*(int64*)&structLeftpointer[offset] = *(int64*)&structRightpointer[offset];
			offset += sizeof(int64);
			break;
		case 'b':
			*(uint64*)&structLeftpointer[offset] = *(uint64*)&structRightpointer[offset];
			offset += sizeof(uint64);
			break;
        case 't':
            *(int64*)&structLeftpointer[offset] = *(int64*)&structRightpointer[offset];
            offset += sizeof(int64);
            break;
		}
	}
}

#define COPY_CONSTRUCTION(classname)\
	classname(const classname& obj )\
	{\
		Copy<classname>( this, &obj);\
	}\
	classname & operator = (const classname& obj)\
	{\
		Copy<classname>( this, &obj);\
		return (*this);\
	}
	

#define NO_KEY_FUNC\
	static const bool HasIndex()\
	{\
		return false;\
	}\
	const uint32 GetKey() const\
	{\
		return 0 ;\
	}\
	static const char* GetKeyName()\
	{\
		return "";\
	}

#define DEF_STRUCT( tableName, format, keyName, key )
/*
		const uint32 GetKey() const\
	{\
		uint32 tempKey = 0;\
		return (uint32)tempKey ;\
	}\
*/

#define STRING_DB_STRUCT_MY_SQL(string_class_name) \
struct string_class_name \
{\
	INIT_GAMEDATA(string_class_name)\
	static const bool HasIndex()\
	{\
		return true;\
	}\
	const uint32 GetKey() const\
	{\
		return Id;\
	}\
	static const char* GetKeyName()\
	{\
		return "Id";\
	}\
	static const char* GetTableName()\
	{\
		return #string_class_name;\
	}\
	static const char* GetFormat()\
	{\
		return "ussssss";\
	}\
	bool IsEmpty() const\
	{\
		return str[0].empty();\
	}\
	uint32 Id;\
	std::string str[LANG_MAX];\
};

#define STRING_DB_STRUCT_CLIENT(string_class_name) \
struct string_class_name \
{\
	INIT_GAMEDATA(string_class_name)\
	static const bool HasIndex()\
	{\
		return true;\
	}\
	const uint32 GetKey() const\
	{\
		return Id;\
	}\
	static const char* GetKeyName()\
	{\
		return "Id";\
	}\
	static const char* GetTableName()\
	{\
		return #string_class_name;\
	}\
	static const char* GetFormat() \
	{\
		return "us";\
	}\
	bool IsEmpty() const\
	{\
		return str.empty();\
	}\
	uint32 Id;\
	std::string str;\
};


#ifdef LOAD_ONE_LANGUAGE
#define STRING_DB_STRUCT STRING_DB_STRUCT_CLIENT
#else
#define STRING_DB_STRUCT STRING_DB_STRUCT_MY_SQL
#endif

enum DAMAGE_TYPE {
	AI_DAMAGE_NONE = -1,
	AI_DAMAGE_MELEE,
	AI_DAMAGE_FIRE,
	AI_DAMAGE_FROST,
	AI_DAMAGE_LIFE,
	AI_DAMAGE_ENERGY,
	AI_DAMAGE_HOLY,
	AI_DAMAGE_SHADOW,
	AI_DAMAGE_RANGE,
	AI_DAMAGE_NUM
};

enum AI_SKILL_RATE_TYPE {
	AI_SKILL_RATE_DODGE = 0,
	AI_SKILL_RATE_PARRY,
	AI_SKILL_RATE_BLOCK,
	AI_SKILL_RATE_CRITICAL_HIT,
	AI_SKILL_RATE_HIT, //For Magic

	AI_SKILL_RATE_NUM

};

enum INVIS_FLAG
{
	INVIS_FLAG_NORMAL, // players and units with no special invisibility flags
	//INVIS_FLAG_SPIRIT1,
	//INVIS_FLAG_SPIRIT2,
	//INVIS_FLAG_TRAP,
	//INVIS_FLAG_QUEST,
	//INVIS_FLAG_GHOST,
	//INVIS_FLAG_UNKNOWN6,
	//INVIS_FLAG_UNKNOWN7,
	//INVIS_FLAG_SHADOWMOON,
	//INVIS_FLAG_NETHERSTORM,
	//INVIS_FLAG_BASHIR,
	INVIS_FLAG_TOTAL
};




enum UNIT_TYPE
{
	UNIT_TYPE_NONE			= 0,
	UNIT_TYPE_HUMANOID,		// 1,
	UNIT_TYPE_BEAST,		// 2,
	UNIT_TYPE_DRAGONKIN,	// 3,
	UNIT_TYPE_ELEMENTAL,	// 4,
	UNIT_TYPE_UNDEAD,		// 5,
	UNIT_TYPE_GIANT,		// 6,
	UNIT_TYPE_DEMON,		// 7,
	UNIT_TYPE_MECHANICAL,	// 8,
	UNIT_TYPE_SMALL_ANIMAL, // 9,

	UNIT_TYPE_NUM			// 10,
	
	//UNIT_TYPE_MISC			= 10,
	//UNIT_TYPE_TOTEM			= 11,
	//UNIT_TYPE_NONCOMBAT_PET	= 12,
	//UNIT_TYPE_GAS_CLOUD		= 13,
	//UNIT_TYPE_CRITTER		= 8,
};

enum UNIT_FACTION_TYPE {
	FACTION_HOSTILE,
	FACTION_FRIEND,
	FACTION_NEUTRAL,
	UNIT_FACTION_TYPE_NUM,
};

enum EliteLevel
{
	LEVEL_MINION,
	LEVEL_NORMAL,
	LEVEL_MASTER,
	LEVEL_BOSS,
	ELITE_LEVEL_NUM,
};

enum MoveType
{
	MOVE_TYPE_WALK = 0,
	MOVE_TYPE_RUN,
	MOVE_TYPE_STATIC,
	MOVE_TYPE_NUM,
};

//enum CombatType
//{
//	COMBAT_TYPE_MELLEE = 0,
//	COMBAT_TYPE_MID_RANGE,
//	COMBAT_TYPE_LONG_RANGE,
//	COMBAT_TYPE_NUM,
//};

enum AttaclType
{
	ATTACL_TYPE_PHYSICAL = 0,
	ATTACL_TYPE_MAGIC,
	ATTACL_TYPE_COMBINE,
	ATTACL_TYPE_NUM,
};
enum ExtraFlags
{
	EXTRA_FLAGS_WONT_DIE,// if this creatuer will not die after hp == 0
	EXTRA_FLAGS_IS_TRAINING_DUMMY,// if it is training Dummy
	
	EXTRA_FLAGS_CAN_NOT_TARGET,
	EXTRA_FLAGS_DISABLE_NPC_ATTACK,
	EXTRA_FLAGS_WILL_NOT_ATTACK_NPC,
	
	EXTRA_FLAGS_NOT_IN_COMBAT,
	EXTRA_FLAGS_NO_INTERACTION,// [0,1] No, Yes
	EXTRA_FLAGS_CAN_NOT_TURN_AROUND,// can not turn around£¬[0,1] No, Yes
	
	EXTRA_FLAGS_CAN_NOT_DISPLAY,// can not display [0,1] No, Yes
	EXTRA_FLAGS_DEATH_STATE,// death state [0,1] No, Yes
	EXTRA_FLAGS_NUM,
};

enum ImmuneFlags
{
	IMMUNE_FLAGS_PHYSICALS = 0,
	IMMUNE_FLAGS_MAGIC,
	IMMUNE_FLAGS_COMBINE,
	IMMUNE_FLAGS_TYPE_NUM,
};

enum StaticObjFlags
{
	OBJ_FLAGS_CAN_NOT_TARGET = 0,
	OBJ_FLAGS_CAN_NOT_DISAPPEAR,// can not display [0,1] No, Yes
	OBJ_FLAGS_ALWAYS_COLLISION,
	OBJ_FLAGS_CAN_NOT_MULT_PICKUP,
	OBJ_FLAGS_NOT_DISPLAY_NAME,
	OBJ_FLAGS_AUTO_STATE_CHANGE,
	OBJ_FLAGS_NOT_DISPLAY_SHADOWN,
	OBJ_FLAGS_SWITCH_COLLISION,
	OBJ_FLAGS_NUM,
};

enum SoundFlags
{
	SOUNDFLAG_LOOP = 0,
	SOUNDFLAG_RANDOM,
	SOUNDFLAG_NUM,
};

/// account level
enum AccountLevel
{
	ACCOUNT_LEVEL_LAWLESS = 0,  ///< lawless player 
	ACCOUNT_LEVEL_FREE,     ///< not paid account
	ACCOUNT_LEVEL_PAID,     ///< already paid money and valid
	ACCOUNT_LEVEL_3,		///< 3 (not used now)
	ACCOUNT_LEVEL_4,		///< 4 (not used now)
	ACCOUNT_LEVEL_5,		///< 5 (not used now)
	ACCOUNT_LEVEL_6,		///< 6 (not used now)
	ACCOUNT_LEVEL_7,		///< 7 (not used now)
	ACCOUNT_LEVEL_8,		///< 8 (not used now)
	ACCOUNT_LEVEL_9,		///< 9 (not used now)
	ACCOUNT_LEVEL_10,		///< 10 (not used now)
	ACCOUNT_LEVEL_GM,		///< GM
	ACCOUNT_LEVEL_GM2,		///< GM2
	ACCOUNT_LEVEL_GM3,		///< GM3

	ACCOUNT_LEVEL_NUM,

	ACCOUNT_LEVEL_ROOT = 0x7FFF,		///< root
};

enum RuneStoneConsumptionType
{
	RUNE_CONSUMPTION_BEGIN = -1,

	RSCT_BUY_ITEM		   = 0,
	RSCT_BUY_CHARACTER_SLOT = 1,
	RSCT_REVIVIFICATION = 2, //client request used this enum as both instance and normal map
	RSCT_MIGRATION = 3,
	RSCT_RELIVE_INSTANCE = 4,
	RSCT_UPGRADE_STORAGE = 5,
	RSCT_LOTTO_CHEST_LV2 = 6, 
	RSCT_LOTTO_CHEST_LV3 = 7,
    RSCT_CHARACTER_RENAME = 8,
    RSCT_CHANGE_APPEARANCE = 9,
    RSCT_CHANGE_RACE_GENDER,

	RUNE_CONSUMPTION_END,
};

enum RuneConsumptionItemId
{
	GAME_ITEM_ID_BEGIN = 0,

	GAME_ITEM_ID_RELIVE = 1, // normal map
	GAME_ITEM_ID_UNLOCK_CHARACTER_SLOT,
	GAME_ITEM_ID_MIGRATION,
	GAME_ITEM_ID_RELIVE_INSTANCE, // instance
    GAME_ITEM_ID_RENAME,

    GAME_ITEM_ID_CHANGE_APPEARANCE = 7,
    GAME_ITEM_ID_CHANGE_RACE_GENDER = 11,

	GAME_ITEM_ID_END,
};

struct RuneConsumptionItem           
{
	RuneConsumptionItem(int id, const std::string& name, int price, const std::string& params)
		: m_id(id), m_name(name), m_price(price), m_params(params) {};
	int                       m_id;
	std::string               m_name;
	int                       m_price;
	std::string               m_params; 
};

struct OSP_GetTransferGoldResult
{
	 int   m_user;    // uid
	 int   m_game;    // ggi
	 int   m_character;    // characterid
	 std::string m_char_name;   
	 int   m_transfer_gold; 
};

enum WebLogType
{
	WEBLOG_GLLIVE_LOGIN,
	WEBLOG_GLLIVE_COMMUNICATION,
	WEBLOG_LOBBY_COMMUNICATION,
	WEBLOG_GS_CONNECTION,
	WEBLOG_ACCOUNT_LOGIN,
	WEBLOG_CHARACTER_LOGIN,
	WEBLOG_ECOMMERCE_SUBMIT_TRANSACTION,
};

enum ReportPlayerResult
{
	REPORT_SUCCESS = 0,
	REPORT_UNKNOWN_ERROR,
	REPORT_CHARACTER_NOT_EXIST,
	REPORT_SEND_MAIL_ERROR,
};

enum TrackClientEventType
{
	TRACK_TYPE_BEGIN = 0,

	TRACK_TYPE_PROMOPUSH = TRACK_TYPE_BEGIN,
	TRACK_TYPE_RATINGGAME,
	TRACK_TYPE_XDAYS_INACTIVITY,
	TRACK_TYPE_ACCESS_STORE,
	TRACK_TYPE_REMOTE_NOTICE_PN,
	TRACK_TYPE_WELCOME_SCREEN,
	TRACK_TYPE_INTERACT_FOLLOW,
	TRACK_TYPE_INAPP_PURCHASE,
	TRACK_TYPE_DBG_CONTROLLER,
    TRACK_TYPE_VOICE_CHAT,
	TRACK_TYPE_LOBBY_CLIENT,
	TRACK_TYPE_NICK_SHOP,
	TRACK_TYPE_COUNT,
};

enum TrackClientEventOperator
{
	// general
	TRACK_OP_YES	= 0,
	TRACK_OP_NO		= 1,
	TRACK_OP_LATER	= 2,
	TRACK_OP_CLOSE	= 6,
	TRACK_OP_DISPLAY = 7,

	// pn
	TRACK_OP_LAUNCH_FROM_PN = 3,
	TRACK_OP_RESUME_FROM_PN = 4,
	TRACK_OP_CANCEL_PN		= 5,

	// welcome screen
	TRACK_OP_REDIR_IAP_SUBSCRIPTION = 8,   // redir is short name of redirection
	TRACK_OP_REDIR_IAP_RUNE			= 9,
	TRACK_OP_REDIR_IAP_GOLD			= 10,
	TRACK_OP_REDIR_APPSTORE			= 11,
	TRACK_OP_REDIR_LOTTERY			= 12,

    // voice chat
    TRACK_OP_VOICE_ROOM_CREATED     = 13,
    TRACK_OP_VOICE_ROOM_JOINED      = 14,
    TRACK_OP_VOICE_ROOM_LEFT        = 15,

	//Lobby Client Login
	TRACK_OP_LOBBY_CLIENT_JOINEDROOM     = 16,


	//nick shop
	TRACK_OP_OPEN_WEB_SHOP		= 17,
	TRACK_OP_SHOW_ITEM_INFO		= 18,
};

#define GM_TEAM_LOWID 3

#endif // __DATA_DATASTRUCT_STRUCTBASE_H__
