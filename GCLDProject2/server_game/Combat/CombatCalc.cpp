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
				*(1 + tacticRatio) //战术克制  应该读表
				*(1 + attacker.terrainPercent/100.0 + attacker.GetTotalFightPercent()/100.0 - defender.guardAddPercent/100.0) //地形加成+buff-对方守卫军加成
				+ aPro.attackEx - dPro.defenceEx),0); //己方穿刺-对方格挡

	double hurt = MAX((hurt2*((double)attacker.stPro.tong/defender.stPro.tong)),(aPro.attack*0.05));
	hurt = ceil(hurt) * 3;		//三个方阵的总伤害
    //hurt *= 3; //三个方阵的总伤害

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

//挂载buff
void Calc::AttachBuffs(GroupLst& attackers, GroupLst& defenders, uint32 attidx)
{
	//LLOG("Calc::AttachBuffs:%d",attidx);
	CombatObj& rhs = *attackers.front();
	
	uint32 skillid = rhs.SkillID();
	int buffid = sSkillDataMgr.GetBuffIDBySkill(skillid);
	//LLOG("skillid = %d",skillid);
	//LLOG("buffid = %d",buffid);
	if(buffid != 0)	//该技能附带buff
	{
		int target = sSkillDataMgr.GetBuffTarget(buffid);
		int attachRow = sSkillDataMgr.GetBuffRows(buffid);
		int rowcount = attachRow;
		//LLOG("target=%d",target);
		if(target == 1)		//加在敌方身上
		{
			uint32 to = attidx==1? 2:1;
			//LLOG("to=%d",to);
			GroupLst::iterator it = defenders.begin();
			int rownum = 0;
			for(; it != defenders.end(); ++it)
			{
				CombatObj* tmpj = (*it);
				if(rownum < attachRow && rowcount>0)		//总行数小于挂载行数
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
		else if(target == 2)	//加在己方身上
		{
			GroupLst::iterator it = attackers.begin();
			int rownum = 0;
			for(; it != defenders.end(); ++it)
			{
				CombatObj* tmpj = (*it);
				if(rownum < attachRow && rowcount>0)		//总行数小于挂载行数
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

//直接技能伤害，特殊效果另外处理  使用技能方，接受伤害的队列，待填充消息，战斗类型
void Calc::_SkillHurt(CombatGroup* pGroup,CombatObj& attacker, GroupLst& lst, int defenderid, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy)
{
 //   LLOG("SkillHurt -- attacker:hp(%d),atk(%d),def(%d),tactic(%d),range(%d)", (int)attacker.ForeHp(), attacker.stPro.attack, attacker.stPro.defence, attacker.tactic, (int)attacker.SkillRange());

 //   Property& aPro = attacker.stPro;	//攻击属性

	//uint8 skillHurtLineCnt = 0;		//造成前几排的伤害
	//int range = 0;
 //   GroupLst::iterator it = lst.begin();	//it为武将
 //   for (; it != lst.end(); ++it)
 //   {
	//	CombatObj& defender = *(*it);
	//	CombatObj* tmpj = (*it);
 //       Property& dPro = defender.stPro;	//防守属性

 //       LLOG("SkillHurt -- defender:hp(%d),atk(%d),def(%d),tactic(%d)", (int)defender.ForeHp(), defender.stPro.attack, defender.stPro.defence, defender.tactic);

 //       double hurt = 0;
 //       double diff = MAX(aPro.attack - dPro.defence, 0);	//可造成的伤害差 攻-防
 //       //if (diff >= 200) {
 //       //    hurt = diff * attacker.SkillRatio() * (1 + attacker.skillPercent / 100.0) + aPro.skillAttack - dPro.skillDefence;
 //       //} else {
 //       //    hurt = MAX(aPro.skillAttack - dPro.skillDefence, 0)
 //       //        *(1 - 1 / (::pow(diff, 1.5) / dPro.defence + 2));
 //       //}
	//	double minhurt1;
	//	double minhurt2;
	//	if(!attacker.IsReinforce())	//不是增援部队
	//	{
	//		minhurt1 = attacker.SkillRatio() * aPro.attack * 0.05 + aPro.skillAttack * 0.1;
	//		minhurt2 = diff * attacker.SkillRatio() * (1 + attacker.GetTotalSkillPercent(tpy,defender.GetType()) / 100.0 - defender.GetTotalDeSkillPercent() / 100.0) + aPro.skillAttack - dPro.skillDefence;
	//		hurt = 3 * MAX(ceil(minhurt1), ceil(minhurt2));	//三个方阵
	//	}
	//	else	//增援部队，则伤害另算
	//	{
	//		Reinforce* rein = dynamic_cast<Reinforce*>(&attacker);
	//		if(rein)
	//			hurt = 3 * MAX((double)(rein->GetDamage()) * (1+attacker.GetTotalSkillPercent(tpy, tmpj->GetType()) / 100.0),1);
	//		else
	//			hurt = 1;
	//	}
	//	

 //       //LLOG("SkillHurt -- hurt1(%d)", (int)hurt);

 //       std::vector<Combat::SoldierRaw>::iterator itr = defender.soldiers.begin();	//iter为每排兵力
 //       for (; itr != defender.soldiers.end(); ++itr)
 //       {
	//		double damagereal;
	//		if(defender.IsReinforce())	//受伤方是攻城部队，则伤害减免90%
	//			damagereal = hurt * 0.1;
	//		else
	//			damagereal = hurt;

 //           int& hp = itr->hp;
 //           int trueHurt = (hp >= (int)damagereal) ? (int)damagereal : hp;		//未溢出的伤害
 //           hp -= (int)damagereal;

 //           //LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

 //  //         attacker.killCntOneLoop += trueHurt;
	//		//defender.deadCntOneLoop += trueHurt;
	//		//attacker.statisticKill += trueHurt;
	//		
	//		//填充伤害消息
	//		//pb::Combat_Damage* damageOne = msg.add_damages();
	//		//damageOne->set_period(1);
	//		//damageOne->set_group(defenderid);
	//		//damageOne->set_range(range++);
	//		//damageOne->set_type(1);
	//		//damageOne->set_damage((int)trueHurt);
	//		//damageOne->set_restblood(hp>0? hp:0);

	//		//填充奖励消息
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

	//if(attacker.IsReinforce())	//是增援部队，直接挂掉
	//{
	//	attacker.soldiers.clear();
	//}
}

bool Calc::_BuffHurt(CombatGroup* pGroup, GroupLst& attackers, GroupLst& defenders, pb::Combat_Buff_AllInfo& msg, CombatType tpy)
{
	////LLOG("Calc::_BuffHurt");
	bool ret = false;
	////攻方
	//int endcount = 0;
	//int range = 0;
	//GroupLst::iterator it = attackers.begin();
	//for (; it != attackers.end(); ++it)
	//{
	//	if(endcount++  > 10)	//最多10个武将，避免国战太多武将
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(tmpj->m_fightBuff.HaveBuff(percentdamage))	//有百分比伤害的buff		//不会立即死亡
	//	{
	//		const BuffInfo* buffone = tmpj->m_fightBuff.GetBuffInfo(percentdamage);
	//		if(buffone)
	//		{
	//			float damagerate = buffone->value1;
	//			if(damagerate > 0.0)
	//			{
	//				int rownum = 0;
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iter为每排兵力
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = hp * damagerate/100;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//未溢出的伤害
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:记录击杀
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//填充伤害消息
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//填充奖励消息
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
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iter为每排兵力
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = damagefixed;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//未溢出的伤害
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:记录击杀
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//填充伤害消息
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//填充奖励消息
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

	////守方
	//endcount = 0;
	//range = 0;
	//it = defenders.begin();
	//for (; it != defenders.end(); ++it)
	//{
	//	if(endcount++  > 10)	//最多10个武将，避免国战太多武将
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(tmpj->m_fightBuff.HaveBuffWork(percentdamage))	//有百分比伤害的buff		//不会立即死亡
	//	{
	//		const BuffInfo* buffone = tmpj->m_fightBuff.GetBuffInfo(percentdamage);
	//		if(buffone)
	//		{
	//			float damagerate = buffone->value1;
	//			if(damagerate > 0.0)
	//			{
	//				int rownum = 0;
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iter为每排兵力
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = hp * damagerate/100;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//未溢出的伤害
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:记录击杀
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//填充伤害消息
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//填充奖励消息
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
	//				std::vector<Combat::SoldierRaw>::iterator itr = tmpj->soldiers.begin();	//iter为每排兵力
	//				for (; itr != tmpj->soldiers.end(); ++itr)
	//				{
	//					//if(rownum <= (int)(buffone->buffRange))
	//					//{
	//						int& hp = itr->hp;
	//						float hurt = damagefixed;
	//						int trueHurt = (hp >= (int)hurt) ? (int)hurt : hp;		//未溢出的伤害
	//						hp -= (int)hurt;

	//						//LLOG("SkillHurt -- trueHurt(%d)", trueHurt);

	//						//TODO:记录击杀
	//						//attacker.killCntOneLoop += trueHurt;
	//						//defender.deadCntOneLoop += trueHurt;

	//						//填充伤害消息
	//						pb::Combat_Damage* damageOne = msg.add_damages();
	//						damageOne->set_period(4);
	//						damageOne->set_group(2);
	//						damageOne->set_range(range++);
	//						damageOne->set_type(4);
	//						damageOne->set_damage((int)trueHurt);
	//						damageOne->set_restblood(hp>0? hp:0);

	//						//填充奖励消息
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

	////头排死亡，
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
	////攻方
	//GroupLst::iterator it = attackers.begin();
	//for (; it != attackers.end(); ++it)
	//{
	//	//LLOG("attacker one ");
	//	if(endcount++  > 10)	//最多10个武将，避免国战太多武将
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//有buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}

	////守方
	//endcount = 0;
	//rowcount = 0;
	//it = defenders.begin();
	//for (; it != defenders.end(); ++it)
	//{
	//	//LLOG("defender one ");
	//	if(endcount++  > 10)	//最多10个武将，避免国战太多武将
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//有buff
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

	////攻方
	//GroupLst::iterator it = attackers.begin();
	//for (; it != attackers.end(); ++it)
	//{
	//	//LLOG("attacker one ");
	//	if(endcount++  > 10)	//最多10个武将，避免国战太多武将
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//有buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}

	////守方
	//endcount = 0;
	//rowcount = 0;
	//it = defenders.begin();
	//for (; it != defenders.end(); ++it)
	//{
	//	//LLOG("defender one ");
	//	if(endcount++  > 10)	//最多10个武将，避免国战太多武将
	//		break;
	//	CombatObj* tmpj = (*it);
	//	if(!tmpj->m_fightBuff.IsBuffEmpty())		//有buff
	//	{
	//		//LLOG("have buff");
	//		tmpj->m_fightBuff.FillBuffInfoMsg(msg,rowcount);
	//	}
	//	rowcount += tmpj->soldiers.size();
	//}
}
//attacker对defender造成伤害，defender获得奖励	attackHurt攻方造成的伤害，defenceHurt守方造成的伤害
void Calc::CalcAwardsOneStep(CombatGroup* pGroup, CombatObj& attacker, CombatObj* defender, pb::Combat_Reward* awards, CombatType typ, int step, int attackHurt, int defenceHurt)
{
//	awards->set_period(step);
//	
//	::Character* hero = dynamic_cast<::Character*>(defender);
//	::Character* attacker_hero =  dynamic_cast<::Character*>(&attacker);
//	//if (hero == NULL ) return;
//
//	//计算跨服伤害加成
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
//	if(step == 1)		//技能阶段
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
//			Property& aPro = attacker.stPro;	//攻方属性
//			Property& dPro = defender->stPro;	//守方属性
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
//			// 远征，不获得经验、奖励
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
//			if(hero->IsPlayer())	//守方是武将，则记录伤亡数，获得经验
//			{
//				pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//				pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatDead,attackHurt,tmp);
//			}
//			if(hero->IsReinforce())		//是增援部队，记录获得经验
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
//		if(attacker_hero && (attacker_hero->IsPlayer() || attacker_hero->IsReinforce()))	//攻方是武将或增援部队，则记录击杀，功勋
//		{
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatKills,attackHurt,tmp);
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatExploit,exploittmp,tmp);
//		}
//	}
//	else if(step == 2 || step == 3 || step == 4)	//对撞阶段 平砍阶段 buff阶段
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
//			//计算击杀比
//			float killRatio = 2;
//			if (attackHurt > 0)
//			{
//				killRatio = (float)defenceHurt / (float)attackHurt;	//击杀/死亡
//			}
//			if (CT_Official == typ) //官战
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
//			// 远征，不获得经验、奖励
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
//				exploittmpDef += hero->m_player->HurtToExploit(ENUM_TYPE_KILL,defenceHurt);//杀敌产生功勋
//				exploittmpDef += hero->m_player->HurtToExploit(ENUM_TYPE_DEATH,attackHurt);//死兵产生功勋
//			}
//			if(attacker_hero && attacker_hero->m_player && !attacker_hero->IsShadow())
//			{
//				exploittmpAtt += attacker_hero->m_player->HurtToExploit(ENUM_TYPE_KILL,attackHurt);//杀敌产生功勋
//				exploittmpAtt += attacker_hero->m_player->HurtToExploit(ENUM_TYPE_DEATH,defenceHurt);//死兵产生功勋
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
//		if(hero && hero->IsPlayer())	//守方是武将，则记录伤亡数，获得经验,功勋
//		{
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatDead,attackHurt,tmp);
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatExploit,exploittmpDef,tmp);
//		}
//		if(hero && hero->IsReinforce())		//是增援部队，记录获得经验
//		{
//			pGroup->RecordCombatGainsInfo(hero->m_player->GetGuid(),pb::CombatPlayerExp,expPlayer,tmp);
//		}
//
//		if(attacker_hero && (attacker_hero->IsPlayer() || attacker_hero->IsReinforce()))	//攻方是武将或增援部队，则记录击杀，功勋
//		{
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatKills,attackHurt,tmp);
//			pGroup->RecordCombatGainsInfo(attacker_hero->m_player->GetGuid(),pb::CombatExploit,exploittmpAtt,tmp);
//		}
//
//	}
}

//攻方，守方，攻方造成的伤害，守方造成的伤害
void Calc::_DoHurt(CombatGroup* pGroup, CombatObj& attacker, CombatObj& defender, int attackerHurt, int defenderHurt, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy, int step)
{
	//double damagereal;
	//if(attacker.IsReinforce())	//受伤方是攻城部队，则伤害减免90%
	//	damagereal = defenderHurt * 0.1;
	//else
	//	damagereal = defenderHurt;
 //   int trueHurtDefender = attacker.HurtHp((int)damagereal);		//守方造成的真实伤害
 //   defender.killCntOneLoop += trueHurtDefender;
 //   attacker.deadCntOneLoop += trueHurtDefender;
 //   //LLOG("DoHurt -- attacker:trueHurt(%d)", trueHurt);

	//if(defender.IsReinforce())	//受伤方是攻城部队，则伤害减免90%
	//	damagereal = attackerHurt * 0.1;
	//else
	//	damagereal = attackerHurt;
 //   int trueHurtAttacker = defender.HurtHp((int)damagereal);	//攻方造成的真实伤害
 //   attacker.killCntOneLoop += trueHurtAttacker;
 //   defender.deadCntOneLoop += trueHurtAttacker;
	////LLOG("DoHurt -- defender:trueHurt(%d)", trueHurt);

	////填充伤害
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

	////填充奖励消息
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
    //第一次是对撞
    int attackerHurt = _CalcCrashHurt(*attacker, *defender);	//攻方造成的伤害
    int defenderHurt = _CalcCrashHurt(*defender, *attacker);	//守方造成的伤害

	////填充伤害
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

    //然后对砍
    while (!CheckDead(pGroup,attacker,defender,msg,false) && !CheckDead(pGroup,defender,attacker,msg,true))
    {
        attackerHurt = _CalcCommonHurt(*attacker, *defender, ratio1);
        defenderHurt = _CalcCommonHurt(*defender, *attacker, ratio2);

		////填充伤害
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


/* 新的战斗计算系统 */
bool Calc::CheckDeadNormal(CombatObj* checkobj)
{
	if(checkobj->soldiers.size() == 0)	//obj已经死亡
	{
		return true;
	}

	if(checkobj->ForeHp() <= 0)	//第一行血量为0
	{
		return true;
	}

	return false;
}

void Calc::_CalcAwardsInOneStepBySide(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, CombatObj* pAttack, CombatObj* pDefence, int attOrgHurtToDef, int defOrgHurtToAtt, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt, int realdProDef, bool isAttGetAward)
{
	//如果守方是武将或者假武将，幻影，增援部队	//可以获得奖励	（其中幻影与增援部队只能获得玩家exp奖励与银币奖励）
	if(pDefence->IsPlayer() || pDefence->IsShadow() || pDefence->IsReinforce())
	{
		/********** 计算开始 ********/
		//计算耗粮
		float needFood = 0.0;
		const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(pDefence->soldierLv);
		if (pTable == NULL) 
		{
			LLOG("[2000 ERROR] _GetAwardsWithOneStep pTable is not found,lv=%d",pDefence->soldierLv);
			return;
		}
		needFood = pTable->NeedFood() * attOrgHurtToDef;

		//计算转换比率
		float foodToExp = FOOD_TO_EXP[pGroup->m_eCombat];

		//计算击杀比
		float killRatio = 2;
		if(serType == pb::CoSe_SkillHurt_Att || serType == pb::CoSe_SkillHurt_Def)	//如果是释放技能阶段
		{
			Property& aPro = pAttack->stPro;	//攻方属性
			Property& dPro = pDefence->stPro;	//守方属性
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
			if(attRealHurtToDef != 0)	//杀敌数不为0
			{
				killRatio = (float)defRealHurtToAtt / (float)attRealHurtToDef;	//击杀比=杀敌数÷损失兵力
				if (CT_Official == pGroup->m_eCombat) //官战
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

		//计算各种加成
		float totalAddtions = 0.0;
		//科技与练兵令,跨服等提供的经验加成
		totalAddtions += (((float)pDefence->GetTotalExpPercent(pGroup->m_eCombat))/((float)100));
		if(pGroup->m_eCombat == CT_Country)	//只有国战才有的加成
		{
			//等级差提供的经验加成
			uint32 levelAtt = pAttack->GetPlayerLevel();
			uint32 levelDef = pDefence->GetPlayerLevel();
			if((levelAtt+16) <= levelDef)	//比对方等级小16级
			{
				uint32 levelDif = levelDef - levelAtt - 15;
				totalAddtions += MIN(((float)levelDif * (float)0.1),(float)1.0);
			}

			//TODO::禁卫区加成		寻路优化先做

			//TODO::武将天赋加成	具体武将具体做

		}

		//跨服倍数
		float crossTimes = 1.0;
#ifdef _SERVER_CROSS_
		crossTimes = 1.5;
#endif

		//计算最终奖励
		int exp = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
		int expPlayer = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
		int money = MAX((int)ceil(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes * 0.2),1);

		if(pGroup->m_eCombat == CT_Crusade || attOrgHurtToDef == 0)	//远征不获得经验与奖励或者攻方对守方造成的伤害为0
		{
			exp = 0;
			expPlayer = 0;
			money = 0;
		}
		/********** 计算结束 ********/


		/********* 发放奖励 ********/
		//之前版本都是在武将死亡或者战斗结束结算奖励与经验，现在改为立即结算是否更方便？ Try it
		uint32 exploitOfDef = 0;
		//真假武将，幻影，增援部队，增加玩家的经验与银币
		if(Player* master = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))
		{
			sReward.Change(*master, Reward::PlayerExp, expPlayer);	//玩家经验
			sReward.Change(*master, Reward::Coin_Silver, money);	//银币
			master->AddActivityStarBoxExp(expPlayer);				//玩家活动经验
			exploitOfDef = master->HurtToExploit(ENUM_TYPE_DEATH,attOrgHurtToDef); //死兵产生功勋
		}

		//真假武将增加武将经验
		if(pDefence->IsPlayer())
		{
			::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
			heroDefence->TryAddXP(exp);
		}
		//武将的幻影增加原武将经验
		if(pDefence->IsShadow())
		{
			::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
			Character* charone = heroDefence->m_player->m_characterStorage->MutableCharacter(heroDefence->GetID());
			if(charone)
				charone->TryAddXP(exp);
		}
		/********* 发放结束 **********/


		/******** 记录各种战斗数据 ******/
		//记录战报
		IntPairVec tmp;
		//真假武将，幻影，增援部队均会增加玩家经验，兵力损失
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatPlayerExp,expPlayer,tmp);
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatDead,attOrgHurtToDef,tmp);
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatExploit,exploitOfDef,tmp);
		pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatCoin,money,tmp);
		/********** 记录结束 **********/


		/********* 增加进度 *********/
		if(::Character* heroDefence = dynamic_cast<::Character*>(pDefence))
		{
			if(heroDefence->GetIsInAutoCountryWar())	//武将在自动国战中
			{
				heroDefence->m_player->m_AutoCountryWarLog->AddExpGet(expPlayer);	//记录自动国战获得经验
			}
		}
		/********* 增加结束 *********/

		/********** 填充消息 *********/
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

	//结算对守方造成伤害攻方所获得的奖励
	//如果攻方是武将或者假武将，幻影，增援部队	//可以获得功勋奖励
	if(pAttack->IsPlayer() || pAttack->IsShadow() || pAttack->IsReinforce())
	{
		uint32 exploitOfAtt = 0;	//攻方获得的功勋
		if(Player* master = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))
		{
			//是国战，则增加功勋
			if(pGroup->m_eCombat == CT_Country)
			{
				exploitOfAtt = master->HurtToExploit(ENUM_TYPE_KILL,attRealHurtToDef);
			}
		}

		/******** 记录各种战斗数据 ******/
		//记录战报
		IntPairVec tmp;
		pGroup->RecordCombatGainsInfo(pAttack->GetPlayerId(),pb::CombatKills,attRealHurtToDef,tmp);
		pGroup->RecordCombatGainsInfo(pAttack->GetPlayerId(),pb::CombatExploit,exploitOfAtt,tmp);
		/********** 记录结束 **********/


		/********* 增加进度 *********/
		if(::Character* heroAttack = dynamic_cast<::Character*>(pAttack))
		{
			if(heroAttack->GetIsInAutoCountryWar())	//武将在自动国战中
			{
				heroAttack->m_player->m_AutoCountryWarLog->AddKillCount(attRealHurtToDef);	//记录自动国战击杀数
				heroAttack->killOnceAHero += attRealHurtToDef;
			}
			CombatGroup::_SaveStaticKillOnce(*heroAttack,pGroup->m_eCombat,attRealHurtToDef);	//记录其他击杀进度 任务进度
		}
		/********* 增加结束 *********/
	}
}

//一次伤害的奖励计算 战斗组，计算阶段，攻对守的伤害，守对攻的伤害 （对撞，技能直接伤害，互砍阶段用） (一个来回，如对撞，护砍至死亡一次，或者一方对另一方释放技能)
void Calc::_GetAwardsWithOneStep(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType, int attRealHurtToDef, int defRealHurtToAtt, int realaProAtt/* = 1*/, int realdProDef/* = 1*/)
{
	//经验=耗粮×转化比例×击杀比修正×（1+各种加成）×跨服倍数
	//银币=经验*0.2

	if(pGroup->m_group_attack.empty() || pGroup->m_group_defence.empty())
	{
		LLOG("[2000 ERROR] _GetAwardsWithOneStep pGourp list is empty");
		return;
	}

	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	//首先计算伤害的加成	//杀敌数翻倍，最大两倍
	int attOrgHurtToDef = attRealHurtToDef;
	int defOrgHurtToAtt = defRealHurtToAtt;
	attRealHurtToDef *= (1 + pAttack->GetTotalDoubleKill(pGroup->m_eCombat)/100);
	defRealHurtToAtt *= (1 + pDefence->GetTotalDoubleKill(pGroup->m_eCombat)/100);

	//结算奖励
	//先计算攻方攻击守方，守方获得的奖励
	_CalcAwardsInOneStepBySide(msg, pGroup,serType,pAttack,pDefence,attOrgHurtToDef,defOrgHurtToAtt,attRealHurtToDef,defRealHurtToAtt,realaProAtt,realdProDef,false);
	
	//后计算守方攻击攻方，攻方获得的奖励
	_CalcAwardsInOneStepBySide(msg, pGroup,serType,pDefence,pAttack,defOrgHurtToAtt,attOrgHurtToDef,defRealHurtToAtt,attRealHurtToDef,realdProDef,realaProAtt,true);

}

void Calc::_MakeHurtsAndAwardsCrash(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double	attHp = pAttack->ForeHp();	//攻方前排血量
	double	defHp = pDefence->ForeHp();	//守方前排血量

	if(attHp <= 0 || defHp <= 0)	return; //如果一方已经死亡，则不进行冲撞

	//增援部队没有冲撞阶段，则只判断溢出
	//if(attObj.IsReinforce())	//攻方是增援部队，则守方对其造成的伤害减免90%
	//	defHurtToAtt *= 0.1;

	//if(defObj.IsReinforce())	//守方是增援部队，则攻方对其造成的伤害减免90%
	//	attHurtToDef *= 0.1;


	//溢出伤害减免
	int attRealHurt = MIN((int)attHurtToDef,(int)defHp);		//攻方对守方造成的真实伤害
	int defRealHurt = MIN((int)defHurtToAtt,(int)attHp);		//守方对攻方造成的真实伤害


	//造成伤害
	pAttack->HurtHp(defRealHurt);
	pDefence->HurtHp(attRealHurt);

	pb::Combat_Damage* damageAtt = msg.add_damagelst();	//攻方收到的伤害与获得的奖励
	damageAtt->set_group(1);
	damageAtt->set_range(0);
	damageAtt->set_damage(defRealHurt);
	damageAtt->set_restblood((int)pAttack->ForeHp());
	damageAtt->set_buffid(0);

	pb::Combat_Damage* damageDef = msg.add_damagelst();	//守方收到的伤害与获得的奖励
	damageDef->set_group(2);
	damageDef->set_range(0);
	damageDef->set_damage(attRealHurt);
	damageDef->set_restblood((int)pDefence->ForeHp());
	damageDef->set_buffid(0);


	//记录杀手
	if(pAttack->ForeHp() == 0)
	{
		if(pAttack->soldiers.front().killerPlayerId == 0)	//如果没有记录过杀手
		{
			pAttack->soldiers.front().killerPlayerId = pDefence->GetPlayerId();
			if(pDefence->IsPlayer() || pDefence->IsShadow())	//是真假武将或者幻影
			{
				::Character* hero = dynamic_cast<::Character*>(pDefence);
				pAttack->soldiers.front().killerCharId = hero->GetID();
			}

			//发放额外奖励
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pAttack)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))	//如果有玩家
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//记录额外奖励
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
		if(pAttack->IsDead())	//如果武将死亡了
		{
			if(pAttack->killerPID == 0)
			{
				pAttack->killerPID = pDefence->GetPlayerId();
				if(pDefence->IsPlayer() || pDefence->IsShadow())	//是真假武将或者幻影
				{
					::Character* hero = dynamic_cast<::Character*>(pDefence);
					pAttack->killerCID = hero->GetID();
				}
			}
		}
	}
	if(pDefence->ForeHp() == 0)
	{
		if(pDefence->soldiers.front().killerPlayerId == 0)	//如果没有记录过杀手
		{
			pDefence->soldiers.front().killerPlayerId = pAttack->GetPlayerId();
			if(pAttack->IsPlayer() || pAttack->IsShadow())	//是真假武将或者幻影
			{
				::Character* hero = dynamic_cast<::Character*>(pAttack);
				pDefence->soldiers.front().killerCharId = hero->GetID();
			}

			//发放额外奖励
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pDefence)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))	//如果有玩家
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//记录额外奖励
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
		if(pDefence->IsDead())	//如果武将死亡了
		{
			if(pDefence->killerPID == 0)
			{
				pDefence->killerPID = pAttack->GetPlayerId();
				if(pAttack->IsPlayer() || pAttack->IsShadow())	//是真假武将或者幻影
				{
					::Character* hero = dynamic_cast<::Character*>(pAttack);
					pDefence->killerCID = hero->GetID();
				}
			}
		}
	}

	//发放奖励
	_GetAwardsWithOneStep(msg, pGroup,pb::CoSe_TacticCrash,attRealHurt,defRealHurt);


	//校准武将血量
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
	double hurt = attacker->ForeHp() * CrashRatio(attacker->tactic, defender->tactic);	//当前己方上阵前排血量*冲撞系数

	//LLOG("CrashHurt -- hurt1(%d)", (int)hurt);

	if (HaveCrashAddition(attacker->tactic, defender->tactic))	//如果克制是完美克制或者不完美克制
	{
		hurt += attacker->GetTerrDebuffAddtion(terrType);
		//LLOG("CrashHurt -- hurt2(%d), crashAddition(%d)", (int)hurt, attacker.crashAddition);
	}
	hurt = MAX(hurt,3);	//最小值为3，即每个方阵最小值为1

	if(hurt > MAX_HURT)
		hurt = MAX_HURT;

	return hurt;
}

void Calc::DoHurt_Crash(pb::Combat_Seque& msg, CombatGroup* pGroup)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double attHurtToDef = _NewCrashHurt(pAttack,pDefence,pGroup->m_terrainType);	//攻方对守方造成的冲撞伤害	满溢伤害	最小值为3
	double defHurtToAtt = _NewCrashHurt(pDefence,pAttack,pGroup->m_terrainType); //守方对攻方造成的冲撞伤害	满溢伤害	最小值为3

	//造成伤害与奖励 冲撞
	_MakeHurtsAndAwardsCrash(msg,pGroup,attHurtToDef,defHurtToAtt);
}

void Calc::_MakeHurtAndAwardsSkill(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj& attacker, GroupLst& lst, pb::E_Combat_Serialize serType)
{
	//单方阵伤害 = max(max（（攻击-防御）,0)*2*技能伤害系数*（1+攻方战法加成-守方战法防御）+破阵-无懈,攻击*5%*技能伤害系数+破阵*0.1)
	Property& aPro = attacker.stPro;			//攻方属性
	uint8 skillRange = attacker.SkillRange();	//释放技能影响排数
	uint8 skillHurtLineCnt = 0;					//影响排数计数

	int realaProAtt = attacker.soldiers.front().buff_fight.GetFinalAttackValue(aPro.attack);	//技能施放方的攻击属性

	int awardSide;
	if(serType == pb::CoSe_SkillHurt_Att)
		awardSide = 2;
	else
		awardSide = 1;


	GroupLst::iterator it = lst.begin();	//it为一个武将单位
	for (; it != lst.end(); ++it)
	{
		CombatObj* pDefender = (*it);
		Property& dPro = pDefender->stPro;	//防守属性
		
		/******* 因为技能释放对象为多排，所以每一排单独计算真实伤害 ******/
		std::vector<Combat::SoldierRaw>::iterator itr = pDefender->soldiers.begin();	//iter为每排兵力
		for (; itr != pDefender->soldiers.end(); ++itr)
		{
			Combat::SoldierRaw& rowOne = *itr;
			int realdProDef = rowOne.buff_fight.GetFinalDefenceValue(dPro.defence);

			/********* 计算攻方对守方造成的满溢伤害 ********/
			int commonHurt = 0;	//通过攻守方属性计算出的满溢伤害
			if(!attacker.IsReinforce())	//如果不是增援部队，则正常计算
			{
				/********计算攻方战法加成********/
				double attAddtion = attacker.GetTotalSkillPercent(pGroup->m_eCombat,pDefender->GetType()) / 100.0;
				/********计算完成********/
				
				/********计算守方战法防御加成********/
				double defAddtion = pDefender->GetTotalDeSkillPercent() / 100.0;	//科技，练兵令，跨服带来的收益
				defAddtion += rowOne.buff_fight.GetFinalDeskillHurtPercent();		//由减少技能伤害的buff带来的收益
				/********计算完成********/
				double finalAddtion = MAX((1+attAddtion-defAddtion),0);

				double diff = MAX(realaProAtt - realdProDef, 0);	//可造成的伤害差  max（（攻击-防御）,0)
				double hurt1 = realaProAtt * 0.05 * attacker.SkillRatio() + aPro.skillAttack * 0.1;
				double hurt2 = diff * 2 * attacker.SkillRatio() * finalAddtion + aPro.skillAttack - dPro.skillDefence;
				double hurt3 = 3 * MAX(ceil(hurt1), ceil(hurt2));	//3个方阵最终伤害
				commonHurt = MAX((int)hurt3,3);	//最小值为3
			}
			else	//增援部队，单独计算伤害
			{
				Reinforce* rein = dynamic_cast<Reinforce*>(&attacker);
				if(rein)
					commonHurt = 3 * MAX(rein->GetDamage(),1);	//最小值为3
				else
					commonHurt = 3;
			}
			/**************** 计算完毕 **************/

		
			if(itr->hp > 0) 	//如果这一排已经死亡，不计算伤害
			{
				//真实伤害
				int realHurt = commonHurt;

				//攻城部队减免90%
				if(pDefender->IsReinforce()) realHurt = MAX((int)(realHurt*0.1),3);

				//去掉溢出伤害并真实扣血
				int& rowhp = itr->hp;
				realHurt = (rowhp >= realHurt) ? realHurt : rowhp;		//未溢出的伤害
				rowhp -= realHurt;

				pb::Combat_Damage* damageone = msg.add_damagelst();	//攻方收到的伤害与获得的奖励
				damageone->set_group(awardSide);
				damageone->set_range(skillHurtLineCnt);
				damageone->set_damage(realHurt);
				damageone->set_restblood(rowhp);
				damageone->set_buffid(0);

				//记录杀手
				if(rowhp <= 0)
				{
					if(itr->killerPlayerId == 0)	//如果没有记录过杀手
					{
						itr->killerPlayerId = attacker.GetPlayerId();
						if(attacker.IsPlayer() || attacker.IsShadow())	//是真假武将或者幻影
						{
							::Character* hero = dynamic_cast<::Character*>(&attacker);
							itr->killerCharId = hero->GetID();
						}

						//发放额外奖励
						if(::Npc* deadnpc = dynamic_cast<::Npc*>((pDefender)))
						{
							if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
							{
								if(Player* player = sPlayerPool.GetByPlayerId(attacker.GetPlayerId()))	//如果有玩家
								{
									sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

									//记录额外奖励
									IntPairVec pairvec;
									pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
									pGroup->RecordCombatGainsInfo(attacker.GetPlayerId(),pb::CombatInfoNum,0,pairvec);

									pb::extra_awards* exAward;
									if(awardSide == 1)	//如果是攻方释放的技能
										exAward = msg.add_att_extra_awards();
									else
										exAward = msg.add_def_extra_awards();
									exAward->set_awardsid(deadnpc->GetExtraAwardID());
									exAward->set_awardsnum(deadnpc->GetExtraAwardNum());
								}
							}
						}

					}
					if(pDefender->IsDead())	//如果武将死亡了
					{
						if(pDefender->killerPID == 0)
						{
							pDefender->killerPID = attacker.GetPlayerId();
							if(attacker.IsPlayer() || attacker.IsShadow())	//是真假武将或者幻影
							{
								::Character* hero = dynamic_cast<::Character*>(&attacker);
								pDefender->killerCID = hero->GetID();
							}
						}
					}
				}

				//发放奖励
				if(serType == pb::CoSe_SkillHurt_Att)
					_GetAwardsWithOneStep(msg,pGroup,serType,realHurt,0,realaProAtt,realdProDef);
				else
					_GetAwardsWithOneStep(msg,pGroup,serType,0,realHurt,realdProDef,realaProAtt);
			}

			skillHurtLineCnt++;	//计算完毕一排

			if(skillHurtLineCnt >= skillRange)		//计算的排数已经满足
				break;
		}
		/******* 单独计算完毕 ******/

		/******* 校准武将血量 ******/
		if(pDefender->IsPlayer())
		{
			::Character* hero = dynamic_cast<::Character*>(pDefender);
			hero->SetCurSolderNum(pDefender->CurHp());
			hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
		}
		/******* 校准完毕 ******/

		if(skillHurtLineCnt >= skillRange)		//外层也要结束计算
			break;
	}
}

void Calc::DoHurt_Skill(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();	//进攻方
	CombatObj* pDefence = pGroup->m_group_defence.front();	//防守方

	if(serType == pb::CoSe_SkillHurt_Att)	//是攻方使用技能
	{
		pAttack->isUsedSkill = true;	//使用了技能
		pAttack->leftSkillTimes -= 1;	//使用次数减一
		LLOG("att skill time = %d pAttack=%llu",pAttack->leftSkillTimes,pAttack);
		pAttack->SetIsUseSkill();	//设置该行士兵使用了技能
		float successRate1 = (float)((pAttack->stPro.yong*STATIC_SKILL_RATE)+(pAttack->stPro.yong*pDefence->stPro.yong)) / ((pAttack->stPro.yong*pDefence->stPro.yong) + (pDefence->stPro.yong*pDefence->stPro.yong));		//成功使用技能的概率
		pAttack->isSkillSuccess = (System::Randf(0, 1) < successRate1);		//使用技能是否成功
		if(pAttack->IsReinforce()) pAttack->isSkillSuccess = true;			//增援部队技能必定释放成功
		if(pAttack->isSkillSuccess)	//如果释放成功，则计算技能伤害与附加buff
		{
			//计算技能伤害与奖励
			_MakeHurtAndAwardsSkill(msg, pGroup,*pAttack,pGroup->m_group_defence,serType);
			
			//挂buff
			_AttachBuff(pGroup,serType);
		}
	}
	
	if(serType == pb::CoSe_SkillHurt_Def)	//是防守方使用技能
	{
		pDefence->isUsedSkill = true;	//使用了技能
		pDefence->leftSkillTimes -= 1;	//使用次数减一
		LLOG("def skill time = %d pDefence=%llu",pDefence->leftSkillTimes,pDefence);
		pDefence->SetIsUseSkill();	//设置该行士兵使用了技能
		float successRate1 = (float)((pDefence->stPro.yong*STATIC_SKILL_RATE)+(pDefence->stPro.yong*pAttack->stPro.yong)) / ((pDefence->stPro.yong*pAttack->stPro.yong) + (pAttack->stPro.yong*pAttack->stPro.yong));		//成功使用技能的概率
		pDefence->isSkillSuccess = (System::Randf(0, 1) < successRate1);		//使用技能是否成功
		if(pDefence->IsReinforce()) pDefence->isSkillSuccess = true;			//增援部队技能必定释放成功
		if(pDefence->isSkillSuccess)	//如果释放成功，则计算技能伤害与附加buff
		{
			//计算技能伤害与奖励
			_MakeHurtAndAwardsSkill(msg, pGroup,*pDefence,pGroup->m_group_attack,serType);

			//挂buff
			_AttachBuff(pGroup,serType);
		}
	}

}


void Calc::_MakeHurtsAndAwardsCut(pb::Combat_Seque& msg, CombatGroup* pGroup, double attHurtToDef, double defHurtToAtt)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double	attHp = pAttack->ForeHp();	//攻方前排血量
	double	defHp = pDefence->ForeHp();	//守方前排血量

	if(attHp <= 0 || defHp <= 0)	return; //如果一方已经死亡，则不进行护砍

	//增援部队没有互砍阶段，则只判断溢出
	//if(attObj.IsReinforce())	//攻方是增援部队，则守方对其造成的伤害减免90%
	//	defHurtToAtt *= 0.1;

	//if(defObj.IsReinforce())	//守方是增援部队，则攻方对其造成的伤害减免90%
	//	attHurtToDef *= 0.1;


	//溢出伤害减免
	int attRealHurt = MIN((int)attHurtToDef,(int)defHp);		//攻方对守方造成的真实伤害
	int defRealHurt = MIN((int)defHurtToAtt,(int)attHp);		//守方对攻方造成的真实伤害

	//检测是否有闪避效果
	if(pAttack->HaveBuffWork(dodge))
	{
		defRealHurt = pAttack->soldiers.front().buff_fight.GetFinalDamageAfterDodge(defRealHurt);
	}

	if(pAttack->HaveBuffWork(dodge))
	{
		attRealHurt = pDefence->soldiers.front().buff_fight.GetFinalDamageAfterDodge(attRealHurt);
	}
	
	//造成伤害
	pAttack->HurtHp(defRealHurt);
	pDefence->HurtHp(attRealHurt);

	pb::Combat_Damage* damageAtt = msg.add_damagelst();	//攻方收到的伤害与获得的奖励
	damageAtt->set_group(1);
	damageAtt->set_range(0);
	damageAtt->set_damage(defRealHurt);
	damageAtt->set_restblood((int)pAttack->ForeHp());
	damageAtt->set_buffid(0);

	pb::Combat_Damage* damageDef = msg.add_damagelst();	//守方收到的伤害与获得的奖励
	damageDef->set_group(2);
	damageDef->set_range(0);
	damageDef->set_damage(attRealHurt);
	damageDef->set_restblood((int)pDefence->ForeHp());
	damageDef->set_buffid(0);


	//记录杀手
	if(pAttack->ForeHp() == 0)
	{
		if(pAttack->soldiers.front().killerPlayerId == 0)	//如果没有记录过杀手
		{
			pAttack->soldiers.front().killerPlayerId = pDefence->GetPlayerId();
			if(pDefence->IsPlayer() || pDefence->IsShadow())	//是真假武将或者幻影
			{
				::Character* hero = dynamic_cast<::Character*>(pDefence);
				pAttack->soldiers.front().killerCharId = hero->GetID();
			}

			//发放额外奖励
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pAttack)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))	//如果有玩家
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//记录额外奖励
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
		if(pAttack->IsDead())	//如果武将死亡了
		{
			if(pAttack->killerPID == 0)
			{
				pAttack->killerPID = pDefence->GetPlayerId();
				if(pDefence->IsPlayer() || pDefence->IsShadow())	//是真假武将或者幻影
				{
					::Character* hero = dynamic_cast<::Character*>(pDefence);
					pAttack->killerCID = hero->GetID();
				}
			}
		}
	}
	if(pDefence->ForeHp() == 0)
	{
		if(pDefence->soldiers.front().killerPlayerId == 0)	//如果没有记录过杀手
		{
			pDefence->soldiers.front().killerPlayerId = pAttack->GetPlayerId();
			if(pAttack->IsPlayer() || pAttack->IsShadow())	//是真假武将或者幻影
			{
				::Character* hero = dynamic_cast<::Character*>(pAttack);
				pDefence->soldiers.front().killerCharId = hero->GetID();
			}

			//发放额外奖励
			if(::Npc* deadnpc = dynamic_cast<::Npc*>((pDefence)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Player* player = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))	//如果有玩家
					{
						sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

						//记录额外奖励
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
		if(pDefence->IsDead())	//如果武将死亡了
		{
			if(pDefence->killerPID == 0)
			{
				pDefence->killerPID = pAttack->GetPlayerId();
				if(pAttack->IsPlayer() || pAttack->IsShadow())	//是真假武将或者幻影
				{
					::Character* hero = dynamic_cast<::Character*>(pAttack);
					pDefence->killerCID = hero->GetID();
				}
			}
		}
	}

	//发放奖励
	_GetAwardsWithOneStep(msg,pGroup,pb::CoSe_CutToDie,attRealHurt,defRealHurt);


	//校准武将血量
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
	//伤害 = max（max(max(己方攻击-对方防御,0)*(0.55+(当前己血/当前他血)*0.5)*(1+战术克制加成)*(1+地形加成-禁卫区加成+其他加成)+己方穿刺-对方格挡,0)*(己统/他统)，攻击*5%）
	Property& aPro = attacker->stPro;	//攻方pro
	Property& dPro = defender->stPro;	//守方pro

	int realaProAtt = attacker->soldiers.front().buff_fight.GetFinalAttackValue(aPro.attack);
	int realdProDef = defender->soldiers.front().buff_fight.GetFinalDefenceValue(dPro.defence);

	float ratioAttToDef = CommonRatio(attacker->tactic, defender->tactic);	//攻方对守方的克制
	
	/********计算攻方伤害加成********/
	//练兵令与科技带来的加成
	double attAddtion = attacker->GetTotalFightPercent()/100.0;
	
	//TODO::禁卫区带来的减益  -----

	/********计算完成********/

	double hurt1 = MAX(realaProAtt - realdProDef, 0);	//max(己方攻击-对方防御,0)

	double hurt2 = MAX((hurt1
		*(0.55 + (attacker->ForeHp()/defender->ForeHp())*0.5)									//(0.55+(当前己血/当前他血)*0.5)
		*(1 + ratioAttToDef)																//(1+战术克制加成)
		*(1 + attacker->GetTerrTalentAddtion(terrType) + attAddtion )								//(1+地形加成 + 其他加成)
		+ aPro.attackEx - dPro.defenceEx),0);												//己方穿刺-对方格挡

	double hurt3 = hurt2 * ((double)attacker->stPro.tong/defender->stPro.tong);

	double hurt4 = aPro.attack*0.05;

	double hurtFinal = MAX(MAX(hurt3,hurt4),1);

	hurtFinal = ceil(hurtFinal) * 3;		//三个方阵的总伤害

	if(hurtFinal > MAX_HURT)
		hurtFinal = MAX_HURT;

	return hurtFinal;
}

void Calc::DoHurt_Cut(pb::Combat_Seque& msg, CombatGroup* pGroup)
{
	CombatObj* pAttack = pGroup->m_group_attack.front();
	CombatObj* pDefence = pGroup->m_group_defence.front();

	double attHurtToDef = _NewCutHurt(pAttack,pDefence,pGroup->m_terrainType);	//攻方对守方造成的平砍伤害	满溢伤害	最小值为3
	double defHurtToAtt = _NewCutHurt(pDefence,pAttack,pGroup->m_terrainType);	//守方对攻方造成的平砍伤害	满溢伤害	最小值为3

	while(!CheckDeadNormal(pAttack) && !CheckDeadNormal(pDefence))
	{
		//造成伤害与奖励 互砍
		_MakeHurtsAndAwardsCut(msg, pGroup,attHurtToDef,defHurtToAtt);
	}

}

void Calc::_AttachBuff(CombatGroup* pGroup, pb::E_Combat_Serialize serType)
{
	if(serType == pb::CoSe_SkillHurt_Att)	//攻方释放技能
	{
		CombatObj* pAttack = pGroup->m_group_attack.front();
		CombatObj* pDefence = pGroup->m_group_defence.front();
		uint32 skillid = pAttack->SkillID();					//技能id
		int buffid = sSkillDataMgr.GetBuffIDBySkill(skillid);//技能附加的buffid
		if(buffid != 0)	//有buff
		{
			int target = sSkillDataMgr.GetBuffTarget(buffid);	//buff作用目标
			int attachRow = sSkillDataMgr.GetBuffRows(buffid);	//buff作用行数
			if(target == 1)	//敌方
				_BuffToRow(pGroup->m_group_defence,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
			else	//自己
				_BuffToRow(pGroup->m_group_attack,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
		}
	}
	else if(serType == pb::CoSe_SkillHurt_Def)		//守方释放技能
	{
		CombatObj* pAttack = pGroup->m_group_defence.front();
		CombatObj* pDefence = pGroup->m_group_attack.front();
		uint32 skillid = pAttack->SkillID();					//技能id
		int buffid = sSkillDataMgr.GetBuffIDBySkill(skillid);//技能附加的buffid
		if(buffid != 0)	//有buff
		{
			int target = sSkillDataMgr.GetBuffTarget(buffid);	//buff作用目标
			int attachRow = sSkillDataMgr.GetBuffRows(buffid);	//buff作用行数
			if(target == 1)	//敌方
				_BuffToRow(pGroup->m_group_attack,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
			else	//自己
				_BuffToRow(pGroup->m_group_defence,pAttack,buffid,attachRow,pGroup->m_CurrentRound);
		}
	}
}

void Calc::_BuffToRow(GroupLst& buffAttacher, CombatObj* buffSender, int buffid, int buffAttchRow, uint64 roundNum)
{
	int rowCount = 0;	//行数计数器
	uint64 buffSenderPID = 0;	//释放方的玩家id
	uint32 buffSenderCID = 0;	//释放方的武将id
	if(buffAttchRow <= 0 || buffid <= 0) return;	//行数或buffid不符合要求 返回

	if(!buffSender->IsNpc())	//如果不是npc，则有玩家
	{
		buffSenderPID = buffSender->GetPlayerId();
		if(!buffSender->IsReinforce())	//不是攻城部队，则为武将或幻影
		{
			::Character* hero = dynamic_cast<::Character*>(buffSender);
			buffSenderCID = hero->GetID();
		}
	}

	GroupLst::iterator it = buffAttacher.begin();	//it为一个武将单位
	for (; it != buffAttacher.end(); ++it)
	{
		CombatObj* pDefender = (*it);

		/******* 多排buff，每一排单独附加 ******/
		std::vector<Combat::SoldierRaw>::iterator itr = pDefender->soldiers.begin();	//iter为每排兵力
		for (; itr != pDefender->soldiers.end(); ++itr)
		{
			Combat::SoldierRaw& soldierRow = *itr;

			if(soldierRow.hp != 0)	//非死亡才添加buff
				soldierRow.buff_fight.AddBuff(buffid,buffSenderPID,buffSenderCID,roundNum,buffSender->stPro.defence,buffSender->GetLevel());

			rowCount++;	//计算完毕一排

			if(rowCount >= buffAttchRow)		//计算的排数已经满足
				break;
		}

		if(rowCount >= buffAttchRow)		//外层也要结束计算
			break;
	}
}

void Calc::DoHurt_Buff(pb::Combat_Seque& msg, CombatGroup* pGroup, pb::E_Combat_Serialize serType)
{
	//攻方的buff作用
	_BuffWork(msg, pGroup, pGroup->m_group_attack,serType,true);
	
	//守方的buff作用
	_BuffWork(msg, pGroup, pGroup->m_group_defence,serType,false);
}

void Calc::_BuffWork(pb::Combat_Seque& msg, CombatGroup* pGroup, GroupLst& workLst, pb::E_Combat_Serialize serType, bool isAttWork)
{
	int herocount = 0;	//作用武将个数纪录
	int range = 0;
	GroupLst::iterator it = workLst.begin();
	for (; it != workLst.end(); ++it)
	{
		if(herocount++  > 6)	//最多6个武将，避免国战太多武将
			break;

		CombatObj* tmpObj = (*it);
		std::vector<Combat::SoldierRaw>::iterator itr = tmpObj->soldiers.begin();	//iter为每排士兵
		for (; itr != tmpObj->soldiers.end(); ++itr)
		{
			Combat::SoldierRaw& soldierRow = *itr;
			if(soldierRow.hp > 0)	//该排士兵还活着
			{
				_BuffRowWork(msg, pGroup, tmpObj, soldierRow, serType, range, isAttWork);
			}
			range++;
		}
		//校准武将血量
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
	if(soldierRow.hp <= 0) return;	//如果该排士兵死亡则退出

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

		if(buffOneInfo->worktime == worktime)	//如果阶段匹配，则生效
		{
			buffOneInfo->isWork = true;		//首先让挂载的buff生效
			switch(buffOneInfo->buffTyp)
			{
			case Combat::percentdeskillhurt:
				break;
			case Combat::fixedattackup:				//百分比加防御
				break;
			case Combat::fixeddefenceup:			//固定加防
				break;
			case Combat::confuse:					//混乱
				break;
			case Combat::percentdamage:				//百分比掉血
				{
					/******* 造成伤害 *******/
					float commonHurt = soldierRow.hp * buffOneInfo->value1;
					int trueHurt = (soldierRow.hp >= (int)commonHurt) ? (int)commonHurt : soldierRow.hp;		//未溢出的伤害
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


					/******* 记录杀手 *******/
					if(soldierRow.hp <= 0)	//如果该排兵死亡
					{
						if(soldierRow.killerPlayerId == 0)
						{
							soldierRow.killerPlayerId = buffOneInfo->fromP;
							soldierRow.killerCharId = buffOneInfo->fromC;

							//发放额外奖励
							if(::Npc* deadnpc = dynamic_cast<::Npc*>((buffOwner)))
							{
								if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
								{
									if(Player* player = sPlayerPool.GetByPlayerId(buffOneInfo->fromP))	//如果有玩家
									{
										sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

										//记录额外奖励
										IntPairVec pairvec;
										pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
										pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatInfoNum,0,pairvec);

										pb::extra_awards* exAward;
										if(isAttWork)	//攻方身上buff起作用
											exAward = msg.add_def_extra_awards();
										else
											exAward = msg.add_def_extra_awards();
										exAward->set_awardsid(deadnpc->GetExtraAwardID());
										exAward->set_awardsnum(deadnpc->GetExtraAwardNum());
									}
								}
							}

						}
						if(buffOwner->IsDead())	//如果武将死亡了
						{
							if(buffOwner->killerPID == 0)
							{
								buffOwner->killerPID = buffOneInfo->fromP;
								buffOwner->killerCID = buffOneInfo->fromC;
							}
						}
					}

					/******* 发放奖励 *******/
					_GetAwardsWithBuffHopOnce(msg, pGroup,buffOwner,buffOneInfo,(int)trueHurt,isAttWork);
				}
				break;
			case Combat::fixeddamage:				//固定血量掉血
				{
					/******* 造成伤害 *******/
					float commonHurt = buffOneInfo->value1;
					int trueHurt = (soldierRow.hp >= (int)commonHurt) ? (int)commonHurt : soldierRow.hp;		//未溢出的伤害
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

					/******* 记录杀手 *******/
					if(soldierRow.hp <= 0)	//如果该排兵死亡
					{
						if(soldierRow.killerPlayerId == 0)
						{
							soldierRow.killerPlayerId = buffOneInfo->fromP;
							soldierRow.killerCharId = buffOneInfo->fromC;

							//发放额外奖励
							if(::Npc* deadnpc = dynamic_cast<::Npc*>((buffOwner)))
							{
								if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
								{
									if(Player* player = sPlayerPool.GetByPlayerId(buffOneInfo->fromP))	//如果有玩家
									{
										sReward.Change(*player, deadnpc->GetExtraAwardID(), deadnpc->GetExtraAwardNum());

										//记录额外奖励
										IntPairVec pairvec;
										pairvec.push_back(std::make_pair(deadnpc->GetExtraAwardID() , deadnpc->GetExtraAwardNum()));
										pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatInfoNum,0,pairvec);

										pb::extra_awards* exAward;
										if(isAttWork)	//攻方身上buff起作用
											exAward = msg.add_def_extra_awards();
										else
											exAward = msg.add_def_extra_awards();
										exAward->set_awardsid(deadnpc->GetExtraAwardID());
										exAward->set_awardsnum(deadnpc->GetExtraAwardNum());
									}
								}
							}

						}
						if(buffOwner->IsDead())	//如果武将死亡了
						{
							if(buffOwner->killerPID == 0)
							{
								buffOwner->killerPID = buffOneInfo->fromP;
								buffOwner->killerCID = buffOneInfo->fromC;
							}
						}
					}

					/******* 发放奖励 *******/
					_GetAwardsWithBuffHopOnce(msg, pGroup,buffOwner,buffOneInfo,(int)trueHurt,isAttWork);
				}
				break;
			case Combat::percentdefenceup:			//百分比加防御
				break;
			case Combat::percentattackup:			//百分比加攻击
				break;
			default:
				break;
			}
		}
		if(soldierRow.hp <= 0) break;	//如果士兵已经死亡，则不再生效buff
	}
	soldierRow.buff_fight.RefreshByStep(worktime);
}

void Calc::_GetAwardsWithBuffHopOnce(pb::Combat_Seque& msg, CombatGroup* pGroup, CombatObj* buffOwner, Combat::BuffInfo* buffOneInfo, uint32 realHurtOfBuff, bool isAttWork)
{
	//经验=耗粮×转化比例×击杀比修正×（1+各种加成）×跨服倍数
	//银币=经验*0.2

	CombatObj* pDefence = buffOwner;

	//首先计算伤害的加成	//杀敌数翻倍，最大两倍
	int attOrgHurtToDef = realHurtOfBuff;	//攻方造成的原始伤害
	int attRealHurtToDef = realHurtOfBuff;	//双倍后击杀

	//结算奖励
	if(attOrgHurtToDef > 0)	//要求守方有伤亡
	{
		//如果守方是武将或者假武将，幻影，增援部队	//可以获得奖励	（其中幻影与增援部队只能获得玩家exp奖励与银币奖励）
		if(pDefence->IsPlayer() || pDefence->IsShadow() || pDefence->IsReinforce())
		{
			/********** 计算开始 ********/
			//计算耗粮
			float needFood = 0.0;
			const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(pDefence->soldierLv);
			if (pTable == NULL) 
			{
				LLOG("[2000 ERROR] _GetAwardsWithOneStep pTable is not found,lv=%d",pDefence->soldierLv);
				return;
			}
			needFood = pTable->NeedFood() * attOrgHurtToDef;

			//计算转换比率
			float foodToExp = FOOD_TO_EXP[pGroup->m_eCombat];

			//计算击杀比
			float killRatio = 2;
			Property& dPro = pDefence->stPro;	//守方属性
			killRatio = (float)buffOneInfo->fromStProAttack / (float)dPro.defence;
			if(killRatio > 2)
				killRatio = 2;
			else if(killRatio < 0.5)
				killRatio = 1;
			else
				killRatio = (killRatio + 1) * 2 / 3;

			//计算各种加成
			float totalAddtions = 0.0;
			//科技与练兵令,跨服等提供的经验加成
			totalAddtions += (((float)pDefence->GetTotalExpPercent(pGroup->m_eCombat))/((float)100));
			if(pGroup->m_eCombat == CT_Country)	//只有国战才有的加成
			{
				//等级差提供的经验加成
				uint32 levelAtt = buffOneInfo->fromStProLevel;
				uint32 levelDef = pDefence->GetPlayerLevel();
				if((levelAtt+16) <= levelDef)	//比对方等级小16级
				{
					uint32 levelDif = levelDef - levelAtt - 15;
					totalAddtions += MIN(((float)levelDif * (float)0.1),(float)1.0);
				}

				//TODO::禁卫区加成		寻路优化先做

				//TODO::武将天赋加成	具体武将具体做

			}

			//跨服倍数
			float crossTimes = 1.0;
#ifdef _SERVER_CROSS_
			crossTimes = 1.5;
#endif

			//计算最终奖励
			int exp = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
			int expPlayer = MAX(int(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes),1);
			int money = MAX((int)ceil(needFood * foodToExp * killRatio * (1 + totalAddtions) * crossTimes * 0.2),1);

			if(pGroup->m_eCombat == CT_Crusade)	//远征不获得经验与奖励
			{
				exp = 0;
				expPlayer = 0;
				money = 0;
			}
			/********** 计算结束 ********/


			/********* 发放奖励 ********/
			//之前版本都是在武将死亡或者战斗结束结算奖励与经验，现在改为立即结算是否更方便？ Try it
			uint32 exploitOfDef = 0;
			//真假武将，幻影，增援部队，增加玩家的经验与银币
			if(Player* master = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))
			{
				sReward.Change(*master, Reward::PlayerExp, expPlayer);	//玩家经验
				sReward.Change(*master, Reward::Coin_Silver, money);	//银币
				master->AddActivityStarBoxExp(expPlayer);				//玩家活动经验
				exploitOfDef = master->HurtToExploit(ENUM_TYPE_DEATH,attOrgHurtToDef); //死兵产生功勋
			}

			//真假武将增加武将经验
			if(pDefence->IsPlayer())
			{
				::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
				heroDefence->TryAddXP(exp);
			}
			//武将的幻影增加原武将经验
			if(pDefence->IsShadow())
			{
				::Character* heroDefence = dynamic_cast<::Character*>(pDefence);
				Character* charone = heroDefence->m_player->m_characterStorage->MutableCharacter(heroDefence->GetID());
				if(charone)
					charone->TryAddXP(exp);
			}
			/********* 发放结束 **********/

			/********** 填充消息 *********/
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

			/******** 记录各种战斗数据 ******/
			//记录战报
			IntPairVec tmp;
			//真假武将，幻影，增援部队均会增加玩家经验，兵力损失
			pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatPlayerExp,expPlayer,tmp);
			pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatDead,attOrgHurtToDef,tmp);
			pGroup->RecordCombatGainsInfo(pDefence->GetPlayerId(),pb::CombatExploit,exploitOfDef,tmp);
			/********** 记录结束 **********/


			/********* 增加进度 *********/
			if(::Character* heroDefence = dynamic_cast<::Character*>(pDefence))
			{
				if(heroDefence->GetIsInAutoCountryWar())	//武将在自动国战中
				{
					heroDefence->m_player->m_AutoCountryWarLog->AddExpGet(expPlayer);	//记录自动国战获得经验
				}
			}
			/********* 增加结束 *********/

		}

		//结算对守方造成伤害攻方所获得的奖励
		//如果攻方是武将或者假武将，幻影，增援部队	//可以获得功勋奖励
		uint32 exploitOfAtt = 0;	//攻方获得的功勋
		if(buffOneInfo->fromP > 0)
		{
			if(Player* player = sPlayerPool.GetByPlayerId(buffOneInfo->fromP))	//如果有玩家
			{
				if(buffOneInfo->fromC > 0)
				{
					if(Character* charone = player->m_characterStorage->GetCharacterByTableId(buffOneInfo->fromC))
					{
						attRealHurtToDef *= (1 + charone->GetTotalDoubleKill(pGroup->m_eCombat)/100);
						
						if(charone->GetIsInAutoCountryWar())	//武将在自动国战中
						{
							charone->m_player->m_AutoCountryWarLog->AddKillCount(attRealHurtToDef);	//记录自动国战击杀数
							charone->killOnceAHero += attRealHurtToDef;
						}
						CombatGroup::_SaveStaticKillOnce(*charone,pGroup->m_eCombat,attRealHurtToDef);	//记录其他击杀进度
					}
				}
				//是国战，则增加功勋
				if(pGroup->m_eCombat == CT_Country)
				{
					exploitOfAtt = player->HurtToExploit(ENUM_TYPE_KILL,attRealHurtToDef);
				}

				/******** 记录各种战斗数据 ******/
				//记录战报
				IntPairVec tmp;
				pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatKills,attRealHurtToDef,tmp);
				pGroup->RecordCombatGainsInfo(buffOneInfo->fromP,pb::CombatExploit,exploitOfAtt,tmp);
				/********** 记录结束 **********/
			}
		}
	}
}



//使用了技能	攻防列表，守方列表，攻防是否使用技能，守方是否使用技能，待填消息，战斗类型
void Calc::UseSkill(CombatGroup* pGroup, GroupLst& attacker, GroupLst& defender, bool isAttakcerUseSkill, bool isDefenderUserSkill, pb::GS2C_Combat_Select_Tactic& msg, CombatType tpy)
{
 //   CombatObj& rhs = *attacker.front();		//进攻方第一个武将
 //   CombatObj& lhs = *defender.front();		//防守方第一个武将

	//if(isAttakcerUseSkill)
	//{
	//	rhs.isUsedSkill = true;
	//	rhs.leftSkillTimes -= 1;
	//	float successRate1 = (float)((rhs.stPro.yong*STATIC_SKILL_RATE)+(rhs.stPro.yong*lhs.stPro.yong)) / ((rhs.stPro.yong*lhs.stPro.yong) + (lhs.stPro.yong*lhs.stPro.yong));		//成功使用技能的概率
	//	//LLOG("successRate1 = %f",successRate1);
	//	rhs.isSkillSuccess = (System::Randf(0, 1) < successRate1);		//使用技能是否成功
	//	if(rhs.IsReinforce())
	//		rhs.isSkillSuccess = true;
	//	if (rhs.isSkillSuccess) 
	//	{
	//		AttachBuffs(attacker,defender,1);
	//		_SkillHurt(pGroup,rhs, defender, 2, msg, tpy);		//成功则计算技能伤害  进攻方，防守序列，受伤方编号，待填充消息
	//	}
	//	msg.set_attskillid(rhs.SkillID());
	//}
	//else
	//	msg.set_attskillid(0);

 //   if (isDefenderUserSkill && lhs.soldiers.size() && lhs.soldiers.front().hp > 0 && rhs.soldiers.size() && rhs.soldiers.front().hp > 0)
 //   {
 //       lhs.isUsedSkill = true;
	//	lhs.leftSkillTimes -= 1;
 //       float successRate2 = (float)((lhs.stPro.yong*STATIC_SKILL_RATE)+(rhs.stPro.yong*lhs.stPro.yong)) / ((rhs.stPro.yong*lhs.stPro.yong) + (rhs.stPro.yong*rhs.stPro.yong));		//成功使用技能的概率
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
 //   //头排没人死，排头两个继续对砍
 //   while (!CheckDead(pGroup,attacker.front(),defender.front(),msg,false) && !CheckDead(pGroup,defender.front(),attacker.front(),msg,true))
 //   {
 //       int attackerHurt = _CalcCommonHurt(rhs, lhs, isAttakcerUseSkill ? 0.1f : 0);
 //       int defenderHurt = _CalcCommonHurt(lhs, rhs, isDefenderUserSkill ? 0.1f : 0);
	//	////填充伤害
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
  //      obj->isSelectTactic = false; //死一排士兵，重选操作
  //      ret = true;

		////如果士兵有奖励，则发放奖励
		//if(Npc* deadnpc = dynamic_cast<Npc*>(obj))
		//{
		//	if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
		//	{
		//		if(Character* killerchar = dynamic_cast<Character*>(killer))
		//		{
		//			pb::extra_awards* awardsmsg;
		//			if(isKillerAtt)	//击杀方为攻击方
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
		obj->isSelectTactic = false; //死一排士兵，重选操作
		ret = true;

		//如果士兵有奖励，则发放奖励
		if(Npc* deadnpc = dynamic_cast<Npc*>(obj))
		{
			if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
			{
				if(Character* killerchar = dynamic_cast<Character*>(killer))
				{
					pb::extra_awards* awardsmsg;
					if(isKillerAtt)	//击杀方为攻击方
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
	//	if(endcount++ > 9)	break;		//只计算前十个武将
	//	LLOG("one obj soldier size=%d",(*it)->soldiers.size());
	//	//if((*it)->CurHp() <= 0)	continue;

	//	CombatObj* comobj = *it;

	//	while ((*it)->soldiers.size() && (*it)->soldiers.front().hp <= 0)
	//	{
	//		//(*it)->m_fightBuff.RefreshByRange();
	//		(*it)->soldiers.erase((*it)->soldiers.begin());
	//		(*it)->isSelectTactic = false;

	//		//如果士兵有奖励，则发放奖励
	//		if(::Npc* deadnpc = dynamic_cast<::Npc*>((*it)))
	//		{
	//			if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
	//			{
	//				if(Character* killerchar = dynamic_cast<Character*>(killer))
	//				{
	//					pb::extra_awards* awardsmsg;
	//					if(isKillerAtt)	//击杀方为攻击方
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
		if(endcount++ > 9)	break;		//只计算前十个武将
		LLOG("one obj soldier size=%d",(*it)->soldiers.size());
		//if((*it)->CurHp() <= 0)	continue;

		CombatObj* comobj = *it;

		while ((*it)->soldiers.size() && (*it)->soldiers.front().hp <= 0)
		{
			//(*it)->m_fightBuff.RefreshByRange();
			(*it)->soldiers.erase((*it)->soldiers.begin());
			(*it)->isSelectTactic = false;

			//如果士兵有奖励，则发放奖励
			if(Npc* deadnpc = dynamic_cast<Npc*>((*it)))
			{
				if(deadnpc->GetExtraAwardID() > 0 && deadnpc->GetExtraAwardNum() > 0)
				{
					if(Character* killerchar = dynamic_cast<Character*>(killer))
					{
						pb::extra_awards* awardsmsg;
						if(isKillerAtt)	//击杀方为攻击方
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