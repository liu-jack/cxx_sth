#include "../object/Player.h"
#include <boost/math/tr1.hpp>
#include "Building/BuildingLogMgr.h"
#include "CrossLogic/PlayerOperationLogic.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "TimerEvent/TimerEventDefine.h"
#include "Opcode.pb.h"
#include "Module.pb.h"
#include "../item/ItemManager.h"
#include "../character/CharacterStorage.h"
#include "../Palace/Table/PalaceTableMgr.h"
#include "../CrossLogic/PlayerBuildingLogic.h"
#include "BuildingManager.h"
#include "Building.h"
#include "../Technology/PlayerTechnology.h"
#include "../Technology/Table/TechnologyTableMgr.h"
#include "Trigger/trigger.h"



using namespace pb;
namespace PlayerHandle
{
	extern void SendOpcodeWithErrCode( Player* player, int opcode, pb::CxGS_ERROR_CODE err_code);
}

#define  PRE_JUDGE_INIT_STAGE_LOGGER  if(!m_buildingLogMgr.get() || !m_timerEventGroup.get()) { return;}
OpHandler(CMSG_BUILDING_STATE)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	C2GS_Building_State state;
	pack >> state;
	uint32 moduleId = state.module_id();
	pb::GS2C_Building_State logInfo;
	m_buildingLogMgr->UpdateModuleGainValue(this,moduleId,GetBuildingIncome(moduleId));
	PlayerBuildingLogic::HandleGetMouleInfo( *this,moduleId,logInfo); 
	Send(SMSG_BUILDING_STATE, logInfo);
// 	if (GetAutoHammerCount() > 0)
// 	{
// 		m_buildingLogMgr->SetCurAutoLvUpModule(moduleId);
// 	}
}

OpHandler(CMSG_BUILDING_LEVEL_UP)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	C2GS_Building_Level_Up msg;
	pack >> msg;
	uint32 modid = msg.module_id();
	uint32 buildingid = msg.building_id();
	if (msg.is_auto())
	{
		if (GetLevel() >= GET_BASE_DEF_INT(BD_AUTO_BUILDING_TRY_USE_LEVEL))
		{
			if (!TestFlag(pb::PLAYER_GET_AUTO_BUILD))
			{
				SendErrorCode(pb::ErrNeedVipToAutoBuild);
				SetAutoHammerCount(0);
				return;
			}
		}
		m_buildingLogMgr->UpdateAutoLvUpTimes(this,modid);	//添加自动建造次数
		DoAutoBuildingLvUp(modid);			//该模型内建筑自动建造
	}
	else
	{
		if (GetAutoHammerCount() > 0)
		{
			DoAutoBuildingLvUp(modid);
		}
		else
		{
			DoManualBuildingLvUp(modid,buildingid);		//单个建筑升级
		}
	}
}

OpHandler(CMSG_BUILDING_GOLD_HAMMER)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	if (m_buildingLogMgr->CurHasBuildingLvUp())
	{
		uint32 curGoldHammerCount = GetGoldHammerCount();
		bool isAutoBuilding = GetAutoHammerCount() > 0 ? true :false;
		if (curGoldHammerCount > 0)
		{
			SetGoldHammerCount(--curGoldHammerCount);
			m_buildingLogMgr->GoldHammerLvUp(boost::bind(&Player::BuildingLevelUp,this,_1,_2,isAutoBuilding));
		}
		else
		{
			uint32 exchangHammGold = GetDbBaseDefineValueUint(pb::BD_GOLD_EXCHANGE_GOLDHAMMER);
			if (EnoughGold(exchangHammGold))
			{
				if(TryDeductCurrency(IR_BUILDING_LV_UP_COST,eSysGold,exchangHammGold))
				{
					m_buildingLogMgr->GoldHammerLvUp(boost::bind(&Player::BuildingLevelUp,this,_1,_2,isAutoBuilding));
				}
			}
			else
			{
				SendErrorCode(ErrNotEnoughGold);
			}
		}
	}
}

OpHandler(CMSG_BUILDING_HONOR)
{
	PRE_JUDGE_INIT_STAGE_LOGGER
	int previous = GetCurrency(eBuildHornor);
	if(PlayerValueOwner::TryDeductCurrency(eBuildHornor,previous))
	{
		AddExploitValue(previous);
	}
}

OpHandler(CMSG_BUILDING_BUY_GAINMODE)
{
	uint32 progId;
	pack >> progId;
	pb::GS2C_Module_Buy_Gain_Mode_Res rspMsg;
	PlayerBuildingLogic::HandlePlayerBuyGainMode( *this,progId,rspMsg); 
	Send(SMSG_BUILDING_BUY_GAINMODE, rspMsg);
}

OpHandler(CMSG_BUY_LABOUR)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerBuyLabor(*this);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_BUY_LABOUR_RSP, err_code);
}

OpHandler(CMSG_START_REBUILD)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerStartRebuild( *this);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_START_REBUILD_RSP, err_code);
}

OpHandler(CMSG_IMPERIAL_REBUILD_INVEST)
{
	uint32 investId;
	pack >> investId;
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerImperialRebuildInvest( *this,investId);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_IMPERIAL_REBUILD_INVEST_RSP, err_code);
}

OpHandler(CMSG_ACCELERATE_REBUILD)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerAccelerateRebuild( *this);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_ACCELERATE_REBUILD_RSP, err_code);
}

OpHandler(CMSG_HARVEST_FOOD)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerHarvestFood( *this);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_HARVEST_FOOD_RSP, err_code);
}

OpHandler(CMSG_DEEP_GEM)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerDeepGem( *this);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_DEEP_GEM_RSP, err_code);
}

OpHandler(CMSG_SUPPORT_HARVEST)
{
	pb::CxGS_ERROR_CODE err_code = (pb::CxGS_ERROR_CODE)PlayerBuildingLogic::HandlePlayerSupportHavest( *this);
	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_SUPPORT_HARVEST_RSP, err_code);
}


void Player::DoManualBuildingLvUp(uint32 moduleId,uint32 buildingid)
{
	GS2C_Building_Level_Up_Begin lvUpMsg;
	lvUpMsg.set_module_id(moduleId);
	if (m_buildingLogMgr->IsRechBuildingLvUpMaxCount() || !m_buildingLogMgr->CanBuildingLvUp(moduleId,buildingid,GetLevel()))
	{
		return;
	}
	if (!m_buildingLogMgr->IsBuildingInLvUp(moduleId,buildingid))
	{
		uint32 lvUpTime = m_buildingLogMgr->GetLvUpTime(moduleId,buildingid);
		if (lvUpTime > 0)
		{
			uint32 coinReq = 0;
			uint32 woodReq = 0;
			if (m_buildingLogMgr->GetBuildingLvUpReq(moduleId,buildingid,coinReq,woodReq))
			{
				if (!EnoughCurrency(eCoin,coinReq))
				{
					SendErrorCode(pb::ErrNotEnoughCoin);
					return;
				}
				if (!EnoughCurrency(eWood,woodReq))
				{
					SendErrorCode(pb::ErrNotEnoughWood);
					return;
				}
				TryDeductCurrency(pb::IR_BUILDING_LV_UP_COST, eCoin,coinReq);
				TryDeductCurrency(pb::IR_BUILDING_LV_UP_COST,eWood,woodReq);
			}
			else
			{
				return;
			}

			time_t bigintime =	AddTimer(moduleId, buildingid,boost::bind(&Player::BuildingLevelUp, this, moduleId,buildingid,false), lvUpTime);
			if (bigintime > 0 && m_buildingLogMgr->AddLvUpBuilding(moduleId,buildingid))
			{
				GS2C_Building_Level_Up_Begin_lv_up_info* budst =  lvUpMsg.add_build_state();
				if (budst)
				{
					budst->set_lvup_start_time(bigintime);
					budst->set_building_id(buildingid);
				}
			}
		}
	}
	if (lvUpMsg.build_state_size() > 0)
	{
		uint32 remainTimes = GetAutoHammerCount();
		lvUpMsg.set_remain_auto_times(remainTimes);
		Send(SMSG_BUILDING_LEVEL_UP_BEGIN,lvUpMsg);
	}
}

void Player::DoAutoBuildingLvUp(uint32 moduleId)
{
	GS2C_Building_Level_Up_Begin lvUpMsg;
	lvUpMsg.set_module_id(moduleId);
	std::vector<uint32> toUpbuildIds;
	m_buildingLogMgr->GetLvUpBuildingIds(this,moduleId,toUpbuildIds,GetLevel());
	for (std::vector<uint32>::iterator iterBuild = toUpbuildIds.begin(); iterBuild != toUpbuildIds.end();)
	{
		uint32 curToUpBuildId = (*iterBuild);
		uint32 coinReq = 0;
		uint32 woodReq = 0;
		if (m_buildingLogMgr->GetBuildingLvUpReq(moduleId,curToUpBuildId,coinReq,woodReq))
		{
			if (!EnoughCurrency(eCoin,coinReq))
			{
				m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
				for(++iterBuild; iterBuild != toUpbuildIds.end(); ++iterBuild)
				{
					curToUpBuildId = (*iterBuild);
					m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
				}
				SendErrorCode(pb::ErrNotEnoughCoin);
				break;
			}
			if (!EnoughCurrency(eWood,woodReq))
			{
				m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
				for(++iterBuild; iterBuild != toUpbuildIds.end(); ++iterBuild)
				{
					curToUpBuildId = (*iterBuild);
					m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
				}
				break;
				SendErrorCode(pb::ErrNotEnoughWood);
				break;
			}
			TryDeductCurrency(pb::IR_BUILDING_LV_UP_COST, eCoin,coinReq);
			TryDeductCurrency(pb::IR_BUILDING_LV_UP_COST,eWood,woodReq);
		}
		else
		{
			m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
			for(++iterBuild; iterBuild != toUpbuildIds.end(); ++iterBuild)
			{
				curToUpBuildId = (*iterBuild);
				m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
			}
			break;
		}
		 ++iterBuild;
		uint32 lvUpTimeReq = m_buildingLogMgr->GetLvUpTime(moduleId,curToUpBuildId);
		if (lvUpTimeReq > 0)
		{
			time_t bigintime =	AddTimer(moduleId, curToUpBuildId,boost::bind(&Player::BuildingLevelUp, this, moduleId,curToUpBuildId,true), lvUpTimeReq);
			if (bigintime > 0)
			{
				GS2C_Building_Level_Up_Begin_lv_up_info* budst =  lvUpMsg.add_build_state();
				if (budst)
				{
					budst->set_lvup_start_time(bigintime);
					budst->set_building_id(curToUpBuildId);
				}
				SetAutoHammerCount(GetAutoHammerCount() > 0 ? (GetAutoHammerCount() - 1):0);
			}
			else
			{
				m_buildingLogMgr->RemoveLvUpBuilding(moduleId,curToUpBuildId);
			}

		}
	}
	if (lvUpMsg.build_state_size() > 0)
	{
		uint32 remainTimes = GetAutoHammerCount();
		lvUpMsg.set_remain_auto_times(remainTimes);
		Send(SMSG_BUILDING_LEVEL_UP_BEGIN,lvUpMsg);
	}
}

void Player::SendLevelUp(uint32 moduleId, uint32 buildingId)
{
	uint32 buildLv = m_buildingLogMgr->BuildLvUp(moduleId,buildingId);
	MMO_ASSERT(buildLv > 1);
	GS2C_Building_Level_Up msgRsp;
	msgRsp.set_module_id(moduleId);
	msgRsp.set_building_id(buildingId);
	msgRsp.set_building_lv(buildLv);
	Send(SMSG_BUILDING_LEVEL_UP, msgRsp);
	if(buildLv > 1)
	{
		PlayerOperationLogic::UpdateBuildingInfoToDB(*this,buildingId);
		uint32 honor = sBuildingManager.GetBuildHonorForLvUp(buildingId,buildLv);
		AddCurrency(eBuildHornor,honor);
		uint32 curexp = sBuildingManager.GetBuildExpForLvUp(buildingId,buildLv);
		//AddCurrency(ePlayerExp,curexp);
		AddXP(curexp);	
	}
	uint32 building_income = GetBuildingIncome(moduleId);
	m_buildingLogMgr->UpdateModuleGainValue(this,moduleId,building_income);
	m_buildingLogMgr->UpdateGrowUpQuestInfo(moduleId,this,GetBuildingIncome(moduleId,true));
}

void Player::BuildingLevelUp(uint32 moduleId, uint32 buildingId,bool isauto)
{
	uint32 canUpCount = GetAutoHammerCount();
	RemoveTimer(moduleId,buildingId);
	SendLevelUp(moduleId,buildingId);
	if (canUpCount > 0)
	{
		DoAutoBuildingLvUp(m_buildingLogMgr->CurAutoLvUpModule());
	}
}

int Player::GetBuildingIncome(const uint32 moduleId,bool no_build_buff/* = false*/)
{
	uint32 officeAdd = 0;
	uint32 module_Income = 0;
	if(!m_buildingLogMgr->IsBuildingUnlock(moduleId))
	{
		if(moduleId == MODULE_BINGYING)
			return GET_BASE_DEF_INT(pb::BD_BASE_TROOPS_CREAT_NUM);
		else 
			return 0;
	}
	int office_id = GetPlaceAideOfficeId();
	const DB_Palace_Proto* table = NULL;
	if(office_id > 0)
	{
		table = sPalaceTableMgr.GetProto(office_id);
	}
	else
	{
		table = sPalaceTableMgr.GetProto(GetPlaceOfficeId());
	}
	if (table)
	{
		if (moduleId == MODULE_MINJU || moduleId == MODULE_MUCHANG || moduleId == MODULE_LONGCHANG )
		{
			if(office_id > 0)
				officeAdd = table->aide_add;
			else
				officeAdd = table->master_add;
		}
		else if(moduleId == MODULE_KUANGCHANG)
		{
			if(sTrigger.Check(this,Palace_Trigger_Iron_Add))//科技GUANYUN
			{
				if(office_id > 0)
					officeAdd = table->aide_iron_add;
				else
					officeAdd = table->master_iron_add;
			}
		}
	}
	//NLOG("Player::GetBuildingIncome +%d",officeAdd + m_buildingLogMgr->GetModuleIncome(moduleId));
	if(no_build_buff)
		module_Income =  m_buildingLogMgr->GetModuleIncomeBase(moduleId);
	else
		module_Income =  m_buildingLogMgr->GetModuleIncome(moduleId);
	if(moduleId == MODULE_BINGYING)
		module_Income += GET_BASE_DEF_INT(pb::BD_BASE_TROOPS_CREAT_NUM);
	if(HaveTechnology(QIANGZHENG))
	{
		const TecTable* tec = sTechnologyTableMgr.GetTable(QIANGZHENG);
		if(tec)
		{	
			if(moduleId == MODULE_BINGYING)
			{
				module_Income += (uint32)(module_Income * tec->Value1() / 100.0);//加20%
			}
		}
	}
#ifdef _MMO_SERVER_
	return officeAdd + module_Income;
#endif

#ifdef _SERVER_CROSS_
	if(moduleId == MODULE_BINGYING)
		return officeAdd + module_Income;
	else
		return 0;
#endif
}

void Player::OnBuildRebuildFinish(uint32 buildId)
{
	const Building* curBuild = sBuildingManager.GetBuildingById(buildId);
	if (curBuild)
	{
		m_buildingLogMgr->UpdateBuildingRebuildState(curBuild->moduleId(),curBuild->id(),true);
		PlayerOperationLogic::UpdateBuildingInfo(*this,buildId);
		SetValueInt(pb::PLAYER_FIELD_CUR_REBUILD_ID,curBuild->NextRebuildId());
	}
	SetBeginRebuild(false);
}

void Player::OnImperialRebuildFinish(uint32 moduleId)
{
	uint32 curRebuildRound = GetAttrUint(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND);
	if (curRebuildRound == MAXIMPERIALREBUILDINVESTITEMS)
	{
		m_buildingLogMgr->UpdateModuleRebuildState(moduleId,true);
		PlayerOperationLogic::UpdateModuleInfo(*this,moduleId);
	}
	else
	{
		SetValueInt(pb::PLAYER_FIELD_CUR_PALACE_REBUILD_ROUND,curRebuildRound+1);
	}
	SetValueInt(pb::PLAYER_FIELD_CUR_ROUND_INVEST1,0);
	SetValueInt(pb::PLAYER_FIELD_CUR_ROUND_INVEST2,0);
	SetValueInt(pb::PLAYER_FIELD_CUR_ROUND_INVEST3,0);
	SetBeginRebuild(false);
}

time_t Player::AddImperialRebuildTimer(uint32 interval)
{
	if (interval > 0)
	{
		time_t time = AddTimer(MODULE_FORREBUILDTYPE,MODULE_IMPERIAL,boost::bind(&Player::OnImperialRebuildFinish, this, MODULE_IMPERIAL), interval);
		if (time > 0)
		{
			SetBeginRebuild(true);
			return time;
		}
	}
	return 0;
}

time_t Player::AddBuildingRebuildTimer(uint32 buildId,uint32 interval)
{
	if (interval > 0)
	{
		time_t time = AddTimer(MODULE_FORREBUILDTYPE,buildId,boost::bind(&Player::OnBuildRebuildFinish, this, buildId), interval);
		if (time > 0)
		{
			SetBeginRebuild(true);
			return time;
		}
	}
	return 0;
}

bool Player::AddModuleGainTimer(uint32 moduleId)
{
	//兵营在未解锁的情况下，也可以产出兵力
	if(moduleId != MODULE_BINGYING)
	{
		if(!m_buildingLogMgr->IsBuildingUnlock(moduleId))
			return false;
	}
	uint32 timeInterval = sBuildingManager.GetMoudleIncreaseTimeInterval(moduleId);
	if (timeInterval > 0)
	{
		AddCallBackTimer(moduleId,boost::bind(&Player::OnBuildingIncome, this, moduleId), timeInterval);
		m_buildingLogMgr->UpdateModuleGainValue(this,moduleId,GetBuildingIncome(moduleId));
		m_buildingLogMgr->UpdateModuleSartGainTimerTime(moduleId,sOS.GetRealTime());
		PlayerOperationLogic::UpdateModuleInfoToClient(*this,moduleId);
		return true;
	}
	return false;
}

bool Player::StartAutoIncreaseSolders()
{
	if (!IsAutoIncreaseSolders())
	{
		AddModuleGainTimer(MODULE_BINGYING);
		SetAutoIncreaseSolders(true);
		return true;
	}
	return false;
}

void Player::OnBuildingIncome(uint32 moduleId)
{
	//NLOG("Player::OnBuildingIncome,moduleId=%d",moduleId);
	uint32 icomItem = m_buildingLogMgr->GetIcomeType(moduleId); 
	uint32 icomSum = GetBuildingIncome(moduleId);
	uint32 icomLimitSum = GetBuildingIncomeLimit(moduleId);
	if (icomItem > 0 && icomSum > 0)
	{
		const LogicItemProto* curItem = sItemMgr.Find(icomItem);
		if (curItem)
		{
			if (curItem->IsCurrency())	//是物资
			{
				int previous = GetCurrency(curItem->category_3);
				if(previous < (int)icomLimitSum)
				{
					AddCurrency(pb::IR_MOUDLE_INCREASE_GET, curItem->category_3, icomSum);
				}
			}
			else if (curItem->IsTroops())		//是兵力
			{
				if (IsAutoIncreaseSolders())
				{
					const CharacterIds& oldBattleArray = m_characterStorage->GetBattleArray();
					bool isNotEnoughFood  = false;
					for (CharacterIds::const_iterator it = oldBattleArray.begin(); it != oldBattleArray.end(); ++it)
					{
						uint32 charaId = *it;
						if (charaId == 0)
						{
							continue;
						}
						Character* ch = m_characterStorage->MutableCharacter( charaId);
						if (ch == NULL)
							continue;
						uint32 curNeedHp = ch->NeedHp();
						if (curNeedHp == 0)
						{
							continue;
						}
						if(ch->isInPrison())		//被捕获，不回血
						{
							continue;
						}

						const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(ch->soldierLv);
						MMO_ASSERT(pTable);
						if (pTable == NULL)
							continue;
						float fNeedFood = pTable->NeedFood();
						int willAddHp = icomSum > curNeedHp ? curNeedHp : icomSum;
						int nAllNeedFood = (int)(willAddHp * fNeedFood *(ch->GetTotalReduceCostHP()/100.0)+ 0.5);
						int curAllFood = GetCurrency(eFood);
						if (curAllFood >= nAllNeedFood)
						{
							uint32 UsedHp = ch->RecoverHp(icomSum);
							if (UsedHp > 0)
							{
								m_characterStorage->SetModifiedID(ch->GetID());
								int nCostUserFood = (int)(UsedHp * fNeedFood *(ch->GetTotalReduceCostHP()/100.0)+ 0.5);
								TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eFood,nCostUserFood);
							}
						}
						else
						{
							PlayerHandle::SendOpcodeWithErrCode(this, SMSG_STOP_AUTO_RECOVER_HP, ErrNotEnoughFood);
							SetAutoIncreaseSolders(false);
							return;
						}
					}
				}
				else
				{
					return;
				}
			}
		}
		
	}
	AddModuleGainTimer(moduleId);
}
//在得到收入上限的时候，用这个函数，不要用m_buildingLogMgr->GetModuleIncomeLimit(moduleId);
//此函数已经包含
uint32 Player::GetBuildingIncomeLimit(uint32 moduleId){//
	uint32 baseLimit = m_buildingLogMgr->GetModuleIncomeLimit(moduleId);
	const TecTable *tec = NULL;
	uint32 IncomeLimit = baseLimit;
	if(HaveTechnology(CANGKUJIESHE)){
		tec = sTechnologyTableMgr.GetTable(CANGKUJIESHE);
		IncomeLimit += (uint32)(baseLimit * tec->Value1() / 100.0);
	}
	if(HaveTechnology(CANGKUGAIJIAN)){
		tec = sTechnologyTableMgr.GetTable(CANGKUGAIJIAN);
		IncomeLimit += (uint32)(baseLimit * tec->Value1() / 100.0);
	}

	if (TestFlag(pb::PLAYER_UP_RES_CAPCITY))
    {
		IncomeLimit += baseLimit;
    }
	return IncomeLimit;
}