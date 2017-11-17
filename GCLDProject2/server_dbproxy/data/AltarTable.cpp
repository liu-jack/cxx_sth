#include "AltarTable.h"
#include "InterServer.pb.h"
#include "Altar.pb.h"

void AltarTable::SaveTo(pb::GS2C_Altar_All_Info& msg)
{
    msg.set_levle(level);
    msg.set_silver_coin(silver_coin_for_build);
    for (int i = 0; i < ARRAY_SIZE(sub_altar_lst); ++i)
    {
        pb::SubAltar* data = msg.add_sub_altar();
        SubAltar& ref = sub_altar_lst[i];
        data->set_is_level_up(ref.is_building_up);
        data->set_have_reward(ref.have_reward);
        data->set_study_times(ref.study_times);
        data->set_last_study_time(ref.last_study_time);
        data->set_take_free_time(ref.last_study_time);
        data->set_reward_count(ref.reward_count);
    }
}
void AltarTable::LoadFrom(const pb::GS2C_Altar_All_Info& msg)
{
    level = msg.levle();
    silver_coin_for_build = msg.silver_coin();

    for (int i = 0; i < msg.sub_altar_size(); ++i) {
        const pb::SubAltar& data = msg.sub_altar(i);
        sub_altar_lst[i].is_building_up = data.is_level_up();
        sub_altar_lst[i].have_reward = data.have_reward();
        sub_altar_lst[i].study_times = data.study_times();
        sub_altar_lst[i].last_study_time = data.last_study_time();
        sub_altar_lst[i].take_free_time = data.take_free_time();
        sub_altar_lst[i].reward_count = data.reward_count();
    }
}
