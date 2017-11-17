#include "PlayerBuildingLogic.h"
#include "datastruct/struct_buff.h"
#include "PlayerOperationLogic.h"
#include "object/Player.h"
#include "Module.pb.h"
#include "OS.h"
#include "../Building/BuildingManager.h"
#include "../Building/BuildingLogMgr.h"
#include "../Building/Building.h"
#include "TimerEvent/TimerEventGroup.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "item/ItemArray.h"
#ifdef _MMO_SERVER_
#include "../Lottery/LotteryLog.h"
#endif
using namespace pb;

void PlayerBuildingLogic::HandlePlayerBuyGainMode( Player& player, uint32 modeProid, pb::GS2C_Module_Buy_Gain_Mode_Res& rspMsg )
{
	rspMsg.set_error_code(ErrCommonFail);
	const Build_buffTable*  curBuildBuff = Build_buffTable::GetTable(modeProid);
	if (curBuildBuff)
	{
        if (!player.EnoughGold(curBuildBuff->GetCost()))
        {
			rspMsg.set_error_code(ErrNotEnoughGold);
			return;
        }
		uint32 curBuffId =  player.m_buildingLogMgr->GetCurGainBuffId(curBuildBuff->GetMoudle());
		uint32 toGetBuffId = curBuildBuff->GetBuffId();
		bool CanBuyBuff = false;
		if (curBuffId == toGetBuffId || curBuffId == 0)
		{
			CanBuyBuff = true;
		}
		else
		{
			const PlayerBuffTable* pCurBuff = PlayerBuffTable::GetTable(curBuffId);
			const PlayerBuffTable* pTogetBuff = PlayerBuffTable::GetTable(toGetBuffId);
			if (pCurBuff && pTogetBuff)
			{
				if (pTogetBuff->GetBuffLv() >= pCurBuff->GetBuffLv())
				{
					CanBuyBuff = true;
				}
			}
		}
		if (CanBuyBuff)
		{
			if (player.m_buildingLogMgr->AddCurGainBuffTime(curBuildBuff->GetMoudle(),toGetBuffId,curBuildBuff->GetEffectSecTime()))
			{
				player.TryDeductCurrency(IR_PLAYER_BUYBUFF_COST,eSysGold,curBuildBuff->GetCost());
				GxDB_Module* pModle = rspMsg.mutable_module_info();
				player.m_buildingLogMgr->SaveModuleInfo(curBuildBuff->GetMoudle(), *pModle);
				PlayerOperationLogic::UpdateModuleInfoToDB(player,curBuildBuff->GetMoudle());
				rspMsg.set_error_code(ErrCommonSuccess);
			}
			else
			{
				rspMsg.set_error_code(ErrCommonFail);
			}
		}
		else
		{
			rspMsg.set_error_code(ErrCommonFail);
		}
	}
}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerBuyLabor( Player& player )
{
	int buyTimesCurDay = player.GetAttrInt(pb::PLAYER_FIELD_BUY_LABOUR_TIMES);
	if (buyTimesCurDay < GET_BASE_DEF_INT(pb::BD_PLAYER_BUY_LABOR_TIMES_PER_DAY))
	{
		int buyLaborsPerTime = GET_BASE_DEF_INT(pb::BD_BUY_LABOR_NUM_EVERYTIME);
		int curLaborNum =  player.GetAttrInt(pb::PLAYER_FIELD_LABOUR_NUM);
		int baseNum = GET_BASE_DEF_INT(pb::BD_BUY_LABOR_GOLD_COEFFICIENT);
		int needGold = baseNum << buyTimesCurDay; 
		if (player.EnoughGold(needGold))
		{
			if(player.TryDeductCurrency(pb::IR_PLAYER_BUY_ITEM_COST,eSysGold,needGold))
			{
				player.SetValueInt(pb::PLAYER_FIELD_LABOUR_NUM,buyLaborsPerTime + curLaborNum);
				player.SetValueInt(pb::PLAYER_FIELD_BUY_LABOUR_TIMES,buyTimesCurDay+1);
				return ErrCommonSuccess;
			}
			else
			{
				return ErrNotEnoughGold;
			}
		}
		else
		{
			return ErrNotEnoughGold;
		}
	}
	else
	{
		return ErrRechMaxTimes;
	}
}

void PlayerBuildingLogic::HandleGetMouleInfo( Player& player,uint32 moduleId, pb::GS2C_Building_State& logInfo )
{
	player.m_buildingLogMgr->SaveModuInfoStateTo(moduleId,player.GetAutoHammerCount(),logInfo);
	int curBuildCount = logInfo.building_state_size();
	for (int i = 0; i< curBuildCount; i++)
	{
		GxDB_Building* curbuildState = logInfo.mutable_building_state(i);
		if (curbuildState)
		{
			time_t startTime = player.m_timerEventGroup->GetTimerStartTime(moduleId,curbuildState->building_id());
			curbuildState->set_lvup_start_time(startTime);
		}
	}
}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerStartRebuild( Player& player )
{
	uint32 curRebuildId = player.GetAttrUint(pb::PLAYER_FIELD_CUR_REBUILD_ID);
	if (curRebuildId > 0)
	{
		const Building* curBuild = sBuildingManager.GetBuildingById(curRebuildId);
		if (curBuild)
		{
			uint32 needbabor = curBuild->RebuildLaborNeed();
			uint32 curLaborNum =  player.GetAttrUint(pb::PLAYER_FIELD_LABOUR_NUM);
			if (curLaborNum  < needbabor)
			{
				return ErrNotEnoughLabour;
			}
			if (player.IsBeginRebuild())
			{
				return ErrAlreadyBegin;
			}
			uint32 needTime = curBuild->RebuildNeedTime();
			time_t startTime = player.AddBuildingRebuildTimer(curBuild->id(),needTime);
			if (startTime > 0)
			{
				uint64 curRebuildTime =  (uint64)needTime + (uint64)startTime;
				player.SetValueUint64(pb::PLAYER_FIELD_CUR_REBUILD_END_TIME,curRebuildTime);
				player.SetValueInt(pb::PLAYER_FIELD_LABOUR_NUM,curLaborNum - needbabor);
				return ErrCommonSuccess;
			}
		}
	}
	else if ( curRebuildId == 0)
	{
		uint32 curRebuildRound = player.GetAttrUint(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND);
		if (curRebuildRound == 0)
		{
			player.SetValueInt(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND,1);
			curRebuildRound = player.GetAttrUint(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND);
		}
		const DB_ImperialRebuild* curReBuildInfo = sBuildingManager.GetRebuildInvestInfo(curRebuildRound);
		if (curReBuildInfo)
		{
			uint32 curLaborNum =  player.GetAttrUint(pb::PLAYER_FIELD_LABOUR_NUM);
			if (curLaborNum < curReBuildInfo->need_labor)
			{
				return ErrNotEnoughLabour;
			}
			if (player.IsBeginRebuild())
			{
				return ErrAlreadyBegin;
			}
			uint32 needTime = curReBuildInfo->need_times;
			time_t startTime = player.AddImperialRebuildTimer(needTime);
			if (startTime > 0)
			{
				uint64 curRebuildTime =  (uint64)needTime + (uint64)startTime;
				player.SetValueUint64(pb::PLAYER_FIELD_CUR_REBUILD_END_TIME,curRebuildTime);
				player.SetValueInt(pb::PLAYER_FIELD_LABOUR_NUM,curLaborNum - curReBuildInfo->need_labor);
				return ErrCommonSuccess;
			}
		}
	}
	return ErrCommonFail;

}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerImperialRebuildInvest( Player& player,uint32 investType )
{
	ItemArray* bag = player.m_bag.get();
	if ( bag == NULL)                                                                
		return ErrCommonFail;

	uint32 curRebuildId = player.GetAttrUint(pb::PLAYER_FIELD_CUR_REBUILD_ID);
	if (curRebuildId == 0 && investType < MAXIMPERIALREBUILDINVESTITEMS)
	{
		uint32 curRebuildRound = player.GetAttrUint(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND);
		if (curRebuildRound == 0)
		{
			player.SetValueInt(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND,1);
			curRebuildRound = player.GetAttrUint(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND);
		}
		const DB_ImperialRebuild* curReBuildInfo = sBuildingManager.GetRebuildInvestInfo(curRebuildRound);
		if (curReBuildInfo)
		{
		  int itemId =	(int)curReBuildInfo->need_item[investType];
		  uint32 needAllCount = curReBuildInfo->need_item_num[investType];
		  uint32 curInverRecode = 0;
		  if (investType == 0)
		  {
			  curInverRecode  = pb::PLAYER_FIELD_CUR_ROUND_INVEST1;
		  }
		  else  if (investType == 1)
		  {
			  curInverRecode  = pb::PLAYER_FIELD_CUR_ROUND_INVEST2;
		  }
		  else  if (investType == 2)
		  {
			  curInverRecode  = pb::PLAYER_FIELD_CUR_ROUND_INVEST3;
		  }
		  uint32 curInvest = player.GetAttrUint(curInverRecode);
		  if (curInvest >= needAllCount)
		  {
			  return ErrAlreadyInvest;
		  }
		  float perCentPerTime = GET_BASE_DEF_FLOAT(pb::BD_REBUILD_INVEST_NUM_PERTIME);
		  int curNeedCount = (int)(needAllCount * perCentPerTime);
		  if(bag->EnoughItem(itemId,curNeedCount))
		  {
			  player.SetValueInt(curInverRecode,curInvest + curNeedCount);
			  player.TakeOutItem(IR_PLAYER_REBUILD_COST,itemId,curNeedCount);
			  return ErrCommonSuccess;
		  }
		  else
		  {
			  return ErrNotEnoughItem;
		  }
		}
	}
	return ErrCommonFail;
}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerAccelerateRebuild( Player& player )
{
	if (player.IsBeginRebuild())
	{
		uint64 curRebuildEndTime = player.GetAttrUint64(pb::PLAYER_FIELD_CUR_REBUILD_END_TIME);
		uint64 curTime = (uint64)sOS.GetRealTime();
		if (curRebuildEndTime > curTime)
		{
			int remainTime = (int)(curRebuildEndTime - curTime);
			uint32 curRebuildId = player.GetAttrUint(pb::PLAYER_FIELD_CUR_REBUILD_ID);
			int buyLaborsPerTime = GET_BASE_DEF_INT(pb::BD_ACCELERATE_REBUILD_TIME_COST)*60;
			int needGold = remainTime/buyLaborsPerTime + (remainTime % buyLaborsPerTime > 0 ? 1:0);
			if(player.EnoughGold(needGold))
			{
				if(player.TryDeductCurrency(IR_PLAYER_REBUILD_COST,eSysGold,needGold))
				{
					if (curRebuildId == 0)
					{
						player.RemoveTimer(MODULE_FORREBUILDTYPE,MODULE_IMPERIAL);
						player.OnImperialRebuildFinish(MODULE_IMPERIAL);
						return ErrCommonSuccess;
					}
					else if (curRebuildId > 0)
					{
						player.RemoveTimer(MODULE_FORREBUILDTYPE,curRebuildId);
						player.OnBuildRebuildFinish(curRebuildId);
						return ErrCommonSuccess;
					}
				}
			}
			else
			{
				return ErrNotEnoughGold;
			}
		}
	}
	return ErrCommonFail;
}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerHarvestFood( Player& player )
{
	if(!player.TestFlag(pb::PLAYER_FLAG_HAD_HARVEST))
	{
		uint32 willGetFood = GET_BASE_DEF_UINT(pb::BD_FARM_REBUILD_EFFECT)*player.GetLevel()*player.GetLevel();
		player.AddCurrency(IR_PLAYER_REBUILD_EFFECT_GET, eFood,willGetFood);
		player.SetFlag(pb::PLAYER_FLAG_HAD_HARVEST);
		return ErrCommonSuccess;
	}
	
	return ErrTodayAlreadyGet;
}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerDeepGem( Player& player )
{

	if(!player.TestFlag(pb::PLAYER_FLAG_HAD_DEEP))
	{
		uint32 supCollectNum = GET_BASE_DEF_UINT(pb::BD_DWELLING_REBUILD_EFFECT);
		player.AddCurrency(IR_PLAYER_REBUILD_EFFECT_GET, eSuperCollectNum,supCollectNum);
		player.SetFlag(pb::PLAYER_FLAG_HAD_DEEP);
		return ErrCommonSuccess;
	}
	return ErrTodayAlreadyGet;
}

PlayerBuildingLogic::ErrCode PlayerBuildingLogic::HandlePlayerSupportHavest( Player& player )
{
#ifdef _MMO_SERVER_
	if(!player.TestFlag(pb::PLAYER_FLAG_HAD_SUPPORT))
	{
		int itemId = LotteryLogic::PlayerLootKuangChangRebuildEffectGem(player);
		if (itemId > 0)
		{
			player.CreateItem(IR_PLAYER_REBUILD_EFFECT_GET,itemId,1);
			player.SetFlag(pb::PLAYER_FLAG_HAD_SUPPORT);
			return ErrCommonSuccess;
		}
	}
#endif
	return ErrTodayAlreadyGet;
}
