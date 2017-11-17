#include "CombatCalc.h"
#include "CombatObj.h"
#include "Base.h"
#include <math.h>
#include "System.h"
#include "Logger.h"
#include "utility/Utility.h"
#include "Combat.pb.h"
#include "datastruct/struct_soldier_food.h"
#include "../character/Character.h"
#include "../character/CharacterStorage.h"
#include "../object/Player.h"
#include "../session/PlayerPool.h"
#include "SkillDataMgr.h"
#include "CombatBuff.h"
#include "Npc/Npc.h"
#include "reward/reward.h"
#include "Activity/Personal/PersonalTask.h"
#include "Reinforcement/Reinforce.h"
#include "CombatGroup.h"
#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"
#include "Combat/CombatBuff.h"

#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossActivity/CrossKill/CrossKill.h"
#endif

#define MAX_HURT 9999999

using namespace Combat;

const char STATIC_SKILL_RATE = 25;

float Calc::CommonRatio(TacticEnum typ1, TacticEnum typ2)
{
    return Tactic_Common[typ1][typ2] / 100.0f;
}
float Calc::CrashRatio(TacticEnum typ1, TacticEnum typ2)
{
    char advantageTyp = Tactic_Crash[typ1][typ2];

    return Tactic_CrashRatio[advantageTyp-1] / 100.0f;
}
bool Calc::HaveCrashAddition(TacticEnum typ1, TacticEnum typ2)
{
    char advantageTyp = Tactic_Crash[typ1][typ2];

    return (advantageTyp == 1 || advantageTyp == 2);
}
int Calc::_CalcCommonHurt(CombatObj& attacker, CombatObj& defender, float tacticRatio)
{
    Property& aPro = attacker.stPro;
    Property& dPro = defender.stPro;

    //LLOG("CommonHurt -- attacker:hp(%d),atk(%d),def(%d),tactic(%d)", (int)attacker.Hp(), attacker.stPro.attack, attacker.stPro.defence, attacker.tactic);
    //LLOG("CommonHurt -- defender:hp(%d),atk(%d),def(%d),tactic(%d)", (int)defender.Hp(), defender.stPro.attack, defender.stPro.defence, defender.tactic);

	double hurt1 = MAX(aPro.attack - dPro.defence, 0);

    double hurt2 = MAX((hurt1*(0.55 + (attacker.ForeHp()/defender.ForeHp())*0.5)
				*(1 + tacticRatio) //ս������  Ӧ�ö���
				*(1 + attacker.terrainPercent/100.0 + attacker.GetTotalFightPercent()/100.0 - defender.guardAddPercent/100.0) //���μӳ�+buff-�Է��������ӳ�
				+ aPro.attackEx - dPro.defenceEx),0); //��������-�Է���

	double hurt = MAX((hurt2*((double)attacker.stPro.tong/defender.stPro.tong)),(aPro.attack*0.05));
	hurt = ceil(hurt) * 3;		//������������˺�
    //hurt *= 3; //������������˺�

    //LLOG("CommonHurt -- hurt1(%d), tacticRatio(%f)", (int)hurt, tacticRatio);

    //hurt = MAX(hurt, 0) * aPro.tong / dPro.tong;

    //LLOG("CommonHurt -- hurt2(%d), tong1(%d), tong2(%d)", (int)hurt, aPro.tong, dPro.tong);
	if(hurt > MAX_HURT)
		hurt = MAX_HURT;
    return (int)hurt;
}
int Calc::_CalcCrashHurt(CombatObj& attacker, CombatObj& defender)
{
    //LLOG("CrashHurt -- attacker:hp(%d),atk(%d),def(%d),tactic(%d)", (int)attacker.Hp(), attacker.stPro.attack, attacker.stPro.defence, attacker.tactic);
    //LLOG("CrashHurt -- defender:hp(%d),atk(%d),def(%d),tactic(%d)", (int)defender.Hp(), defender.stPro.attack, defender.stPro.defence, defender.tactic);

    double hurt = attacker.ForeHp() * CrashRatio(attacker.tactic, defender.tactic);

    //LLOG("CrashHurt -- hurt1(%d)", (int)hurt);

    if (HaveCrashAddition(attacker.tactic, defender.tactic))	
    {
        //hurt += attacker.crashAddition;
        //LLOG("CrashHurt -- hurt2(%d), crashAddition(%d)", (int)hurt, attacker.crashAddition);
    }
	hurt = MAX((int)hurt,1);

	if(hurt > MAX_HURT)
		hurt = MAX_HURT;

    return (int)hurt;
}

//����buff
void Calc::AttachBuffs(GroupLst& attackers, GroupLst& defenders, uint32 attidx)
{
	//LLOG("Calc::AttachBuffs:%d",attidx);
	CombatObj& rhs = *attackers.front();
	
	uint32 skillid = rhs.SkillID();
	int buffid = sSkillDataMgr.GetBuffIDBySkill(skillid);
	//LLOG("skillid = %d",skillid);
	//LLOG("buffid = %d",buffid);
	if(buffid != 0)	//�ü��ܸ���buff
	{
		int target = sSkillDataMgr.GetBuffTarget(buffid);
		int attachRow = sSkillDataMgr.GetBuffRows(buffid);
		int rowcount = attachRow;
		//LLOG("target=%d",target);
		if(target == 1)		//���ڵз�����
		{
			uint32 to = attidx==1? 2:1;
			//LLOG("to=%d",to);
			GroupLst::iterator it = defenders.begin();
			int rownum = 0;
			for(; it != defenders.end(); ++it)
			{
				CombatObj* tmpj = (*it);
				if(rownum < attachRow && rowcount>0)		//������С�ڹ�������
				{
					int rangenum = rowcount>(int)tmpj->soldiers.size()? tmpj->soldiers.size():rowcount;
					//tmpj->m_fightBuff.AddBuff(buffid,rangenum,attidx,to);
					rownum += tmpj->soldiers.size();
					rowcount -= rangenum;
				}
				else
					break;
			}
		}
		else if(target == 2)	//���ڼ�������
		{
			GroupLst::iterator it = attackers.begin();
			int rownum = 0;
			for(; it != defenders.end(); ++it)
			{
				CombatObj* tmpj = (*it);
				if(rownum < attachRow && rowcount>0)		//������С�ڹ�������
				{
					int rangenum = rowcount>(int)tmpj->soldiers.size()? tmpj->soldiers.size():rowcount;
					//tmpj->m_fightBuff.AddBuff(buffid,rangenum,attidx,attidx);
					rownum += tmpj->soldiers.size();
					rowcount -= rangenum;
				}
				else
					break;
			}
		}
	}

}

//ֱ�Ӽ����˺�������Ч�����⴦��  ʹ�ü��ܷ��������˺��Ķ��У��������Ϣ��ս������
void Calc::_SkillHurt(CombatGroup* pGroup,CombatObj& attacker, GroupLst& lst, int defenderid, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy)
{
 //   LLOG("SkillHurt -- attacker:hp(%d),atk(%d),def(%d),tactic(%d),range(%d)", (int)attacker.ForeHp(), attacker.stPro.attack, attacker.stPro.defence, attacker.tactic, (int)attacker.SkillRange());

 //   Property& aPro = attacker.stPro;	//��������

	//uint8 skillHurtLineCnt = 0;		//���ǰ���ŵ��˺�
	//int range = 0;
 //   GroupLst::iterator it = lst.begin();	//itΪ�佫
 //   for (; it != lst.end(); ++it)
 //   {
	//	CombatObj& defender = *(*it);
	//	CombatObj* tmpj = (*it);
 //       Property& dPro = defender.stPro;	//��������

 //       LLOG("SkillHurt -- defender:hp(%d),atk(%d),def(%d),tactic(%d)", (int)defender.ForeHp(), defender.stPro.attack, defender.stPro.defence, defender.tactic);

 //       double hurt = 0;
 //       double diff = MAX(aPro.attack - dPro.defence, 0);	//����ɵ��˺��� ��-��
 //       //if (diff >= 200) {
 //       //    hurt = diff * attacker.SkillRatio() * (1 + attacker.skillPercent / 100.0) + aPro.skillAttack - dPro.skillDefence;
 //       //} else {
 //       //    hurt = MAX(aPro.skillAttack - dPro.skillDefence, 0)
 //       //        *(1 - 1 / (::pow(diff, 1.5) / dPro.defence + 2));
 //       //}
	//	double minhurt1;
	//	double minhurt2;
	//	if(!attacker.IsReinforce())	//������Ԯ����
	//	{
	//		minhurt1 = attacker.SkillRatio() * aPro.attack * 0.05 + aPro.skillAttack * 0.1;
	//		minhurt2 = diff * attacker.SkillRatio() * (1 + attacker.GetTotalSkillPercent(tpy,defender.GetType()) / 100.0 - defender.GetTotalDeSkillPercent() / 100.0) + aPro.skillAttack - dPro.skillDefence;
	//		hurt = 3 * MAX(ceil(minhurt1), ceil(minhurt2));	//��������
	//	}
	//	else	//��Ԯ���ӣ����˺�����
	//	{
	//		Reinforce* rein = dynamic_cast<Reinforce*>(&attacker);
	//		if(rein)
	//			hurt = 3 * MAX((double)(rein->GetDamage()) * (1+attacker.GetTotalSkillPercent(tpy, tmpj->GetType()) / 100.0),1);
	//		else
	//			hurt = 1;
	//	}
	//	

 //       //LLOG("SkillHurt -- hurt1(%d)", (int)hurt);

 //       std::vector<Combat::SoldierRaw>::iterator itr = defender.soldiers.begin();	//iterΪÿ�ű���
 //       for (; itr != defender.soldiers.end(); ++itr)
 //       {
	//		double damagereal;
	//		if(defender.IsReinforce())	//���˷��ǹ��ǲ��ӣ����˺�����90%
	//			damagereal = hurt * 0.1;
	//		else
	//			damagereal = hurt;

 //           int& hp = itr->hp;
 //           int trueHurt = (hp >= (int)damagereal) ? (int)damagereal : hp;		//δ������˺�
 //           hp -= (int)damagereal;

 //           //LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

 //  //         attacker.killCntOneLoop += trueHurt;
	//		//defender.deadCntOneLoop += trueHurt;
	//		//attacker.statisticKill += trueHurt;
	//		
	//		//����˺���Ϣ
	//		//pb::Combat_Damage* damageOne = msg.add_damages();
	//		//damageOne->set_period(1);
	//		//damageOne->set_group(defenderid);
	//		//damageOne->set_range(range++);
	//		//damageOne->set_type(1);
	//		//damageOne->set_damage((int)trueHurt);
	//		//damageOne->set_restblood(hp>0? hp:0);

	//		//��佱����Ϣ
	//		pb::Combat_Reward* awardOne = msg.add_rewards();
	//		awardOne->set_group(defenderid);
	//		CalcAwardsOneStep(pGroup,attacker,tmpj,awardOne,tpy,1,trueHurt);

	//		//LLOG("get award side = %d, exp = %d, coin = %d, doHurt = %d",defenderid,awardOne->exp(), awardOne->coin(), trueHurt);

 //           if (++skillHurtLineCnt >= attacker.SkillRange())
 //           {
 //               break;
 //           }
 //       }
	//	if(defender.IsPlayer())
	//	{
	//		Player* player = sPlayerPool.GetByPlayerId(defender.GetPlayerId());
	//		::Character* hero = dynamic_cast<::Character*>(&defender);
	//		hero->SetCurSolderNum(defender.CurHp());
	//		player->m_characterStorage->SetModifiedID(hero->GetID());
	//	}
	//	if (skillHurtLineCnt >= attacker.SkillRange())
	//	{
	//		break;
	//	}
 //   }

	//if(attacker.IsReinforce())	//����Ԯ���ӣ�ֱ�ӹҵ�
	//{
	//	attacker.soldiers.clear();
	//}
}

bool Calc::_BuffHurt(CombatGroup* pGroup, GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg, CombatType tpy)
{
	////LLOG("Calc::_BuffHurt");
	bool ret = false;
	////����
	//int endcount = 0;
	//int range = 0;
	//GroupLst::iterator it = attackers.begin();
	//for (; it != attackers.end(); ++it)
	//{
	//	if(endcount++  > 10)	//���10���佫�������ս̫���佫
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(tmpj->m_fightBuff.HaveBuff(percentdamage))	//�аٷֱ��˺���buff		//������������
	//	{
	//		const BuffInfo* buffone = tmpj->m_fightBuff.GetBuffInfo(percentdamage);
	//		if(buffone)
	//		{
	//			float damagerate = buffone->value1;
	//			if(damagerate > 0.0)
	//			{
	//				int rownum = 0;
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iterΪÿ�ű���
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = hp * damagerate/100;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//δ������˺�
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:��¼��ɱ
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//����˺���Ϣ
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//��佱����Ϣ
	//						pb::Combat_Reward* awardOne = msg.add_rewards();
	//						awardOne->set_group(2);
	//						CalcAwardsOneStep(pGroup,*tmpj,tmpj,awardOne,tpy,4,trueHurt);

	//						if(tmpj->IsPlayer())
	//						{
	//							Player* player = sPlayerPool.GetByPlayerId(tmpj->GetPlayerId());
	//							::Character* hero = dynamic_cast<::Character*>(tmpj);
	//							hero->SetCurSolderNum(tmpj->CurHp());
	//							player->m_characterStorage->SetModifiedID(hero->GetID());
	//						}

	//					//}
	//					rownum++;
	//				}
	//			}
	//		}
	//	}

	//	if(tmpj->m_fightBuff.HaveBuff(fixeddamage))
	//	{
	//		const BuffInfo* buffone = tmpj->m_fightBuff.GetBuffInfo(fixeddamage);
	//		if(buffone)
	//		{
	//			float damagefixed = buffone->value1;
	//			if(damagefixed > 0.0)
	//			{
	//				int rownum = 0;
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iterΪÿ�ű���
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = damagefixed;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//δ������˺�
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:��¼��ɱ
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//����˺���Ϣ
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//��佱����Ϣ
	//						pb::Combat_Reward* awardOne = msg.add_rewards();
	//						awardOne->set_group(2);
	//						CalcAwardsOneStep(pGroup,*tmpj,tmpj,awardOne,tpy,4,trueHurt);
	//					//}
	//					rownum++;
	//				}
	//			}
	//		}
	//	}
	//}

	////�ط�
	//endcount = 0;
	//range = 0;
	//it = defenders.begin();
	//for (; it != defenders.end(); ++it)
	//{
	//	if(endcount++  > 10)	//���10���佫�������ս̫���佫
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(tmpj->m_fightBuff.HaveBuffWork(percentdamage))	//�аٷֱ��˺���buff		//������������
	//	{
	//		const BuffInfo* buffone = tmpj->m_fightBuff.GetBuffInfo(percentdamage);
	//		if(buffone)
	//		{
	//			float damagerate = buffone->value1;
	//			if(damagerate > 0.0)
	//			{
	//				int rownum = 0;
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iterΪÿ�ű���
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = hp * damagerate/100;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//δ������˺�
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:��¼��ɱ
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//����˺���Ϣ
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//��佱����Ϣ
	//						pb::Combat_Reward* awardOne = msg.add_rewards();
	//						awardOne->set_group(2);
	//						CalcAwardsOneStep(pGroup,*tmpj,tmpj,awardOne,tpy,4,trueHurt);

	//						if(tmpj->IsPlayer())
	//						{
	//							Player* player = sPlayerPool.GetByPlayerId(tmpj->GetPlayerId());
	//							::Character* hero = dynamic_cast<::Character*>(tmpj);
	//							hero->SetCurSolderNum(tmpj->CurHp());
	//							player->m_characterStorage->SetModifiedID(hero->GetID());
	//						}

	//					//}
	//					rownum++;
	//				}
	//			}
	//		}
	//	}

	//	if(tmpj->m_fightBuff.HaveBuffWork(fixeddamage))
	//	{
	//		const BuffInfo* buffone = tmpj->m_fightBuff.GetBuffInfo(fixeddamage);
	//		if(buffone)
	//		{
	//			float damagefixed = buffone->value1;
	//			if(damagefixed > 0.0)
	//			{
	//				int rownum = 0;
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iterΪÿ�ű���
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = damagefixed;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//δ������˺�
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:��¼��ɱ
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//����˺���Ϣ
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//��佱����Ϣ
	//						pb::Combat_Reward* awardOne = msg.add_rewards();
	//						awardOne->set_group(2);
	//						CalcAwardsOneStep(pGroup,*tmpj,tmpj,awardOne,tpy,4,trueHurt);
	//					//}
	//					rownum++;
	//				}
	//			}
	//		}
	//	}
	//}

	////ͷ��������
	//if(CheckDead(pGroup,attackers.front(),defenders.front(),msg,false) || CheckDead(pGroup,defenders.front(),attackers.front(),msg,true))
	//{
	//	ret = true;
	//}

	//RefreshSoldiers(attackers,defenders.front(),msg,false);
	//RefreshSoldiers(defenders,attackers.front(),msg,true);

	return ret;
}

void Calc::_GetBuffs(GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg)
{
	////LLOG("Calc::_GetBuffs1");
	//int endcount = 0;
	//int rowcount = 0;
	//
	////����
	//GroupLst::iterator it = attackers.begin();
	//for (; it != attackers.end(); ++it)
	//{
	//	//LLOG("attacker one ");
	//	if(endcount++  > 10)	//���10���佫�������ս̫���佫
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//��buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}

	////�ط�
	//endcount = 0;
	//rowcount = 0;
	//it = defenders.begin();
	//for (; it != defenders.end(); ++it)
	//{
	//	//LLOG("defender one ");
	//	if(endcount++  > 10)	//���10���佫�������ս̫���佫
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//��buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}
}

void Calc::_GetBuffs(GroupLst& attackers, GroupLst& defenders, pb::GS2C_Combat_Select_Tactic& msg)
{
	////LLOG("Calc::_GetBuffs2");
	//int endcount = 0;
	//int rowcount = 0;

	////����
	//GroupLst::iterator it = attackers.begin();
	//for (; it != attackers.end(); ++it)
	//{
	//	//LLOG("attacker one ");
	//	if(endcount++  > 10)	//���10���佫�������ս̫���佫
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//��buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}

	////�ط�
	//endcount = 0;
	//rowcount = 0;
	//it = defenders.begin();
	//for (; it != defenders.end(); ++it)
	//{
	//	//LLOG("defender one ");
	//	if(endcount++  > 10)	//���10���佫�������ս̫���佫
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//��buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}
}
//attacker��defender����˺���defender��ý���	attackHurt������ɵ��˺���defenceHurt�ط���ɵ��˺�
void Calc::CalcAwardsOneStep(CombatGroup* pGroup, CombatObj& attacker, CombatObj* defender, pb::Combat_Reward* awards, CombatType typ, int step, int attackHurt, int defenceHurt)
{
//	awards->set_period(step);
//	
//	::Character* hero = dynamic_cast<::Character*>(defender);
//	::Character* attacker_hero =  dynamic_cast<::Character*>(&attacker);
//	//if (hero == NULL ) return;
//
//	//�������˺��ӳ�
//	if(hero && !hero->IsShadow())
//	{
//		Player* defPlayer = hero->m_player;
//		if(defPlayer)
//		{
//			defenceHurt *= defPlayer->GetCrossDoubleKill();
//		}
//	}
//
//	if(attacker_hero && !attacker_hero->IsShadow())
//	{
//		Player* attPlayer = attacker_hero->m_player;
//		if(attPlayer)
//		{
//			attackHurt *= attPlayer->GetCrossDoubleKill();
//		}
//	}
//
//
//
//	if(step == 1)		//���ܽ׶�
//	{
//		//if (hero == NULL) return;
//
//		int exp = 0;
//		int expPlayer = 0;
//		int money = 0;
//		IntPairVec tmp;
//
//		if(hero)
//		{
//			const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(hero->soldierLv);
//			MMO_ASSERT(pTable);
//			if (pTable == NULL) return;
//			float needFood = pTable->NeedFood();
//
//			float killRatio = 2;
//			Property& aPro = attacker.stPro;	//��������
//			Property& dPro = defender->stPro;	//�ط�����
//			killRatio = (float)aPro.attack / (float)dPro.defence;
//			if(killRatio > 2)
//				killRatio = 2;
//			else if(killRatio < 0.5)
//				killRatio = 1;
//			else
//				killRatio = (killRatio + 1) * 2 / 3;
//
//			float crossTimes = 1.0;
//	#ifdef _SERVER_CROSS_
//			crossTimes = 1.5;
//	#endif
//			if (hero->m_player)
//			{
//				exp = int(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * (1 + hero->GetTotalExpPercent(typ) / 100.0 + hero->m_player->GetHerosExpUpPercent()/100.0) * crossTimes);
//				expPlayer = int(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * (1 + hero->GetTotalExpPercent(typ) / 100.0 ) * crossTimes);
//				money = (int)ceil(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * 0.2);
//			}
//			else
//			{
//				exp = int(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * (1 + hero->GetTotalExpPercent(typ) / 100.0) * crossTimes);
//			}
//
//			expPlayer = MAX(expPlayer,1);
//			money = MAX(money,1);
//			if (hero->m_player)
//			{
//				//LLOG("calc %d,%f,%f,%f,%d,%u",attackHurt,needFood,FOOD_TO_EXP[typ],killRatio,hero->GetTotalExpPercent(typ),hero->m_player->GetHerosExpUpPercent());
//			}
//
//			// Զ��������þ��顢����
//			if (CT_Crusade == typ) {
//				exp = 0;
//				money = 0;
//				expPlayer = 0;
//			}
//			awards->set_exp(expPlayer);
//			awards->set_coin(money);
//			
//			hero->statisticExp += exp;
//			hero->statisticExpPlayer += expPlayer;
//			hero->statisticMoney += money;
//			hero->statisticDead += attackHurt;//hero->deadCntOneLoop;
//			hero->statisticKill += 0;//hero->killCntOneLoop;
//
//			if(hero->IsPlayer())	//�ط����佫�����¼����������þ���
//			{
//				pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//				pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatDead,attackHurt,tmp);
//			}
//			if(hero->IsReinforce())		//����Ԯ���ӣ���¼��þ���
//			{
//				pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//			}
//		}
//		//hero->deadCntOneLoop += attackHurt;
//		//hero->killCntOneLoop += 0;
//		if(attacker_hero && !attacker_hero->IsShadow())
//		{
//			attacker_hero->statisticKill += attackHurt;
//			attacker_hero->killCntOneLoop += attackHurt;
//			attacker_hero->killOnceAHero += attackHurt;
//		}
//		//attacker.statisticKill += attackHurt;
//		//attacker.killCntOneLoop += attackHurt;
//		uint32 exploittmp = 0;
//		if((typ == CT_Country || typ == CT_Monster )  )
//		{
//			////////zhoulunhao
//			if(attacker_hero && attacker_hero->m_player)
//			{
//				sPersonTask.addProgress(KILL_IN_COUNTRY,attackHurt,*(attacker_hero->m_player));
//#ifdef _SERVER_CROSS_
//				sCrossKill.addProgress(KILL_IN_COUNTRY,attackHurt,*(attacker_hero->m_player));
//#endif
//			}
//			if(attacker_hero && attacker_hero->m_player && !attacker_hero->IsShadow() )
//			{
//				exploittmp = attacker_hero->m_player->HurtToExploit(ENUM_TYPE_KILL,attackHurt);
//			}
//		}
//		if(typ == CT_Country)
//		{
//			if(hero && hero->m_player)
//			{
//				hero->m_player->AddActivityStarBoxExp(expPlayer);
//			}
//		}
//		if(attacker_hero && (attacker_hero->IsPlayer() || attacker_hero->IsReinforce()))	//�������佫����Ԯ���ӣ����¼��ɱ����ѫ
//		{
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatKills,attackHurt,tmp);
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatExploit,exploittmp,tmp);
//		}
//	}
//	else if(step == 2 || step == 3 || step == 4)	//��ײ�׶� ƽ���׶� buff�׶�
//	{
//		//if (hero == NULL) return;
//
//		int exp =0;
//		int expPlayer = 0;
//		int money = 0;
//		IntPairVec tmp;
//
//		if(hero)
//		{
//			const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(hero->soldierLv);
//			MMO_ASSERT(pTable);
//			if (pTable == NULL) return;
//
//			float needFood = pTable->NeedFood();
//
//			//------------------------------------------
//			//�����ɱ��
//			float killRatio = 2;
//			if (attackHurt > 0)
//			{
//				killRatio = (float)defenceHurt / (float)attackHurt;	//��ɱ/����
//			}
//			if (CT_Official == typ) //��ս
//			{
//				killRatio = (killRatio >= 3) ? 2 : 1 + killRatio / 3;
//			}
//			else
//			{
//				if (killRatio >= 2)
//					killRatio = 2;
//				else if (killRatio <= 0.5)
//					killRatio = 1;
//				else
//					killRatio = (killRatio + 1) * 2 / 3;
//			}
//			//------------------------------------------
//
//			float crossTimes = 1.0;
//	#ifdef _SERVER_CROSS_
//			crossTimes = 1.5;
//	#endif
//
//			if (hero->m_player)
//			{
//				exp = int(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * (1 + hero->GetTotalExpPercent(typ) / 100.0 + hero->m_player->GetHerosExpUpPercent()/100.0) * crossTimes);
//				expPlayer = int(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * (1 + hero->GetTotalExpPercent(typ) / 100.0 ) * crossTimes);
//				money = (int)ceil(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * 0.2);
//			}
//			else
//			{
//				exp = int(attackHurt * needFood * FOOD_TO_EXP[typ] * killRatio * (1 + hero->GetTotalExpPercent(typ) / 100.0) * crossTimes);
//			}
//
//			expPlayer = MAX(expPlayer,1);
//			money = MAX(money,1);
//
//			
//			if (hero->m_player)
//			{
//				//LLOG("calc %d,%f,%f,%f,%d,%u",hero->deadCntOneLoop,needFood,FOOD_TO_EXP[typ],killRatio,hero->GetTotalExpPercent(typ),hero->m_player->GetHerosExpUpPercent());
//			}
//
//			// Զ��������þ��顢����
//			if (CT_Crusade == typ) {
//				exp = 0;
//				money = 0;
//				expPlayer = 0;
//			}
//			awards->set_exp(expPlayer);
//			awards->set_coin(money);
//			hero->statisticExp += exp;
//			hero->statisticExpPlayer += expPlayer;
//			hero->statisticMoney += money;
//			hero->statisticDead += attackHurt;//hero->deadCntOneLoop;
//			hero->statisticKill += defenceHurt;//hero->killCntOneLoop;
//			hero->killOnceAHero += defenceHurt;
//		}
//		//hero->deadCntOneLoop += attackHurt;
//		//hero->killCntOneLoop += defenceHurt;
//
//		uint32 exploittmpAtt = 0;
//		uint32 exploittmpDef = 0;
//		if(typ == CT_Country || typ == CT_Monster  )
//		{
//			////////zhoulunhao
//			if(hero && hero->m_player)
//			{
//				sPersonTask.addProgress(KILL_IN_COUNTRY,defenceHurt,*(hero->m_player));
//#ifdef _SERVER_CROSS_
//				sCrossKill.addProgress(KILL_IN_COUNTRY,defenceHurt,*(hero->m_player));
//#endif
//			}
//			if(attacker_hero && attacker_hero->m_player)
//			{
//				sPersonTask.addProgress(KILL_IN_COUNTRY,attackHurt,*(attacker_hero->m_player));
//#ifdef _SERVER_CROSS_
//				sCrossKill.addProgress(KILL_IN_COUNTRY,attackHurt,*(attacker_hero->m_player));
//#endif
//			}
//			if(hero && hero->m_player && !hero->IsShadow())
//			{
//				exploittmpDef += hero->m_player->HurtToExploit(ENUM_TYPE_KILL,defenceHurt);//ɱ�в�����ѫ
//				exploittmpDef += hero->m_player->HurtToExploit(ENUM_TYPE_DEATH,attackHurt);//����������ѫ
//			}
//			if(attacker_hero && attacker_hero->m_player && !attacker_hero->IsShadow())
//			{
//				exploittmpAtt += attacker_hero->m_player->HurtToExploit(ENUM_TYPE_KILL,attackHurt);//ɱ�в�����ѫ
//				exploittmpAtt += attacker_hero->m_player->HurtToExploit(ENUM_TYPE_DEATH,defenceHurt);//����������ѫ
//			}
//		}
//		if(typ == CT_Country)
//		{
//			if(hero && hero->m_player)
//			{
//				hero->m_player->AddActivityStarBoxExp(expPlayer);
//			}
//		}
//
//		if(hero && hero->IsPlayer())	//�ط����佫�����¼����������þ���,��ѫ
//		{
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatDead,attackHurt,tmp);
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatExploit,exploittmpDef,tmp);
//		}
//		if(hero && hero->IsReinforce())		//����Ԯ���ӣ���¼��þ���
//		{
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//		}
//
//		if(attacker_hero && (attacker_hero->IsPlayer() || attacker_hero->IsReinforce()))	//�������佫����Ԯ���ӣ����¼��ɱ����ѫ
//		{
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatKills,attackHurt,tmp);
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatExploit,exploittmpAtt,tmp);
//		}
//
//	}
}

//�������ط���������ɵ��˺����ط���ɵ��˺�
void Calc::_DoHurt(CombatGroup* pGroup, CombatObj& attacker, CombatObj& defender, int attackerHurt, int defenderHurt, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy, int step)
{
	//double damagereal;
	//if(attacker.IsReinforce())	//���˷��ǹ��ǲ��ӣ����˺�����90%
	//	damagereal = defenderHurt * 0.1;
	//else
	//	damagereal = defenderHurt;
 //   int trueHurtDefender = attacker.HurtHp((int)damagereal);		//�ط���ɵ���ʵ�˺�
 //   defender.killCntOneLoop += trueHurtDefender;
 //   attacker.deadCntOneLoop += trueHurtDefender;
 //   //LLOG("DoHurt -- attacker:trueHurt(%d)", trueHurt);

	//if(defender.IsReinforce())	//���˷��ǹ��ǲ��ӣ����˺�����90%
	//	damagereal = attackerHurt * 0.1;
	//else
	//	damagereal = attackerHurt;
 //   int trueHurtAttacker = defender.HurtHp((int)damagereal);	//������ɵ���ʵ�˺�
 //   attacker.killCntOneLoop += trueHurtAttacker;
 //   defender.deadCntOneLoop += trueHurtAttacker;
	////LLOG("DoHurt -- defender:trueHurt(%d)", trueHurt);

	////����˺�
	//pb::Combat_Damage* damageOne = msg.add_damages();
	//damageOne->set_period(step);
	//damageOne->set_group(2);
	//damageOne->set_range(0);
	//damageOne->set_type(step);
	//damageOne->set_damage(trueHurtAttacker);
	//damageOne->set_restblood((defender.soldiers.front().hp<0? 0:defender.soldiers.front().hp));

	//pb::Combat_Damage* damageTwo = msg.add_damages();
	//damageTwo->set_period(step);
	//damageTwo->set_group(1);
	//damageTwo->set_range(0);
	//damageTwo->set_type(step);
	//damageTwo->set_damage(trueHurtDefender);
	//damageTwo->set_restblood((attacker.soldiers.front().hp<0? 0:attacker.soldiers.front().hp));

	////��佱����Ϣ
	//pb::Combat_Reward* awardDefender = msg.add_rewards();
	//awardDefender->set_group(2);
	//CombatObj* tmpj = &defender;
	//CalcAwardsOneStep(pGroup,attacker,tmpj,awardDefender,tpy,step,trueHurtAttacker,trueHurtDefender);

	////LLOG("get award side = 2,step = %d, exp = %d, coin = %d, doTurtAtt = %d, doHurtDef = %d",
	////	step,awardDefender->exp(), awardDefender->coin(),trueHurtAttacker,trueHurtDefender );

	//pb::Combat_Reward* awardAttacker = msg.add_rewards();
	//awardAttacker->set_group(1);
	//CombatObj* tmpk = &attacker;
	//CalcAwardsOneStep(pGroup,defender,tmpk,awardAttacker,tpy,step,trueHurtDefender,trueHurtAttacker);

	////LLOG("get award side = 1,step = %d, exp = %d, coin = %d, doTurtAtt = %d, doHurtDef = %d",
	////	step,awardAttacker->exp(), awardAttacker->coin(),trueHurtDefender,trueHurtAttacker );

	//if(attacker.IsPlayer())
	//{
	//	Player* player = sPlayerPool.GetByPlayerId(attacker.GetPlayerId());
	//	::Character* hero = dynamic_cast<::Character*>(&attacker);
	//	hero->SetCurSolderNum(attacker.CurHp());
	//	player->m_characterStorage->SetModifiedID(hero->GetID());
	//}

	//if(defender.IsPlayer())
	//{
	//	Player* player = sPlayerPool.GetByPlayerId(defender.GetPlayerId());
	//	::Character* hero = dynamic_cast<::Character*>(&defender);
	//	hero->SetCurSolderNum(defender.CurHp());
	//	player->m_characterStorage->SetModifiedID(hero->GetID());
	//}
}

void Calc::Attack(CombatGroup* pGroup, CombatObj* attacker, CombatObj* defender, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy)
{
    //��һ���Ƕ�ײ
    int attackerHurt = _CalcCrashHurt(*attacker, *defender);	//������ɵ��˺�
    int defenderHurt = _CalcCrashHurt(*defender, *attacker);	//�ط���ɵ��˺�

	////����˺�
	//pb::Combat_Damage* damageOne = msg.add_damages();
	//damageOne->set_period(2);
	//damageOne->set_group(2);
	//damageOne->set_range(0);
	//damageOne->set_type(2);
	//damageOne->set_damage(attackerHurt);

	//pb::Combat_Damage* damageTwo = msg.add_damages();
	//damageTwo->set_period(2);
	//damageTwo->set_group(1);
	//damageTwo->set_range(0);
	//damageTwo->set_type(2);
	//damageTwo->set_damage(defenderHurt);

    _DoHurt(pGroup, *attacker, *defender, attackerHurt, defenderHurt,msg ,tpy,2);

    float ratio1 = CommonRatio(attacker->tactic, defender->tactic);
    float ratio2 = CommonRatio(defender->tactic, attacker->tactic);

    //Ȼ��Կ�
    while (!CheckDead(pGroup,attacker,defender,msg,false) && !CheckDead(pGroup,defender,attacker,msg,true))
    {
        attackerHurt = _CalcCommonHurt(*attacker, *defender, ratio1);
        defenderHurt = _CalcCommonHurt(*defender, *attacker, ratio2);

		////����˺�
		//pb::Combat_Damage* damageOne = msg.add_damages();
		//damageOne->set_period(3);
		//damageOne->set_group(2);
		//damageOne->set_range(0);
		//damageOne->set_type(3);
		//damageOne->set_damage(attackerHurt);

		//pb::Combat_Damage* damageTwo = msg.add_damages();
		//damageTwo->set_period(3);
		//damageTwo->set_group(1);
		//damageTwo->set_range(0);
		//damageTwo->set_type(3);
		//damageTwo->set_damage(defenderHurt);

        _DoHurt(pGroup, *attacker, *defender, attackerHurt, defenderHurt, msg ,tpy,3);
		//CheckDead(attacker);
		//CheckDead(defender);
    }
	//RefreshSoldiers(attacker);
	//RefreshSoldiers(defender);
	//attacker.isSelectTactic = false;
	//defender.isSelectTactic = false;
}


/* �µ�ս������ϵͳ */
bool Calc::CheckDeadNormal(CombatObj* checkobj)
{
	if(checkobj->soldiers.size() == 0)	//obj�Ѿ�����
	{
		return true;
	}

	if(checkobj->ForeHp() <= 0)	//��һ��Ѫ��Ϊ0
	{
		return true;
	}

	return false;
}

void Calc::_CalcAwardsInOneStepBySide(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, CombatObj* pAttack, CombatObj* pDefence, int attOrgHurtToDef, int defOrgHurtToAtt, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt, int realdProDef, bool isAttGetAward)
{
	//����ط����佫���߼��佫����Ӱ����Ԯ����	//���Ի�ý���	�����л�Ӱ����Ԯ����ֻ�ܻ�����exp���������ҽ�����
	if(pDefence->IsPlayer() || pDefence->IsShadow() || pDefence->IsReinforce())
	{
		/********** ���㿪ʼ ********/
		//�������
		float needFood = 0.0;
		const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(pDefence->soldierLv);
		if (pTable == NULL) 
		{
			LLOG("[2000 ERROR] _GetAwardsWithOneStep pTable is not found,lv=%d",pDefence->soldierLv);
			return;
		}
		needFood = pTable->NeedFood() * attOrgHurtToDef;

		//����ת������
		float foodToExp = FOOD_TO_EXP[pGroup->m_eCombat];

		//�����ɱ��
		float killRatio = 2;
		if(serType == pb::CoSe_SkillHurt_Att || serType == pb::CoSe_SkillHurt_Def)	//������ͷż��ܽ׶�
		{
			Property& aPro = pAttack->stPro;	//��������
			Property& dPro = pDefence->stPro;	//�ط�����
			killRatio = (float)realaProAtt / (float)realdProDef;
			if(killRatio > 2)
				killRatio = 2;
			else if(killRatio < 0.5)
				killRatio = 1;
			else
				killRatio = (killRatio + 1) * 2 / 3;
		}
		else
		{
			if(attRealHurtToDef != 0)	//ɱ������Ϊ0
			{
				killRatio = (float)defRealHurtToAtt / (float)attRealHurtToDef;	//��ɱ��=ɱ��������ʧ����
				if (CT_Official == pGroup->m_eCombat) //��ս
				{
					killRatio = (killRatio >= 3) ? 2 : 1 + killRatio / 3;
				}
				else
				{
					if (killRatio >= 2)
						killRatio = 2;
					else if (killRatio <= 0.5)
						killRatio = 1;
					else
						killRatio = (killRatio + 1) * 2 / 3;
				}
			}
		}

		//������ּӳ�
		float totalAddtions = 0.0;
		//�Ƽ���������,������ṩ�ľ���ӳ�
		totalAddtions += (((float)pDefence->GetTotalExpPercent(pGroup->m_eCombat))/((float)100));
		if(pGroup->m_eCombat == CT_Country)	//ֻ�й�ս���еļӳ�
		{
			//�ȼ����ṩ�ľ���ӳ�
			uint32 levelAtt = pAttack->GetPlayerLevel();
			uint32 levelDef = pDefence->GetPlayerLevel();
			if((levelAtt+16) <= levelDef)	//�ȶԷ��ȼ�С16��
			{
				uint32 levelDif = levelDef - levelAtt - 15;
				totalAddtions += MIN(((float)levelDif * (float)0.1),(float)1.0);
			}

			//TODO::�������ӳ�		Ѱ·�Ż�����

			//TODO::�佫�츳�ӳ�	�����佫������

		}

		//�������
		float crossTimes = 1.0;
#ifdef _SERVER_CROSS_
		crossTimes = 1.5;
#endif

		//�������ս���
		int exp = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
		int expPlayer = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
		int money = MAX((int)ceil(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes * 0.2),1);

		if(pGroup->m_eCombat == CT_Crusade || attOrgHurtToDef == 0)	//Զ������þ����뽱�����߹������ط���ɵ��˺�Ϊ0
		{
			exp = 0;
			expPlayer = 0;
			money = 0;
		}
		/********** ������� ********/


		/********* ���Ž��� ********/
		//֮ǰ�汾�������佫��������ս���������㽱���뾭�飬���ڸ�Ϊ���������Ƿ�����㣿 Try it
		uint32 exploitOfDef = 0;
		//����佫����Ӱ����Ԯ���ӣ�������ҵľ���������
		if(Player* master = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))
		{
			sReward.Change(*master, Reward::PlayerExp, expPlayer);	//��Ҿ���
			sReward.Change(*master, Reward::Coin_Silver, money);	//����
			master->AddActivityStarBoxExp(expPlayer);				//��һ����
			exploitOfDef = master->HurtToExploit(ENUM_TYPE_DEATH,attOrgHurtToDef); //����������ѫ
		}

		//����佫�����佫����
		if(pDefence->IsPlayer())
		{
			::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
			heroDefence->TryAddXP(exp);
		}
		//�佫�Ļ�Ӱ����ԭ�佫����
		if(pDefence->IsShadow())
		{
			::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
			Character* charone = heroDefence->m_player->m_characterStorage->MutableCharacter(heroDefence->GetID());
			if(charone)
				charone->TryAddXP(exp);
		}
		/********* ���Ž��� **********/


		/******** ��¼����ս������ ******/
		//��¼ս��
		IntPairVec tmp;
		//����佫����Ӱ����Ԯ���Ӿ���������Ҿ��飬������ʧ
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatPlayerExp,expPlayer,tmp);
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatDead,attOrgHurtToDef,tmp);
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatExploit,exploitOfDef,tmp);
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatCoin,money,tmp);
		/********** ��¼���� **********/


		/********* ���ӽ��� *********/
		if(::Character* heroDefence = dynamic_cast<::Character*>(pDefence))
		{
			if(heroDefence->GetIsInAutoCountryWar())	//�佫���Զ���ս��
			{
				heroDefence->m_player->m_AutoCountryWarLog->AddExpGet(expPlayer);	//��¼�Զ���ս��þ���
			}
		}
		/********* ���ӽ��� *********/

		/********** �����Ϣ *********/
		if(expPlayer > 0)
		{
			pb::Combat_Reward* rewardsOne = msg.add_rewardlst();
			if(isAttGetAward)
				rewardsOne->set_group(1);
			else
				rewardsOne->set_group(2);
			rewardsOne->set_exp(expPlayer);
			rewardsOne->set_coin(money);
			rewardsOne->set_playerid(pDefence->GetPlayerId());
		}
		/*****************************/

	}

	//������ط�����˺���������õĽ���
	//����������佫���߼��佫����Ӱ����Ԯ����	//���Ի�ù�ѫ����
	if(pAttack->IsPlayer() || pAttack->IsShadow() || pAttack->IsReinforce())
	{
		uint32 exploitOfAtt = 0;	//������õĹ�ѫ
		if(Player* master = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))
		{
			//�ǹ�ս�������ӹ�ѫ
			if(pGroup->m_eCombat == CT_Country)
			{
				exploitOfAtt = master->HurtToExploit(ENUM_TYPE_KILL,attRealHurtToDef);
			}
		}

		/******** ��¼����ս������ ******/
		//��¼ս��
		IntPairVec tmp;
		pGroup->RecordCombatGainsInfo(pAttack->GetPlayerId(),pb::CombatKills,attRealHurtToDef,tmp);
		pGroup->RecordCombatGainsInfo(pAttack->GetPlayerId(),pb::CombatExploit,exploitOfAtt,tmp);
		/********** ��¼���� **********/


		/********* ���ӽ��� *********/
		if(::Character* heroAttack = dynamic_cast<::Character*>(pAttack))
		{
			if(heroAttack->GetIsInAutoCountryWar())	//�佫���Զ���ս��
			{
				heroAttack->m_player->m_AutoCountryWarLog->AddKillCount(attRealHurtToDef);	//��¼�Զ���ս��ɱ��
				heroAttack->killOnceAHero += attRealHurtToDef;
			}
			CombatGroup::_SaveStaticKillOnce(*heroAttack,pGroup->m_eCombat,attRealHurtToDef);	//��¼������ɱ���� �������
		}
		/********* ���ӽ��� *********/
	}
}

//һ���˺��Ľ������� ս���飬����׶Σ������ص��˺����ضԹ����˺� ����ײ������ֱ���˺��������׶��ã� (һ�����أ����ײ������������һ�Σ�����һ������һ���ͷż���)
void Calc::_GetAwardsWithOneStep(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt/* = 1*/, int realdProDef/* = 1*/)
{
	//����=������ת����������ɱ����������1+���ּӳɣ����������
	//����=����*0.2

	if(pGroup->m_group_attack.empty() || pGroup->m_group_defence.empty())
	{
		LLOG("[2000 ERROR] _GetAwardsWithOneStep pGourp list is empty");
		return;
	}

	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	//���ȼ����˺��ļӳ�	//ɱ�����������������
	int attOrgHurtToDef = attRealHurtToDef;
	int defOrgHurtToAtt = defRealHurtToAtt;
	attRealHurtToDef *= (1 + pAttack->GetTotalDoubleKill(pGroup->m_eCombat)/100);
	defRealHurtToAtt *= (1 + pDefence->GetTotalDoubleKill(pGroup->m_eCombat)/100);

	//���㽱��
	//�ȼ��㹥�������ط����ط���õĽ���
	_CalcAwardsInOneStepBySide(msg, pGroup,serType,pAttack,pDefence,attOrgHurtToDef,defOrgHurtToAtt,attRealHurtToDef,defRealHurtToAtt,realaProAtt,realdProDef,false);
	
	//������ط�����������������õĽ���
	_CalcAwardsInOneStepBySide(msg, pGroup,serType,pDefence,pAttack,defOrgHurtToAtt,attOrgHurtToDef,defRealHurtToAtt,attRealHurtToDef,realdProDef,realaProAtt,true);

}

void Calc::_MakeHurtsAndAwardsCrash(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double	attHp = pAttack->ForeHp();	//����ǰ��Ѫ��
	double	defHp = pDefence->ForeHp();	//�ط�ǰ��Ѫ��

	if(attHp <= 0 || defHp <= 0)	return; //���һ���Ѿ��������򲻽��г�ײ

	//��Ԯ����û�г�ײ�׶Σ���ֻ�ж����
	//if(attObj.IsReinforce())	//��������Ԯ���ӣ����ط�������ɵ��˺�����90%
	//	defHurtToAtt *= 0.1;

	//if(defObj.IsReinforce())	//�ط�����Ԯ���ӣ��򹥷�������ɵ��˺�����90%
	//	attHurtToDef *= 0.1;


	//����˺�����
	int attRealHurt = MIN((int)attHurtToDef,(int)defHp);		//�������ط���ɵ���ʵ�˺�
	int defRealHurt = MIN((int)defHurtToAtt,(int)attHp);		//�ط��Թ�����ɵ���ʵ�˺�


	//����˺�
	pAttack->HurtHp(defRealHurt);
	pDefence->HurtHp(attRealHurt);

	pb::Combat_Damage* damageAtt = msg.add_damagelst();	//�����յ����˺����õĽ���
	damageAtt->set_group(1);
	damageAtt->set_range(0);
	damageAtt->set_damage(defRealHurt);
	damageAtt->set_restblood((int)pAttack->ForeHp());
	damageAtt->set_buffid(0);

	pb::Combat_Damage* damageDef = msg.add_damagelst();	//�ط��յ����˺����õĽ���
	damageDef->set_group(2);
	damageDef->set_range(0);
	damageDef->set_damage(attRealHurt);
	damageDef->set_restblood((int)pDefence->ForeHp());
	damageDef->set_buffid(0);


	//��¼ɱ��
	if(pAttack->ForeHp() == 0)
	{
		if(pAttack->soldiers.front().killerPlayerId == 0)	//���û�м�¼��ɱ��
		{
			pAttack->soldiers.front().killerPlayerId = pDefence->GetPlayerId();
			if(pDefence->IsPlayer() || pDefence->IsShadow())	//������佫���߻�Ӱ
			{
				::Character* hero = dynamic_cast<::Character*>(pDefence);
				pAttack->soldiers.front().killerCharId = hero->GetID();
			}

			//���Ŷ��⽱��
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pAttack)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))	//��������
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//��¼���⽱��
						IntPairVec pairvec;
						pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
						pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatInfoNum,0,pairvec);

						pb::extra_awards* exAwardDef = msg.add_def_extra_awards();
						exAwardDef->set_awardsid(deadnpc->GetExtraAwardID());
						exAwardDef->set_awardsnum(deadnpc->GetExtraAwardNum());
					}
				}
			}

		}
		if(pAttack->IsDead())	//����佫������
		{
			if(pAttack->killerPID == 0)
			{
				pAttack->killerPID = pDefence->GetPlayerId();
				if(pDefence->IsPlayer() || pDefence->IsShadow())	//������佫���߻�Ӱ
				{
					::Character* hero = dynamic_cast<::Character*>(pDefence);
					pAttack->killerCID = hero->GetID();
				}
			}
		}
	}
	if(pDefence->ForeHp() == 0)
	{
		if(pDefence->soldiers.front().killerPlayerId == 0)	//���û�м�¼��ɱ��
		{
			pDefence->soldiers.front().killerPlayerId = pAttack->GetPlayerId();
			if(pAttack->IsPlayer() || pAttack->IsShadow())	//������佫���߻�Ӱ
			{
				::Character* hero = dynamic_cast<::Character*>(pAttack);
				pDefence->soldiers.front().killerCharId = hero->GetID();
			}

			//���Ŷ��⽱��
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pDefence)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))	//��������
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//��¼���⽱��
						IntPairVec pairvec;
						pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
						pGroup->RecordCombatGainsInfo(pAttack->GetPlayerId(),pb::CombatInfoNum,0,pairvec);

						pb::extra_awards* exAwardAtt = msg.add_att_extra_awards();
						exAwardAtt->set_awardsid(deadnpc->GetExtraAwardID());
						exAwardAtt->set_awardsnum(deadnpc->GetExtraAwardNum());
					}
				}
			}

		}
		if(pDefence->IsDead())	//����佫������
		{
			if(pDefence->killerPID == 0)
			{
				pDefence->killerPID = pAttack->GetPlayerId();
				if(pAttack->IsPlayer() || pAttack->IsShadow())	//������佫���߻�Ӱ
				{
					::Character* hero = dynamic_cast<::Character*>(pAttack);
					pDefence->killerCID = hero->GetID();
				}
			}
		}
	}

	//���Ž���
	_GetAwardsWithOneStep(msg, pGroup,pb::CoSe_TacticCrash,attRealHurt,defRealHurt);


	//У׼�佫Ѫ��
	if(pAttack->IsPlayer())
	{
		::Character* hero = dynamic_cast<::Character*>(pAttack);
		hero->SetCurSolderNum(pAttack->CurHp());
		hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
	}
	if(pDefence->IsPlayer())
	{
		::Character* hero = dynamic_cast<::Character*>(pDefence);
		hero->SetCurSolderNum(pDefence->CurHp());
		hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
	}

}

double Calc::_NewCrashHurt(CombatObj* attacker, CombatObj* defender, uint32 terrType)
{
	double hurt = attacker->ForeHp() * CrashRatio(attacker->tactic, defender->tactic);	//��ǰ��������ǰ��Ѫ��*��ײϵ��

	//LLOG("CrashHurt -- hurt1(%d)", (int)hurt);

	if (HaveCrashAddition(attacker->tactic, defender->tactic))	//����������������ƻ��߲���������
	{
		hurt += attacker->GetTerrDebuffAddtion(terrType);
		//LLOG("CrashHurt -- hurt2(%d), crashAddition(%d)", (int)hurt, attacker.crashAddition);
	}
	hurt = MAX(hurt,3);	//��СֵΪ3����ÿ��������СֵΪ1

	if(hurt > MAX_HURT)
		hurt = MAX_HURT;

	return hurt;
}

void Calc::DoHurt_Crash(pb::Combat_Seque& msg, CombatGroup* pGroup)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double attHurtToDef = _NewCrashHurt(pAttack,pDefence,pGroup->m_terrainType);	//�������ط���ɵĳ�ײ�˺�	�����˺�	��СֵΪ3
	double defHurtToAtt = _NewCrashHurt(pDefence,pAttack,pGroup->m_terrainType); //�ط��Թ�����ɵĳ�ײ�˺�	�����˺�	��СֵΪ3

	//����˺��뽱�� ��ײ
	_MakeHurtsAndAwardsCrash(msg,pGroup,attHurtToDef,defHurtToAtt);
}

void Calc::_MakeHurtAndAwardsSkill(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj& attacker, GroupLst& lst, pb::E_Combat_Serialize serType)
{
	//�������˺� = max(max��������-������,0)*2*�����˺�ϵ��*��1+����ս���ӳ�-�ط�ս��������+����-��и,����*5%*�����˺�ϵ��+����*0.1)
	Property& aPro = attacker.stPro;			//��������
	uint8 skillRange = attacker.SkillRange();	//�ͷż���Ӱ������
	uint8 skillHurtLineCnt = 0;					//Ӱ����������

	int realaProAtt = attacker.soldiers.front().buff_fight.GetFinalAttackValue(aPro.attack);	//����ʩ�ŷ��Ĺ�������

	int awardSide;
	if(serType == pb::CoSe_SkillHurt_Att)
		awardSide = 2;
	else
		awardSide = 1;


	GroupLst::iterator it = lst.begin();	//itΪһ���佫��λ
	for (; it != lst.end(); ++it)
	{
		CombatObj* pDefender = (*it);
		Property& dPro = pDefender->stPro;	//��������
		
		/******* ��Ϊ�����ͷŶ���Ϊ���ţ�����ÿһ�ŵ���������ʵ�˺� ******/
		std::vector<Combat::SoldierRaw>::iterator itr = pDefender->soldiers.begin();	//iterΪÿ�ű���
		for (; itr != pDefender->soldiers.end(); ++itr)
		{
			Combat::SoldierRaw& rowOne = *itr;
			int realdProDef = rowOne.buff_fight.GetFinalDefenceValue(dPro.defence);

			/********* ���㹥�����ط���ɵ������˺� ********/
			int commonHurt = 0;	//ͨ�����ط����Լ�����������˺�
			if(!attacker.IsReinforce())	//���������Ԯ���ӣ�����������
			{
				/********���㹥��ս���ӳ�********/
				double attAddtion = attacker.GetTotalSkillPercent(pGroup->m_eCombat,pDefender->GetType()) / 100.0;
				/********�������********/
				
				/********�����ط�ս�������ӳ�********/
				double defAddtion = pDefender->GetTotalDeSkillPercent() / 100.0;	//�Ƽ���������������������
				defAddtion += rowOne.buff_fight.GetFinalDeskillHurtPercent();		//�ɼ��ټ����˺���buff����������
				/********�������********/
				double finalAddtion = MAX((1+attAddtion-defAddtion),0);

				double diff = MAX(realaProAtt - realdProDef, 0);	//����ɵ��˺���  max��������-������,0)
				double hurt1 = realaProAtt * 0.05 * attacker.SkillRatio() + aPro.skillAttack * 0.1;
				double hurt2 = diff * 2 * attacker.SkillRatio() * finalAddtion + aPro.skillAttack - dPro.skillDefence;
				double hurt3 = 3 * MAX(ceil(hurt1), ceil(hurt2));	//3�����������˺�
				commonHurt = MAX((int)hurt3,3);	//��СֵΪ3
			}
			else	//��Ԯ���ӣ����������˺�
			{
				Reinforce* rein = dynamic_cast<Reinforce*>(&attacker);
				if(rein)
					commonHurt = 3 * MAX(rein->GetDamage(),1);	//��СֵΪ3
				else
					commonHurt = 3;
			}
			/**************** ������� **************/

		
			if(itr->hp > 0) 	//�����һ���Ѿ��������������˺�
			{
				//��ʵ�˺�
				int realHurt = commonHurt;

				//���ǲ��Ӽ���90%
				if(pDefender->IsReinforce()) realHurt = MAX((int)(realHurt*0.1),3);

				//ȥ������˺�����ʵ��Ѫ
				int& rowhp = itr->hp;
				realHurt = (rowhp >= realHurt) ? realHurt : rowhp;		//δ������˺�
				rowhp -= realHurt;

				pb::Combat_Damage* damageone = msg.add_damagelst();	//�����յ����˺����õĽ���
				damageone->set_group(awardSide);
				damageone->set_range(skillHurtLineCnt);
				damageone->set_damage(realHurt);
				damageone->set_restblood(rowhp);
				damageone->set_buffid(0);

				//��¼ɱ��
				if(rowhp <= 0)
				{
					if(itr->killerPlayerId == 0)	//���û�м�¼��ɱ��
					{
						itr->killerPlayerId = attacker.GetPlayerId();
						if(attacker.IsPlayer() || attacker.IsShadow())	//������佫���߻�Ӱ
						{
							::Character* hero = dynamic_cast<::Character*>(&attacker);
							itr->killerCharId = hero->GetID();
						}

						//���Ŷ��⽱��
						if(::Npc* deadnpc = dynamic_cast<::Npc*>((pDefender)))
						{
							if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
							{
								if(Player* player = sPlayerPool.GetByPlayerId(attacker.GetPlayerId()))	//��������
								{
									sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

									//��¼���⽱��
									IntPairVec pairvec;
									pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
									pGroup->RecordCombatGainsInfo(attacker.GetPlayerId(),pb::CombatInfoNum,0,pairvec);

									pb::extra_awards* exAward;
									if(awardSide == 1)	//����ǹ����ͷŵļ���
										exAward = msg.add_att_extra_awards();
									else
										exAward = msg.add_def_extra_awards();
									exAward->set_awardsid(deadnpc->GetExtraAwardID());
									exAward->set_awardsnum(deadnpc->GetExtraAwardNum());
								}
							}
						}

					}
					if(pDefender->IsDead())	//����佫������
					{
						if(pDefender->killerPID == 0)
						{
							pDefender->killerPID = attacker.GetPlayerId();
							if(attacker.IsPlayer() || attacker.IsShadow())	//������佫���߻�Ӱ
							{
								::Character* hero = dynamic_cast<::Character*>(&attacker);
								pDefender->killerCID = hero->GetID();
							}
						}
					}
				}

				//���Ž���
				if(serType == pb::CoSe_SkillHurt_Att)
					_GetAwardsWithOneStep(msg,pGroup,serType,realHurt,0,realaProAtt,realdProDef);
				else
					_GetAwardsWithOneStep(msg,pGroup,serType,0,realHurt,realdProDef,realaProAtt);
			}

			skillHurtLineCnt++;	//�������һ��

			if(skillHurtLineCnt >= skillRange)		//����������Ѿ�����
				break;
		}
		/******* ����������� ******/

		/******* У׼�佫Ѫ�� ******/
		if(pDefender->IsPlayer())
		{
			::Character* hero = dynamic_cast<::Character*>(pDefender);
			hero->SetCurSolderNum(pDefender->CurHp());
			hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
		}
		/******* У׼��� ******/

		if(skillHurtLineCnt >= skillRange)		//���ҲҪ��������
			break;
	}
}

void Calc::DoHurt_Skill(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();	//������
	CombatObj* pDefence = pGroup->m_group_defence.front();	//���ط�

	if(serType == pb::CoSe_SkillHurt_Att)	//�ǹ���ʹ�ü���
	{
		pAttack->isUsedSkill = true;	//ʹ���˼���
		pAttack->leftSkillTimes -= 1;	//ʹ�ô�����һ
		LLOG("att skill time = %d pAttack=%llu",pAttack->leftSkillTimes,pAttack);
		pAttack->SetIsUseSkill();	//���ø���ʿ��ʹ���˼���
		float successRate1 = (float)((pAttack->stPro.yong*STATIC_SKILL_RATE)+(pAttack->stPro.yong*pDefence->stPro.yong)) / ((pAttack->stPro.yong*pDefence->stPro.yong) + (pDefence->stPro.yong*pDefence->stPro.yong));		//�ɹ�ʹ�ü��ܵĸ���
		pAttack->isSkillSuccess = (System::Randf(0, 1) < successRate1);		//ʹ�ü����Ƿ�ɹ�
		if(pAttack->IsReinforce()) pAttack->isSkillSuccess = true;			//��Ԯ���Ӽ��ܱض��ͷųɹ�
		if(pAttack->isSkillSuccess)	//����ͷųɹ�������㼼���˺��븽��buff
		{
			//���㼼���˺��뽱��
			_MakeHurtAndAwardsSkill(msg, pGroup,*pAttack,pGroup->m_group_defence,serType);
			
			//��buff
			_AttachBuff(pGroup,serType);
		}
	}
	
	if(serType == pb::CoSe_SkillHurt_Def)	//�Ƿ��ط�ʹ�ü���
	{
		pDefence->isUsedSkill = true;	//ʹ���˼���
		pDefence->leftSkillTimes -= 1;	//ʹ�ô�����һ
		LLOG("def skill time = %d pDefence=%llu",pDefence->leftSkillTimes,pDefence);
		pDefence->SetIsUseSkill();	//���ø���ʿ��ʹ���˼���
		float successRate1 = (float)((pDefence->stPro.yong*STATIC_SKILL_RATE)+(pDefence->stPro.yong*pAttack->stPro.yong)) / ((pDefence->stPro.yong*pAttack->stPro.yong) + (pAttack->stPro.yong*pAttack->stPro.yong));		//�ɹ�ʹ�ü��ܵĸ���
		pDefence->isSkillSuccess = (System::Randf(0, 1) < successRate1);		//ʹ�ü����Ƿ�ɹ�
		if(pDefence->IsReinforce()) pDefence->isSkillSuccess = true;			//��Ԯ���Ӽ��ܱض��ͷųɹ�
		if(pDefence->isSkillSuccess)	//����ͷųɹ�������㼼���˺��븽��buff
		{
			//���㼼���˺��뽱��
			_MakeHurtAndAwardsSkill(msg, pGroup,*pDefence,pGroup->m_group_attack,serType);

			//��buff
			_AttachBuff(pGroup,serType);
		}
	}

}


void Calc::_MakeHurtsAndAwardsCut(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double	attHp = pAttack->ForeHp();	//����ǰ��Ѫ��
	double	defHp = pDefence->ForeHp();	//�ط�ǰ��Ѫ��

	if(attHp <= 0 || defHp <= 0)	return; //���һ���Ѿ��������򲻽��л���

	//��Ԯ����û�л����׶Σ���ֻ�ж����
	//if(attObj.IsReinforce())	//��������Ԯ���ӣ����ط�������ɵ��˺�����90%
	//	defHurtToAtt *= 0.1;

	//if(defObj.IsReinforce())	//�ط�����Ԯ���ӣ��򹥷�������ɵ��˺�����90%
	//	attHurtToDef *= 0.1;


	//����˺�����
	int attRealHurt = MIN((int)attHurtToDef,(int)defHp);		//�������ط���ɵ���ʵ�˺�
	int defRealHurt = MIN((int)defHurtToAtt,(int)attHp);		//�ط��Թ�����ɵ���ʵ�˺�

	//����Ƿ�������Ч��
	if(pAttack->HaveBuffWork(dodge))
	{
		defRealHurt = pAttack->soldiers.front().buff_fight.GetFinalDamageAfterDodge(defRealHurt);
	}

	if(pAttack->HaveBuffWork(dodge))
	{
		attRealHurt = pDefence->soldiers.front().buff_fight.GetFinalDamageAfterDodge(attRealHurt);
	}
	
	//����˺�
	pAttack->HurtHp(defRealHurt);
	pDefence->HurtHp(attRealHurt);

	pb::Combat_Damage* damageAtt = msg.add_damagelst();	//�����յ����˺����õĽ���
	damageAtt->set_group(1);
	damageAtt->set_range(0);
	damageAtt->set_damage(defRealHurt);
	damageAtt->set_restblood((int)pAttack->ForeHp());
	damageAtt->set_buffid(0);

	pb::Combat_Damage* damageDef = msg.add_damagelst();	//�ط��յ����˺����õĽ���
	damageDef->set_group(2);
	damageDef->set_range(0);
	damageDef->set_damage(attRealHurt);
	damageDef->set_restblood((int)pDefence->ForeHp());
	damageDef->set_buffid(0);


	//��¼ɱ��
	if(pAttack->ForeHp() == 0)
	{
		if(pAttack->soldiers.front().killerPlayerId == 0)	//���û�м�¼��ɱ��
		{
			pAttack->soldiers.front().killerPlayerId = pDefence->GetPlayerId();
			if(pDefence->IsPlayer() || pDefence->IsShadow())	//������佫���߻�Ӱ
			{
				::Character* hero = dynamic_cast<::Character*>(pDefence);
				pAttack->soldiers.front().killerCharId = hero->GetID();
			}

			//���Ŷ��⽱��
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pAttack)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))	//��������
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//��¼���⽱��
						IntPairVec pairvec;
						pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
						pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatInfoNum,0,pairvec);

						pb::extra_awards* exAwardDef = msg.add_def_extra_awards();
						exAwardDef->set_awardsid(deadnpc->GetExtraAwardID());
						exAwardDef->set_awardsnum(deadnpc->GetExtraAwardNum());
					}
				}
			}

		}
		if(pAttack->IsDead())	//����佫������
		{
			if(pAttack->killerPID == 0)
			{
				pAttack->killerPID = pDefence->GetPlayerId();
				if(pDefence->IsPlayer() || pDefence->IsShadow())	//������佫���߻�Ӱ
				{
					::Character* hero = dynamic_cast<::Character*>(pDefence);
					pAttack->killerCID = hero->GetID();
				}
			}
		}
	}
	if(pDefence->ForeHp() == 0)
	{
		if(pDefence->soldiers.front().killerPlayerId == 0)	//���û�м�¼��ɱ��
		{
			pDefence->soldiers.front().killerPlayerId = pAttack->GetPlayerId();
			if(pAttack->IsPlayer() || pAttack->IsShadow())	//������佫���߻�Ӱ
			{
				::Character* hero = dynamic_cast<::Character*>(pAttack);
				pDefence->soldiers.front().killerCharId = hero->GetID();
			}

			//���Ŷ��⽱��
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pDefence)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))	//��������
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//��¼���⽱��
						IntPairVec pairvec;
						pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
						pGroup->RecordCombatGainsInfo(pAttack->GetPlayerId(),pb::CombatInfoNum,0,pairvec);

						pb::extra_awards* exAwardAtt = msg.add_att_extra_awards();
						exAwardAtt->set_awardsid(deadnpc->GetExtraAwardID());
						exAwardAtt->set_awardsnum(deadnpc->GetExtraAwardNum());
					}
				}
			}

		}
		if(pDefence->IsDead())	//����佫������
		{
			if(pDefence->killerPID == 0)
			{
				pDefence->killerPID = pAttack->GetPlayerId();
				if(pAttack->IsPlayer() || pAttack->IsShadow())	//������佫���߻�Ӱ
				{
					::Character* hero = dynamic_cast<::Character*>(pAttack);
					pDefence->killerCID = hero->GetID();
				}
			}
		}
	}

	//���Ž���
	_GetAwardsWithOneStep(msg,pGroup,pb::CoSe_CutToDie,attRealHurt,defRealHurt);


	//У׼�佫Ѫ��
	if(pAttack->IsPlayer())
	{
		::Character* hero = dynamic_cast<::Character*>(pAttack);
		hero->SetCurSolderNum(pAttack->CurHp());
		hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
	}
	if(pDefence->IsPlayer())
	{
		::Character* hero = dynamic_cast<::Character*>(pDefence);
		hero->SetCurSolderNum(pDefence->CurHp());
		hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
	}
}

double Calc::_NewCutHurt(CombatObj* attacker, CombatObj* defender, uint32 terrType)
{
	//�˺� = max��max(max(��������-�Է�����,0)*(0.55+(��ǰ��Ѫ/��ǰ��Ѫ)*0.5)*(1+ս�����Ƽӳ�)*(1+���μӳ�-�������ӳ�+�����ӳ�)+��������-�Է���,0)*(��ͳ/��ͳ)������*5%��
	Property& aPro = attacker->stPro;	//����pro
	Property& dPro = defender->stPro;	//�ط�pro

	int realaProAtt = attacker->soldiers.front().buff_fight.GetFinalAttackValue(aPro.attack);
	int realdProDef = defender->soldiers.front().buff_fight.GetFinalDefenceValue(dPro.defence);

	float ratioAttToDef = CommonRatio(attacker->tactic, defender->tactic);	//�������ط��Ŀ���
	
	/********���㹥���˺��ӳ�********/
	//��������Ƽ������ļӳ�
	double attAddtion = attacker->GetTotalFightPercent()/100.0;
	
	//TODO::�����������ļ���  -----

	/********�������********/

	double hurt1 = MAX(realaProAtt - realdProDef, 0);	//max(��������-�Է�����,0)

	double hurt2 = MAX((hurt1
		*(0.55 + (attacker->ForeHp()/defender->ForeHp())*0.5)									//(0.55+(��ǰ��Ѫ/��ǰ��Ѫ)*0.5)
		*(1 + ratioAttToDef)																//(1+ս�����Ƽӳ�)
		*(1 + attacker->GetTerrTalentAddtion(terrType) + attAddtion )								//(1+���μӳ� + �����ӳ�)
		+ aPro.attackEx - dPro.defenceEx),0);												//��������-�Է���

	double hurt3 = hurt2 * ((double)attacker->stPro.tong/defender->stPro.tong);

	double hurt4 = aPro.attack*0.05;

	double hurtFinal = MAX(MAX(hurt3,hurt4),1);

	hurtFinal = ceil(hurtFinal) * 3;		//������������˺�

	if(hurtFinal > MAX_HURT)
		hurtFinal = MAX_HURT;

	return hurtFinal;
}

void Calc::DoHurt_Cut(pb::Combat_Seque& msg, CombatGroup* pGroup)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double attHurtToDef = _NewCutHurt(pAttack,pDefence,pGroup->m_terrainType);	//�������ط���ɵ�ƽ���˺�	�����˺�	��СֵΪ3
	double defHurtToAtt = _NewCutHurt(pDefence,pAttack,pGroup->m_terrainType);	//�ط��Թ�����ɵ�ƽ���˺�	�����˺�	��СֵΪ3

	while(!CheckDeadNormal(pAttack) && !CheckDeadNormal(pDefence))
	{
		//����˺��뽱�� ����
		_MakeHurtsAndAwardsCut(msg, pGroup,attHurtToDef,defHurtToAtt);
	}

}

void Calc::_AttachBuff(CombatGroup* pGroup, pb::E_Combat_Serialize serType)
{
	if(serType == pb::CoSe_SkillHurt_Att)	//�����ͷż���
	{
		CombatObj* pAttack = pGroup->m_group_attack.front();
		CombatObj* pDefence = pGroup->m_group_defence.front();
		uint32 skillid = pAttack->SkillID();					//����id
		int buffid = sSkillDataMgr.GetBuffIDBySkill(skillid);//���ܸ��ӵ�buffid
		if(buffid != 0)	//��buff
		{
			int target = sSkillDataMgr.GetBuffTarget(buffid);	//buff����Ŀ��
			int attachRow = sSkillDataMgr.GetBuffRows(buffid);	//buff��������
			if(target == 1)	//�з�
				_BuffToRow(pGroup->m_group_defence,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
			else	//�Լ�
				_BuffToRow(pGroup->m_group_attack,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
		}
	}
	else if(serType == pb::CoSe_SkillHurt_Def)		//�ط��ͷż���
	{
		CombatObj* pAttack = pGroup->m_group_defence.front();
		CombatObj* pDefence = pGroup->m_group_attack.front();
		uint32 skillid = pAttack->SkillID();					//����id
		int buffid = sSkillDataMgr.GetBuffIDBySkill(skillid);//���ܸ��ӵ�buffid
		if(buffid != 0)	//��buff
		{
			int target = sSkillDataMgr.GetBuffTarget(buffid);	//buff����Ŀ��
			int attachRow = sSkillDataMgr.GetBuffRows(buffid);	//buff��������
			if(target == 1)	//�з�
				_BuffToRow(pGroup->m_group_attack,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
			else	//�Լ�
				_BuffToRow(pGroup->m_group_defence,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
		}
	}
}

void Calc::_BuffToRow(GroupLst& buffAttacher, CombatObj* buffSender, int buffid, int buffAttchRow, uint64 roundNum)
{
	int rowCount = 0;	//����������
	uint64 buffSenderPID = 0;	//�ͷŷ������id
	uint32 buffSenderCID = 0;	//�ͷŷ����佫id
	if(buffAttchRow <= 0 || buffid <= 0) return;	//������buffid������Ҫ�� ����

	if(!buffSender->IsNpc())	//�������npc���������
	{
		buffSenderPID = buffSender->GetPlayerId();
		if(!buffSender->IsReinforce())	//���ǹ��ǲ��ӣ���Ϊ�佫���Ӱ
		{
			::Character* hero = dynamic_cast<::Character*>(buffSender);
			buffSenderCID = hero->GetID();
		}
	}

	GroupLst::iterator it = buffAttacher.begin();	//itΪһ���佫��λ
	for (; it != buffAttacher.end(); ++it)
	{
		CombatObj* pDefender = (*it);

		/******* ����buff��ÿһ�ŵ������� ******/
		std::vector<Combat::SoldierRaw>::iterator itr = pDefender->soldiers.begin();	//iterΪÿ�ű���
		for (; itr != pDefender->soldiers.end(); ++itr)
		{
			Combat::SoldierRaw& soldierRow = *itr;

			if(soldierRow.hp != 0)	//�����������buff
				soldierRow.buff_fight.AddBuff(buffid,buffSenderPID,buffSenderCID,roundNum,buffSender->stPro.defence,buffSender->GetLevel());

			rowCount++;	//�������һ��

			if(rowCount >= buffAttchRow)		//����������Ѿ�����
				break;
		}

		if(rowCount >= buffAttchRow)		//���ҲҪ��������
			break;
	}
}

void Calc::DoHurt_Buff(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType)
{
	//������buff����
	_BuffWork(msg, pGroup, pGroup->m_group_attack,serType,true);
	
	//�ط���buff����
	_BuffWork(msg, pGroup, pGroup->m_group_defence,serType,false);
}

void Calc::_BuffWork(pb::Combat_Seque& msg, CombatGroup* pGroup, GroupLst& workLst, pb::E_Combat_Serialize serType, bool isAttWork)
{
	int herocount = 0;	//�����佫������¼
	int range = 0;
	GroupLst::iterator it = workLst.begin();
	for (; it != workLst.end(); ++it)
	{
		if(herocount++  > 6)	//���6���佫�������ս̫���佫
			break;

		CombatObj* tmpObj = (*it);
		std::vector<Combat::SoldierRaw>::iterator itr = tmpObj->soldiers.begin();	//iterΪÿ��ʿ��
		for (; itr != tmpObj->soldiers.end(); ++itr)
		{
			Combat::SoldierRaw& soldierRow = *itr;
			if(soldierRow.hp > 0)	//����ʿ��������
			{
				_BuffRowWork(msg, pGroup, tmpObj, soldierRow, serType, range, isAttWork);
			}
			range++;
		}
		//У׼�佫Ѫ��
		if(tmpObj->IsPlayer())
		{
			::Character* hero = dynamic_cast<::Character*>(tmpObj);
			hero->SetCurSolderNum(hero->CurHp());
			hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
		}
	}
}

void Calc::_BuffRowWork(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj* buffOwner, Combat::SoldierRaw& soldierRow, pb::E_Combat_Serialize serType, int rownum, bool isAttWork)
{
	if(soldierRow.hp <= 0) return;	//�������ʿ���������˳�

	uint32 worktime = 0;
	if(serType == pb::CoSe_BuffBefore)
		worktime = 1;
	else if(serType == pb::CoSe_BuffAfter)
		worktime = 2;
	else if(serType == pb::CoSe_BuffMove)
		worktime = 3;

	FWorkBuffMap buffMap = soldierRow.buff_fight.GetFinalBuffMap();
	for(FWorkBuffMap::reverse_iterator rit=buffMap.rbegin();rit!=buffMap.rend();rit++) 
	{
		BuffInfo* buffOneInfo = rit->second;

		if(buffOneInfo->worktime == worktime)	//����׶�ƥ�䣬����Ч
		{
			buffOneInfo->isWork = true;		//�����ù��ص�buff��Ч
			switch(buffOneInfo->buffTyp)
			{
			case Combat::percentdeskillhurt:
				break;
			case Combat::fixedattackup:				//�ٷֱȼӷ���
				break;
			case Combat::fixeddefenceup:			//�̶��ӷ�
				break;
			case Combat::confuse:					//����
				break;
			case Combat::percentdamage:				//�ٷֱȵ�Ѫ
				{
					/******* ����˺� *******/
					float commonHurt = soldierRow.hp * buffOneInfo->value1;
					int trueHurt = (soldierRow.hp >= (int)commonHurt) ? (int)commonHurt : soldierRow.hp;		//δ������˺�
					soldierRow.hp -= (int)trueHurt;
					
					pb::Combat_Damage* damageone = msg.add_damagelst();
					if(isAttWork)
						damageone->set_group(1);
					else
						damageone->set_group(2);
					damageone->set_range(rownum);
					damageone->set_damage(trueHurt);
					damageone->set_restblood(soldierRow.hp);
					damageone->set_buffid(buffOneInfo->buffId);


					/******* ��¼ɱ�� *******/
					if(soldierRow.hp <= 0)	//������ű�����
					{
						if(soldierRow.killerPlayerId == 0)
						{
							soldierRow.killerPlayerId = buffOneInfo->fromP;
							soldierRow.killerCharId = buffOneInfo->fromC;

							//���Ŷ��⽱��
							if(::Npc* deadnpc = dynamic_cast<::Npc*>((buffOwner)))
							{
								if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
								{
									if(Player* player = sPlayerPool.GetByPlayerId(buffOneInfo->fromP))	//��������
									{
										sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

										//��¼���⽱��
										IntPairVec pairvec;
										pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
										pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatInfoNum,0,pairvec);

										pb::extra_awards* exAward;
										if(isAttWork)	//��������buff������
											exAward = msg.add_def_extra_awards();
										else
											exAward = msg.add_def_extra_awards();
										exAward->set_awardsid(deadnpc->GetExtraAwardID());
										exAward->set_awardsnum(deadnpc->GetExtraAwardNum());
									}
								}
							}

						}
						if(buffOwner->IsDead())	//����佫������
						{
							if(buffOwner->killerPID == 0)
							{
								buffOwner->killerPID = buffOneInfo->fromP;
								buffOwner->killerCID = buffOneInfo->fromC;
							}
						}
					}

					/******* ���Ž��� *******/
					_GetAwardsWithBuffHopOnce(msg, pGroup,buffOwner,buffOneInfo,(int)trueHurt,isAttWork);
				}
				break;
			case Combat::fixeddamage:				//�̶�Ѫ����Ѫ
				{
					/******* ����˺� *******/
					float commonHurt = buffOneInfo->value1;
					int trueHurt = (soldierRow.hp >= (int)commonHurt) ? (int)commonHurt : soldierRow.hp;		//δ������˺�
					soldierRow.hp -= (int)trueHurt;

					pb::Combat_Damage* damageone = msg.add_damagelst();
					if(isAttWork)
						damageone->set_group(1);
					else
						damageone->set_group(2);
					damageone->set_range(rownum);
					damageone->set_damage(trueHurt);
					damageone->set_restblood(soldierRow.hp);
					damageone->set_buffid(buffOneInfo->buffId);

					/******* ��¼ɱ�� *******/
					if(soldierRow.hp <= 0)	//������ű�����
					{
						if(soldierRow.killerPlayerId == 0)
						{
							soldierRow.killerPlayerId = buffOneInfo->fromP;
							soldierRow.killerCharId = buffOneInfo->fromC;

							//���Ŷ��⽱��
							if(::Npc* deadnpc = dynamic_cast<::Npc*>((buffOwner)))
							{
								if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
								{
									if(Player* player = sPlayerPool.GetByPlayerId(buffOneInfo->fromP))	//��������
									{
										sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

										//��¼���⽱��
										IntPairVec pairvec;
										pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
										pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatInfoNum,0,pairvec);

										pb::extra_awards* exAward;
										if(isAttWork)	//��������buff������
											exAward = msg.add_def_extra_awards();
										else
											exAward = msg.add_def_extra_awards();
										exAward->set_awardsid(deadnpc->GetExtraAwardID());
										exAward->set_awardsnum(deadnpc->GetExtraAwardNum());
									}
								}
							}

						}
						if(buffOwner->IsDead())	//����佫������
						{
							if(buffOwner->killerPID == 0)
							{
								buffOwner->killerPID = buffOneInfo->fromP;
								buffOwner->killerCID = buffOneInfo->fromC;
							}
						}
					}

					/******* ���Ž��� *******/
					_GetAwardsWithBuffHopOnce(msg, pGroup,buffOwner,buffOneInfo,(int)trueHurt,isAttWork);
				}
				break;
			case Combat::percentdefenceup:			//�ٷֱȼӷ���
				break;
			case Combat::percentattackup:			//�ٷֱȼӹ���
				break;
			default:
				break;
			}
		}
		if(soldierRow.hp <= 0) break;	//���ʿ���Ѿ�������������Чbuff
	}
	soldierRow.buff_fight.RefreshByStep(worktime);
}

void Calc::_GetAwardsWithBuffHopOnce(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj* buffOwner, Combat::BuffInfo* buffOneInfo, uint32 realHurtOfBuff, bool isAttWork)
{
	//����=������ת����������ɱ����������1+���ּӳɣ����������
	//����=����*0.2

	CombatObj* pDefence = buffOwner;

	//���ȼ����˺��ļӳ�	//ɱ�����������������
	int attOrgHurtToDef = realHurtOfBuff;	//������ɵ�ԭʼ�˺�
	int attRealHurtToDef = realHurtOfBuff;	//˫�����ɱ

	//���㽱��
	if(attOrgHurtToDef > 0)	//Ҫ���ط�������
	{
		//����ط����佫���߼��佫����Ӱ����Ԯ����	//���Ի�ý���	�����л�Ӱ����Ԯ����ֻ�ܻ�����exp���������ҽ�����
		if(pDefence->IsPlayer() || pDefence->IsShadow() || pDefence->IsReinforce())
		{
			/********** ���㿪ʼ ********/
			//�������
			float needFood = 0.0;
			const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(pDefence->soldierLv);
			if (pTable == NULL) 
			{
				LLOG("[2000 ERROR] _GetAwardsWithOneStep pTable is not found,lv=%d",pDefence->soldierLv);
				return;
			}
			needFood = pTable->NeedFood() * attOrgHurtToDef;

			//����ת������
			float foodToExp = FOOD_TO_EXP[pGroup->m_eCombat];

			//�����ɱ��
			float killRatio = 2;
			Property& dPro = pDefence->stPro;	//�ط�����
			killRatio = (float)buffOneInfo->fromStProAttack / (float)dPro.defence;
			if(killRatio > 2)
				killRatio = 2;
			else if(killRatio < 0.5)
				killRatio = 1;
			else
				killRatio = (killRatio + 1) * 2 / 3;

			//������ּӳ�
			float totalAddtions = 0.0;
			//�Ƽ���������,������ṩ�ľ���ӳ�
			totalAddtions += (((float)pDefence->GetTotalExpPercent(pGroup->m_eCombat))/((float)100));
			if(pGroup->m_eCombat == CT_Country)	//ֻ�й�ս���еļӳ�
			{
				//�ȼ����ṩ�ľ���ӳ�
				uint32 levelAtt = buffOneInfo->fromStProLevel;
				uint32 levelDef = pDefence->GetPlayerLevel();
				if((levelAtt+16) <= levelDef)	//�ȶԷ��ȼ�С16��
				{
					uint32 levelDif = levelDef - levelAtt - 15;
					totalAddtions += MIN(((float)levelDif * (float)0.1),(float)1.0);
				}

				//TODO::�������ӳ�		Ѱ·�Ż�����

				//TODO::�佫�츳�ӳ�	�����佫������

			}

			//�������
			float crossTimes = 1.0;
#ifdef _SERVER_CROSS_
			crossTimes = 1.5;
#endif

			//�������ս���
			int exp = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
			int expPlayer = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
			int money = MAX((int)ceil(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes * 0.2),1);

			if(pGroup->m_eCombat == CT_Crusade)	//Զ������þ����뽱��
			{
				exp = 0;
				expPlayer = 0;
				money = 0;
			}
			/********** ������� ********/


			/********* ���Ž��� ********/
			//֮ǰ�汾�������佫��������ս���������㽱���뾭�飬���ڸ�Ϊ���������Ƿ�����㣿 Try it
			uint32 exploitOfDef = 0;
			//����佫����Ӱ����Ԯ���ӣ�������ҵľ���������
			if(Player* master = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))
			{
				sReward.Change(*master, Reward::PlayerExp, expPlayer);	//��Ҿ���
				sReward.Change(*master, Reward::Coin_Silver, money);	//����
				master->AddActivityStarBoxExp(expPlayer);				//��һ����
				exploitOfDef = master->HurtToExploit(ENUM_TYPE_DEATH,attOrgHurtToDef); //����������ѫ
			}

			//����佫�����佫����
			if(pDefence->IsPlayer())
			{
				::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
				heroDefence->TryAddXP(exp);
			}
			//�佫�Ļ�Ӱ����ԭ�佫����
			if(pDefence->IsShadow())
			{
				::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
				Character* charone = heroDefence->m_player->m_characterStorage->MutableCharacter(heroDefence->GetID());
				if(charone)
					charone->TryAddXP(exp);
			}
			/********* ���Ž��� **********/

			/********** �����Ϣ *********/
			if(expPlayer > 0)
			{
				pb::Combat_Reward* rewardsOne = msg.add_rewardlst();
				if(isAttWork)
					rewardsOne->set_group(1);
				else
					rewardsOne->set_group(2);
				rewardsOne->set_exp(expPlayer);
				rewardsOne->set_coin(money);
				rewardsOne->set_playerid(pDefence->GetPlayerId());
			}
			/*****************************/

			/******** ��¼����ս������ ******/
			//��¼ս��
			IntPairVec tmp;
			//����佫����Ӱ����Ԯ���Ӿ���������Ҿ��飬������ʧ
			pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatPlayerExp,expPlayer,tmp);
			pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatDead,attOrgHurtToDef,tmp);
			pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatExploit,exploitOfDef,tmp);
			/********** ��¼���� **********/


			/********* ���ӽ��� *********/
			if(::Character* heroDefence = dynamic_cast<::Character*>(pDefence))
			{
				if(heroDefence->GetIsInAutoCountryWar())	//�佫���Զ���ս��
				{
					heroDefence->m_player->m_AutoCountryWarLog->AddExpGet(expPlayer);	//��¼�Զ���ս��þ���
				}
			}
			/********* ���ӽ��� *********/

		}

		//������ط�����˺���������õĽ���
		//����������佫���߼��佫����Ӱ����Ԯ����	//���Ի�ù�ѫ����
		uint32 exploitOfAtt = 0;	//������õĹ�ѫ
		if(buffOneInfo->fromP > 0)
		{
			if(Player* player = sPlayerPool.GetByPlayerId(buffOneInfo->fromP))	//��������
			{
				if(buffOneInfo->fromC > 0)
				{
					if(Character* charone = player->m_characterStorage->GetCharacterByTableId(buffOneInfo->fromC))
					{
						attRealHurtToDef *= (1 + charone->GetTotalDoubleKill(pGroup->m_eCombat)/100);
						
						if(charone->GetIsInAutoCountryWar())	//�佫���Զ���ս��
						{
							charone->m_player->m_AutoCountryWarLog->AddKillCount(attRealHurtToDef);	//��¼�Զ���ս��ɱ��
							charone->killOnceAHero += attRealHurtToDef;
						}
						CombatGroup::_SaveStaticKillOnce(*charone,pGroup->m_eCombat,attRealHurtToDef);	//��¼������ɱ����
					}
				}
				//�ǹ�ս�������ӹ�ѫ
				if(pGroup->m_eCombat == CT_Country)
				{
					exploitOfAtt = player->HurtToExploit(ENUM_TYPE_KILL,attRealHurtToDef);
				}

				/******** ��¼����ս������ ******/
				//��¼ս��
				IntPairVec tmp;
				pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatKills,attRealHurtToDef,tmp);
				pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatExploit,exploitOfAtt,tmp);
				/********** ��¼���� **********/
			}
		}
	}
}



//ʹ���˼���	�����б��ط��б������Ƿ�ʹ�ü��ܣ��ط��Ƿ�ʹ�ü��ܣ�������Ϣ��ս������
void Calc::UseSkill(CombatGroup* pGroup, GroupLst& attacker, GroupLst& defender, bool isAttakcerUseSkill, bool isDefenderUserSkill, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy)
{
 //   CombatObj& rhs = *attacker.front();		//��������һ���佫
 //   CombatObj& lhs = *defender.front();		//���ط���һ���佫

	//if(isAttakcerUseSkill)
	//{
	//	rhs.isUsedSkill = true;
	//	rhs.leftSkillTimes -= 1;
	//	float successRate1 = (float)((rhs.stPro.yong*STATIC_SKILL_RATE)+(rhs.stPro.yong*lhs.stPro.yong)) / ((rhs.stPro.yong*lhs.stPro.yong) + (lhs.stPro.yong*lhs.stPro.yong));		//�ɹ�ʹ�ü��ܵĸ���
	//	//LLOG("successRate1 = %f",successRate1);
	//	rhs.isSkillSuccess = (System::Randf(0, 1) < successRate1);		//ʹ�ü����Ƿ�ɹ�
	//	if(rhs.IsReinforce())
	//		rhs.isSkillSuccess = true;
	//	if (rhs.isSkillSuccess) 
	//	{
	//		AttachBuffs(attacker,defender,1);
	//		_SkillHurt(pGroup,rhs, defender, 2, msg, tpy);		//�ɹ�����㼼���˺�  ���������������У����˷���ţ��������Ϣ
	//	}
	//	msg.set_attskillid(rhs.SkillID());
	//}
	//else
	//	msg.set_attskillid(0);

 //   if (isDefenderUserSkill && lhs.soldiers.size() && lhs.soldiers.front().hp > 0 && rhs.soldiers.size() && rhs.soldiers.front().hp > 0)
 //   {
 //       lhs.isUsedSkill = true;
	//	lhs.leftSkillTimes -= 1;
 //       float successRate2 = (float)((lhs.stPro.yong*STATIC_SKILL_RATE)+(rhs.stPro.yong*lhs.stPro.yong)) / ((rhs.stPro.yong*lhs.stPro.yong) + (rhs.stPro.yong*rhs.stPro.yong));		//�ɹ�ʹ�ü��ܵĸ���
 //       //LLOG("successRate2 = %f",successRate2);
	//	lhs.isSkillSuccess = (System::Randf(0, 1) < successRate2);
	//	if(lhs.IsReinforce())
	//		lhs.isSkillSuccess = true;
 //       if (lhs.isSkillSuccess) 
	//	{
	//		AttachBuffs(defender,attacker,2);
	//		_SkillHurt(pGroup, lhs, attacker, 1, msg, tpy);
	//	}
	//	msg.set_defskillid(lhs.SkillID());
	//}
	//else
	//	msg.set_defskillid(0);

	//_GetBuffs(attacker, defender, msg);

	////if(isAttakcerUseSkill)
	////{
	////	if(rhs.isSkillSuccess)
	////	{
	////		_SkillHurt(rhs, defender, 2, msg, tpy);
	////	}
	////	msg.set_attskillid(rhs.SkillID());
	////}
	////else
	////	msg.set_attskillid(0);

	////if(isDefenderUserSkill && !lhs.IsDead())
	////{
	////	if(lhs.isSkillSuccess) 
	////	{
	////		_SkillHurt(lhs, attacker, 1, msg, tpy);
	////	}
	////	msg.set_defskillid(lhs.SkillID());
	////}
	////else
	////	msg.set_defskillid(0);
	//
 //   //ͷ��û��������ͷ���������Կ�
 //   while (!CheckDead(pGroup,attacker.front(),defender.front(),msg,false) && !CheckDead(pGroup,defender.front(),attacker.front(),msg,true))
 //   {
 //       int attackerHurt = _CalcCommonHurt(rhs, lhs, isAttakcerUseSkill ? 0.1f : 0);
 //       int defenderHurt = _CalcCommonHurt(lhs, rhs, isDefenderUserSkill ? 0.1f : 0);
	//	////����˺�
	//	//pb::Combat_Damage* damageOne = msg.add_damages();
	//	//damageOne->set_period(3);
	//	//damageOne->set_group(2);
	//	//damageOne->set_range(0);
	//	//damageOne->set_type(3);
	//	//damageOne->set_damage(attackerHurt);

	//	//pb::Combat_Damage* damageTwo = msg.add_damages();
	//	//damageTwo->set_period(3);
	//	//damageTwo->set_group(1);
	//	//damageTwo->set_range(0);
	//	//damageTwo->set_type(3);
	//	//damageTwo->set_damage(defenderHurt);

 //       _DoHurt(pGroup,rhs, lhs, attackerHurt, defenderHurt, msg ,tpy,3);
 //   }
	
	//RefreshSoldiers(attacker);
	//RefreshSoldiers(defender);
	//rhs.isSelectTactic = false;
	//lhs.isSelectTactic = false;
}
bool Calc::CheckDead(CombatGroup* pGroup, CombatObj* obj, CombatObj* killer, pb::GS2C_Combat_Select_Tactic& msg, bool isKillerAtt)
{
    bool ret = false;
  //  if((obj->soldiers.size() && obj->soldiers.front().hp <= 0) || obj->soldiers.size() == 0)
  //  {
		////LLOG("one dead");
		////obj->m_fightBuff.RefreshByRange();
		//if(obj->soldiers.size() != 0)
		//	obj->soldiers.erase(obj->soldiers.begin());
  //      obj->isSelectTactic = false; //��һ��ʿ������ѡ����
  //      ret = true;

		////���ʿ���н������򷢷Ž���
		//if(Npc* deadnpc = dynamic_cast<Npc*>(obj))
		//{
		//	if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
		//	{
		//		if(Character* killerchar = dynamic_cast<Character*>(killer))
		//		{
		//			pb::extra_awards* awardsmsg;
		//			if(isKillerAtt)	//��ɱ��Ϊ������
		//				awardsmsg = msg.add_att_extra_awards();
		//			else
		//				awardsmsg = msg.add_def_extra_awards();
		//			awardsmsg->set_awardsid(deadnpc->GetExtraAwardID() );
		//			awardsmsg->set_awardsnum(deadnpc->GetExtraAwardNum());

		//			if (killerchar->m_player)
		//			{
		//				sReward.Change(*(killerchar->m_player), deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum());
		//				
		//				IntPairVec pairvec;
		//				pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
		//				pGroup->RecordCombatGainsInfo(killerchar->m_player->GetGuid(),pb::CombatInfoNum,0,pairvec);

		//				bool isgot = false;
		//				for(IntPairVec::iterator it = killerchar->m_extraAwards.begin(); it != killerchar->m_extraAwards.end(); it++)
		//				{
		//					if(it->first == deadnpc->GetExtraAwardID())
		//					{
		//						isgot = true;
		//						it->second += deadnpc->GetExtraAwardNum();
		//						break;
		//					}
		//				}
		//				if(!isgot)
		//					killerchar->m_extraAwards.push_back(std::make_pair(deadnpc->GetExtraAwardID(),deadnpc->GetExtraAwardNum()));
		//			}
		//		}
		//	}
		//}

  //  }
    return ret;
}

bool Calc::CheckDead(CombatGroup* pGroup, CombatObj* obj, CombatObj* killer, pb::Combat_Buff_AllInfo& msg, bool isKillerAtt)
{
	bool ret = false;
	if(obj->soldiers.size() && obj->soldiers.front().hp <= 0)
	{
		//LLOG("one dead");
		//obj->m_fightBuff.RefreshByRange();
		obj->soldiers.erase(obj->soldiers.begin());
		obj->isSelectTactic = false; //��һ��ʿ������ѡ����
		ret = true;

		//���ʿ���н������򷢷Ž���
		if(Npc* deadnpc = dynamic_cast<Npc*>(obj))
		{
			if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
			{
				if(Character* killerchar = dynamic_cast<Character*>(killer))
				{
					pb::extra_awards* awardsmsg;
					if(isKillerAtt)	//��ɱ��Ϊ������
						awardsmsg = msg.add_att_extra_awards();
					else
						awardsmsg = msg.add_def_extra_awards();
					awardsmsg->set_awardsid(deadnpc->GetExtraAwardID() );
					awardsmsg->set_awardsnum(deadnpc->GetExtraAwardNum() );

					if (killerchar->m_player)
					{
						sReward.Change(*(killerchar->m_player), deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum() );

						bool isgot = false;
						for(IntPairVec::iterator it = killerchar->m_extraAwards.begin(); it != killerchar->m_extraAwards.end(); it++)
						{
							if(it->first == deadnpc->GetExtraAwardID())
							{
								isgot = true;
								it->second += deadnpc->GetExtraAwardNum();
								break;
							}
						}
						if(!isgot)
							killerchar->m_extraAwards.push_back(std::make_pair(deadnpc->GetExtraAwardID(),deadnpc->GetExtraAwardNum()));
					}
				}
			}
		}

	}
	return ret;
}


void Calc::RefreshSoldiers(GroupLst& lst, CombatObj* killer, pb::GS2C_Combat_Select_Tactic& msg, bool isKillerAtt)
{
	LLOG("Calc::RefreshSoldiers");
	//int endcount = 0;
	//GroupLst::iterator it = lst.begin();
	//for(; it != lst.end(); ++it)
	//{
	//	if(endcount++ > 9)	break;		//ֻ����ǰʮ���佫
	//	LLOG("one obj soldier size=%d",(*it)->soldiers.size());
	//	//if((*it)->CurHp() <= 0)	continue;

	//	CombatObj* comobj = *it;

	//	while ((*it)->soldiers.size() && (*it)->soldiers.front().hp <= 0)
	//	{
	//		//(*it)->m_fightBuff.RefreshByRange();
	//		(*it)->soldiers.erase((*it)->soldiers.begin());
	//		(*it)->isSelectTactic = false;

	//		//���ʿ���н������򷢷Ž���
	//		if(::Npc* deadnpc = dynamic_cast<::Npc*>((*it)))
	//		{
	//			if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
	//			{
	//				if(Character* killerchar = dynamic_cast<Character*>(killer))
	//				{
	//					pb::extra_awards* awardsmsg;
	//					if(isKillerAtt)	//��ɱ��Ϊ������
	//						awardsmsg = msg.add_att_extra_awards();
	//					else
	//						awardsmsg = msg.add_def_extra_awards();
	//					awardsmsg->set_awardsid(deadnpc->GetExtraAwardID());
	//					awardsmsg->set_awardsnum(deadnpc->GetExtraAwardNum());

	//					if (killerchar->m_player)
	//					{
	//						sReward.Change(*(killerchar->m_player), deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void Calc::RefreshSoldiers(GroupLst& lst, CombatObj* killer, pb::Combat_Buff_AllInfo& msg, bool isKillerAtt)
{
	LLOG("Calc::RefreshSoldiers");
	int endcount = 0;
	GroupLst::iterator it = lst.begin();
	for(; it != lst.end(); ++it)
	{
		if(endcount++ > 9)	break;		//ֻ����ǰʮ���佫
		LLOG("one obj soldier size=%d",(*it)->soldiers.size());
		//if((*it)->CurHp() <= 0)	continue;

		CombatObj* comobj = *it;

		while ((*it)->soldiers.size() && (*it)->soldiers.front().hp <= 0)
		{
			//(*it)->m_fightBuff.RefreshByRange();
			(*it)->soldiers.erase((*it)->soldiers.begin());
			(*it)->isSelectTactic = false;

			//���ʿ���н������򷢷Ž���
			if(Npc* deadnpc = dynamic_cast<Npc*>((*it)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Character* killerchar = dynamic_cast<Character*>(killer))
					{
						pb::extra_awards* awardsmsg;
						if(isKillerAtt)	//��ɱ��Ϊ������
							awardsmsg = msg.add_att_extra_awards();
						else
							awardsmsg = msg.add_def_extra_awards();
						awardsmsg->set_awardsid(deadnpc->GetExtraAwardID());
						awardsmsg->set_awardsnum(deadnpc->GetExtraAwardNum());

						if (killerchar->m_player)
						{
							sReward.Change(*(killerchar->m_player), deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());
						}
					}
				}
			}
		}
	}
}