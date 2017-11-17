#include "TechnologyTable.h"
#include "Technology.pb.h"

void TechnologyStudyTable::SaveTo(pb::TechnologyStudy& msg)
{
    msg.set_tec_id(tec_id);
    msg.set_star_cnt(star);
    msg.set_end_time(end_time);
	msg.set_is_finish_study(is_finished);
}
void TechnologyStudyTable::LoadFrom(const pb::TechnologyStudy& msg)
{
    end_time = msg.end_time();
    tec_id = msg.tec_id();
    star = msg.star_cnt();
	is_finished = msg.is_finish_study();
}