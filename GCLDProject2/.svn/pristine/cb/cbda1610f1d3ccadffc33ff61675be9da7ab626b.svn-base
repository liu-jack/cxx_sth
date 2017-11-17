/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __ACTIVITY_MONEY_TO_GOLD_H
#define __ACTIVITY_MONEY_TO_GOLD_H

#include "game_data/datastruct/struct_recharge_rebate.h"
#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"


class ActivityRechargeRebate
{
public:
	ActivityRechargeRebate(const DB_ActivityRechargeRebate& db);
	uint32 Id() const;
	uint32 NeedCharge() const;
	uint32 Reward() const;

private:
	const DB_ActivityRechargeRebate & db_;
};



typedef PtrMap<uint32,ActivityRechargeRebate> ActivityRechargeRebateMap;

class ActivityRechargeRebateTableMgr:public SimpleSingleton<ActivityRechargeRebateTableMgr>
{
public:
	ActivityRechargeRebateTableMgr();
	void Init();
	const ActivityRechargeRebate* getActivityRechargeRebate(const uint32 id);
	const ActivityRechargeRebateMap& getActivityRechargeRebateMap() const;
private:
	ActivityRechargeRebateMap activity_recharge_rebate_map_;
	
};



#define sActivityRechargeRebateMgr ActivityRechargeRebateTableMgr::Instance()

#endif