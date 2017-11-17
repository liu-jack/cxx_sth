#pragma once
#include "combat_define.h"
#include <list>
#include "def/TypeDef.h"
#include "Combat.pb.h"
#include "Combat/CombatObj.h"

class Character;
namespace pb
{
	class GS2C_Combat_Select_Tactic;
	class Combat_Damage;
	class Combat_Reward;
	class Combat_Buff_AllInfo;
	class Combat_Seque;
}

namespace Combat{

class CombatObj;
class CombatGroup;
//class Player;
//计算战斗中应考虑玩家的科技水平
class Calc {
public:
    static Calc& Instance(){ static Calc T; return T; }

    //战斗过程相关的计算------------------------------------------------------
    void Attack(CombatGroup* pGroup, CombatObj* attacker, CombatObj* defender, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy);
	//void UseSkill(GroupLst& attacker, GroupLst& defender, bool isAllUseSkill, pb::GS2C_Combat_Select_Tactic& msg);
	void UseSkill(CombatGroup* pGroup, GroupLst& attacker, GroupLst& defender, bool isAttakcerUseSkill, bool isDefenderUserSkill, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy);
	bool CheckDead(CombatGroup* pGroup, CombatObj* obj, CombatObj* killer, pb::GS2C_Combat_Select_Tactic& msg, bool isKillerAtt);	//常规伤害检测
    //bool CheckDead(GroupLst& lst);	//仅检查第一排 RefreshSoldiers
	bool CheckDead(CombatGroup* pGroup, CombatObj* obj, CombatObj* killer, pb::Combat_Buff_AllInfo& msg, bool isKillerAtt);	//buff伤害检测

	//阶段奖励结算 不写入数据库，只做通知用
	void CalcAwardsOneStep(CombatGroup* pGroup,CombatObj& attacker,CombatObj* defender, pb::Combat_Reward* awards, CombatType typ, int step,int damageone, int damagetwo = 0);

	//返回是否结束当前回合
	bool _BuffHurt(CombatGroup* pGroup,GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg, CombatType tpy);

	void RefreshSoldiers(GroupLst& lst, CombatObj* killer, pb::GS2C_Combat_Select_Tactic& msg, bool isKillerAtt);	//buff后刷新
	void RefreshSoldiers(GroupLst& lst, CombatObj* killer, pb::Combat_Buff_AllInfo& msg, bool isKillerAtt);	//常规伤害后刷新
	void _GetBuffs(GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg);
	void _GetBuffs(GroupLst& attackers, GroupLst& defenders, pb::GS2C_Combat_Select_Tactic& msg);
private:
    float CommonRatio(TacticEnum typ1, TacticEnum typ2); //战术加成
    float CrashRatio(TacticEnum typ1, TacticEnum typ2);
    bool  HaveCrashAddition(TacticEnum typ1, TacticEnum typ2);
private:
	void AttachBuffs(GroupLst& attacker, GroupLst& defender, uint32 attidx);	//附加buff
    void _SkillHurt(CombatGroup* pGroup,CombatObj& attacker, GroupLst& lst, int defender, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy);	//defender受伤方，1位攻击方受伤，2位防守方受伤
    int _CalcCrashHurt(CombatObj& attacker, CombatObj& defender);
    int _CalcCommonHurt(CombatObj& attacker, CombatObj& defender, float tacticRatio);

    void _DoHurt(CombatGroup* pGroup,CombatObj& attacker, CombatObj& defender, int attackerHurt, int defenderHurt, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy, int step);

//新战斗计算
public:
	bool CheckDeadNormal(CombatObj* checkobj);	//检测是否死亡  只检测第一行是否死亡

	//一次伤害的奖励结算  (战斗组，攻方，守方，伤害阶段，攻方对守方伤害，守方对攻方伤害)  （对撞，护砍阶段用）
	void _GetAwardsWithOneStep(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt = 1, int realdProDef = 1);
	//一次伤害结算中一侧对另一侧的计算，不分攻守方
	void _CalcAwardsInOneStepBySide(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, CombatObj* attacker, CombatObj* defender, int attOrgHurtToDef, int defOrgHurtToAtt, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt, int realdProDef, bool isAttGetAward);
	//一次buff伤害奖励结算
	void _GetAwardsWithBuffHopOnce(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj* buffOwner, Combat::BuffInfo* buffOneInfo, uint32 realHurtOfBuff, bool isAttWork);

	//对撞
	void DoHurt_Crash(pb::Combat_Seque& msg, CombatGroup* pGroup);
	double _NewCrashHurt(CombatObj* attacker, CombatObj* defender, uint32 terrType);	//计算冲撞造成的伤害
	void _MakeHurtsAndAwardsCrash(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt);		//计算伤害与奖励 常规伤害 冲撞
	
	//使用技能
	void DoHurt_Skill(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType);
	void _MakeHurtAndAwardsSkill(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj& attacker, GroupLst& lst, pb::E_Combat_Serialize serType);
	void _AttachBuff(CombatGroup* pGroup, pb::E_Combat_Serialize serType);	//挂载buff
	void _BuffToRow(GroupLst& buffAttacher, CombatObj* buffSender, int buffid, int buffAttchRow, uint64 roundNum);	//buff作用到行
	
	//互砍
	void DoHurt_Cut(pb::Combat_Seque& msg, CombatGroup* pGroup);
	double _NewCutHurt(CombatObj* attacker, CombatObj* defender, uint32 terrType);	//计算护砍一次造成的伤害
	void _MakeHurtsAndAwardsCut(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt);

	//buff作用
	void DoHurt_Buff(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType);
	void _BuffWork(pb::Combat_Seque& msg, CombatGroup* pGroup,GroupLst& workLst, pb::E_Combat_Serialize serType, bool isAttWork);
	void _BuffRowWork(pb::Combat_Seque& msg, CombatGroup* pGroup,CombatObj* buffOwner, Combat::SoldierRaw& soldierRow, pb::E_Combat_Serialize serType, int rownum, bool isAttWork);	//单排士兵的buff起作用

public:
};

}
#define sCombatCalc Combat::Calc::Instance()