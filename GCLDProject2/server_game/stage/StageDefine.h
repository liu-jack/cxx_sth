#ifndef _Stage_define_h_
#define _Stage_define_h_


enum  stage_model_type
{
	STAGE_NORMAL,   // 普通模式
	STAGE_ELITE,    // 精英模式
	STAGE_BOSS,     // boss模式
    STAGE_GOLD,     // gold模式
    STAGE_NIGHTMARE,  // 暂时无用
    STAGE_TYPE_COUNT,
};

enum StageLevelelTypeMainType
{
    STAGE_LEVEL_TYPE_MAIN_NORMAL_MODE   = 0 , // 普通模式
    STAGE_LEVEL_TYPE_MAIN_ARENA_MODE    = 1 , // 竞技场模式
    STAGE_LEVEL_TYPE_MAIN_ENDLESS_MODE  = 2 , // 无尽模式
    STAGE_LEVEL_TYPE_MAIN_BOSS_MODE     = 3 , // 
    STAGE_LEVEL_TYPE_MAIN_GOLD_MODE     = 4 , //

    STAGE_LEVEL_TYPE_COUNT, 
} ;

enum  level_type
{
	LEVEL_TYPE_NONE =-1,
	LEVEL_TYPE_SMALL =0,
	LEVEL_TYPE_BIG   =1,
};

enum level_reset_type
{
	PER_DAY,
	PER_WEEK,
	PER_MONTH,
	PER_3_HOUR,
    PER_4_HOUR,
    PER_6_HOUR,
    PER_12_HOUR,
};


enum level_consume_type
{
	CONSU_NONE         =0,
	CONSU_GOLDEN        =1,
};

enum stage_unlock_type
{
	STAGE_UNLOCK_FOR_STAGE,//章节
	STAGE_UNLOCK_FOR_GRADE,//等级
	STAGE_UNLOCK_FOR_FIGHTING,//战斗力
};

enum level_unlock_type
{
	LEVEL_UNLOCK_FOR_NONE      =-1,//没限制
	LEVEL_UNLOCK_FOR_LEVEL     =0,//关卡
	LEVEL_UNLOCK_FOR_GRADE     =1,//等级
	LEVEL_UNLOCK_FOR_FIGHTING  =2,//战斗力
};

#define  TIME_LONG_DAY 86400
#define  TIME_LONG_WEEK 604800
#define  TIMT_LONG_PER_HOUR (60*60)

#endif // !_Stage_define_h_

