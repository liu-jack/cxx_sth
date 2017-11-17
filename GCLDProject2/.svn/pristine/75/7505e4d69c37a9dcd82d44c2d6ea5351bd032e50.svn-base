#ifndef __BASE_GAMEDEF_OBJECTDEFINES_H__
#define __BASE_GAMEDEF_OBJECTDEFINES_H__

// card def
static const uint32 MAX_CHARACTER_LEVEL    = 156;
static const uint32 CHARACTER_VALUES_COUNT = 12;

// item def
static const uint32 MAX_ITEM_LEVEL = 60;
static const uint32 MIN_ITEM_LEVEL = 1;
static const uint32 MAX_ITEM_REINFORCE = 60;
static const uint32 MIN_ITEM_REINFORCE = 0;



static const uint32 MIN_CHAR_LEVEL = 1;
static const uint32 MIN_CHAR_QUALITY = 1;
static const uint32 MIN_CHAR_REINFORCE = 1;



enum CurrencyType
{
	eCoin                   =  0, // 银币
	eSysGold                =  1, // 系统金币		对于所有需要扣除金币的操作，统一用这个做判断，接口中自动处理
	eIron					=  2, //铁矿
	eFood					=  3,  //粮食
	eWood					=  4,  //木头
	eHornor                 =  5, // 荣誉点   //作废，
	eBuildHornor            =  6, // 建筑荣誉点
	eTraining               =  7,//练兵令
	eTroopsNum              =  8,//增兵次数
	eFreeCollectNum         =  9,//免费征收次数
	eSuperCollectNum        =  10,//超级征收次数
	eTroopsOder             =  11, // 征兵令
	eGoldHammer             =  12, // 黄金锤
	eCoupon					=  13, // 点券
	eMarketBuyNum           =  14, // 集市购买次数
	eFreePurify             =  15, // 免费洗练
	eExtrmePurify           =  16, // 至尊洗练
	eGodStonePurify         =  17, // 神石洗练
	eCashGold				=  18, // 现金金币
	ePlayerExp              =  19, // 玩家经验
	eVipExp					=  20, // vip经验
	eDailyScore				=  21, // 每日任务积分
	eTrainingFight			=  22,
	eTrainingDouble			=  23,
	eTrainingReduce			=  24,
	ePeopleLoyal			=  25,//民忠
	eZhengWu			    =  26,//政务
	eRenown					=  27,//名望
	eStarCrystal			=  28,//星耀晶
	eBoard                  =  29,//集成板
	eGeneCode               =  30,//基因码
	eBeef					=  31,//牛肉
	eWine					=  32,//美酒
	eCurrencyNum
};

inline bool IsValidCurrencyType( int32 type )
{
    return type >= eCoin && type < eCurrencyNum ;
}

enum CHAR_CLASS
{
    Tank = 0,
    DPSer = 1,
    Healer = 3,
};


enum CHAR_ATTR 
{
    UNIT_TROOPS = 0,//兵力
    UNIT_ATTACK,	//攻击力
    UNIT_DEFENSE, //防御力
    UNIT_PUNCTURE,//
    UNIT_PARRY, //
    UNIT_BROKEN_ARRAY, //
    UNIT_IMPECCABLE, //
    UNIT_TONG, //
    UINT_YONG, //
    UA_OUT_DAMAGE_FACTOR, //
    UA_IN_DAMAGE_FACTOR, //
};


enum UNIT_GENDER
{
	UNIT_MALE   = 0, // 男
	UNIT_FEMALE = 1, // 女
	UNIT_GENDER_COUNT
};

#define PLAYER_FIELD_BUSINESS_SKILL_GATE_ELEMENT 1000
#define PLAYER_FIELD_BUSINESS_SKILL_LEVEL_UP_MUL 0.8f
#define PLAYER_FIELD_BUSINESS_SKILL_VALID_DIF 200
enum InteractiveDisableFlag
{
	eDisableNone		= 0,
	eDisableQuest		= 0x00000001,
	eDisableChat		= 0x00000002,
	eDisableAll			= eDisableQuest | eDisableChat,
};

// GM等级
enum GM_LEVEL
{
	GM_LEVEL_NONE = -1,
	GM_LEVEL_0,
	GM_LEVEL_1,
	GM_LEVEL_2,
	GM_LEVEL_COUNT,
};


#define PATH_MAX_POLY_COUNT 32
#define MIN_DIST_TO_BE_SAME 0.02f

#define SS_HEART_BEAT_INTERVAL_TIME 600 // 服务器之间心跳包存活间隔时间
#define CS_HEART_BEAT_INTERVAL_TIME 60 // 客户端服务器心跳包存活间隔时间

#define FLOAT_CAST(value)                       (    *((float*)&value)  )
#define INT32_CAST(value)                       (    *((int32*)&value)  )

// Type Defines
enum OBJECT_TYPE
{
	TYPE_OBJECT            = 0,
	TYPE_LOCATABLE         = 1,
	TYPE_ITEM              = 2,
	TYPE_UNIT              = 3,
	TYPE_STATIC_OBJECT     = 4,
	TYPE_DYNAMIC_OBJECT    = 5,
	TYPE_CORPSE            = 6,
	TYPE_CONTAINER         = 7,
	TYPE_AREA_TRIGGER      = 8,
	TYPE_PLAYER            = 9,
	TYPE_CREATURE          = 10,
	TYPE_ETHEREAL          = 11,

	//client only
	TYPE_HERO              = 12,
	TYPE_EFFECT            = 13,

	//pet
	TYPE_PET               = 14,

	//virtual item
	TYPE_VIRTUAL_ITEM      = 15,

	TYPE_MAP_HOOK          = 16,
	TYPE_DUNGEON_HOOK      = 17,
	TYPE_ARENA_HOOK        = 18,
	TYPE_BF_HOOK           = 19,

	TYPE_ARENA_TEAM        = 20,

	//bullet
	TYPE_BULLET            = 21,

	TYPE_MAIL              = 22,
    TYPE_EXCHANG_ORDER     = 23,
	TYPE_GUILD             = 24,


	OBJECT_TYPE_COUNT
} ;

enum OPEN_WAY_TYPE
{
	TYPE_NONE = 0,
	REGIST_WAY =1,
	MANAL_AN_TIME_WAY = 2,
};
// Replace End
#endif // __BASE_GAMEDEF_OBJECTDEFINES_H__
