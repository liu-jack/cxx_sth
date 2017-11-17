#include "BuildingLogMgr.h"
#include <boost/smart_ptr.hpp>
#include "utility/STLUtility.h"
#include "System.h"
#include "BuildingLog.h"
#include "ModuleLog.h"
#include "BuildingManager.h"
#include "Module.pb.h"
#include <boost/bind.hpp>
#include "Building.h"
//#include <boost/function.hpp>
//#include "object/PlayerDataManager.h"
//#include "object/PlayerValueOwner.h"
#include "object/Player.h"
#include "Base.h"
using namespace pb;

BuildingLogMgr::BuildingLogMgr()
:m_curAutoLvUpBuildModule(0)
{
}

BuildingLogMgr::~BuildingLogMgr()
{
}

void BuildingLogMgr::Init()
{
	std::vector<uint32> vecBuildIds;
	sBuildingManager.GetBuildingIds(vecBuildIds);
	int nBuild = vecBuildIds.size();
	for (int i = 0; i< nBuild; i++)
	{
		uint32 curBuildId = vecBuildIds[i];
		const Building* buildingpro = sBuildingManager.GetBuildingById(curBuildId);
		if (buildingpro)
		{
			uint32 moduleId = buildingpro->moduleId();
			ModuleLog* curModuleLog = m_ModuleLogMap.pfind(moduleId);
			if (curModuleLog)
			{
				BuildingLog* newBuildLog = curModuleLog->GetBuildingLog(curBuildId);
				if (!newBuildLog)
				{
					newBuildLog = new BuildingLog(buildingpro);
					curModuleLog->AddBuildingLog(newBuildLog);
				}
			}
			else
			{
				const Module* modulepro = sBuildingManager.GetModuleById(moduleId);
				if (modulepro)
				{
					ModuleLog* newModLog = new ModuleLog(modulepro);
					if (newModLog)
					{
						BuildingLog* newBuildLog = new BuildingLog(buildingpro);
						if (newBuildLog)
						{
							m_ModuleLogMap[moduleId] = newModLog;
							newModLog->AddBuildingLog(newBuildLog);
						}
						else
						{
							delete newModLog;
						}
					}
				}
			}
		}
	}
}

void BuildingLogMgr::LoadModuleFrom( const pb::GxDB_Modules_Info& logInfo )
{
	int nmudule = logInfo.module_arr_size();
	for (int i = 0; i < nmudule; i++)
	{	
		const GxDB_Module& modu = logInfo.module_arr(i);
		int moid = modu.module_id();

		ModuleLog* curModuleLog = m_ModuleLogMap.pfind(moid);
		if (curModuleLog)
		{
			curModuleLog->LoadFrom(modu);
		}
		else
		{
			const Module* modulepro = sBuildingManager.GetModuleById(moid);
			if (modulepro)
			{
				ModuleLog* newModLog = new ModuleLog(modulepro);
				if (newModLog)
				{
					newModLog->LoadFrom(modu);
					m_ModuleLogMap[moid] = newModLog;
				}
			}
		}
		
	}
}

void BuildingLogMgr::LoadBuildingFrom( const pb::GxDB_Buildings_Info& logInfo )
{
	int nbuilding = logInfo.building_arr_size();
	for (int i = 0; i < nbuilding; i++)
	{	
		const GxDB_Building& gxbuild = logInfo.building_arr(i);
		int buildid = gxbuild.building_id();
		const Building* buildingpro = sBuildingManager.GetBuildingById(buildid);
		if (buildingpro)
		{
			uint32 curModuleId = buildingpro->moduleId();
			ModuleLog* curModuleLog = m_ModuleLogMap.pfind(curModuleId);
			if (curModuleLog)
			{
				BuildingLog* newBuildLog = curModuleLog->GetBuildingLog(buildid);
				if (newBuildLog)
				{
					newBuildLog->LoadFrom(gxbuild);
				}
				else
				{
					BuildingLog* newBuildLog = new BuildingLog(buildingpro);
					if (newBuildLog)
					{
						newBuildLog->LoadFrom(gxbuild);
						curModuleLog->AddBuildingLog(newBuildLog);
					}
				}
			}
			else
			{
				const Module* modulepro = sBuildingManager.GetModuleById(curModuleId);
				if (modulepro)
				{
					ModuleLog* newModLog = new ModuleLog(modulepro);
					if (newModLog)
					{
						BuildingLog* newBuildLog = new BuildingLog(buildingpro);
						if (newBuildLog)
						{
							newBuildLog->LoadFrom(gxbuild);
							m_ModuleLogMap[curModuleId] = newModLog;
							newModLog->AddBuildingLog(newBuildLog);
						}
						else
						{
							delete newModLog;
						}
					}
				}
			}
		}
	}
}

void BuildingLogMgr::SaveModuInfoTo(pb::GxDB_Modules_Info& logInfo)
{
	ModuleLogMap::iterator modulIter = m_ModuleLogMap.begin();
	for (; modulIter != m_ModuleLogMap.end(); ++modulIter)
	{
	    ModuleLog* pModule = modulIter->second;
		if (pModule)
		{
			pModule->SaveTo(*logInfo.add_module_arr());
		}
	}

	/*std::for_each(
		m_ModuleLogMap.begin(),
		m_ModuleLogMap.end(),
		boost::bind( &ModuleLog::SaveTo, boost::bind(&ModuleLogMap::value_type::second,_1),boost::ref(*logInfo.add_module_arr()))
		);*/
}

void BuildingLogMgr::SaveBuildingTo(pb::GxDB_Buildings_Info& logInfo)
{
	ModuleLogMap::iterator modulIter = m_ModuleLogMap.begin();
	for (; modulIter != m_ModuleLogMap.end(); ++modulIter)
	{
		ModuleLog* pModule = modulIter->second;
		if (pModule)
		{
			pModule->SaveBuildInfoTo(logInfo);
		}
	}
}

void BuildingLogMgr::SaveModuInfoStateTo(uint32 moduleId,uint32 autoHammerCount, pb::GS2C_Building_State& logInfo)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
	     curModule->SaveToState(logInfo);
	}
	else
	{
		const Module* modulepro = sBuildingManager.GetModuleById(moduleId);
		if (modulepro)
		{
			ModuleLog* newModLog = new ModuleLog(modulepro);
			if (newModLog)
			{
				m_ModuleLogMap[moduleId] = newModLog;
			}
			newModLog->SaveToState(logInfo);
		}
	}
	logInfo.set_remain_auto_times(autoHammerCount);
}

void BuildingLogMgr::SaveModuleInfo( uint32 moduleId,pb::GxDB_Module& moduldMsg )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		curModule->SaveTo(moduldMsg);
	}
}

void BuildingLogMgr::SaveBuildingInfo( uint32 buildId,pb::GxDB_Building& buildMsg )
{
	const Building*  curbuild = sBuildingManager.GetBuildingById(buildId);
	if (curbuild)
	{
		ModuleLog* curModule = m_ModuleLogMap.pfind(curbuild->moduleId());
		if(curModule)
		{
			curModule->SaveBuildInfoTo(buildId,buildMsg);
		}
	}
}

uint32 BuildingLogMgr::GetLvUpTime( uint32 moduleId, uint32 buildId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->GetLvUpTime(buildId);
	}
	else
	{
		const Module* modulepro = sBuildingManager.GetModuleById(moduleId);
		if (modulepro)
		{
			ModuleLog* newModLog = new ModuleLog(modulepro);
			if (newModLog)
			{
				m_ModuleLogMap[moduleId] = newModLog;
			}
			return newModLog->GetLvUpTime(buildId);
		}
	}

	return 0;
}

uint32 BuildingLogMgr::BuildLvUp( uint32 moduleId, uint32 buildId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		uint32 upLv = curModule->BuildLvUp(buildId);
		if (upLv > 1)
		{
			if (m_curLvUpBuildIds.find(moduleId) != m_curLvUpBuildIds.end())
			{
				m_curLvUpBuildIds[moduleId].erase(buildId);
			}
			return upLv;
		}
	}
	return 1;
}

uint32 BuildingLogMgr::GetBuildCurLv( uint32 moduleId, uint32 buildId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if (curModule)
	{
		 return curModule->GetBuildCurLv(buildId);
	}
	return 0;
}

void BuildingLogMgr::SetCurAutoLvUpModule(uint32 moduleId)
{
	if (m_curAutoLvUpBuildModule != moduleId)	//当前自动建造的模型
	{
		m_curAutoLvUpBuildModule = moduleId;
	}
}

uint32 BuildingLogMgr::GetUnlockBuildingCurlv(uint32 ModuleId)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(ModuleId);
	if (curModule == NULL)
	{
		return 1;
	}
	const BuildingLogMap& buildlogmap = curModule->GetBuildingLogMap();
	uint32 min_build_lv = 1;
	std::vector<uint32> lvVec;
	for(BuildingLogMap::const_iterator iter = buildlogmap.begin();iter != buildlogmap.end();++iter)
	{
		const BuildingLog* pbuild = iter->second;
		if (pbuild && pbuild->IsUnlock())
		{
			uint32 buildlv =  pbuild->CurLv();
			lvVec.push_back(buildlv);
		}
	}
	std::sort(lvVec.begin(),lvVec.end());
	if ((int)lvVec.size() > 0)
	{
		min_build_lv = lvVec.front();
	}
	return min_build_lv;
}
uint32 BuildingLogMgr::GetBuildCurLv( uint32 buildId )
{
	const Building*  curbuild = sBuildingManager.GetBuildingById(buildId);
	if (curbuild)
	{
		ModuleLog* curModule = m_ModuleLogMap.pfind(curbuild->moduleId());
		if(curModule)
		{
			return curModule->GetBuildCurLv(buildId);
		}
	}
	return 0;
}

bool BuildingLogMgr::CanBuildingLvUp( uint32 buildId,uint32 curPlayerLv )
{
	const Building*  curbuild = sBuildingManager.GetBuildingById(buildId);
	if (curbuild)
	{
		ModuleLog* curModule = m_ModuleLogMap.pfind(curbuild->moduleId());
		if(curModule)
		{
			return curModule->CanBuildingLvUp(buildId,curPlayerLv);
		}
	}
	return false;
}

bool BuildingLogMgr::CanBuildingLvUp( uint32 moduleId, uint32 buildId,uint32 curPlayerLv )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->CanBuildingLvUp(buildId,curPlayerLv);
	}
	return false;
}

bool BuildingLogMgr::GetBuildingLvUpReq( uint32 moduleId, uint32 buildId,uint32& coinReq, uint32& woodReq )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->GetBuildingLvUpReq(buildId,coinReq,woodReq);
	}
	return false;
}

void BuildingLogMgr::UpdateAutoLvUpTimes(PlayerValueOwner* pValue,  uint32 moduleId)
{
	SetCurAutoLvUpModule(moduleId);
	if(pValue->GetAutoHammerCount()  < AUTOLVUPHAMMERTIMES)
	{
		pValue->SetAutoHammerCount(AUTOLVUPHAMMERTIMES);
	}
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		curModule->CheckAutoLvUpBuildings();
	}

}

void BuildingLogMgr::GetLvUpBuildingIds(PlayerValueOwner* pValue, uint32 moduleId, std::vector<uint32>& buildIds,uint32 curPlayerLv)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if (curModule && pValue)
	{
		uint32 curAutoLvUpTimes = pValue->GetAutoHammerCount();
		if (curAutoLvUpTimes > 0)
		{
			curModule->GetMinLevelUpBuildIds(buildIds,moduleId,curAutoLvUpTimes,curPlayerLv,m_curLvUpBuildIds);
		}
	}
}

bool BuildingLogMgr::IsBuildingInLvUp(uint32 moduleId,uint32 buildId)
{
	return m_curLvUpBuildIds[moduleId].find(buildId) != m_curLvUpBuildIds[moduleId].end();
}

bool BuildingLogMgr::AddLvUpBuilding(uint32 moduleId,uint32 buildId)
{
	if (m_curLvUpBuildIds[moduleId].find(buildId) == m_curLvUpBuildIds[moduleId].end())
	{
		m_curLvUpBuildIds[moduleId].insert(buildId);
		return true;
	}
	else
	{
		return false;
	}
}

bool BuildingLogMgr::RemoveLvUpBuilding(uint32 moduleId,uint32 buildId)
{
	if (m_curLvUpBuildIds[moduleId].find(buildId) != m_curLvUpBuildIds[moduleId].end())
	{
		m_curLvUpBuildIds[moduleId].erase(buildId);
		return true;
	}
	else
	{
		return false;
	}
}

bool BuildingLogMgr::IsRechBuildingLvUpMaxCount()
{
	int curIdCoount = 0;
	for (ModuleLvUpInfo::iterator iter = m_curLvUpBuildIds.begin(); iter != m_curLvUpBuildIds.end(); ++iter)
	{
		curIdCoount += iter->second.size();
	}
	if(curIdCoount >= MAXBUILDINGLVUPHAMMERS)
	{
		return true;
	}
	return false;
}

bool  BuildingLogMgr::CurHasBuildingLvUp()
{
	int curIdCoount = 0;
	for (ModuleLvUpInfo::iterator iter = m_curLvUpBuildIds.begin(); iter != m_curLvUpBuildIds.end(); ++iter)
	{
		curIdCoount += iter->second.size();
	}
	if(curIdCoount > 0)
	{
		return true;
	}
	return false;
}

void BuildingLogMgr::GoldHammerLvUp( boost::function<void(uint32,uint32)> f )
{
	ModuleLvUpInfo tempMapData;
	std::copy(m_curLvUpBuildIds.begin(),m_curLvUpBuildIds.end(),std::inserter(tempMapData, tempMapData.begin()));
	for (ModuleLvUpInfo::iterator iter = tempMapData.begin(); iter != tempMapData.end(); ++iter)
	{
		uint32 moduleId = iter->first;
		BuildingSet& idSet = iter->second;
		for (BuildingSet::iterator iditer = idSet.begin(); iditer != idSet.end(); ++ iditer)
		{
			uint32 buildId = *iditer;
			f(moduleId,buildId);
		}
	}
}

uint32 BuildingLogMgr::GetModuleIncome(uint32 moduleId)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->Icome();
	}
	return 0;
}
uint32 BuildingLogMgr::GetModuleIncomeBase(uint32 moduleId)
{
    ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
    if (curModule)
    {
        return curModule->IcomeBase();
    }
    return 0;
}
uint32 BuildingLogMgr::GetModuleIncomeLimit( uint32 moduleId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->IcomeLimit();
	}
	return 0;
}

uint32 BuildingLogMgr::GetIcomeType( uint32 moduleId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->IncomeType();
	}
	return 0;
}

uint32 BuildingLogMgr::GetCurGainBuffId( uint32 moduleId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->CurGainBuffId();
	}
	return 0;
}

bool BuildingLogMgr::AddCurGainBuffTime( uint32 moduleId, uint32 gainBuffId, uint32 moreTime)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->AddExpireTime(gainBuffId,moreTime);
	}
	return false;
}

void BuildingLogMgr::UpdateModuleSartGainTimerTime( uint32 moduleId,uint64 time )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		curModule->SetSartGainTimerTime(time);
	}
}

void BuildingLogMgr::UpdateModuleGainValue(Player*player, const uint32 moduleId,const uint32 value )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		curModule->SetGainValue(value);
	}
}

void BuildingLogMgr::UpdateGrowUpQuestInfo(const uint32 moduleId,Player* player,const uint32 value)
{
	//成长任务
	if(moduleId == MODULE_MINJU)
	{
		player->UpdateAllGrowUpQuestInfo(pb::SOLIVER_QUAILTY,value);
	}
	if(moduleId == MODULE_MUCHANG)
	{
		player->UpdateAllGrowUpQuestInfo(pb::JINGKUANG_QUAILTY,value);
	}
	if(moduleId == MODULE_LONGCHANG)
	{
		player->UpdateAllGrowUpQuestInfo(pb::QIKUANG_QUAILTY,value);
	}
	if(moduleId == MODULE_BINGYING)
	{
		player->UpdateAllGrowUpQuestInfo(pb::BINYING_QUAILTY,value);
	}
	//////////////////////////////////
}

time_t BuildingLogMgr::GetModuleSartGainTimerTime( uint32 moduleId)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->GetSartGainTimerTime();
	}
	return 0;
}

uint32 BuildingLogMgr::GetModuleGainValue( uint32 moduleId)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		return curModule->GetGainValue();
	}
	return 0;
}

void BuildingLogMgr::UpdateModuleRebuildState( uint32 moduleId,bool isRebuild )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		curModule->SetRebuildState(isRebuild);
	}
}

void BuildingLogMgr::UpdateBuildingRebuildState( uint32 moduleId, uint32 buildId, bool isRebuild )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(moduleId);
	if(curModule)
	{
		BuildingLog* curbuildLog = curModule->GetBuildingLog(buildId);
		curbuildLog->SetRebuildState(isRebuild);
	}
}

void BuildingLogMgr::CheckBuildingIsUnlock(Player* player, uint32 player_lv )
{
	ModuleLogMap::iterator modulIter = m_ModuleLogMap.begin();
	for (; modulIter != m_ModuleLogMap.end(); ++modulIter)
	{
		ModuleLog* pModule = modulIter->second;
		if (pModule)
		{
			pModule->CheckBuildingIsUnlock(player_lv);
			UpdateModuleGainValue(player,pModule->id(),player->GetBuildingIncome(pModule->id()));
		}
	}
}

bool BuildingLogMgr::IsBuildingUnlock(uint32 ModuleId)
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(ModuleId);
	if(curModule)
	{
		return curModule->IsBuildingUnlock();
	}
	return false;
}

uint32 BuildingLogMgr::GetUnlockBuildingSize( uint32 ModuleId )
{
	ModuleLog* curModule = m_ModuleLogMap.pfind(ModuleId);
	if (curModule == NULL)
	{
		return 0;
	}
	const BuildingLogMap& buildlogmap = curModule->GetBuildingLogMap();
	uint32 count = 0;
	for(BuildingLogMap::const_iterator iter = buildlogmap.begin();iter != buildlogmap.end();++iter)
	{
		const BuildingLog* pbuild = iter->second;
		if (pbuild && pbuild->IsUnlock())
		{
			count++;
		}
	}
	return count;
}