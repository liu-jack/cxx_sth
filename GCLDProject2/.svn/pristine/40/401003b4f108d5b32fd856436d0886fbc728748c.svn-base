#include "StageCrusadeTable.h"
#include "InterServer.pb.h"
#include "CommomDef.pb.h"

void StageCrusadeTable::SaveTo(pb::GxDB_Combat_Crusade& msg)
{
	msg.set_level_id(currStage);
	msg.set_stage_state(isDead);
	msg.set_rewards_state(award_state);
	msg.set_reset_lefttimes(restReset);
	msg.set_add_lefttimes(addCharTimes);
	msg.set_is_crusade_over(isCrusadeOver);
	for(int i = 0; i < ARRAY_SIZE(CharTimes);++i)
	{
		pb::PairValue * value = msg.add_chartimelist();
		value->set_first(i+1);
		value->set_second(CharTimes[i]);
	}
}

void StageCrusadeTable::LoadFrom(const pb::GxDB_Combat_Crusade& msg)
{
	restReset = msg.reset_lefttimes();
	currStage = msg.level_id();
	isDead = msg.stage_state();
	award_state = msg.rewards_state();
	addCharTimes = msg.add_lefttimes();
	isCrusadeOver = msg.is_crusade_over();
	for(int i = 0;i < msg.chartimelist_size();++i)
	{
		const pb::PairValue & list = msg.chartimelist(i);
		CharTimes[list.first()-1] = list.second();
	}
}