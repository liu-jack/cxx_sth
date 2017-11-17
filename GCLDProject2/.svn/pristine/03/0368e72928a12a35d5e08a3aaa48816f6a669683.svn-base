#include "FeedbackTable.h"
#include <time.h>
#include "InterServer.pb.h"

void FeedbackTable::LoadFrom( const pb::SG2C_FeedBack& info )
{
	playerId = info.playerid();
	playerName = info.player_name();
	feedTime = ::time( NULL );
	feedback = info.feedback();
}
