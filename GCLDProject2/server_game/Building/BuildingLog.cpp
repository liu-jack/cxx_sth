#include "BuildingLog.h"
#include "datastruct/struct_building.h"
#include "Module.pb.h"
#include "BuildingDefine.h"
#include "Building.h"
BuildingLog::BuildingLog( const Building* buildingProto )
:m_pBuildingProto(buildingProto)
,m_curBuildLv(0)
,m_lvUpStartTime(0)
,m_isRebuild(false)
,m_isUnlock(false)
{

}
BuildingLog::~BuildingLog()
{
}

uint32 BuildingLog::id() const
{
	return m_pBuildingProto->id();
}

void BuildingLog::SaveTo( pb::GxDB_Building& msg ) const
{
	//ASSERT( id() == msg.building_id());
	msg.set_building_id(id());
	msg.set_building_lv(m_curBuildLv);
	msg.set_lvup_start_time(m_lvUpStartTime);
	msg.set_is_rebuild(m_isRebuild);
}

void BuildingLog::LoadFrom( const pb::GxDB_Building& msg )
{
	ASSERT( id() == msg.building_id());
	m_curBuildLv = msg.building_lv();
	m_lvUpStartTime = msg.lvup_start_time();
	m_isRebuild = msg.is_rebuild();
}

uint32 BuildingLog::ModuleId() const
{
	return m_pBuildingProto->moduleId();
}

uint32 BuildingLog::LvUpTime() const
{
	if (IsRechMaxLv())
	{
		return 0;
	}
	return m_pBuildingProto->LvUpTime(m_curBuildLv);
}

uint32 BuildingLog::BuildLvUp()
{
	uint32 maxLv = m_pBuildingProto->maxLv();
	if (m_curBuildLv < maxLv)
	{
		++m_curBuildLv;
		return m_curBuildLv;
	}
	return m_curBuildLv;
}

bool BuildingLog::IsRechMaxLv() const
{
	if (!m_pBuildingProto)
	{
		return false;
	}
	if (m_curBuildLv >= m_pBuildingProto->maxLv())
	{
		return true;
	}
	return false;
}

bool BuildingLog::IsUnlock() const
{
	return m_isUnlock;
}
uint32 BuildingLog::PlayerLvReq() const
{
	return m_pBuildingProto->LvReq();
}

uint32 BuildingLog::Icome() const
{
	return m_pBuildingProto->Icome(m_curBuildLv);
}

uint32 BuildingLog::IcomeLimit() const
{
	return m_pBuildingProto->IcomeLimit(m_curBuildLv);
}

uint32 BuildingLog::SetRebuildState(bool isRebuild)
{
	return m_isRebuild = isRebuild;
}

bool BuildingLog::LvUpReq(uint32& coinReq, uint32& woodReq ) const
{
	if(IsRechMaxLv())
	{
		return false;
	}
	else
	{
		return m_pBuildingProto->LvUpReq(m_curBuildLv,coinReq,woodReq);
	}
}

void  BuildingLog::CheckBuildingIsUnlock( uint32 player_lv )
{
	if (player_lv >= PlayerLvReq() && !m_isUnlock)
	{
		m_isUnlock = true;
		if(m_curBuildLv ==0)
			m_curBuildLv = 1;
	}
}
