#include "BuffFight.h"
#include "datastruct/struct_skill_slg.h"
#include "utility/Utility.h"

BuffFight::BuffFight(const DB_Buff_Fight& db)
	:m_dbBuffFight(db)
{
}

int BuffFight::GetBuffType()
{
	return (int)m_dbBuffFight.buff_type;
}

int BuffFight::GetLastRoundsCount()
{
	return (int)m_dbBuffFight.last_time;
}

int BuffFight::GetBuffTarget()
{
	return (int)m_dbBuffFight.target;
}

int BuffFight::GetRows()
{
	return (int)m_dbBuffFight.rows;
}

float BuffFight::GetValue1()
{
	return m_dbBuffFight.value1;
}

float BuffFight::GetValue2()
{
	return m_dbBuffFight.value2;
}

float BuffFight::GetValue3()
{
	return m_dbBuffFight.value3;
}

int BuffFight::GetWeight()
{
	return m_dbBuffFight.who_first;
}

int BuffFight::GetEffectStage()
{
	return m_dbBuffFight.when_take_effect;
}

BuffRelation::BuffRelation(const DB_Buff_Relation& db)
:m_dbBuffRelation(db)
{
	Utility::SplitStr(m_dbBuffRelation.buff_relation,relationLst,',');
}

int BuffRelation::GetRelations(uint32 id)
{
	if(id < relationLst.size())
		return relationLst[id];

	LLOG("BuffRelation GetRelations Id Is Not Match");
	return 0;
}

uint32 BuffRelation::GetTypeID()
{
	return m_dbBuffRelation.buff_type;
}

std::vector<int>& BuffRelation::GetRelationLst()
{
	return relationLst;
}