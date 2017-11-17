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
	eCoin                   =  0, // ����
	eSysGold                =  1, // ϵͳ���		����������Ҫ�۳���ҵĲ�����ͳһ��������жϣ��ӿ����Զ�����
	eIron					=  2, //����
	eFood					=  3,  //��ʳ
	eWood					=  4,  //ľͷ
	eHornor                 =  5, // ������   //���ϣ�
	eBuildHornor            =  6, // ����������
	eTraining               =  7,//������
	eTroopsNum              =  8,//��������
	eFreeCollectNum         =  9,//������մ���
	eSuperCollectNum        =  10,//�������մ���
	eTroopsOder             =  11, // ������
	eGoldHammer             =  12, // �ƽ�
	eCoupon					=  13, // ��ȯ
	eMarketBuyNum           =  14, // ���й������
	eFreePurify             =  15, // ���ϴ��
	eExtrmePurify           =  16, // ����ϴ��
	eGodStonePurify         =  17, // ��ʯϴ��
	eCashGold				=  18, // �ֽ���
	ePlayerExp              =  19, // ��Ҿ���
	eVipExp					=  20, // vip����
	eDailyScore				=  21, // ÿ���������
	eTrainingFight			=  22,
	eTrainingDouble			=  23,
	eTrainingReduce			=  24,
	ePeopleLoyal			=  25,//����
	eZhengWu			    =  26,//����
	eRenown					=  27,//����
	eStarCrystal			=  28,//��ҫ��
	eBoard                  =  29,//���ɰ�
	eGeneCode               =  30,//������
	eBeef					=  31,//ţ��
	eWine					=  32,//����
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
    UNIT_TROOPS = 0,//����
    UNIT_ATTACK,	//������
    UNIT_DEFENSE, //������
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
	UNIT_MALE   = 0, // ��
	UNIT_FEMALE = 1, // Ů
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

// GM�ȼ�
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

#define SS_HEART_BEAT_INTERVAL_TIME 600 // ������֮�������������ʱ��
#define CS_HEART_BEAT_INTERVAL_TIME 60 // �ͻ��˷����������������ʱ��

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
