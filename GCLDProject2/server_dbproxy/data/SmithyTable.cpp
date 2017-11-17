#include "SmithyTable.h"
#include "Smithy.pb.h"
void SmithyTable::SaveTo( pb::GS2C_Smithy_All_Info& msg )
{
	msg.set_levle(levle);
	msg.set_silver_coin(silver_coin);
	for (int i =0; i< 5; i++)
	{
		pb::SubSmelter* smelter =	msg.add_sub_smelter();
		if (smelter)
		{
			smelter->set_star(start[i]);
			smelter->set_smelt_times(smelt_times[i]);
		}
	}
	

}

void SmithyTable::LoadFrom( const pb::GS2C_Smithy_All_Info& msg )
{
    levle = msg.levle();
	silver_coin = msg.silver_coin();
	for (int i = 0; i<msg.sub_smelter_size();i++)
	{
		const pb::SubSmelter& sumMet = msg.sub_smelter(i);
		start[i] = sumMet.star();
		smelt_times[i] = sumMet.smelt_times();
	}
}
