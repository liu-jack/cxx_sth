#include "activity_reward.h"
#include "InterServer.pb.h"



void ActivityRewardTable::SaveTo( pb::SC2G_activity_reward& info )
{
	info.set_activity_id(activity_id);
	info.set_conditon(condition);
	info.set_cost_discount(cost_discount);
	info.set_table_id(id);
	info.set_is_has_discount(is_has_discount);
	info.set_reward_list(reward_list);
	info.set_cost_price(cost_price);
}