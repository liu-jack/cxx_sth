#ifndef _GERROR_H_
#define _GERROR_H_

/*
add by szz
Gameplay Global Error Code Define.

*/

enum GErrorCode
{
	CommonSuccess	= 0,
	CommonUnknownError,
	CommonNoWarning,
	CommonError		= 0x00001000,
	CantFindPlayer,
	ComsumeFailed,

	NetCommandBegin				= 0x00002000,
	NetCommandUnhandled,
	NetCommandSuccess,

	SessionHandleBegin			= 0x00003000,
	SessionHandleKill,
	SessionHandleWarning,
	SessionHandleError,
	SessionHandleUnhandle,
	SessionHandleSuccess,
	SessionHandlePending,
	SessionHandleForward,

	SessionKickNormal			= 0x00003100,
	SessionKickByGM,
	SessionKickMapServerExit,
	SessionKickAccountRelogin,
	SessionKickInitDataError,
	SessionKickDataError,
	SessionKickTimeOut,
	SessionKickMaintenance,
	SessionKickHacker,

	OSP_BEGIN					= 0x00003200,
	OSP_SAVE_ERROR,

	LoginBegin		= 0x00004000,
	LOGIN_SUCCESS,
	LOGIN_ERROR_VERSION_NOT_MATCH,
	LOGIN_ERROR_ACCOUNT_NOT_EXIST,
	LOGIN_ERROR_CHARACTER_ONLINE,
	LOGIN_ERROR_CHARACTER_NOT_EXSIT,
	LOGIN_ERROR_MAP_SERVER_OFFLINE,
	LOGIN_ERROR_SAMEACCOUNT_LOGIN_AT_THE_SAME_TIME,
	LOGIN_QUEUE,
	LOGIN_ERROR,
	LOGIN_ERROR_ACCOUNT_LEVEL_LAWLESS,
	LOGIN_ERROR_ACCOUNT_FREE_MAX_LEVEL_LIMIT,
	LOGIN_ERROR_ACCOUNT_FREE_MAX_TIME_LIMIT,
	LOGIN_ERROR_INVALID_IP,
	LOGIN_ERROR_MUST_RENAME,
	LOGIN_ERROR_ACCOUNT_EXPIRED,
	LOGIN_ERROR_SERVER_BUSY,
    LOGIN_ERROR_CLIENT_DATA_CORRUPTION, 
	
	CharacterBegin	= 0x00005000,
	CharCreateSuccess,
	CharDeleteSuccess, 
	CharNotExist,
	CharNameAlreadyExist,
	CharNameInvalid,
	CharNumFull,
	CharMailToSelf,
	CharMigrateGuildMaster,
	CharMigrateHasMailOrItem,
	CharMigrateRename,
	CharRenameCanot,
	CharMigrateNotReady,
    CharChangeAppearanceSuccess,
    CharChangeAppearanceFailed,

	
	ItemBegin		= 0x00006000,

	QuestBegin		= 0x00007000,
	YouLoginFul,

	SpellBegin		= 0x00008000,
	SpellCastBegin  = SpellBegin,
	SkillBegin		= 0x00008500,
	SkillCastBegin  = SkillBegin,

	SpellTalent		= 0x00008900,
	TalentInvalid,
	TalentInvalidSkillset,
	TalentRequireMoreCategoryTalents,
	TalentRequireParentTalent,
	TalentBeActived,
	TalentNotEnoughPoints,

	WarningBegin	= 0x00009000,
	YourLevelTooLow,


	SocialBegin		= 0x0000A000,
	TargetIsBusy,
	CantInviteNow,
	InviteRefuse,
	NotInTeam,
	NotTeamLeader,
	InviterIsnotExsit,
	YourTeamNotInQueueAnymore,
	PlayerIsInATeam,
	YouTeamIsFull,
	TeamPermissionDenied,

	CantAddYouself,
	PlayerAlreadyExist,
	TheyTeamIsFull,

	CanNotDuelInArea,
	CanNotAttackYouself,
	CanNotFindTarget,
	DuelRequestRefused,
	TooFarAway,
	AlreadyInDuel,
	PlayerIsBusy,

	InvalidArenaType,
	InvalidMemberCount,
	AlreadyInArenaQueue,

	TargetLevelTooLow,
	InArenaCombat,

	YouGuildIsFull,
	PlayerNotOnline,
	PlayerIsInAGuild,
	TheyGuildIsFull,
	NotInGuild,
	NotGuildLeader,

	YouFriendIsFull,
	TheyFriendListIsFull,
	IsInYourInviteList,
	YourHaveInviteThePlayer,
	IsAreadyYouFriend,
	YouIgnoreIsFull,
	YouHaveNotFriendInfo,
	CantNotDelYouself,
	ThisPlayerIsNotExist,
	IsRepeatedInvitation,

	VitalityNotEnough,




	CreatureBegin   = 0x0000B000,
	INTERACTIVE_TOO_FAR,
	INTERACTIVE_INVALID,
	CHAT_MENU_INVALID,

	PlayerTradeBegin= 0x0000C000,
	Trade_Target_Offline,
	Trade_Target_Trading,
	Trade_Successful,
	Trade_Failed,

	GlAuthBegin = 0x0000D000,
	GlAuth_Successful,
	GlAuth_AccountNotExist,
	GlAuth_PwdWrong,

	DungeonBegin = 0x0000E000,
	DungeonResetFailed,
	DungeonResetSuccess,
	InstanceStartFailed,
	InstanceIsFull,
	CannotPerformInDungeon,
	DungeonEnd,

	CanNotDoItInArena = 0xF000,
};


enum SkillCastError
{
	SKILL_ERROR_NULL											 = SkillCastBegin,
	SKILL_FAILED_SUCCESS										 = SKILL_ERROR_NULL,
	SKILL_FAILED_UNKNOWN_CASTER,
	SKILL_FAILED_INVALID_TARGET,
	SKILL_FAILED_NOT_ENOUGH_POWER,
	SKILL_FAILED_TARGETS_DEAD,
	SKILL_FAILED_INVALID_SKILL,
	SKILL_FAILED_INTERRUPTED,
	SKILL_FAILED_GATHER_INTERRUPTED,
	SKILL_FAILED_SKILL_IN_PROGRESS,
	SKILL_FAILED_NOT_READY,
	SKILL_FAILED_OUT_OF_RANGE,
	SKILL_FAILED_NOT_FRONT,
	SKILL_FAILED_NEED_TARGET_BACK,
	SKILL_FAILED_INVALID_CASTER,
	SKILL_FAILED_NEED_TARGET,
	SKILL_FAILED_NO_RESPONSE,
	SKILL_FAILED_NOT_LEARNED,
	SKILL_FAILED_CANT_CAST_PASSIVE,
	SKILL_FAILED_NOT_IN_WORLD,
	SKILL_FAILED_CANT_CAST_IN_CURRENT_AREA,
	SKILL_FAILED_NO_MESSAGE,
	SKILL_FAILED_DISABLE_IN_COMBAT,
	SKILL_FAILED_NEED_MAINHAND_WEAPON,
	SKILL_FAILED_NEED_OFFHAND_WEAPON,
	SKILL_FAILED_NEED_RANGED_WEAPON,
	SKILL_FAILED_NOT_ENOUGH_MATERIAL,
	SKILL_FAILED_REPETITION_LEATN_SKILL,
	SKILL_FAILED_BAG_FULL,
	SKILL_FAILED_CANNOT_LEARN_SKILL,
	SKILL_FAILED_SILENCED,
	SKILL_FAILED_NOT_ENOUGH_MORALE,
	SKILL_FAILED_NOT_ENOUGH_VIGOR,
	SKILL_FAILED_OUT_OF_CTRL,
	SKILL_FAILED_CANNOT_CAST_IN_ARENA,
	//SKILL_FAILED_IMMUED,
	SKILL_FAILED_NUM,
};

enum ItemOperateError
{
	ITEM_OP_LEVEL_ERROR           = 1, // 等级不足
	ITEM_OP_RECE_ERROR            = 2, // 种族不符
	ITEM_OP_CLASS_ERROR           = 3, // 职业不符
	ITEM_OP_UNIQUE_ERROR          = 4, // 唯一性问题
	ITEM_OP_TARGET_ERROR          = 5, // 找不到操作的物品
	ITEM_OP_OPERATE_ERROR         = 6, // 该物品不能执行这个操作
	ITEM_OP_PROPERTY_ERROR        = 7, // 属性不足
	ITEM_OP_COOLDOWN_ERROR        = 8, // 冷却中
	ITEM_OP_PART_SIZE_ERROR       = 9, // 栏目容量不足
	ITEM_OP_HAD_EXPIRE_ERROR      =10, // 该物品已经过期
	ITEM_OP_SHORCUT_INDEX_ERROR   =11, // 快捷栏索引不对
	ITEM_OP_NO_TARGET_OBJ         =12, // 没有选中Npc
	ITEM_OP_TARGET_OBJ_ID         =13, // 选定对象Id不对
	ITEM_OP_ITEM_PART_ERROR       =14, // 物品栏目错误
	ITEM_OP_ITEM_SLOT_ERROR       =15, // 物品位置错误
	ITEM_OP_ITEM_SELL_ID_ERROR    =16, // 销售ID，找不到对应物品
	ITEM_OP_ITEM_MONEY_LESS       =17, // 金额不足( 会在消息的附加值哪里返回需要的类型 )
	ITEM_OP_ITEM_LESS             =18, // 物品不够( 附加值返物品原形Id )
	ITEM_OP_BUY_BACK_LIST_EMPTY   =19, // 回购列表为空
	ITEM_OP_BUY_REMIAN_ITEM_COUNT =20, // 剩余数量不足( 附加值返回剩余数量 )
	ITEM_OP_CAN_NOT_SELL          =21, // 不能出售
	ITEM_OP_HOLD_UNIQUE           =22, // 持有唯一，已经持有一个了
	ITEM_OP_SPAWN_ID_ERROR        =23, // 对象spawn id 不对
	ITEM_OP_NO_COTAIN_IN_ZONE     =24, // 不在指定区域内
	ITEM_OP_NO_DATABASE_DATA      =25, // 数据库中找不到可用数据
	ITEM_OP_NEED_IDENTIFY         =26, // 使用前需要鉴定
	ITEM_OP_RECV_DATA_ERROR       =27, // 收到数据错误
	ITEM_OP_NEED_UNLOCK           =28, // 使用前需要解锁
	ITEM_OP_BUSINESS_LEVEL_LOW    =29, // 商业技能等级不足
	ITEM_OP_FORMULA_LEARNED      =30, // 配方已学习
	ITEM_OP_BUSINESS_NOT_ACTIVE   =31, // 学科未激活
	ITEM_OP_ERROR_COUNT
} ;


enum InventoryError
{
	InvErrorOK						= ItemBegin,
	ErrInvIsFull,
	ErrSlotNotEmpty,
	ErrNotEnough,
	ErrInvalidInv,
	ErrInvalidProto,
	ErrInvalidSpell,
	ErrInvalidSlot,
	ErrInvalidOwner,
	ErrUseNotReady,
	ErrUseCant,
	ErrUseAlreadyHaveQuest,
	ErrVenderTooFar,
	ErrVenderInvalidItem,
	ErrVenderInvalidVender,
	ErrVenderInvalidSlot,
	ErrNotEnoughMoney,
	ErrExchangeUnexpectSrcItem,
	ErrEquipCant,
	ErrItemNotFound,
	ErrNotEnoughSpecialCurrency,
	ErrItemBinded,
	ErrItemUnique,
	ErrItemEquipUnique,
	ErrItemExpire,
	InvErrorEnd,
	RuneStoneOK,
	RuneStoneNotEnough,
	RuneStoneOtherFailed,

	// for lotto box
	ItemSendToBag,	
	ItemSendByMail,

	ErrItemCannotUseInArena,

	ErrItemPriceChange,
    ErrItemPriceInternalError,
	
	ErrNotEnoughExchangeItem,
};

enum RuneConsumptionError
{
	RUNE_CONSUMPTION_UNKNOWN = 0,
	RUNE_CONSUMPTION_OK,
	RUNE_CONSUMPTION_NOT_ENOUGH,
	RUNE_CONSUMPTION_OSP_ERROR,
};

enum InAppPurchaseBuyError
{
	INAPPPURCHASE_BUY_OK,
	INAPPPURCHASE_BUY_CANCEL,
	INAPPPURCHASE_BUY_FAIL,
};

//const char* GetSkillErrorMsg(SkillCastError error);
//const char* GetInventoryErrorMsg(InventoryError error);
//const char* GetErrorMessage(GErrorCode code);
//
///// gllive error
///// @pStr: when unknown error occurred, func_id and err_code are included in msg, and the space is in str
//const char* GetGLLiveErrorMessage(int func_id, int err_code, char* pStr=0, int len=0 );


#endif
