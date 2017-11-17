#ifndef _Stage_define_h_
#define _Stage_define_h_


enum  stage_model_type
{
	STAGE_NORMAL,   // ��ͨģʽ
	STAGE_ELITE,    // ��Ӣģʽ
	STAGE_BOSS,     // bossģʽ
    STAGE_GOLD,     // goldģʽ
    STAGE_NIGHTMARE,  // ��ʱ����
    STAGE_TYPE_COUNT,
};

enum StageLevelelTypeMainType
{
    STAGE_LEVEL_TYPE_MAIN_NORMAL_MODE   = 0 , // ��ͨģʽ
    STAGE_LEVEL_TYPE_MAIN_ARENA_MODE    = 1 , // ������ģʽ
    STAGE_LEVEL_TYPE_MAIN_ENDLESS_MODE  = 2 , // �޾�ģʽ
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
	STAGE_UNLOCK_FOR_STAGE,//�½�
	STAGE_UNLOCK_FOR_GRADE,//�ȼ�
	STAGE_UNLOCK_FOR_FIGHTING,//ս����
};

enum level_unlock_type
{
	LEVEL_UNLOCK_FOR_NONE      =-1,//û����
	LEVEL_UNLOCK_FOR_LEVEL     =0,//�ؿ�
	LEVEL_UNLOCK_FOR_GRADE     =1,//�ȼ�
	LEVEL_UNLOCK_FOR_FIGHTING  =2,//ս����
};

#define  TIME_LONG_DAY 86400
#define  TIME_LONG_WEEK 604800
#define  TIMT_LONG_PER_HOUR (60*60)

#endif // !_Stage_define_h_

