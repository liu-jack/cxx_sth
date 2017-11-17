#include "PlayerOperationLogic.h"

#include "OS.h"
#include "boost/typeof/typeof.hpp"
#include "boost/foreach.hpp"
#include "object/Player.h"
#include "utility/DailyTimeFrame.h"
#include "server_client/DbProxyServerClient.h"


#include "Loot/LootList.h" 
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Module.pb.h"
#include "Enum.pb.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h" 
#include "Building/BuildingManager.h"
#include "Building/BuildingDefine.h"
#include "Building/BuildingLogMgr.h"
#include "datastruct/struct_panzer.h"
#include "../Trigger/trigger.h"
using namespace pb;

namespace
{
    using namespace boost::gregorian;
    using namespace boost::posix_time;
    pb::CxGS_ERROR_CODE CanTakeBonus( const ptime& now, const ptime& last, const DailyTimeFrame& fram)
    {
        if ( !fram.Contains(now))
        {
            return pb::ErrTimeNotMatch;
        }
        else
        {
            if ( now.date() != last.date())
            {
                return pb::ErrCommonSuccess;
            }
            else
            {
                if ( fram.Contains( last))
                {
                    return pb::ErrNowBonusHasTake; 
                }
                else
                {
                    return pb::ErrCommonSuccess;
                }
            }

        }        
    }
//#ifdef _MMO_SERVER_  //define but not use well build error in linux
//#include <ctime>
//    static ptime create_time(::std::tm* current)
//    {
//        ptime::date_type d(static_cast<unsigned short>(current->tm_year + 1900),
//            static_cast<unsigned short>(current->tm_mon + 1),
//            static_cast<unsigned short>(current->tm_mday));
//        ptime::time_duration_type td(current->tm_hour,
//            current->tm_min,
//            current->tm_sec);
//        return ptime(d,td);
//    }

    //static ptime local_time()
    //{
    //    ::std::time_t t = sOS.GetRealTime();
    //    ::std::tm *curr_ptr;
    //    curr_ptr = ::localtime(&t);
    //    return create_time(curr_ptr);
    //}
//#endif
}




uint32 PlayerOperationLogic::HandlePlayerStartAutoIncreaseSolders( Player& player )
{
	NLOG("TEST PlayerOperationLogic::HandlePlayerStartAutoIncreaseSolders");
	if (!player.IsAutoIncreaseSolders())
	{
		int curAllFood = player.GetCurrency(eFood);
		if (curAllFood <= 0)
		{
			return ErrNotEnoughFood;
		}
		player.StartAutoIncreaseSolders();
	}
	return ErrCommonSuccess;
}

void PlayerOperationLogic::UpdateBuildingInfo(Player &player, uint32 buildId)
{
	uint32 moduleId = sBuildingManager.GetBuildingModuleId(buildId);
	if (moduleId > 0)
	{
		pb::GS2C_Building_Update buildMsg;
		buildMsg.set_module_id(moduleId);
		player.m_buildingLogMgr->SaveBuildingInfo(buildId,*buildMsg.mutable_building_info());
		sDbProxyClient.SendPlayerPack( player, pb::SG2D_UPDATE_BUILDING_RECORD,buildMsg.building_info());
		player.Send( SMSG_BUILDING_UPDATE, buildMsg);
	}
}

void PlayerOperationLogic::UpdateBuildingInfoToDB(Player &player, uint32 buildId)
{
	pb::GxDB_Building buildMsg;
	player.m_buildingLogMgr->SaveBuildingInfo(buildId,buildMsg);
	sDbProxyClient.SendPlayerPack( player, pb::SG2D_UPDATE_BUILDING_RECORD,buildMsg);
}

void PlayerOperationLogic::UpdateBuildingInfoToClient(Player &player, uint32 buildId)
{
	uint32 moduleId = sBuildingManager.GetBuildingModuleId(buildId);
	if (moduleId > 0)
	{
		pb::GS2C_Building_Update buildMsg;
		buildMsg.set_module_id(moduleId);
		player.m_buildingLogMgr->SaveBuildingInfo(buildId,*buildMsg.mutable_building_info());
		player.Send( SMSG_BUILDING_UPDATE, buildMsg);
	}
}

void PlayerOperationLogic::UpdateModuleInfo(Player &player, uint32 moduleId)
{
	pb::GxDB_Module moduleMsg;
	player.m_buildingLogMgr->SaveModuleInfo(moduleId,moduleMsg);
	sDbProxyClient.SendPlayerPack( player, pb::SG2D_UPDATE_MODULE_RECORD,moduleMsg);
	player.Send( SMSG_MODULE_UPDATE, moduleMsg);
}

void PlayerOperationLogic::UpdateModuleInfoToDB(Player &player, uint32 moduleId)
{
	pb::GxDB_Module moduleMsg;
	player.m_buildingLogMgr->SaveModuleInfo(moduleId,moduleMsg);
	sDbProxyClient.SendPlayerPack( player, pb::SG2D_UPDATE_MODULE_RECORD,moduleMsg);
}

void PlayerOperationLogic::UpdateModuleInfoToClient(Player &player, uint32 moduleId)
{
	pb::GxDB_Module moduleMsg;
	player.m_buildingLogMgr->SaveModuleInfo(moduleId,moduleMsg);
	player.Send( SMSG_MODULE_UPDATE, moduleMsg);
}

void PlayerOperationLogic::AddIncomeTimer(Player *player)
{
	//NLOG("TEST PlayerOperationLogic::AddIncomeTimer");
	std::vector<uint32> vecModuleIds;
	sBuildingManager.GetMouduleIds(vecModuleIds);
	int nBuild = vecModuleIds.size();
	for (int i = 0; i< nBuild; i++)
	{
		uint32 moduleId = vecModuleIds[i];
		player->AddModuleGainTimer(moduleId);
	}
}