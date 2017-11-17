#ifndef PLAYER_TABLE_H__
#define PLAYER_TABLE_H__

#include "../mysql_common/structDef.h"

namespace pb
{
    class ObjectFullValue;
    class ObjectValueUpdate;
    class GS2C_PlayerString;
}
//set byte alignment 
#pragma pack(push,1)

struct PlayerTable : public BaseStruct
{
public:
    typedef uint64 Key ;

public:
    INIT_GAMEDATA( PlayerTable ) ;

public:
    static const bool HasIndex()
    {
        return true;
    }

    const Key& GetKey( void ) const
    {
        return playerId ;
    } 

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "playerId" ;
    }

    static const char* GetTableName()
    {
        return "player";
    }

    static const char* GetFormat()
    {

        return "bbbbu"
            "uuuuuuuuuuuuuuuuuu"
            "uuuuuuuuuuuuuuuuuuuuuuuu"
			"uuuuuuuuuuuuuuuuuu"
            "sss"
            "tttttttttt"
			"ttbtttttt"
			"uuuuuuuuuu"
			"uuuuuuuuuu"
			"uuuuuuuuuu"
			"uuuuuuuuuu"
			"uuuuuuuuuu"
			"uuuuuuuuu";
    }

    void SaveTo( pb::ObjectFullValue& msg) const;
    void LoadFrom( const pb::ObjectFullValue& msg);
    bool LoadUpdateFrom( const pb::ObjectValueUpdate& msg);


    float* GetIndexFloatPtr( uint32 index);
    int32* GetIndexInt32Ptr( uint32 index);
    uint64* GetIndexUint64Ptr( uint32 index);
    std::string* GetIndexStringPtr( uint32 index);

    const float* GetIndexFloatCPtr( uint32 index) const;
	bool CheckNeedSyncSaveIndexUint64( uint32 index ) const;
	bool CheckNeedSyncSaveIndexInt32( uint32 index ) const;
	bool CheckNeedSyncSaveIndexString( uint32 index ) const;
	bool CheckNeedSyncSaveIndexFloat( uint32 index ) const;
	const int32* GetIndexInt32CPtr( uint32 index) const;
    const uint64* GetIndexUint64CPtr( uint32 index) const;
    const std::string* GetIndexStringCPtr( uint32 index) const;


public:
    IdType      id;   //8
    uint64      accountId; //8
    uint64      playerId;// 8
	uint64      guildId;//8 
	uint32      lockReason;// 4

	//18
    uint32      regionId;//4
    uint32      serverId;//4
	uint32      preserverId;//4
    uint32      slot;//4
	uint32      platformId;//4
	uint32      channelId;//4
	uint32      continLoginDay;//4
	uint32      flags;
	uint32      cardHeadIconId ;//玩家头像id
	uint32      guideProcess; // 新手引导
    uint32      level;
	uint32      vipLv;
	uint32      coin;
	uint32      sysGold; //系统金币
	uint32      iron;
	uint32      food;
	uint32      wood;
	uint32      honor;//

	//25
	uint32      buildingHonor;//建筑功勋值
    uint32      training;//练兵令
    uint32      troopsNum;//增兵次数
    uint32      freeCollectNum;//免费征收次数
    uint32      superCollectNum;//超级征收次数
    uint32      troopsOder; //征兵令
    uint32      goldHammer;
	uint32      coupon;//点券
	uint32      marketBuyNum ;//集市购买次数
	uint32      freePurify;//免费洗练
	uint32      extrmePurify;//至尊洗练
	uint32      godStonePurify;//神石洗练
	uint32      cashGold; //充值金币
	uint32      curExp;// 玩家经验
	uint32      vipExp; //会员经验
	uint32      dailyScore;//每日任务积分
	uint32      autoHammer;//自动建造锤
	uint32      login7DayRewardTakeCount;
	uint32      loginMonthlyRewardTakeCount;
	uint32      preBattleId;
    uint32      palaceLowOfficeId;
	uint32      panzerId;
	uint32      bagCapacity;
	uint32      isRegistCrossServer;
	uint32      isInCrossServer;
	
	//10
	uint32      isAutoIncreaseTroops;
	uint32      buyLabourTimesCurDay;
	uint32      curLabourNum;
	uint32      curRebuildId;
	uint32      isBeginRebuild;
	uint32      curPalaceRebuildRound;
	uint32      curRoundInvest1;
	uint32      curRoundInvest2;
	uint32      curRoundInvest3;
	uint32		herosExpUpPercent;	//武将经验加成

	//9
	uint32		exploitRankValue;//功勋值
	uint32		militarypower;//总的战力值
	uint32		exploitRankNormalbox;
	uint32		exploitRankAdvanceBox;
	uint32		exploitRankBoxLevel;
	uint32		trickRankValue;
	uint32		occupyRankValue;

    std::string name;
	std::string guildName;
	std::string sdkUserId;
	
	time_t      lastLogin;
	time_t      registTime;
	time_t      resetHornorTime; //下一次荣誉点重置时间
	time_t      resetMaketBuyNumTime; //下一次集市可购买次数重置时间
	time_t      resetFreePurifyTime; //免费洗练次数重置时间
    time_t      resetDailyScoreTime; //每日任务积分重置时间
	time_t      resetSmithyBuyNumTime;
    time_t      login7DayRewardTakeTime;
    time_t      loginMonthlyRewardTakeTime;
	time_t      curRebuildEndTime;
	time_t      palaceTakeSalaryTime;//官职，领取俸禄
	time_t      lockTime;
	uint64		killNum;			//杀敌数
	time_t		enter_world_time;//进入世界的时间
	time_t		buy_month_card_normal;//购买普通月卡
	time_t		buy_month_card_advance;//购买高级月卡
	time_t		take_month_card_normal;//领取普通月卡的奖励
	time_t		take_month_card_advance;//领取高级月卡的奖励
	time_t		congratulate_active_time; //恭贺激活时间

	uint32		SumDinnerNumber; //总的宴会次数
	uint32		DinnerNumber;//剩余的宴会次数
	uint32		trainingFight;//战斗练兵令
	uint32		trainingDouble;//翻倍练兵令
	uint32		trainingReduce;//减耗练兵令
	uint32		normalExploitboxLimit;//功勋榜普通宝箱上限
	uint32		advanceExploitboxLimit; //功勋榜高级宝箱上限
	uint32		draftOrderLimit;//征兵令上限
	uint32		troopsOrderLimit;//增兵令上限
	uint32		trickrewardstep;//诱敌榜已领的步数

	uint32		occupyrewardstep;//工程榜已领的步数
	uint32		is_advancebox;	//是不是高级宝箱
	uint32		characterSlotnum;//可以招募的武将数量
	uint32		createShadownum;//创建幻影的数量
	uint32		today_add_xp;//当天增加的经验
	uint32		vip_reward_take_flag;//领取vip等级奖励记录
	uint32		exploit_reward_step_id;//功勋宝箱领奖步数
	uint32		daily_quest_reward_take_flag;//领取每日任务的记录
	uint32		grow_up_quest_exploit_value;//成长任务的功勋值
	uint32		xilian_count;//洗练次数

	uint32		char_row_num;//武将带几排兵
	uint32		sign_up_days;//签到天数
	uint32		retroactive_days;//补签天数
	uint32		daily_sign_take_extra_reward_flag;//每日签到额外奖励
	uint32		activity_star_box_level;//异星宝箱等级
	uint32		use_free_quick_combat_times;//使用免费快速战斗的次数
	uint32		open_fog_count;//迷雾数量
	uint32		is_fog_take_reward;//迷雾是否领取奖励
	uint32		activity_xilian_count;//洗练活动次数
	uint32      is_xilian_take_reward;////洗练是否领奖

	uint32		iron_collect_times;//铁矿征收次数
	uint32		iron_collect_take_flag;//领取奖励的flag
	uint32		iron_collect_time_ten;//十次铁矿征收
	uint32		activity_xilian_ten;//洗炼活动十次
	uint32		people_loyal;//民忠
	uint32		zhengwu_times;//政务次数
	uint32		renown;//声望
	uint32		star_crystal;//星耀晶
	uint32		board;//集成板
	uint32		gene_code;//基因码

	uint32		visit_famous_take_reward_state;//结交名仕领取
	uint32		limited_shop_flag;///限时商店
	uint32      beef;				//牛肉
	uint32		wine;			//美酒
	uint32		reward_box_num;//reward——box 
	uint32		activity_gold_num;  //活动送金
	uint32		activity_recharge_take_flag;//充值送金的领取
	uint32		attack_city_num;//攻打城市的数量
	uint32		world_fight_take_reward_flag;//国战成就的领取宝箱情况
	uint32		cur_general_reward_char_id;//当前犒赏武将ID

	uint32		login_days;		//登录天数
	uint32		occupy_city_num;//占领城池数
	uint32		main_quest_exploit_value;//主线任务功勋值
	uint32		today_occupy_city;//今日攻城
	uint32		today_zhuzheng;//今日助阵
	uint32		today_zhugong;//今日助攻
	uint32		today_continue_occpuy_city;//今日连城数量
	uint32		activity_personal_occupy_city;//战场活动个人攻城
	uint32		activity_personal_add_soldier;//战场活动个人增兵
} ;



struct PlayerStringTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( PlayerStringTable ) ; 

    const Key& GetKey( void ) const
    {
        return id ;
    } 

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint32 GetLogicalKey( void) const
    {
        return str_type;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "str_type" ;
    }

    static const char* GetTableName()
    {
        return "player_string";
    }

    static const char* GetFormat()
    {
        return "bbus";
    }

    void SaveTo(pb::GS2C_PlayerString& msg);
    void LoadFrom(const pb::GS2C_PlayerString& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint32      str_type;
    string      str;
} ;



#pragma pack(pop)

#endif

