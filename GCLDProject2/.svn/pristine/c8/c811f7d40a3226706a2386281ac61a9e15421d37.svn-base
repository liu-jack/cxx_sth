

#include "ZhengWuTable.h"
#include "Activity.pb.h"

void ZhengWuTable::SaveTo(pb::GxDB_GovAffairsInfo& msg)
{
	msg.set_cost_diamond(cost_diamond);
	msg.set_left_id(left_id);
	msg.set_left_reward_count(left_reward_count);
	msg.set_left_reward_type(left_reward_type);
	msg.set_right_id(right_id);
	msg.set_right_reward_count(right_reward_count);
	msg.set_right_reward_type(right_reward_type);
	msg.set_title_id(title_id);
	msg.set_image_name(image_name);
}

void ZhengWuTable::LoadFrom(const pb::GxDB_GovAffairsInfo& msg)
{
	left_id = msg.left_id();
	right_id = msg.right_id();
	cost_diamond = msg.cost_diamond();
	image_name = msg.image_name();
	left_reward_count  = msg.left_reward_count();
	left_reward_type = msg.left_reward_type();
	title_id = msg.title_id();
	right_reward_count = msg.right_reward_count();
	right_reward_type = msg.right_reward_type();
}
