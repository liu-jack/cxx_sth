#ifndef _COMBAT_BUFF_
#define _COMBAT_BUFF_
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"

namespace pb
{
	class Combat_Buff_AllInfo;
	class GS2C_Combat_Select_Tactic;
	class Combat_All_Buff;
}

namespace Combat {

enum BuffType		//buff类型，与buff_type对应
{
	None = 0,
	percentdeskillhurt = 8,		//降低技能造成的伤害
	fixedattackup = 15,			//固定值加攻击buff
	fixeddefenceup = 16,		//固定值加防御buff
	confuse = 29,				//混乱
	percentdamage = 30,			//百分比增加伤害
	fixeddamage = 31,			//固定增加伤害
	percentdefenceup = 34,		//百分比加防御buff
	percentattackup = 35,		//百分比加攻击buff
	dodge = 36,					//闪避
	disturb = 37,				//扰乱
	silence = 38,				//沉默
};

struct BuffInfo
{
	uint32 buffId;
	BuffType buffTyp;	//buff类型
	uint32 buffRemain;	//buff剩余持续作用回合数
	float  value1;
	float  value2;
	float  value3;
	uint64 fromP;	//玩家来源
	uint32 fromC;	//武将来源
	uint32 fromStProAttack;	//来源方的攻击属性
	uint32 fromStProLevel;	//来源方等级
	uint64 addBuffRound;	//添加buff时间，同权值先添加先生效 战斗回合数
	uint32 worktime;	//生效阶段 1：攻出手 2：守出手 3：移动后
	uint32 weight;		//生效权值
	bool isWork;		//是否生效 在挂载之后会有生效时机
	uint64 uniqueid;
	BuffInfo()
		:buffId(0)
		,buffTyp(None)
		,buffRemain(0)
		,value1(0.0)
		,value2(0.0)
		,value3(0.0)
		,fromP(0)
		,fromC(0)
		,fromStProAttack(0)
		,fromStProLevel(0)
		,addBuffRound(0)
		,worktime(0)
		,weight(0)
		,isWork(false)
		,uniqueid(0)
	{
	}
};

typedef std::map<uint64, BuffInfo*> FWorkBuffMap;	//按权值排好的buff

class FBuff
{
public:
	typedef std::multimap<uint32, BuffInfo> FBuffMap;
	FBuff();
	~FBuff();
public:
	bool HaveBuff(uint32 id) const; //根据buff id判断是否有该buff
	bool HaveBuffWork(BuffType typ) const; //根据bufftype判断是否有该buff	快速判断是否有混乱效果等
	void AddBuff(uint32 buffid, uint64 fromP, uint32 fromC, uint64 addBuffRound, uint32 fromStProAttack, uint32 fromStProLevel);	//添加buff，根据buff互相关系添加
	void DelBuff(uint32 buffid);	//根据buffid删除buff
	void DelBuff(BuffType typ);	//根据bufftype删除buff

	int GetFinalDefenceValue(int orgDefence); 
	int GetFinalAttackValue(int orgAttack);
	int GetFinalDamageAfterDodge(int damage);	//获得混乱后的伤害
	double GetFinalDeskillHurtPercent();


	void RefreshByRound();		//每回合结束刷新一次

	void RefreshByStep(uint32 step);	//每结算一次刷新一次

	bool IsBuffEmpty();

	void ClearAllBuff();

	const BuffInfo* GetBuffInfo(BuffType typ);	//根据type获得buff信息
	FWorkBuffMap GetFinalBuffMap();	//获得经过权值排序的buff列表

	//void FillBuffInfoMsg(pb::Combat_Buff_AllInfo& msg, uint32 startrow);
	//void FillBuffInfoMsg(pb::GS2C_Combat_Select_Tactic& msg, uint32 startrow);
	void FillBuffInfoMsg(pb::Combat_All_Buff& msg, bool isAtt, uint32 rowNum);
public:
	FBuffMap m_buff;  //<buffid,buffinfo>
	//FWorkBuffMap m_workBuff;	//<finalweight,buffinfo>
};

}

#endif