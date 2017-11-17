/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef INCLUDE_CONGRATULATE_H
#define INCLUDE_CONGRATULATE_H

#include "SimpleSingleton.h"
#include "def/ConstDef.h"
#include "def/TypeDef.h"
#include "game_data/datastruct/struct_congratulate_config.h"
#include "ptr_container/PtrMap.h"
class CongratulateConfig
{
	const DB_CongratulateConfig& db_;
	IntPairVec congratulate_rewrad_;
	IntPairVec be_congratulate_reward_;
public:
	CongratulateConfig(const DB_CongratulateConfig& db);
	uint32 Id() const;
	uint32 CongratulateType() const;
	uint32 Cat1() const;
	uint32 BeforeRank() const;
	const IntPairVec& GetCongratulateReward() const;
	const IntPairVec& GetBeCongratulateReward() const;
};

typedef PtrMap<uint32,CongratulateConfig> CongratulateConfigMap;
class CongratulateMgr:public SimpleSingleton<CongratulateMgr>
{
public:
	uint32 congratulate_shen_qi_[TYPE_CONGRATULATE_SHEN_QI_COUNT];
	uint32 suit_pri[TYPE_SUIT_PRI_COUNT];
	uint32 suit_adv[TYPE_SUIT_ADV_COUNT];
	uint32 suit_mid[TYPE_SUIT_MID_COUNT];
	CongratulateMgr();
private:
	CongratulateConfigMap congratulate_config_map_;
public:
	uint32 GetTableId(const uint32 congratualte_type,const uint32 cat1,uint32 &before_rank);
	const IntPairVec* GetCongratulateReward(const uint32 table_id);
	const CongratulateConfigMap& getCongratulateConfigMap() const;
};

#define sCongratulateMgr (CongratulateMgr::Instance())

#endif