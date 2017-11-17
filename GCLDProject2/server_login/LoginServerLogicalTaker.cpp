

#include "LoginServerLogicalTaker.h"

#include "Logger.h"
#include "OS.h"

#include "def/MmoAssert.h"
#include "utility/STLUtility.h"
#include "server_client/CenterServerClient.h"
#include "server_client/EventLogServerClient.h"

#include "Opcode.pb.h"


LoginServerLogicalTaker::LoginServerLogicalTaker( void )
{
}


LoginServerLogicalTaker::~LoginServerLogicalTaker( void )
{
}


void LoginServerLogicalTaker::BeforeSeesionUpdate( uint64 nowTimeMS, int64 diffMS )
{
	sOS.Update() ;

	sCenterClient.Update( nowTimeMS, diffMS ) ;
	sEventLogClient.Update( nowTimeMS, diffMS ) ;
}

void LoginServerLogicalTaker::AfterSessionUpdate( uint64 nowTimeMS, int64 diffMS )
{
}



