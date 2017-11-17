#include "OpenFog.h"
#include "datastruct/struct_city.h"

uint32 OpenFog::Id() const
{
	return open_fog_.id;
}

uint32 OpenFog::OpenNum() const
{
	return open_fog_.open_num;
}

uint32 OpenFog::RewardId() const
{
	return open_fog_.reward_id;
}

uint32 OpenFog::RewardNum() const
{
	return open_fog_.reward_num;
}

OpenFog::OpenFog( const DB_OpenFog& db )
:open_fog_(db)
{

}