#include "PalaceTable.h"
#include "Palace.pb.h"

void PalaceOfficeTable::SaveTo(pb::OfficeInfo& msg)
{
    msg.set_country_id(country_id);
    msg.set_office_id(office_id);
    msg.set_skill_use_time(skillUseTime);
    msg.mutable_player()->set_player_id(playerId);
    for (int i = 0; i < ARRAY_SIZE(aideLst); ++i)
    {
        msg.add_aide_lst()->set_player_id(aideLst[i]);
    }
    for (int i = 0; i < ARRAY_SIZE(aideApplyLst); ++i)
    {
        msg.add_aide_apply_lst()->set_player_id(aideApplyLst[i]);
    }
}
void PalaceOfficeTable::LoadFrom(const pb::OfficeInfo& msg)
{
    country_id = msg.country_id();
    office_id = msg.office_id();
    playerId = msg.player().player_id();
    skillUseTime = msg.skill_use_time();
    for (int i = 0; i < msg.aide_lst_size(); ++i)
    {
        aideLst[i] = msg.aide_lst(i).player_id();
    }
    for (int i = 0; i < msg.aide_apply_lst_size(); ++i)
    {
        aideApplyLst[i] = msg.aide_apply_lst(i).player_id();
    }
}

void PalaceOfficeAchievement::SaveTo(pb::PalaceAchievementInfo& msg)
{
	msg.set_office_id(office_id);
	msg.set_office_state(office_state);
}

void PalaceOfficeAchievement::LoadFrom(const pb::PalaceAchievementInfo& msg)
{
	office_id = msg.office_id();
	office_state = msg.office_state();
}
