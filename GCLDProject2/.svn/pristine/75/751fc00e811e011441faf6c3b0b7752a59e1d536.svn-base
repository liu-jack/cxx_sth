#include "GiftBagTable.h"

#include "Web.pb.h"
#include "utility/TimeParser.h"

void GiftBagTable::LoadFrom( const pb::CreateGiftCode& info )
{
    giftId = info.giftid();
	giftName = info.giftname();
	String2TimeT(info.begintime(),beginTime);
	String2TimeT(info.endtime(),endTime);
	valid = 1;
	gifts = info.gift();
	reuse = info.reuse();
	createTime = ::time( NULL );
}
