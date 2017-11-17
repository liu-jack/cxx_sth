#include "SkillDataMgr.h"
#include "DbTool.h"
#include "datastruct/struct_skill_slg.h"

void SkillDataMgr::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_SkillSlg)
	{
		m_skillSlgMap[ptr1->id] = new SkillSlg(*ptr1);
	}

	FOREACH_DB_ITEM(ptr2, DB_Buff_Fight)
	{
		m_buffFightMap[ptr2->id] = new BuffFight(*ptr2);
	}

	FOREACH_DB_ITEM(ptr3, DB_Buff_Relation)
	{
		m_buffRelationMapDetail[ptr3->id] = new BuffRelation(*ptr3);
		m_buffRelationTypeToId[m_buffRelationMapDetail[ptr3->id]->GetTypeID()] = ptr3->id;
	}

}

int SkillDataMgr::GetObjSkillMaxTimes(uint32 skillID)
{
	SkillSlg* it = m_skillSlgMap.pfind(skillID);
	if(it)
	{
		return (it->GetReleaseTimes());
	}
	return 0;
}

int SkillDataMgr::GetObjSkillRange(uint32 skillID)
{
	SkillSlg* it = m_skillSlgMap.pfind(skillID);
	if(it)
	{
		return (it->GetAttactRange());
	}
	return 0;
}

float SkillDataMgr::GetDamageFactor(uint32 skillID)
{
	SkillSlg* it = m_skillSlgMap.pfind(skillID);
	if(it)
	{
		return (it->GetDamageFactor());
	}
	return 0;
}

int SkillDataMgr::GetBuffIDBySkill(uint32 skillID)
{
	SkillSlg* it = m_skillSlgMap.pfind(skillID);
	if(it)
	{
		return (it->GetAdditionBuffID());
	}
	return 0;
}

int SkillDataMgr::GetBuffType(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetBuffType();

	return 0;
}

int SkillDataMgr::GetBuffLastRoundsCount(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetLastRoundsCount();

	return 0;
}

int SkillDataMgr::GetBuffTarget(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetBuffTarget();

	return 0;
}

int SkillDataMgr::GetBuffRows(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetRows();

	return 0;
}

float SkillDataMgr::GetBuffValue1(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetValue1();

	return 0.0;
}

float SkillDataMgr::GetBuffValue2(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetValue2();

	return 0.0;
}

float SkillDataMgr::GetBuffValue3(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetValue3();

	return 0.0;
}

int SkillDataMgr::GetBuffRelationship(uint32 typeA, uint32 typeB)
{
	uint32 idA, idB;
	std::map<uint32, uint32>::iterator idAIte = m_buffRelationTypeToId.find(typeA);
	std::map<uint32, uint32>::iterator idBIte = m_buffRelationTypeToId.find(typeB);

	if(idAIte != m_buffRelationTypeToId.end() && idBIte != m_buffRelationTypeToId.end())
	{
		idA = idAIte->second;
		idB = idBIte->second;
	}
	else
		return 0;

	BuffRelationMap::iterator itr = m_buffRelationMapDetail.find(idA);
	if(itr != m_buffRelationMapDetail.end())
	{
		return itr->second->GetRelations(idB);
	}
	else
		return 0;
}

int SkillDataMgr::GetWeight(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetWeight();

	return 0;
}

int SkillDataMgr::GetEffectStage(uint32 buffid)
{
	BuffFight* it = m_buffFightMap.pfind(buffid);
	if(it)
		return it->GetEffectStage();

	return 0;
}