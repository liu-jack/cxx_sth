#pragma once
#include <queue>
#include "Base.h"
#include "combat_define.h"
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "../buff/Buff.h"
#include "CombatBuff.h"
#include <boost/smart_ptr.hpp>

class CMap;
namespace pb
{
    class Msg_CombatObj;
}
namespace Combat {
struct Property {
    int tong;           //统
    int yong;           //勇
    int attack;         //攻击
    int defence;        //防御
    int skillAttack;    //破阵，增加战法攻击力
    int skillDefence;   //无懈，增加战法防御力
    int attackEx;       //穿刺，攻击时附加伤害
    int defenceEx;      //格挡，被攻击降低伤害
    int rowSoldierMaxCnt;  //单排士兵总数

	Property(){ ZeroMemoryThis; }
	Property& operator=(const Property& other)
	{
		if(this == &other)
			return *this;
		else
		{
			tong = other.tong;
			yong = other.yong;
			attack = other.attack;
			defence = other.defence;
			skillAttack = other.skillAttack;
			skillDefence = other.skillDefence;
			attackEx = other.attackEx;
			defenceEx = other.defenceEx;
			rowSoldierMaxCnt = other.rowSoldierMaxCnt;
		}
		return *this;
	}
};

struct SoldierRaw
{
	int	hp;	//剩余血量
	bool isSoldierSelectTactic;
	bool isSoldierUseSkill;
	uint32 killerCharId;
	uint64 killerPlayerId;
	FBuff buff_fight;	//战斗类buff
	SoldierRaw();
	~SoldierRaw();
};

class CombatObj {
public:
    std::vector<SoldierRaw> soldiers; //每排士兵数，死光的移除
    uint8       soldierLv;		//士兵等级	只影响恢复兵力时消耗的粮食，因此只在玩家武将身上有作用，其他类型模型默认值为1
    Property    stPro; //战斗属性数据
    bool        isAutoFight;	//自动战斗
	bool		has_fighted;  //参加过战斗
public: //内存数据，无需入库		未实装TODO:数据加成后续完成

	/************************* 总加成数据 **************************/
	int		total_fightPercent;
	int		total_skillPercent;
	int     total_expPercent;
	int		total_reduceCostHP;
	int		total_doubleKill;
	int		total_deSkillPercent;


	int		 terrainPercent;	//地形加成
	int		 guardAddPercent;	//守卫军加成

	/***********  仅提供练兵令的加成，其他的加成不用这个  ***********/
    int      fightPercent; //战力提升百分之几
    int      skillPercent; //战法提升百分比
    int      expPercent;   //国战经验提高百分之几	武将与玩家均提高
	int		 reduceCostHP;	//恢复兵力减耗百分之几
	int		 doubleKill;	//双倍击杀百分比
	uint64	 trainUpStartTime;	//提升buff开始时间
	

	/***********  跨服的双倍击杀令  ***********/
	int		 crossDoubleKill;	//双倍击杀百分比


	/******** 科技加成记录在这里 *******/
	int		 keji_deskillPercent;//技能防御百分之几
	int		 keji_fightPercent;//科技提升战力百分比
	int		keji_expPercent;//科技经验提升百分比	武将与玩家均提高
	int		keji_skillPercent;//科技技能提升百分比


	/************ 固定数据 ***********/



    //每次战斗开始时清零的数据--------------------------------------------------------
    bool        isUsedSkill;	//是否使用技能	//现在仅作判断，是否已经释放了技能并且进入了下一轮
    bool        isSkillSuccess;	//技能是否成功使用
    bool        isInCombat;		//武将是否在战斗
    bool        isFighting;		//已上阵，前6排上阵
    bool        isSelectTactic;	//是否选择了战术	放弃使用，挂载在士兵身上
    TacticEnum  tactic;      //选择的战术
    int      deadCntOneLoop; //本轮死亡数
    int      killCntOneLoop; //本轮击杀数

	//武将统计 战斗开始时记录，死亡或战斗结束结算成实际奖励
    int      statisticExp;		//战斗获得的总经验		//武将增加经验
	int		 statisticExpPlayer;//玩家增加经验
    int      statisticMoney;	//战斗获得的总银币
    int      statisticDead;		//死亡数量
    int      statisticKill;		//杀敌数量
	int		 killOnceAHero;	//击杀对方一个武将造成的伤害 自动国战中，对一个击杀的目标造成的伤害
    uint32   oldCombatGroupUniqueID; //被诱敌时，旧战斗组id

	uint64	killerPID;	//杀手
	uint32	killerCID;	//杀手武将

	int		leftSkillTimes;		//剩余技能使用次数		//判断能否释放技能
	uint32	terrainType;		//obj所处地形地形

	uint32		 nowCombatGroupID;
	uint32		 combatType;

	int	m_maxHP;		//最大血量

	uint32 indexInCombat;		//在战斗队列中的序列
	bool   isAttactInCombat;	//在战斗队列中是否攻击方
	FBuff		m_fightBuff;		//战斗的buff

	bool isInTraining;		//是否在练兵中

	bool isLifeGuard;
	bool isExpedition;
	uint32 expeid;			//所属远征军的id

	IntPairVec m_extraAwards;//战斗中额外奖励
	
	CombatObjType m_objType;

protected:
    ::CMap*  m_pMap;
    CBuff       m_Buff;


public:
    CombatObj();
	virtual ~CombatObj(){};
    void SaveTo(pb::Msg_CombatObj& msg);
	bool IsPlayer(){ return (m_objType == Obj_Char || m_objType == Obj_FakeChar); }	//是否玩家	(武将或者是假武将)
    bool IsNpc(){ return m_objType == Obj_Npc; }		//是否npc
	bool IsReinforce() { return m_objType == Obj_Reinforce; }
	bool IsShadow() { return m_objType == Obj_Shadow; }
	bool IsFakeChar() { return m_objType == Obj_FakeChar; }
    bool IsDead();		//武将是否死亡
    double ForeHp();	//第一排血量
	int CurHp();		
	int HurtHp(int hurt);	//扣血
    void HurtPerHp(float percent); //当前血量百分比扣血
	bool AutoTactic();		//自动施放战术

    void SelectTactic(TacticEnum typ);	//选择战术

	void setRestHP(int restHp);		//设置剩余血量

	void SaveToGeneral(pb::Msg_CombatObj& msg);

	void resetTrainAdditionUp();		//重置练兵令造成的加成
	void TrainAddtionUpEnd();			//练兵令造成的加成结束

	//void reCalcPower();		//重新计算战力相关属性

	void SetObjType(CombatObjType type) { m_objType = type; }
public: // Buff
    void RefreshBuff();
    void AddBuff(uint32 buffId, uint32 lastMsec = 0);
    void DelBuff(uint32 buffId);
    bool HaveBuff(uint32 buffId) const;
    bool HaveBuff(CBuff::Type typ) const;
    void DelOneTypeBuff(CBuff::Type typ);
	TacticEnum RandTactic();			//随机一个战术
	bool isTacticLegal(TacticEnum typ);		//判断战术是否合法
	//int GetSkillOrTactic();		//选择战术还是技能，返回0则没有选择权力，返回1则为技能，返回2则为战术
	bool GetIsSelectTactic();		//第一排士兵是否选择了战术
	void SetIsSelectTactic();		//设置第一排士兵选择战术

	bool GetIsUseSkill();			//第一排士兵是否释放过技能	用于多次技能释放的时候一排只能放一次的检测
	void SetIsUseSkill();			//设置第一排士兵释放技能

	static TacticEnum TransIntToTactic(uint32 tacNum);	//将二进制战术转化为战术
	uint32 GetTacticAndSkills();			//获得所有技能与三个战术
	uint32 GetTacticsAndSkillsEnable();		//获得可以点击的技能或战术
	TacticEnum RandEnableTactic();		//获得随机战术
	TacticEnum GetKeZhiTactic(TacticEnum typ);	//获得克制战术
	TacticEnum GetBeiKeZhiTactic(TacticEnum typ);	//获得被克制战术
	TacticEnum GetFinalTacticOrSkill(bool canUseSkill, TacticEnum otherTac, uint32 aiLevel);	//获得最终的技能战术选择，传入是否能放技能，敌对战术，ai等级（0普通，1高级）

	bool HaveBuffWork(Combat::BuffType tpy);
public: // 虚方法
	virtual void    ClearCombatStatus();		//战斗结束后用于清理战斗中的局部属性，统计等属性不清理
	virtual void    OnDead(CombatObj* killer);
	virtual void    MoveTo(uint32 cityId);
	virtual void    InitCombatData();		//全部属性重置，用于初始化
	virtual uint64  GetPlayerId(); //npc返回内存池id
	virtual const string& GetPlayerName(){ static string s_name; return s_name; };
	virtual uint32  GetTableId() = 0;
    virtual float   SkillRatio() = 0;
    virtual uint8   SkillRange() = 0;
    virtual uint32  GetCountryId() = 0;
    virtual uint32  GetCurCity() = 0;
    virtual int32   GetLevel() const;
	virtual uint32	GetMaxRow() = 0;
	virtual void	InitCombatDataSecond(uint32 geography = 0);	//战斗开始属性重置，用于重置本次战斗会用到的局部属性
	virtual uint32	SkillID() = 0;
	virtual uint32	GetEnableTactics(uint32 geography = 0) = 0;	//获得在该地形下可以选择的战术，不包括技能
	//virtual	bool	IsShadow() = 0;
	virtual bool NewAutoTactic(CombatType cTyp = Combat::CT_Country, TacticEnum tTyp = Combat::Attack) = 0;	//智能获取随机战术	新方法

	/*virtual void reCalcPower() = 0;*/
	virtual int GetTotalFightPercent() = 0;
	virtual int GetTotalSkillPercent(const Combat::CombatType type,const uint32 obj_type = 0) = 0;
	virtual int GetTotalExpPercent(Combat::CombatType type ) = 0;
	virtual int GetTotalReduceCostHP() = 0;
	virtual int GetTotalDoubleKill(Combat::CombatType type) = 0;
	virtual int GetTotalDeSkillPercent() = 0;
	virtual void AddSkillPercentBySeigeArmySpecificId(int add_percent, int enemyType = 0) = 0;
	virtual uint32 GetType() const;	

	virtual uint32 GetTerrDebuffAddtion(uint32 terrType) = 0;	//获得冲撞城池克制加成
	virtual double GetTerrTalentAddtion(uint32 terrType) = 0;	//获得互砍地形天赋加成
	virtual uint32 GetPlayerLevel() = 0;
	virtual int GetMaxHp() = 0;
};

}
