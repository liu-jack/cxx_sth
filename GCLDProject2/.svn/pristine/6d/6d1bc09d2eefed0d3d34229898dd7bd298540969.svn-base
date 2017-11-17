#include "FuBenTable.h"
#include "Fuben.pb.h"
#include "InterServer.pb.h"

void FuBenBaseTable::SaveTo(pb::SG2D_UpdataFuBenBase& msg)
{
    msg.set_cur_fuben_id(cur_fuben_id);
    msg.set_cur_fuben_id(cur_stage_id);
	for(size_t i = 0;i < ARRAY_SIZE(NR_stars);++i)
	{
		msg.add_nr_stars(NR_stars[i]);
	}
}
void FuBenBaseTable::LoadFrom(const pb::SG2D_UpdataFuBenBase& msg)
{
    cur_fuben_id = msg.cur_fuben_id();
    cur_stage_id = msg.cur_stage_id();
	for(int i = 0;i < msg.nr_stars_size();++i)
	{
		NR_stars[i] = msg.nr_stars(i);
	}
	
}
void FuBenHideTable::SaveTo(pb::HideFubenInfo& msg)
{
    msg.set_hide_stage_id(stage_id);
    msg.set_free_times(free_tiems);
    msg.set_kill_monster(kill_monsters);
	msg.set_count_times(today_fight_times);
}
void FuBenHideTable::LoadFrom(const pb::HideFubenInfo& msg)
{
    stage_id = msg.hide_stage_id();
	free_tiems = msg.free_times();
    kill_monsters = msg.kill_monster();
	today_fight_times = msg.count_times();
}
void FuBenPassStageTable::SaveTo(pb::Stage_Info& msg)
{
    msg.set_stage_type(stage_type);
	msg.set_pass_count(pass_count);
	msg.set_is_take_reward(is_take_reward);
}
void FuBenPassStageTable::LoadFrom(const pb::Stage_Info& msg)
{
    stage_type = msg.stage_type();
	pass_count = msg.pass_count();
	is_take_reward = msg.is_take_reward();
}

void BranchFuBenTable::SaveTo(pb::BranchFubenInfo& msg)
{
	msg.set_stage_id(stage_id);
	msg.set_is_passed(is_passed);
	msg.set_is_take_award(is_take_star > 0 ? true:false);
	msg.set_branch_group_id(branch_fuben_group_id);
}

void BranchFuBenTable::LoadFrom(const pb::BranchFubenInfo& msg)
{
	stage_id = msg.stage_id();
	is_passed = msg.is_passed();
	is_take_star = msg.is_take_award();
	branch_fuben_group_id = msg.branch_group_id();
}
