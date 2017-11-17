//TODO #include "preHeader.h"
#include "GErrorCode.h"
//#include "game/strings/StringManager.h"
//#include "game/strings/String_IDs.h"
#include <string.h>
#include <glitch/glitch.h>

//TODO: should use resource string.
const int SpellErrorDesc[] = 
{1
	////SKILL_ERROR_NULL											 = SpellCastBegin,
	//WARNING_SPELLCASTSUCCESS,//"Spell Cast Success",					//SKILL_FAILED_SUCCESS                                         = SKILL_ERROR_NULL,
	//WARNING_UNKNOWNCASTER, //"Unknown Caster",		//SKILL_FAILED_UNKNOWN_CASTER,
	//WARNING_INVALIDTARGET, //"Invalid Target",	//SKILL_FAILED_INVALID_TARGET,
	//WARNING_SPELL_NO_POWER,//"Not Enough Power",	//SKILL_FAILED_NOT_ENOUGH_POWER,
	//WARNING_TARGETDEAD, //"Target Is Dead",	//SKILL_FAILED_TARGETS_DEAD,
	//WARNING_SPELLINVALID, //"Spell Invalid",	//SKILL_FAILED_INVALID_SPELL,
	//WARNING_SPELLINTERRUPTED, //"Spell Interrupted",	//SKILL_FAILED_INTERRUPTED,
	//WARNING_GATHER_SPELLINTERRUPTED, //"Gathering interrupted.",	//SKILL_FAILED_GATHER_INTERRUPTED,
	//WARNING_NO_MESSAGE, //"Spell In Progress",	//SKILL_FAILED_SPELL_IN_PROGRESS,
	//WARNING_SPELLNOTREADY, //"Spell Not Ready",	//SKILL_FAILED_NOT_READY,
	//WARNING_TARGETOUTOFRANGE, //"Target Out Of Range",	//SKILL_FAILED_OUT_OF_RANGE,
	//WARNING_TARGETNOTINFRONT, //"Target Not In Front",	//SKILL_FAILED_NOT_FRONT,
	//WARNING_NEEDTARGETBACK, //"Need Target Back",	//SKILL_FAILED_NEED_TARGET_BACK,
	//WARNING_INVALIDCASTER, //"Invalid Caster",	//SKILL_FAILED_INVALID_CASTER,
	//WARNING_NEEDATARGET, //"Need A Target",	//SKILL_FAILED_NEED_TARGET,
	//WARNING_SPELL_NO_RESPONSE,//"No Response",	//SKILL_FAILED_NO_RESPONSE,
	//WARNING_SPELL_NOT_LEARNED, //"Spell not learned",	//SKILL_FAILED_NOT_LEARNED,
	//WARNING_SPELL_CAST_PASSIVE,//"Can not cast a passive spell",	//SKILL_FAILED_CANT_CAST_PASSIVE,
	//WARNING_SPELL_NOT_IN_WORLD, //"Spell failed not in the world",	//SKILL_FAILED_NOT_IN_WORLD,
	//WARNING_SPELL_WRONG_AREA, //"You can't do this in current area ",	//SKILL_FAILED_CANT_CAST_IN_CURRENT_AREA,
	//WARNING_NO_MESSAGE, //"",	//SKILL_FAILED_NO_MESSAGE,
	//WARNING_SPELL_INVALID_COMBAT,//"Can not cast in combat",	//SKILL_FAILED_DISABLE_IN_COMBAT,
	//WARNING_SPELL_NEED_MAINHAND_WEAPON, //"You need an applicable weapon in your main-hand",//SKILL_FAILED_NEED_MAINHAND_WEAPON,
	//WARNING_SPELL_NEED_OFFHAND_WEAPON, //"You need an applicable weapon in you off-hand",//SKILL_FAILED_NEED_OFFHAND_WEAPON,
	//WARNING_SPELL_NEED_RANGED_WEAPON, //"You need equip an ranged weapon",//SKILL_FAILED_NEED_RANGED_WEAPON,
	//WARNING_SPELL_NO_MATERIAL,
	//WARNING_SPELL_REPETITION_LEARN_SPELL,
	//WARNING_SPELL_BAG_FULL,
	//WARNING_SPELL_CANNOT_LEARN_SPELL,
	//WARNING_SPELL_SILENCED,
	//WARNING_SPELL_NO_MORALE,
	//WARNING_SPELL_NO_VIGOR,
	//ERROR_USE_CANT,
	//UI_ARENA_SPELL_CANNOT_USE, // SKILL_FAILED_CANNOT_CAST_IN_ARENA

    /*
     * fix me: SKILL_FAILED_IMMUED don't have a description here.
     */
};
const char * SpellErrorDescStr[] = 
{
	//SKILL_ERROR_NULL											 = SpellCastBegin,
	"Spell Cast Success",					//SKILL_FAILED_SUCCESS                                         = SKILL_ERROR_NULL,
	"GENERAL_SKILL_FAILED_UNKNOWN_CASTER",		//SKILL_FAILED_UNKNOWN_CASTER,
	"GENERAL_SKILL_FAILED_INVALID_TARGET",	//SKILL_FAILED_INVALID_TARGET,
	"GENERAL_SKILL_FAILED_NOT_ENOUGH_POWER",	//SKILL_FAILED_NOT_ENOUGH_POWER,
	"GENERAL_SKILL_FAILED_TARGETS_DEAD",	//SKILL_FAILED_TARGETS_DEAD,
	"GENERSL_SKILL_FAILED_INVALID_SPELL",	//SKILL_FAILED_INVALID_SPELL,
	"GENERAL_SKILL_FAILED_INTERRUPTED",	//SKILL_FAILED_INTERRUPTED,
	"GENERAL_SKILL_FAILED_GATHER_INTERRUPTED",	//SKILL_FAILED_GATHER_INTERRUPTED,
	"GENERAL_SKILL_FAILED_SPELL_IN_PROGRESS",	//SKILL_FAILED_SPELL_IN_PROGRESS,
	"GENERAL_SKILL_FAILED_NOT_READY",	//SKILL_FAILED_NOT_READY,
	"GENERAL_SKILL_FAILED_OUT_OF_RANGE",	//SKILL_FAILED_OUT_OF_RANGE,
	"GENERAL_SKILL_FAILED_NOT_FRONT",	//SKILL_FAILED_NOT_FRONT,
	"GENERAL_SKILL_FAILED_NEED_TARGET_BACK",	//SKILL_FAILED_NEED_TARGET_BACK,
	"GENERAL_SKILL_FAILED_INVALID_CASTER",	//SKILL_FAILED_INVALID_CASTER,
	"GENERAL_SKILL_FAILURE_NOTARGET",	//SKILL_FAILED_NEED_TARGET,
	"GENERAL_SKILL_FAILED_NO_RESPONSE",	//SKILL_FAILED_NO_RESPONSE,
	"GENERAL_SKILL_FAILED_NOT_LEARNED",	//SKILL_FAILED_NOT_LEARNED,
	"GENERAL_SKILL_FAILED_CANT_CAST_PASSIVE",	//SKILL_FAILED_CANT_CAST_PASSIVE,
	"GENERAL_SKILL_FAILED_NOT_IN_WORLD",	//SKILL_FAILED_NOT_IN_WORLD,
	"GENERAL_SKILL_FAILED_CANT_CAST_IN_CURRENT_AREA",	//SKILL_FAILED_CANT_CAST_IN_CURRENT_AREA,
	"GENERAL_SKILL_FAILED_NO_MESSAGE",	//SKILL_FAILED_NO_MESSAGE,
	"GENERAL_SKILL_FAILED_DISABLE_IN_COMBAT",	//SKILL_FAILED_DISABLE_IN_COMBAT,
	"You need an applicable weapon in your main-hand",//SKILL_FAILED_NEED_MAINHAND_WEAPON,
	"You need an applicable weapon in you off-hand",//SKILL_FAILED_NEED_OFFHAND_WEAPON,
	"You need equip an ranged weapon",//SKILL_FAILED_NEED_RANGED_WEAPON,
	"WARNING_SPELL_NO_MATERIAL",
	"WARNING_SPELL_REPETITION_LEARN_SPELL",
	"WARNING_SPELL_BAG_FULL",
	"WARNING_SPELL_CANNOT_LEARN_SPELL",
	"WARNING_SPELL_SILENCED",
	"WARNING_SPELL_NO_MORALE",
	"WARNING_SPELL_NO_VIGOR",
	"ERROR_USE_CANT",
	"UI_ARENA_SPELL_CANNOT_USE", // SKILL_FAILED_CANNOT_CAST_IN_ARENA

    /*
     * fix me: SKILL_FAILED_IMMUED don't have a description here.
     */
};
const int spell_error_number = sizeof(SpellErrorDescStr) / sizeof(int);

//const char* GetSkillErrorMsg(SkillCastError error)
//{
//	if((error < SkillCastBegin) || (error > (SkillCastBegin + spell_error_number)))
//	{
//#ifdef _DEBUG
//		return "Warning: Unknown Error.";//CStringManager::GetString(WARNING_UNKNOWNERROR);
//#else
//		return "";
//#endif
//	}
//	return CStringManager::GetString(SpellErrorDescStr[error - SkillCastBegin]);
//}
	

const int InventoryErrorDescr[] = 
{1
	////InvErrorOK				= ItemBegin,	//	"Inventory error OK",
	//ERROR_INV_ISFULL,//"Inventory is full,//",
	//ERROR_SLOT_NOT_EMPTY,//"Slot isn't empty,//",
	//ERROR_NOT_ENOUGH,//"Not Enough,//",
	//ERROR_INVALID_INV,//"Invalid inventory,//",
	//ERROR_INVALID_INV_PROTO,//"Invalid prototype,//",
	//ERROR_INVALID_INV_SPELL,//"Invalid Spell,//",
	//ERROR_INVALID_INV_SLOT,//"Invalid slot,//",
	//ERROR_INVALID_INV_OWNER,//"Invalid Owner,//",
	//ERROR_USE_NOT_READY,//"Item not ready,//",
	//ERROR_USE_CANT,//"Can't use it yet,//",
	//ERROR_USE_ALREADY_HAVE_QUEST,//"You had the quest,//",
	//ERROR_VENDER_TOO_FAR,//"Vender is too far,//",
	//ERROR_VENDER_INVALID_ITEM,//"Vend invalid item,//",
	//ERROR_VENDER_INVALID_VENDER,//"Invalid vender,//",
	//ERROR_VENDER_INVALID_SLOT,//"Invalid slot,//",
	//ERROR_NOT_ENOUGH_MONEY,//"Not enough money,//",
	//ERROR_EXCHANGE_UNEXPECT_SRC_ITEM,//"Exchange Unexpect source item,//",
	//ERROR_EQUIP_CANT,//"You can't equip it,//", 
	//ERROR_ITEM_NOT_FOUND,//"Item not found,//",
	//ERROR_NOT_ENOUGH_SPECIAL_MONEY,//"Not enough special money,//",
	//ERROR_ITEM_BINDED,//This item has been binded.
	//ERROR_ITEM_UNIQUE,// You can't get this item any more.
	//UI_ROLL_EQUIPED_SAME_UNIQUE,//ErrItemEquipUnique
	//NULL,//ErrItemExpire
	
};
const int inventory_error_nb = sizeof(InventoryErrorDescr) / sizeof(int);

//const char* GetInventoryErrorMsg(InventoryError error)
//{
//	if( error <= ItemBegin)
//	{
//#ifdef _DEBUG
//		return "Warning: Unknown Error.";//CStringManager::GetString(WARNING_UNKNOWNERROR);
//#else
//		return "";
//#endif
//	}
//	return CStringManager::GetString(InventoryErrorDescr[error-ItemBegin - 1]);
//}


const int ErrorCodeStr_SessionHandle[] = {1
	//ERROR_SESSION_HANDLE_BEGIN	,//  SessionHandleBegin	
	//ERROR_SESSION_HANDLE_KILL,		 //  SessionHandleKill,
	//ERROR_SESSION_HANDLE_WARNING,	 //  SessionHandleWarning,
	//ERROR_SESSION_HANDLE_ERROR,		 //  SessionHandleError,
	//ERROR_SESSION_HANDLE_UNHANDLE,	 //  SessionHandleUnhandle
	//ERROR_SESSION_HANDLE_SUCCESS,	 //  SessionHandleSuccess,
};
const int errorCodeSessionHandleNb = sizeof(ErrorCodeStr_SessionHandle) /sizeof(int);

const int ErrorCodeStr_SessionKick[] ={1
	//ERROR_SESSION_KICK_NORMAL,			//	SessionKickNormal		
	//ERROR_SESSION_KICK_BY_GM,			//	SessionKickByGM,
	//ERROR_SESSION_KICK_MAP_SERVER_EXIT,	//	SessionKickMapServerExit,
	//ERROR_SESSION_KICK_ACCOUNT_RELOGIN,	//	SessionKickAccountRelogin
	//ERROR_SESSION_KICK_INIT_DATA_ERROR,	//	SessionKickInitDataError,
};
const int errorCodeSessionKickNb = sizeof(ErrorCodeStr_SessionKick) /sizeof(int);

const int ErrorCodeStr_Login[] = {1
//	ERROR_LOGIN_BEGIN						//	LoginBegin		= 0x00004000,
	//ERROR_LOGIN_SUCCESS,					//	LOGIN_SUCCESS,
	//ERROR_LOGIN_ERROR_VERSION_NOT_MATCH,	//	LOGIN_ERROR_VERSION_NOT_MATCH,
	//ERROR_LOGIN_ERROR_ACCOUNT_NOT_EXIST,	//	LOGIN_ERROR_ACCOUNT_NOT_EXIST,
	//ERROR_LOGIN_ERROR_CHARACTER_ONLINE,		//	LOGIN_ERROR_CHARACTER_ONLINE,
	//ERROR_LOGIN_ERROR_CHARACTER_NOT_EXSIT,	//	LOGIN_ERROR_CHARACTER_NOT_EXSIT
	//ERROR_LOGIN_ERROR_MAP_SERVER_OFFLINE,	//	LOGIN_ERROR_MAP_SERVER_OFFLINE,
	//ERROR_LOGIN_ERROR_SAMEACCOUNT_LOGIN_AT_THE_SAME_TIME,	//
	//ERROR_LOGIN_QUEUE,						//	LOGIN_QUEUE,
	//ERROR_LOGIN_UNKNOW_ERROR,
	//ERROR_LOGIN_ACCOUNT_LEVEL_LAWLESS,
	//ERROR_LOGIN_ACCOUNT_FREE_MAX_LEVEL_LIMIT,
	//ERROR_LOGIN_ACCOUNT_FREE_MAX_TIME_LIMIT,
	//ERROR_LOGIN_INVALID_IP,
	//UI_CHANGE_SERVER_NAME_USED,
	//WARNING_LIMITED_FREETIME_SUBSCRIBE, // expired
	//UI_SERVER_BUSY, // server busy
 //   ERROR_LOGIN_ERROR_DATA_CORRUPTION,      // LOGIN_ERROR_CLIENT_DATA_CORRUPTION
};
const int errorCodeLoginNb = sizeof(ErrorCodeStr_Login) /sizeof(int);

const int ErrorCodeStr_Char[]= {1
//	ERROR_CHARACTER_BEGIN,			//	CharacterBegin	= 0x
	//ERROR_CHAR_CREATE_SUCCESS,		//	CharCreateSuccess,
	//ERROR_CHAR_DELETE_SUCCESS,		//	CharDeleteSuccess, 
	//ERROR_CHAR_NOT_EXIST,			//	CharNotExist,
	//ERROR_CHAR_NAME_ALREADY_EXIST,	//	CharNameAlreadyExist
	//ERROR_CHAR_NAME_INVALID,		//	CharNameInvalid,
	//ERROR_CHAR_NUM_FULL,			//	CharNumFull,
	//ERROR_CHAR_MAIL_TO_SELF,		//  CharMailToSelf
	//UI_CHANGE_SERVER_WARNING_GUILD_LEADER,			//	CharMigrateGuildMaster,
	//UI_CHANGE_SERVER_WARNING_MAIL_AUCTION,			//	CharMigrateHasMailOrItem,
	//UI_CHANGE_SERVER_NAME_USED,						//	CharMigrateRename,
	//UI_CHANGE_SERVER_RENAME_FAILED,					//	CharRenameCanot,
};
const int errorCodeCharNb = sizeof(ErrorCodeStr_Char) / sizeof(int);

//ERROR_ITEM_BEGIN
//
//ERROR_QUEST_BEGIN
//
//ERROR_SPELL_BEGIN

const int ErrorCodeStr_Talent[] ={1
												//  SpellTalent
	//ERROR_TALENT_INVALID,						//	TalentInvalid,
	//ERROR_TALENT_INVALID_SKILL_SET,				//	TalentInvalidSkillset,
	//ERROR_TALENT_REQUIRE_MORE_CATEGORY_TALENTS,	//	TalentRequireMoreCategoryTalents
	//ERROR_TALENT_REQUIRE_PARENT_TALENT,			//	TalentRequireParentTalent,
	//ERROR_TALENT_BE_ACTIVED, 					//	TalentBeActived,
};
const int errorCodeTalentNb = sizeof(ErrorCodeStr_Talent) / sizeof(int);

//ERROR_WARNING_BEGIN
const int ErrorCodeStr_Warning[] ={
		0,
/*TODO:75
YourLevelTooLow,
*/
};
const int errorCodeWarningNb = sizeof(ErrorCodeStr_Warning) / sizeof(int);

const int ErrorCodeStr_Social[] ={1
////ERROR_SOCIAL_BEGIN,				//  SocialBegin
//ERROR_TARGET_IS_BUSY,				//	TargetIsBusy,
//ERROR_CANT_INVITE_NOW,				//	CantInviteNow,
//ERROR_INVITE_REFUSE,				//	InviteRefuse,
//ERROR_NOT_IN_TEAM,					//	NotInTeam,
//ERROR_NOT_TEAM_LEADER,				//	NotTeamLeader,
//ERROR_INVITER_IS_NOT_EXSIT,			//	InviterIsnotExsit
//ERROR_TEAM_NOT_IN_QUEUE,            //  YourTeamNotInQueueAnymore,
//ERROR_PLAYER_IN_TEAM,               //  PlayerIsInATeam
//ERROR_TEAM_IS_FULL,                 //  TeamIsFull
//ERROR_CANT_ADD_YOURSELF,			//	CantAddYouself,
//ERROR_PLAYER_ALREADY_EXIST_,		//	PlayerAlreadyExist
//ERROR_LIST_IS_FULL_,				//	ListIsFull,
//
//ERROR_CAN_NOT_DUEL_IN_AREA,         //  CanNotDuelInArea
//ERROR_CAN_NOT_ATTACK_YOURSELF,		//	CanNotAttackYouself
//ERROR_DUEL_REQUEST_REFUSED,			//	CanNotFindTarget,
//ERROR_TOO_FAR_AWAY,					//	DuelRequestRefused,
//ERROR_CAN_NOT_FIND_TARGET,			//	TooFarAway,
//ERROR_ALREADY_IN_DUEL,				//	AlreadyInDuel,
//ERROR_PLAYER_BUSY,
//
//									
//ERROR_PLAYER_BUSY,					//InvalidArenaType,
//ERROR_PLAYER_BUSY,					//InvalidMemberCount,
//ERROR_PLAYER_BUSY,					//AlreadyInArenaQueue,
//UI_DUNGEON_SUMMON_FAILED_TARGET_LEVEL_LOW,	//TargetLevelTooLow
									
};
const int errorCodeSocialNb = sizeof(ErrorCodeStr_Social) / sizeof(int);


//ERROR_INTERACTIVE_TOO_FAR,			//	
//ERROR_INTERACTIVE_INVALID,			//
//ERROR_CHAT_MENU_INVALID,			//
									
		//ERROR_CHAT_BEGIN
const int ErrorCodeStr_Creature[] ={1
	//ERROR_INTERACTIVE_TOO_FAR,			//	
	//ERROR_INTERACTIVE_INVALID,			//};							
	//ERROR_CHAT_MENU_INVALID,			////#define ERROR_DESC(x) return #x;	
};

const int errorCodeCreatureNb = sizeof(ErrorCodeStr_Creature) / sizeof(int);

const int ErrorCodeStr_PlayerTrade[] = {1
	//UI_TRADE_REPORT_OFFLINE,
	//UI_TRADE_REPORT_TRADING,
	//UI_TRADE_REPORT_SUCCESSFUL,
	//UI_TRADE_REPORT_FAILED,
};
const int errorCodePlayerTradeNb = sizeof(ErrorCodeStr_PlayerTrade) / sizeof(int);



const int ErrorCodeStr_Dungeon[] = {1
	//UI_DUNGEON_RESET_FAILED,
	//UI_DUNGEON_RESET_SUCCESS,
	//UI_DUNGEON_START_FAILED,
	//UI_DUNGEON_IS_FULL,
	//UI_DUNGEON_MENU_CANNOT_USE,
	//TODO:75 wait text
//	,
//	,
};
const int errorCodeDungeonNb = sizeof(ErrorCodeStr_Dungeon) / sizeof(int);



//const char* GetErrorMessage(GErrorCode code)
//{
//	if( ItemBegin < code && code < InvErrorEnd )
//	{
//		return GetInventoryErrorMsg((InventoryError)code);
//	}
//	if( SpellBegin < code && code < SKILL_FAILED_NUM)
//	{
//		return GetSkillErrorMsg((SkillCastError) code);
//	}
//
//	if( code > SessionHandleBegin && code <= SessionHandleBegin + errorCodeSessionHandleNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_SessionHandle[code - SessionHandleBegin  - 1]);
//	}
//
//	if( code >= SessionKickNormal && code < SessionKickNormal + errorCodeSessionKickNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_SessionKick[code - SessionKickNormal]);
//	}
//
//	if( code > LoginBegin && code <= LoginBegin + errorCodeLoginNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_Login[code - LoginBegin  - 1]);
//	}
//
//	if( code > CharacterBegin && code <= CharacterBegin + errorCodeCharNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_Char[code - CharacterBegin  - 1]);
//	}
//
//	if( code > SpellTalent && code <= SpellTalent + errorCodeTalentNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_Talent[code - SpellTalent  - 1]);
//	}
//	if( code > WarningBegin && code <= WarningBegin + errorCodeWarningNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_Warning[code - WarningBegin  - 1]);
//	}
//	if( code > SocialBegin && code <= SocialBegin + errorCodeSocialNb)
//	{
//		return CStringManager::GetString(ErrorCodeStr_Social[code - SocialBegin  - 1]);
//	}	
//	
//	if( code > CreatureBegin && code <= CreatureBegin +  errorCodeCreatureNb )
//	{
//		return CStringManager::GetString(ErrorCodeStr_Creature[code - CreatureBegin  - 1]);
//	}
//
//	if( code > PlayerTradeBegin && code <= PlayerTradeBegin +  errorCodePlayerTradeNb )
//	{
//		return CStringManager::GetString(ErrorCodeStr_PlayerTrade[code - PlayerTradeBegin  - 1]);
//	}
//	
//	if( code > DungeonBegin && code <= DungeonBegin +  errorCodeDungeonNb )
//	{
//		return CStringManager::GetString(ErrorCodeStr_Dungeon[code - DungeonBegin  - 1]);
//	}
//
//	switch( code )
//	{
//	case CantFindPlayer:	
//		//return CStringManager::GetString(ERROR_CANT_FIND_PLAYER);
//	case YouLoginFul:
//		//return CStringManager::GetString(UI_MSG_YOUR_LOG_FULL);
//	case ComsumeFailed:
//		//return CStringManager::GetString(WORNING_NOT_ENOUGH_STONE);
//	case ErrItemCannotUseInArena:
//		//return CStringManager::GetString(UI_ARENA_ITEM_CANNOT_USE);
//	case CanNotDoItInArena:
//		//return CStringManager::GetString(UI_ARENA_MENU_CANNOT_USE);
//	default:
//#ifdef _DEBUG
//		return "Error: Unknown Error.";//CStringManager::GetString(ERROR_UNKNOWN_ERROR);
//#else
//		return "";
//#endif
//	}
//}

//#include "XPlayer_defines.h"
/// in order not to include xplayer head file, so redefine below err code
enum GLLiveErrorCode
{
	GLLIVE_ERROR_NOT_REGISTERED             = 4,   
	GLLIVE_ERROR_NO_RESPONSE                = 27,
	GLLIVE_ERROR_NO_USERNAME                = 46,
	GLLIVE_ERROR_NO_PASSWORD                = 47,
	GLLIVE_ERROR_INVALID_USERNAMEORPASSWORD = 49,
	GLLIVE_ERROR_USERNAME_NOTEXISTS         = 67,
	GLLIVE_ERROR_REQUEST_TIME_OUT           = 41,
	GLLIVE_ERROR_USER_IN_BANNED_LIST        = 206,
	GLLIVE_ERROR_CONNECTION                 = -2,
};


//const wchar_t s_gliveErrorStr[128] = { 0 };
//const char* GetGLLiveErrorMessage(int func_id, int err_code, char* pStr/*=NULL*/, int len/*=0*/)
//{
//	if( err_code > 9995 ) err_code -= 10000;
//	int strid = -1;
//	switch( err_code )
//	{
////err response of login
//	case GLLIVE_ERROR_NOT_REGISTERED:
//		//strid = ERROR_LOGIN_INVALID_ACCOUNT;
//		break;
//	case GLLIVE_ERROR_NO_RESPONSE:
//		//strid = ERROR_LOGIN_NO_NETWORK;
//		break;
//	case GLLIVE_ERROR_NO_USERNAME:
//		//strid = ERROR_LOGIN_NO_USERNAME;
//		break;
//	case GLLIVE_ERROR_NO_PASSWORD:
//		//strid = ERROR_LOGIN_NO_PASSWORD;
//		break;
//	case GLLIVE_ERROR_INVALID_USERNAMEORPASSWORD:
//		//strid = ERROR_LOGIN_INVALID_PASSWORD;
//		break;
////	case ERROR_LOGINED:	
//	case GLLIVE_ERROR_USERNAME_NOTEXISTS:	
//		//strid = ERROR_LOGIN_INVALID_ACCOUNT;
//		break;
//	case GLLIVE_ERROR_USER_IN_BANNED_LIST:
//		//strid = UI_ACCOUNT_BANNED;
//		break;
////	case func_id,ERROR_NO_VERSION:
//
////other response
//	case GLLIVE_ERROR_REQUEST_TIME_OUT:
//		//strid = ERROR_LOGIN_NO_NETWORK;
//		break;
//	case GLLIVE_ERROR_CONNECTION:
//		//strid = ERROR_CONNECT_LOST;
//		break;
//
//	default:	
//		if( pStr != NULL )
//		{
//			//strid = ERROR_GL_LOGIN_UNKNOW_ERROR;
//		}
//	}
//	//if( strid < 0 ) strid = ERROR_GL_LOGIN_UNKNOW_ERROR;
//
//	const char * pRawErrStr = CStringManager::GetString(strid);
//	if( pStr == NULL ) return pRawErrStr;
//
//	if( strstr( pRawErrStr, "%s" ) != NULL )
//	{
//		char tmp[100] = {0};
//		snprintf( tmp, sizeof(tmp)-1, "%d-%d", func_id, err_code );
//		snprintf( pStr, len, pRawErrStr, tmp );
//		return pStr;
//	}
//	return pRawErrStr;
//}