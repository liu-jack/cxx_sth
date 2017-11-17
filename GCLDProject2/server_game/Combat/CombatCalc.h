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
//����ս����Ӧ������ҵĿƼ�ˮƽ
class Calc {
public:
    static Calc& Instance(){ static Calc T; return T; }

    //ս��������صļ���------------------------------------------------------
    void Attack(CombatGroup* pGroup, CombatObj* attacker, CombatObj* defender, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy);
	//void UseSkill(GroupLst& attacker, GroupLst& defender, bool isAllUseSkill, pb::GS2C_Combat_Select_Tactic& msg);
	void UseSkill(CombatGroup* pGroup, GroupLst& attacker, GroupLst& defender, bool isAttakcerUseSkill, bool isDefenderUserSkill, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy);
	bool CheckDead(CombatGroup* pGroup, CombatObj* obj, CombatObj* killer, pb::GS2C_Combat_Select_Tactic& msg, bool isKillerAtt);	//�����˺����
    //bool CheckDead(GroupLst& lst);	//������һ�� RefreshSoldiers
	bool CheckDead(CombatGroup* pGroup, CombatObj* obj, CombatObj* killer, pb::Combat_Buff_AllInfo& msg, bool isKillerAtt);	//buff�˺����

	//�׶ν������� ��д�����ݿ⣬ֻ��֪ͨ��
	void CalcAwardsOneStep(CombatGroup* pGroup,CombatObj& attacker,CombatObj* defender, pb::Combat_Reward* awards, CombatType typ, int step,int damageone, int damagetwo = 0);

	//�����Ƿ������ǰ�غ�
	bool _BuffHurt(CombatGroup* pGroup,GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg, CombatType tpy);

	void RefreshSoldiers(GroupLst& lst, CombatObj* killer, pb::GS2C_Combat_Select_Tactic& msg, bool isKillerAtt);	//buff��ˢ��
	void RefreshSoldiers(GroupLst& lst, CombatObj* killer, pb::Combat_Buff_AllInfo& msg, bool isKillerAtt);	//�����˺���ˢ��
	void _GetBuffs(GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg);
	void _GetBuffs(GroupLst& attackers, GroupLst& defenders, pb::GS2C_Combat_Select_Tactic& msg);
private:
    float CommonRatio(TacticEnum typ1, TacticEnum typ2); //ս���ӳ�
    float CrashRatio(TacticEnum typ1, TacticEnum typ2);
    bool  HaveCrashAddition(TacticEnum typ1, TacticEnum typ2);
private:
	void AttachBuffs(GroupLst& attacker, GroupLst& defender, uint32 attidx);	//����buff
    void _SkillHurt(CombatGroup* pGroup,CombatObj& attacker, GroupLst& lst, int defender, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy);	//defender���˷���1λ���������ˣ�2λ���ط�����
    int _CalcCrashHurt(CombatObj& attacker, CombatObj& defender);
    int _CalcCommonHurt(CombatObj& attacker, CombatObj& defender, float tacticRatio);

    void _DoHurt(CombatGroup* pGroup,CombatObj& attacker, CombatObj& defender, int attackerHurt, int defenderHurt, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy, int step);

//��ս������
public:
	bool CheckDeadNormal(CombatObj* checkobj);	//����Ƿ�����  ֻ����һ���Ƿ�����

	//һ���˺��Ľ�������  (ս���飬�������ط����˺��׶Σ��������ط��˺����ط��Թ����˺�)  ����ײ�������׶��ã�
	void _GetAwardsWithOneStep(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt = 1, int realdProDef = 1);
	//һ���˺�������һ�����һ��ļ��㣬���ֹ��ط�
	void _CalcAwardsInOneStepBySide(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, CombatObj* attacker, CombatObj* defender, int attOrgHurtToDef, int defOrgHurtToAtt, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt, int realdProDef, bool isAttGetAward);
	//һ��buff�˺���������
	void _GetAwardsWithBuffHopOnce(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj* buffOwner, Combat::BuffInfo* buffOneInfo, uint32 realHurtOfBuff, bool isAttWork);

	//��ײ
	void DoHurt_Crash(pb::Combat_Seque& msg, CombatGroup* pGroup);
	double _NewCrashHurt(CombatObj* attacker, CombatObj* defender, uint32 terrType);	//�����ײ��ɵ��˺�
	void _MakeHurtsAndAwardsCrash(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt);		//�����˺��뽱�� �����˺� ��ײ
	
	//ʹ�ü���
	void DoHurt_Skill(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType);
	void _MakeHurtAndAwardsSkill(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj& attacker, GroupLst& lst, pb::E_Combat_Serialize serType);
	void _AttachBuff(CombatGroup* pGroup, pb::E_Combat_Serialize serType);	//����buff
	void _BuffToRow(GroupLst& buffAttacher, CombatObj* buffSender, int buffid, int buffAttchRow, uint64 roundNum);	//buff���õ���
	
	//����
	void DoHurt_Cut(pb::Combat_Seque& msg, CombatGroup* pGroup);
	double _NewCutHurt(CombatObj* attacker, CombatObj* defender, uint32 terrType);	//���㻤��һ����ɵ��˺�
	void _MakeHurtsAndAwardsCut(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt);

	//buff����
	void DoHurt_Buff(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType);
	void _BuffWork(pb::Combat_Seque& msg, CombatGroup* pGroup,GroupLst& workLst, pb::E_Combat_Serialize serType, bool isAttWork);
	void _BuffRowWork(pb::Combat_Seque& msg, CombatGroup* pGroup,CombatObj* buffOwner, Combat::SoldierRaw& soldierRow, pb::E_Combat_Serialize serType, int rownum, bool isAttWork);	//����ʿ����buff������

public:
};

}
#define sCombatCalc Combat::Calc::Instance()