#include "Module.h"
#include "datastruct/struct_building.h"



Module::Module(const DB_Module & dbModuleProto)
	:m_dbModuleProto(dbModuleProto)
{
}

Module::~Module()
{
}

uint32 Module::id() const
{
	return m_dbModuleProto.id;
}

void Module::AddBuildId(uint32 Id)
{
	m_buildIdSet.insert(Id);
}

void Module::GetBuildIds( std::vector<uint32>& vecIds ) const
{
	std::copy(m_buildIdSet.begin(),m_buildIdSet.end(),std::back_inserter(vecIds));
}

uint32 Module::TimeType() const
{
	return m_dbModuleProto.time_type;	//1，分钟恢复；2，小时恢复
}

uint32 Module::IncomeType() const
{
	return m_dbModuleProto.income_type;	//收益物品id
}
