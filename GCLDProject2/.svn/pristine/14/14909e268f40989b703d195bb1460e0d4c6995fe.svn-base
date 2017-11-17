/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "ActivityMoneyToGoldTableMgr.h"
#include "DbTool.h"

ActivityRechargeRebate::ActivityRechargeRebate(const DB_ActivityRechargeRebate& db)
:db_(db)
{

}

uint32 ActivityRechargeRebate::Id() const
{
	return db_.id;
}

uint32 ActivityRechargeRebate::NeedCharge() const
{	
	return db_.need_charge;
}

uint32 ActivityRechargeRebate::Reward() const
{
	return db_.reward;
}

ActivityRechargeRebateTableMgr::ActivityRechargeRebateTableMgr()
{
	
}
void ActivityRechargeRebateTableMgr::Init()
{
	FOREACH_DB_ITEM(ptr1,DB_ActivityRechargeRebate)
	{
		ActivityRechargeRebate *c = new ActivityRechargeRebate(*ptr1);
		activity_recharge_rebate_map_[ptr1->id] = c;
	}
}

const ActivityRechargeRebate* ActivityRechargeRebateTableMgr::getActivityRechargeRebate(const uint32 id)
{
	return activity_recharge_rebate_map_.pfind(id);
}
const ActivityRechargeRebateMap& ActivityRechargeRebateTableMgr::getActivityRechargeRebateMap() const
{
	return activity_recharge_rebate_map_;
}
