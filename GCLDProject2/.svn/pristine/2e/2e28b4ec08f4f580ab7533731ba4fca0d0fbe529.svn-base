
#include "SignUpTable.h"
#include "Quest.pb.h"


void SignUpTable::SaveTo( pb::GxDB_Sign_info& msg )
{
	msg.set_issigned(is_signed);
	msg.set_type(sign_type);
	msg.set_dayth(day);
}

void SignUpTable::LoadFrom( const pb::GxDB_Sign_info& msg )
{
	is_signed = msg.issigned();
	sign_type = msg.type();
	day = msg.dayth();
}