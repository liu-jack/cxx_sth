#include "PlayerData.h"
// PlayerData.cpp 部分实现
//////////////////////////////////////////////////////////////////////////
#include <boost/bind.hpp>
#include <boost/function.hpp>     
#include <boost/typeof/typeof.hpp>
//#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "Module.pb.h"
bool PlayerData::save_mod_player_building()
{
	m_BuildingTable.SaveMod();
	return true;
}

bool PlayerData::save_mod_player_module()
{
	m_ModuleTable.SaveMod();
	return true;
}

void PlayerData::WriteModuleInfo( pb::GxDB_Modules_Info &allmodule )
{
    for ( BOOST_AUTO( it , m_ModuleTable.Begin());
        it != m_ModuleTable.End(); ++it )
    {
        MsgTool::SaveToRepeatField( it->second, allmodule.mutable_module_arr());
    }
}

void PlayerData::WriteBuildInfo( pb::GxDB_Buildings_Info &allbuilding )
{
	for ( BOOST_AUTO( it , m_BuildingTable.Begin());
		it != m_BuildingTable.End(); ++it )
	{
		MsgTool::SaveToRepeatField( it->second, allbuilding.mutable_building_arr());
	}
}

void PlayerData::UpdateModuleInfo( pb::GxDB_Module &module )
{
	ModuleTable* cell = m_ModuleTable.GetElement(module.module_id());
	if (cell)
	{
		SetIsDirty( DIRTY_PLAYER_MODULE);
		ASSERT( cell->module_id == module.module_id());
		cell->LoadFrom(module);
		m_ModuleTable.SetModifiedID( module.module_id());
	} 
	else
	{
		ModuleTable elem;
		elem.player_id = GetPlayerId();
		elem.LoadFrom(module);
		m_ModuleTable.AddAndAddToCache( &elem);
	}
}

void PlayerData::UpdateBuildInfo( pb::GxDB_Building &building )
{
	BuildingTable* cell = m_BuildingTable.GetElement(building.building_id());
	if (cell)
	{
        SetIsDirty( DIRTY_PLAYER_BUILDING);
        ASSERT( cell->building_id == building.building_id() );
        cell->LoadFrom(building);
        m_BuildingTable.SetModifiedID( building.building_id());
	} 
	else
	{
        BuildingTable elem;
        elem.player_id = GetPlayerId();
        elem.LoadFrom(building);
        m_BuildingTable.AddAndAddToCache( &elem);
	}

}