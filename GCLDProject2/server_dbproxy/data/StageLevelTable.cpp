#include "StageLevelTable.h"

#include "Stage.pb.h"





void StageLevelTable::SaveTo( pb::GS2C_StageLevel& msg )
{
	msg.set_level_id(level_id);
	msg.set_is_clearance( is_clearance > 0 );
	msg.set_fight_count( fight_count);
}

void StageLevelTable::LoadFrom( const pb::GS2C_StageLevel& msg )
{
	level_id                               = msg.level_id();
	is_clearance                           = msg.is_clearance();
	fight_count							   = msg.fight_count();
}

void EndlessLevelTable::SaveTo( pb::GS2C_StageLevel& msg )
{
    msg.set_level_id(level_id);
}

void EndlessLevelTable::LoadFrom( const pb::GS2C_StageLevel& msg )
{
    level_id                        = msg.level_id();
}
