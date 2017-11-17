#include "CombatBuff.h"
#include "SkillDataMgr.h"
#include "BuffFight.h"
#include "Logger.h"
#include "Combat.pb.h"
#include "utility/Utility.h"
#include "System.h"

using namespace Combat;


bool FBuff::HaveBuff(uint32 id) const
{
	if(m_buff.size()<=0) return false;

	FBuffMap::const_iterator it = m_buff.find(id);
	if(it == m_buff.end())
		return true;

	return false;
}

bool FBuff::HaveBuffWork(BuffType typ) const
{
	if(m_buff.size()<=0) return false;

	for(FBuffMap::const_iterator it = m_buff.begin(); it != m_buff.end(); ++it)
	{
		if(it->second.buffTyp == typ && it->second.isWork)
			return true;
	}
	return false;
}

void FBuff::AddBuff(uint32 buffid, uint64 fromP, uint32 fromC, uint64 addBuffRound, uint32 fromStProAttack, uint32 fromStProLevel)
{
	uint32 maxweight = 1;
	FBuffMap::iterator itGet = m_buff.end();
	BuffType getBuffType = (BuffType)sSkillDataMgr.GetBuffType(buffid);	//传入的buff类型
	if(getBuffType == Combat::None) return;

	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end(); ++it)
	{
		BuffInfo& buffinfoone = it->second;
		uint32 relation = sSkillDataMgr.GetBuffRelationship((uint32)getBuffType,(uint32)buffinfoone.buffTyp);
		if(relation > maxweight)
		{
			maxweight = relation;
			itGet = it;
		}
	}

	if(maxweight > 1)	//有其他关系
	{
		BuffInfo& buffInfoMax = itGet->second;

		switch(maxweight)
		{
		case 2:	//覆盖
			buffInfoMax.buffId = buffid;
			buffInfoMax.buffTyp = getBuffType;
			buffInfoMax.buffRemain = sSkillDataMgr.GetBuffLastRoundsCount(buffid);
			buffInfoMax.value1 = sSkillDataMgr.GetBuffValue1(buffid);
			buffInfoMax.value2 = sSkillDataMgr.GetBuffValue2(buffid);
			buffInfoMax.value3 = sSkillDataMgr.GetBuffValue3(buffid);
			buffInfoMax.fromP = fromP;
			buffInfoMax.fromC = fromC;
			buffInfoMax.fromStProAttack = fromStProAttack;
			buffInfoMax.fromStProLevel = fromStProLevel;
			buffInfoMax.addBuffRound = addBuffRound;
			buffInfoMax.worktime = sSkillDataMgr.GetEffectStage(buffid);
			buffInfoMax.weight = sSkillDataMgr.GetWeight(buffid);
			buffInfoMax.uniqueid = (buffInfoMax.weight << 16) + addBuffRound;
			break;
		case 3:	//融合
			buffInfoMax.buffRemain += sSkillDataMgr.GetBuffLastRoundsCount(buffid);
			buffInfoMax.value1 += sSkillDataMgr.GetBuffValue1(buffid);
			buffInfoMax.value2 += sSkillDataMgr.GetBuffValue2(buffid);
			buffInfoMax.value3 += sSkillDataMgr.GetBuffValue3(buffid);
			buffInfoMax.fromP = fromP;
			buffInfoMax.fromC = fromC;
			buffInfoMax.fromStProAttack = fromStProAttack;
			buffInfoMax.fromStProLevel = fromStProLevel;
			buffInfoMax.addBuffRound = addBuffRound;
			buffInfoMax.uniqueid = (buffInfoMax.weight << 16) + addBuffRound;
			break;
		case 4:	//互斥
			break;
		}
		return;
	}
	else
	{
		BuffInfo buffinfo;
		buffinfo.buffId = buffid;
		buffinfo.buffTyp = getBuffType;
		buffinfo.buffRemain = sSkillDataMgr.GetBuffLastRoundsCount(buffid);
		buffinfo.value1 = sSkillDataMgr.GetBuffValue1(buffid);
		buffinfo.value2 = sSkillDataMgr.GetBuffValue2(buffid);
		buffinfo.value3 = sSkillDataMgr.GetBuffValue3(buffid);
		buffinfo.fromP = fromP;
		buffinfo.fromC = fromC;
		buffinfo.fromStProAttack = fromStProAttack;
		buffinfo.fromStProLevel = fromStProLevel;
		buffinfo.addBuffRound = addBuffRound;
		buffinfo.worktime = sSkillDataMgr.GetEffectStage(buffid);
		buffinfo.weight = sSkillDataMgr.GetWeight(buffid);
		buffinfo.isWork = false;
		buffinfo.uniqueid = (buffinfo.weight << 16) + addBuffRound;
		m_buff.insert(std::make_pair(buffid,buffinfo));
	}
}

void FBuff::DelBuff(uint32 buffid)
{
	m_buff.erase(buffid);
}

void FBuff::DelBuff(BuffType typ)
{
	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end();)
	{
		BuffInfo& buffinfoone = it->second;
		if(buffinfoone.buffTyp == typ)
			it = m_buff.erase(it);
		else
			it++;
	}
}

bool FBuff::IsBuffEmpty()
{
	return m_buff.empty();
}

//回合结束刷新
void FBuff::RefreshByRound()
{
	if(m_buff.empty())
		return;

	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end();)
	{
		if(it->second.buffRemain <= 0)		//永久
		{
			it++;
			continue;
		}
		else
		{
			it->second.buffRemain--;
			if(it->second.buffRemain <= 0)
			{
				m_buff.erase(it++);
				continue;;
			}
		}
		it++;
	}
}

//刷新时机为三个阶段
void FBuff::RefreshByStep(uint32 step)
{
	if(m_buff.empty())
		return;

	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end();)
	{
		if(it->second.buffRemain <= 0)		//永久
		{
			it++;
			continue;
		}
		else
		{
			if(it->second.worktime == step)
			{
				it->second.buffRemain--;
				if(it->second.buffRemain <= 0)
				{
					m_buff.erase(it++);
					continue;;
				}
			}	
		}
		it++;
	}
}

void FBuff::ClearAllBuff()
{
	if(!m_buff.empty())
	{
		m_buff.clear();
	}
}

const BuffInfo* FBuff::GetBuffInfo(BuffType typ)
{
	if(m_buff.empty())
		return NULL;

	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end(); it++)
	{
		if(it->second.buffTyp == typ)
			return &(it->second);
	}
	return NULL;
}


FWorkBuffMap FBuff::GetFinalBuffMap()
{
	FWorkBuffMap m_workBuff;
	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end(); it++)
	{
		//uint64 finalWeight = (it->second.weight << 16) + it->second.addBuffRound;
		m_workBuff.insert(std::make_pair(it->second.uniqueid,&(it->second)));
	}
	return m_workBuff;
}

void FBuff::FillBuffInfoMsg(pb::Combat_All_Buff& msg, bool isAtt, uint32 rowNum)
{
	FWorkBuffMap mapOfWeight = GetFinalBuffMap();
	for(FWorkBuffMap::reverse_iterator rit=mapOfWeight.rbegin();rit!=mapOfWeight.rend();rit++) 
	{
		BuffInfo* buffone = rit->second;
		//if(buffone->buffRemain > 0)		//如果buff还在生效
		//{
			if(isAtt)
			{
				pb::Combat_buff* buffmsgone = msg.add_buff_one_att();
				buffmsgone->set_buff_tid(buffone->buffId);
				buffmsgone->set_buff_unique_id(buffone->uniqueid);
				buffmsgone->set_generic(false);
				buffmsgone->set_group(1);
				buffmsgone->set_rowindex(rowNum);
				buffmsgone->set_leftroundcount(buffone->buffRemain);
				LLOG("buff info: id=%d,remain=%d",buffone->buffId,buffone->buffRemain);
			}
			else
			{
				pb::Combat_buff* buffmsgone = msg.add_buff_one_def();
				buffmsgone->set_buff_tid(buffone->buffId);
				buffmsgone->set_buff_unique_id(buffone->uniqueid);
				buffmsgone->set_generic(false);
				buffmsgone->set_group(2);
				buffmsgone->set_rowindex(rowNum);
				buffmsgone->set_leftroundcount(buffone->buffRemain);
				LLOG("buff info: id=%d,remain=%d",buffone->buffId,buffone->buffRemain);
			}

		//}
	}
}

//void FBuff::FillBuffInfoMsg(pb::Combat_Buff_AllInfo& msg, uint32 startrow)
//{
//	//NLOG("FBuff::FillBuffInfoMsg1");
//	//for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end(); it++)
//	//{
//	//	int buffRange = it->second.buffRange;
//	//	NLOG("buffRange=%d",buffRange);
//	//	for(int i=0; i<buffRange; i++)
//	//	{
//	//		pb::Combat_Buff_Remain* buffone = msg.add_buffremians();
//	//		buffone->set_groupfrom(it->second.form);
//	//		buffone->set_groupto(it->second.to);
//	//		buffone->set_range(startrow+i);
//	//		buffone->set_buffid(it->second.buffId);
//	//	}
//
//	//}
//	
//}
//
//void FBuff::FillBuffInfoMsg(pb::GS2C_Combat_Select_Tactic& msg, uint32 startrow)
//{
//	//NLOG("FBuff::FillBuffInfoMsg2");
//	//for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end(); it++)
//	//{
//	//	int buffRange = it->second.buffRange;
//	//	NLOG("buffRange=%d",buffRange);
//	//	for(int i=0; i<buffRange; i++)
//	//	{
//	//		pb::Combat_Buff_Remain* buffone = msg.add_buffsremain();
//	//		buffone->set_groupfrom(it->second.form);
//	//		buffone->set_groupto(it->second.to);
//	//		buffone->set_range(startrow+i);
//	//		buffone->set_buffid(it->second.buffId);
//	//	}
//
//	//}
//
//}

int FBuff::GetFinalDefenceValue(int orgDefence)
{
	int retVelue = orgDefence;
	FWorkBuffMap mapOfWeight = GetFinalBuffMap();
	for(FWorkBuffMap::reverse_iterator rit=mapOfWeight.rbegin();rit!=mapOfWeight.rend();rit++) 
	{
		BuffInfo* buffone = rit->second;
		if(buffone->isWork)	//起效了
		{
			if(buffone->buffTyp == percentdefenceup)		//百分比加防
				retVelue = (int)(retVelue*(1+buffone->value1));

			if(buffone->buffTyp == fixeddefenceup)			//固定加防
				retVelue += (int)buffone->value1;

		}
	}
	return retVelue;
}

int FBuff::GetFinalAttackValue(int orgAttack)
{
	int retVelue = orgAttack;
	FWorkBuffMap mapOfWeight = GetFinalBuffMap();
	for(FWorkBuffMap::reverse_iterator rit=mapOfWeight.rbegin();rit!=mapOfWeight.rend();rit++) 
	{
		BuffInfo* buffone = rit->second;
		if(buffone->isWork)	//起效了
		{
			if(buffone->buffTyp == percentattackup)		//百分比加攻击
				retVelue = (int)(retVelue*(1+buffone->value1));

			if(buffone->buffTyp == fixedattackup)			//固定加攻击
				retVelue += (int)buffone->value1;

		}
	}
	return retVelue;
}

int FBuff::GetFinalDamageAfterDodge(int damage)
{
	const BuffInfo* dodgeInfo = GetBuffInfo(dodge);
	if(dodgeInfo)
	{
		float dice = System::Randf(0.0f, 1.0f );
		if(dice < dodgeInfo->value1)	//闪避成功
			return 0;
		else
			return damage;
	}
	return damage;
}

double FBuff::GetFinalDeskillHurtPercent()
{
	double retnum = 0;
	for(FBuffMap::iterator it = m_buff.begin(); it != m_buff.end(); it++)
	{
		if(it->second.buffTyp == percentdeskillhurt)
			retnum += it->second.value1;
	}
	return retnum;
}

Combat::FBuff::FBuff()
{

}

Combat::FBuff::~FBuff()
{

}
