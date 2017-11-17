/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "ContinueOccupyCityMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"

ContinueOccupyCity::ContinueOccupyCity(const DB_ContinueOccupyCity& db)
:db_(db)
{
	Utility::SplitStr(db_.country_count,p_,',');
}

uint32 ContinueOccupyCity::getRewardCount() const
{
	return db_.rewrad_count;
}

const IntPair& ContinueOccupyCity::getIntpair() const
{
	return p_;
}

uint32 ContinueOccupyCity::getContinCityNum() const
{
	return db_.serial_count;
}

float ContinueOccupyCity::getProbality() const
{
	return db_.probability;
}

void ContinueOccupyCityTableMgr::Init()
{
	FOREACH_DB_ITEM(p,DB_ContinueOccupyCity)
	{
		ContinueOccupyCity * a = new ContinueOccupyCity(*p);
		continue_occupy_city_map_[p->id] = a;
		queue_of_reward_.push_back(std::make_pair(a->getContinCityNum(),a->getRewardCount()));
	}
}

float ContinueOccupyCityTableMgr::getProbalityByCityNum(const int city_num)
{
	for(ContinueOccupyCityMap::iterator iter = continue_occupy_city_map_.begin();iter != continue_occupy_city_map_.end();++iter)
	{
		const IntPair& p = iter->second->getIntpair();
		if(city_num >= p.first && city_num < p.second )
		{
			return iter->second->getProbality();
		}
	}
	return 0;
}

std::deque<std::pair<uint32,uint32> >& ContinueOccupyCityTableMgr::getQueue()
{
	return queue_of_reward_;
}
