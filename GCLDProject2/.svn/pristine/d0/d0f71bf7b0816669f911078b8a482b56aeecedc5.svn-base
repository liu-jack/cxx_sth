#ifndef QuestDef_h__
#define QuestDef_h__

#include "def/TypeDef.h"

struct DB_QuestReward; 
typedef uint32 QuestID; 
typedef std::set< QuestID>  QuestIDSet;
typedef std::list<const DB_QuestReward*> RewardList;


enum EVENT_TYPE
{
    EVENT_TYPE_NORMAL = 0, //永久累计
    EVENT_TYPE_DAILY = 1, 
    EVENT_TYPE_COUNT,
};



// quest category
enum QuestCategory1
{
    QC1_PVE     = 0,
    QC1_PVP     = 1,
    QC1_EQUIP   = 2,
    QC1_ACC     = 3,    //饰品
    QC1_RUNE    = 4,    //符文
    QC1_SHOP    = 5,    //商店
    QC1_LOTTERY = 6,    //抽奖
    QC1_FRIEND  = 7,    //好友
    QC1_GUID    = 8,    //工会
    QC1_CHAR    = 9,    //角色
    QC1_PLAYER  = 10,   //玩家
    QC1_BUILDING = 11,  //建筑
    QC1_BAO_QI  = 12,   //宝器
    QC1_CITY    = 13,   //城池
    QC1_HERO    = 14,   //武将
    QC1_OPERATE = 15,   //操作
    QC1_TECHNOLOGY = 16,//科技
    QC1_WORLD_BOX = 17, //世界宝箱
    QC1_WORLD_MINES = 18, //世界矿场
	QC1_FIGHT = 19,   ///战斗
	QC1_WORLD_TEACH = 20,//世界教学
	QC1_GET_EXPLOIT	=22,//获得功勋
};
enum QuestCategory2
{
    QC2_UNDEFINED = 0,

    //pve
    QC2_FUBEN_STAGE = 0, //副本关卡
	QC2_FUBEN_SOURECE = 1,//资源副本
    //装备
    QC2_EQUIP_ITEM_ID = 0, //特定装备
    QC2_EQUIP_QUALITY = 2, //特定品质
    QC2_EQUIP_QUALITY_STAR = 3, //特定品质及以上、1星及以上装备
	QC2_EQUIP_CLASS =		4,// 特定类型
	QC2_EQUIP_QUALITY_AND_STARS = 5,//特定品质及以上，任意星级

    //建筑
    QC2_BUILDING_TYPE = 0, //建筑类型
	QC2_BUILDING_ID = 1, //建筑ID
    //宝器
    //城池
	QC2_ATTACK_CITY   = 0, //攻占城池
    //武将
	QC2_HERO_ANY	 =0,//任意武将
    QC2_HERO_TABLE_ID = 1, //武将tid

    //操作
    QC2_OPERATE_BUTTON = 0, //按钮tid
	QC2_OPERATE_NAMED = 1,//角色取名

    //科技
    QC2_TECHNOLOGY_TABLE_ID = 0, //科技tid

	QC2_KILL  = 0,
	QC2_TRICK= 1,
	QC2_ADD_SOLIDER = 2,

	QC2_MAIN_QUEST  =0,
	QC2_BRANCH_QUEST = 1,

    //世界宝箱
    //世界矿场

    QC2_ARENA = 0,
    QC2_SPECIFIC_ITEM = 0,
    QC2_ANY_ITEM = 1,

    QC2_BUY_SPECIFIC_ITEM = 0,
    QC2_BUY_IN_ORDINARY_SHOP = 1,
    QC2_BUY_IN_CHARACTER_SHOP = 2,
    QC2_BUY_IN_EQUIP_SHOP = 3,

    QC2_SILVER_LOTTO = 0,
    QC2_GOLD_LOTTO = 1,
    QC2_EVENT_LOTTO = 2,

    QC2_GIVE_FRIEN_AP = 0,

    QC2_CHAR_LEVEL_REACH = 0,
    QC2_CHAR_QUALITY_REACH = 1,
    QC2_CHAR_REINFORCE_REACH = 2,
    QC2_CHAR_SKILL_LEVEL_REACH = 3,


    QC2_PLAYER_VIP_LEVEL_REACH = 0,
    QC2_PLAYER_LEVEL_REACH = 1,
    QC2_PLAYER_COMBAT_POWER = 2,
    QC2_PLAYER_ACHIEVEMENT_POINT = 3,
    QC2_PLAYER_LOGIN_DAY = 4,
    QC2_PLAYER_CONTINUE_LOGIN_DAY = 5,
};
enum QuestCategory3
{
    QC3_UNDEFINED = 0,

    //pve
    QC3_FUBEN_PASS = 0, //副本通关

    //装备
    QC3_EQUIP_BUY = 0,  //购买
    QC3_EQUIP_WEAR = 1, //穿戴 OR 特定品质装备-- 获得

    QC3_EQUIP_GET = 0,  //特定品质装备 - 获得
	QC3_EQUIP_HAVE = 2,//特定品质装备----拥有

    //建筑
    QC3_BUILDING_LEVEL = 0, //建筑等级
    QC3_BUILDING_OUTPUT = 1, //建筑产量

    //宝器
    QC3_BAO_QI_LEVEL = 0, //宝器升级

    //城池
    QC3_CITY_GET = 0,   //攻占城池

    //武将
    QC3_HERO_MOVE = 0, //移动
    QC3_HERO_UNLOCK = 1,  //解锁
	QC3_HERO_ZHAOMU = 2,//招募

    //操作
    QC3_OPERATE_BUTTON_CLICK = 0, //按钮 - 点击
    QC3_OPERATE_BUTTON_CHANGE = 1, //按钮 - 交换

    //科技
    QC3_TECHNOLOGY_STUDY = 0,    //研究
    QC3_TECHNOLOGY_COMPLETE = 1, //完成
	QC3_TECHNOLOGY_UPSTAR = 2,//升星（注资）

    //世界宝箱
    QC3_WORLD_BOX_GET = 0,

    //世界矿场
    QC3_WORLD_MINES_OCCUPY = 0,

    QC3_SUCCESS_TIME = 0,
    QC3_GET_STAR_COUNT = 1,
    QC3_BRANCH_SUCCESS = 2,
    QC3_REACH_LAYER = 1,
    QC3_DO_PVP = 0,
    QC3_PVP_SUCCESS = 1,


    QC3_COMBINED = 0,
    QC3_LEVEL_UP = 1,
    QC3_REINFORCE_UP = 2,

    QC3_BUY = 0,
};

enum REWARD_TYPE
{
    REWARD_TYPE_EXP,
    REWARD_TYPE_CURR,
    REWARD_TYPE_ACHIEVE_POINT,
    REWARD_TYPE_ITEM,
	REWARD_TYPE_EQUIP,
    REWARD_TYPE_COUNT,
};

enum QUEST_GM_CMD
{
    QUEST_GM_ACCEPT,
    QUEST_GM_FINISH,
};

enum QUEST_EXP_REWARD_TYPE
{
	QUEST_REWARD_PLAYER_EXP,
	QUEST_REWARD_CHAR_EXP,
};



// 自动兼容 uint64
union BehaviorType
{
    friend bool operator < ( const BehaviorType &lhs, const BehaviorType &rhs );
    friend bool operator == ( const BehaviorType &lhs, const BehaviorType &rhs );
public:
    ~BehaviorType( void ) {}
    BehaviorType( void );
    BehaviorType( const uint64 &type );

    explicit BehaviorType( uint32 category1, uint32 category2, uint32 category3 = 0, uint32 condition = 0, uint32 eventType = 0); 

public:
    inline uint64& MutableUint64()      {    return m_behaviorType ;    }  
    inline uint64 GetUint64() const     {    return m_behaviorType ;    }

public:
    uint64 m_behaviorType ;
    struct
    {
        uint32 m_condition  :32; // 低
        uint32 m_cat3       :8;  // 
        uint32 m_cat2       :8;  // 
        uint32 m_cat1       :8;  // 高
        uint32 m_eventType  :8;
    };
    struct
    {
        uint32 m_condition_ :32; // 低
        uint32 m_counterType:32; // 
    } ;
} ;


bool operator < ( const BehaviorType &lhs, const BehaviorType &rhs );
bool operator == ( const BehaviorType &lhs, const BehaviorType &rhs );

#endif // QuestDef_h__

